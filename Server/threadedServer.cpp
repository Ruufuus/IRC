#include "const.h"
#include "user.h"
#include "room.h"
#include "thread_data_t.h"
#include "message_handling.h"
#include <pthread.h>

class room;         //informacja o istnieniu klasy room
class user;         //informacja o istnieniu klasy user
/*
    Funkcja wysyłająca do wszystkich userów podłączonych do pokoju
    informacje z listą obecnie podłączonych użytkowników
*/
void send_actual_user_list(thread_data_t * th_data){
    char * message = th_data->room_list[th_data->room_index].get_user_list();
    th_data->room_list[th_data->room_index].send_to_everyone(message);
    delete message;
}
/*
    Funkcja iterując się po liście pokojów tworzy z ich nazw listę,
    która jest następnie wysyłana do wszystkich userów podłączonych do serwera.
*/
void send_actual_room_list(thread_data_t * th_data){
    char * buff = new char [BUFF_SIZE-1];
    memset(buff,'\0',sizeof(char)*BUFF_SIZE-1);
    strcpy(buff,"$room_list ");
    pthread_mutex_lock(&(th_data->room_list_mutex));
    for(int i=0;i<MAX_ROOMS;i++){
        if(!th_data->room_list[i].get_if_alive())
            continue;
        strcat(buff,th_data->room_list[i].get_room_name().c_str());
        strcat(buff," ");
    }
    pthread_mutex_unlock(&(th_data->room_list_mutex));
    strcat(buff,"\n");
    for(int i=0;i<MAX_ROOMS;i++){
        if(!th_data->room_list[i].get_if_alive())
            continue;
        th_data->room_list[i].send_to_everyone(buff);
    }
    
    delete buff;
}
/*
    Funkcja wyszukuje pokoju o nazwie podanej w zmiennej buffor,
    a nastepnie dolacza uzytkownika do kanalu, gdy jest w nim miejsce,
    a jak nie ma to wysyla wiadomosc o niemozliwosci dolaczenia
    Funkcja zwraca informacje o tym, czy pokoj o zadanej nazwie istnieje.
*/
bool joining_to_channel(thread_data_t *th_data,char * buffor){
    bool czy_dolaczono=false;       //z
    pthread_mutex_lock(&(th_data->room_list_mutex));            //opuszczenie semaforu listy pokojow
    for(int i=0;i<MAX_ROOMS;i++){                               //iterowanie sie po liscie pokojow w poszukiwaniu pokoju o zadanej nazwie
        if(!strcmp(th_data->room_list[i].get_room_name().c_str(),buffor)){
            user temp_user = th_data->room_list[th_data->room_index].get_user(th_data->connection_socket_descriptor);
            if(th_data->room_list[i].add_user(temp_user))               //dodawanie do pokoju uzytkownika, funkcja dodawajaca zwraca boola mowiacego o tym czy operacja sie udala
            {
                th_data->room_list[th_data->room_index].remove_user(th_data->connection_socket_descriptor); 
                send_actual_user_list(th_data);    
                th_data->room_index=i;                      //gdy operacja sie udala usuwamy uzytkownika z poprzedniego pokoju i zmieniamy index pokoju
                send_actual_user_list(th_data);             //ponad to wysylamy informacje o zmianie listy uzytkownikow
                printf("User polaczony na sockecie %d dolaczyl do kanalu %s\n",th_data->connection_socket_descriptor,buffor);
            }
            else{
                char * error = new char [100];
                memset(error,'\0',sizeof(char)*100);
                strcpy(error,"$error Nie udalo sie dolaczyc do kanalu");
                th_data->room_list[th_data->room_index].sending_mutex_lock();   //w przypadku braku miejsc w pokoju nastepuje 
                sending_message(th_data->connection_socket_descriptor,error);   //wyslanie informacji o niemoznosci dolaczenia do pokoju
                th_data->room_list[th_data->room_index].sending_mutex_unlock();
                delete error;
            }
            czy_dolaczono=true; 
            break;
        }
    }
    pthread_mutex_unlock(&(th_data->room_list_mutex));      //podniesienie semaforu listy pokojow
    return czy_dolaczono;
}
/*
    Funkcja tworzy pokoj o nazwie podanej w zmiennej buffor,
    a nastepnie dodaje tworce pokoju do niego
*/
void creating_channel(thread_data_t *th_data, char* buffor){
    bool czy_stworzono=false;                       //zmienna typu bool informujaca czy user stworzyl pokoj
    pthread_mutex_lock(&(th_data->room_list_mutex));//opuszczenie semafora lista pokojow
    for(int i=1;i<MAX_ROOMS;i++){                   //iterowanie sie po liscie pokojow w celu znalezienia pustego pokoju
        if(!th_data->room_list[i].get_if_alive()){
            user temp_user = th_data->room_list[th_data->room_index].get_user(th_data->connection_socket_descriptor);
            th_data->room_list[i].set_room_name(std::string(buffor));
            th_data->room_list[i].add_user(temp_user);              //zmienienie parametrow pokoju i dodanie do niego usera
            th_data->room_list[th_data->room_index].remove_user(th_data->connection_socket_descriptor); //usuniecie uzytkownika z poprzedniego pokoju
            send_actual_user_list(th_data); //poinformowanie uzytkownikow pokoju o opuszczeniu usera
            th_data->room_index=i;          //zmienienia room_indexu
            czy_stworzono=true;
            printf("User polaczony na sockecie %d stworzyl kanal %s\n",th_data->connection_socket_descriptor,buffor);
            break;
        }
    }
    pthread_mutex_unlock(&(th_data->room_list_mutex));//podniesienie semafora lista pokojow
    if(!czy_stworzono){
        char * error = new char [100];
        memset(error,'\0',sizeof(char)*100);
        strcpy(error,"$error Nie udalo sie utworzyc kanalu");
        th_data->room_list[th_data->room_index].sending_mutex_lock();       //wyslanie errora w przypadku niemoznosci stworzenia pokoju
        sending_message(th_data->connection_socket_descriptor,error);
        th_data->room_list[th_data->room_index].sending_mutex_unlock();
        delete error;
        printf("User'owi polaczonemu na sockecie %d nie udalo sie dolaczyc do kanalu llub go stworzyc\n",th_data->connection_socket_descriptor);
    }
    else{
            send_actual_room_list(th_data);//poinformowanie uzytkownikow serwera o dodaniu pokoju
    }
}
/*
    Funkcja obslugujaca wykrycie komendy $join x
    W momencie gdy pokoj x istnieje sprobuje do niego dolaczyc,
    a w momencie gdy nie istnieje pokoj x to sprobuje go stworzyc
*/
void join_command(thread_data_t * th_data,char * buffor){
    bool czy_dolaczono=joining_to_channel(th_data,buffor);
    if(!czy_dolaczono){
        creating_channel(th_data,buffor);
    }
}
/*
    Funkcja obslugujaca wykrycie komendy $username x.
    Zmienia nick usera na x
*/
void username_command(thread_data_t * th_data,char * buffor){
    th_data->room_list[th_data->room_index].change_username(th_data->connection_socket_descriptor,buffor);
    send_actual_user_list(th_data);
}
/*
    Funkcja obslugujaca wykrycie komendy $color x
    Zmienia kolor usera na x
*/
void color_command(thread_data_t * th_data,char * buffor){
    th_data->room_list[th_data->room_index].change_color(th_data->connection_socket_descriptor,buffor);
    send_actual_user_list(th_data);
}
/*
    Funkcja obslugujaca wykrycie komendy $room_list
    Iteruje sie po liscie pokojow i tworzy z ich nazw liste, 
    ktora nastepnie jest odsylana do usera.
*/
void room_list_command(thread_data_t * th_data){
    char * buff = new char [BUFF_SIZE-1];
    memset(buff,'\0',sizeof(char)*BUFF_SIZE-1);
    strcpy(buff,"$room_list ");
    pthread_mutex_lock(&(th_data->room_list_mutex));
    for(int i=0;i<MAX_ROOMS;i++){
        if(!th_data->room_list[i].get_if_alive())
            continue;
        strcat(buff,th_data->room_list[i].get_room_name().c_str());
        strcat(buff," ");
    }
    strcat(buff,"\n\0");
    pthread_mutex_unlock(&(th_data->room_list_mutex));
    th_data->room_list[th_data->room_index].sending_mutex_lock();
    sending_message(th_data->connection_socket_descriptor,buff);
    th_data->room_list[th_data->room_index].sending_mutex_unlock();
    
    delete buff;
}
/*
    Funkcja obslugujaca wykrycie komendy $user_list.
    Funckja wysyla do usera liste userow podlaczonych do kanalu
*/
void user_list_command(thread_data_t * th_data){
    char * message = th_data->room_list[th_data->room_index].get_user_list();
    th_data->room_list[th_data->room_index].sending_mutex_lock();
    sending_message(th_data->connection_socket_descriptor,message);
    th_data->room_list[th_data->room_index].sending_mutex_unlock();
    delete message;
}
/*
    Funkcja formatujaca wiadomosc tak,
    aby byla zrozumiala dla klienta
*/
char * formatting_message(thread_data_t *th_data,char * buffor){
    char * formatted_message = new char [BUFF_SIZE-1];
    memset(formatted_message,'\0',sizeof(char)*BUFF_SIZE-1);
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(formatted_message,BUFF_SIZE-1,"%H:%M:%S ",timeinfo);
    user temp_user = th_data->room_list[th_data->room_index].get_user(th_data->connection_socket_descriptor);
    strcat(formatted_message,temp_user.get_username().c_str());
    strcat(formatted_message,"@");
    strcat(formatted_message,temp_user.get_color().c_str());
    strcat(formatted_message," ");
    strcat(formatted_message,buffor);
    return formatted_message;
}
/*
    command number list:
    0-wykrycie $leave -rozlaczenie z serwerem
    1-wykrycie $join -stworzenie/dolaczenie do kanalu
    2-wykrycie $username x -zmiana username'u na x
    3-wykrycie $color y -zmiana koloru username'u osoby na y
    4-wykrycie $room_list -wyslanie do klienta informacji o liscie pokoi
    5-wykrycie $user_list -wyslanie do klient informacji o liscie user'ow podlaczonych do pokoju
*/
void *ThreadBehavior(void *t_data)
{
    pthread_detach(pthread_self());
    bool connected=true;                             //zmienna warunkujaca petle while - przechowuje informacje czy klient jest podlaczony
    thread_data_t *th_data = (thread_data_t*)t_data; //rzutowanie z typu void* na thread_data_t*
    char * buffor;                                  //zmienna wskaźnikowa wskazujaca na miejsce w pamieci, gdzie bedzie przechowywana otrzymana od usera wiadomosc
    user new_user(th_data->connection_socket_descriptor);
    th_data->room_list[0].add_user(new_user);
    pthread_mutex_lock(&(th_data->csd_array_mutex));
    for(int i = 0; i < MAX_USERS_CONNECTED_TO_CHANNEL*MAX_ROOMS;i++){
        if(th_data->connection_descriptor_array[i]==-1){
            th_data->connection_descriptor_array[i]=th_data->connection_socket_descriptor;
            break;
        }
    }
    pthread_mutex_unlock(&(th_data->csd_array_mutex));
    send_actual_user_list(th_data); 
    send_actual_room_list(th_data);                 //wyslanie informacji o liscie userow w pokoju deafault'owym oraz liscie kanalow
    while(connected && *(th_data->is_server_alive))
    {
        buffor=reading_message(th_data->connection_socket_descriptor,&connected);   //odczytanie wiadomosci i zapisanie jej w zmiennej buffor
        if (!buffor){
            break;
            }
        int command_number = command_detection(buffor,th_data->command);            
        switch(command_number){          
        case 0:
            connected=false;
            break;
        case 1:
            join_command(th_data,buffor);
            break;
        case 2:
            username_command(th_data,buffor);
            break;
        case 3:
            color_command(th_data,buffor);
            break;
        case 4:
            room_list_command(th_data);
            break;
        case 5:
            user_list_command(th_data);
            break;
        }
        /*
            rozeslanie wiadomosci po pokoju pod warunkiem,
            ze user jest nadal polaczony i jego wiadomosc nie zostala uznana jako komenda
            format wiadomosci:
            HH::MM::SS username@color message
        */
        if(connected && command_number==-1)
        {

            char * formatted_message=formatting_message(th_data,buffor);
            printf("Wysylanie wiadomosci o tresci %s",formatted_message);
            th_data->room_list[th_data->room_index].send_to_everyone(formatted_message);
            delete formatted_message;
        }
        /*
            rozlaczanie uzytkownika pod warunkiem
            ze jego wiadomosc zostala wykryta jako komenda $leave 
            lub connected zostalo ustawione na wartosc false.
            Po usunieciu uzytkownika zostaje rozeslana wiadomosc o aktualnej liscie uzytkownikow pokoju.
        */
        else if(!connected)
        {
            printf("Uzytkownik podlaczony do socketu %d rozlaczyl sie!\n",th_data->connection_socket_descriptor);
            th_data->room_list[th_data->room_index].remove_user(th_data->connection_socket_descriptor);
            send_actual_user_list(th_data);
        }
        delete buffor;      //zwalnianie pamieci zajmowanej przez buffor
    }
    pthread_mutex_lock(&(th_data->csd_array_mutex));
    for(int i = 0; i < MAX_USERS_CONNECTED_TO_CHANNEL*MAX_ROOMS;i++){
        if(th_data->connection_descriptor_array[i]==th_data->connection_socket_descriptor){
            th_data->connection_descriptor_array[i]=-1;
            break;
        }
    }
    pthread_mutex_unlock(&(th_data->csd_array_mutex));
    close(th_data->connection_socket_descriptor);             //zamykanie polaczenia na sockecie
    printf("Zakonczono poleczenie na sockecie %d!\n",th_data->connection_socket_descriptor);         
    delete th_data;                                         //zwalnianie pamieci zajmowanej przez zmienna przekazywana do watku
    pthread_exit(NULL);                                     //wychodzenie z watku    
}

//funkcja obsługująca połączenie z nowym klientem
void handleConnection(int connection_socket_descriptor,char ** command,room * room_list,pthread_mutex_t room_list_mutex,bool * is_server_alive,pthread_mutex_t csd_array_mutex,int * connection_descriptor_array) {
    //wynik funkcji tworzącej wątek
    int create_result = 0;

    //uchwyt na wątek
    pthread_t thread1;
    thread_data_t* t_data=new thread_data_t;
    t_data->connection_socket_descriptor=connection_socket_descriptor;          //wypelnianie pol struktury przekazywanej do watku
    t_data->command=command;
    t_data->room_list=room_list;
    t_data->room_index=0;
    t_data->room_list_mutex=room_list_mutex;
    t_data->is_server_alive=is_server_alive;
    t_data->csd_array_mutex=csd_array_mutex;
    t_data->connection_descriptor_array=connection_descriptor_array;
    create_result = pthread_create(&thread1, NULL, ThreadBehavior, (void *)t_data);     //tworzenie watku z obsluga bledu
    if (create_result){
    printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
    exit(-1);
    }
}
void *server_off(void * is_server_alive_)
{
    pthread_detach(pthread_self());
    bool *is_server_alive = (bool*)is_server_alive_;  
    char * buffor = new char [BUFF_SIZE];
        while(*is_server_alive){
        fgets(buffor,BUFF_SIZE,stdin);
        if(!strncmp(buffor,"stop",strlen("stop")))
        {
            printf("Trwa wylaczanie serwera!\n");
            *is_server_alive=false;
        }     
    }
    pthread_exit(NULL);  
    

}
int main(int argc, char* argv[])
{
    if (argc != 2)
   {
     fprintf(stderr, "Sposób użycia: ./server.exe port_number\n");
     exit(1);
   }
   int server_port=atoi(argv[1]);
   room * room_list = new room[MAX_ROOMS];              //deklarowanie zmiennej wskaznikowej wskazujacej na liste pokoi
   char ** command = new char * [COMMAND_ARRAY_SIZE];   //deklarowanie zmiennej wskaznikowej wskazujacej na liste polecen
   for (int i = 0; i<COMMAND_ARRAY_SIZE;i++)
        {
            command[i]=new char [15];
        }
    strcpy(command[0],"$leave");
    strcpy(command[1],"$join ");
    strcpy(command[2],"$username ");                    //wpisywanie listy kommend pod zadane adresy
    strcpy(command[3],"$color ");
    strcpy(command[4],"$room_list");
    strcpy(command[5],"$user_list");
   int server_socket_descriptor;                        //zmienna przechowujaca socket descriptor serwera
   int connection_socket_descriptor;                    //zmienna przechowujaca socket descriptor usera
   int bind_result;                                     //zmienna przechowujaca wynik operacji bind
   int listen_result;                                   //zmienna przechowujaca wynik operacji result
   int flags;                                           //zmienna przechowujaca flagi deskryptora serwera
   int fcntl_result;                                    //zmienna przechowujaca wynik operacji fcntl
   int * connection_descriptor_array = new int [MAX_ROOMS*MAX_USERS_CONNECTED_TO_CHANNEL];
   memset(connection_descriptor_array,-1,MAX_ROOMS*MAX_USERS_CONNECTED_TO_CHANNEL*sizeof(int));
   char reuse_addr_val = 1;                             
   pthread_mutex_t room_list_mutex;                     //mutex, ktory bedzie chronil przed wspolbiezna modyfikacja listy pokoi
   pthread_t thread1;
   pthread_mutex_t csd_array_mutex;
   thread_data_t* t_data=new thread_data_t;
   bool * is_server_alive=new bool;
   *is_server_alive=true;
   t_data->is_server_alive=is_server_alive;
   int mutex_room_list_init_result = pthread_mutex_init(&room_list_mutex,NULL); // inicjalizacja mutexa z odczytywaniem ewentualnych bledow
        if (mutex_room_list_init_result < 0)
        {
            fprintf(stderr, "Błąd przy próbie zainicjalizowania mutexa");
            exit(1);
        }
    int mutex_csd_array_init_result = pthread_mutex_init(&csd_array_mutex,NULL);
    if (mutex_csd_array_init_result < 0)
        {
            fprintf(stderr, "Błąd przy próbie zainicjalizowania mutexa");
            exit(1);
        }
   struct sockaddr_in server_address;                   

   //inicjalizacja gniazda serwera
   memset(&server_address, 0, sizeof(struct sockaddr));
   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = htonl(INADDR_ANY);
   server_address.sin_port = htons(server_port);

   server_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
   if (server_socket_descriptor < 0)
   {
       fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda..\n", argv[0]);
       exit(1);
   }
   flags = fcntl(server_socket_descriptor,F_GETFL);
   fcntl_result = fcntl(server_socket_descriptor, F_SETFL, flags | O_NONBLOCK);
   if(fcntl_result<0){
       fprintf(stderr, "%s: Blad przy ustawianiu flag deskryptora serwera..\n", argv[0]);
       exit(1);
   }
   setsockopt(server_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse_addr_val, sizeof(reuse_addr_val));
   printf("Socket serwera: %d\n",server_socket_descriptor);
   bind_result = bind(server_socket_descriptor, (struct sockaddr*)&server_address, sizeof(struct sockaddr));
   if (bind_result < 0)
   {
       fprintf(stderr, "%s: Błąd przy próbie dowiązania adresu IP i numeru portu do gniazda.\n", argv[0]);
       exit(1);
   }

   listen_result = listen(server_socket_descriptor, QUEUE_SIZE);
   if (listen_result < 0) {
       fprintf(stderr, "%s: Błąd przy próbie ustawienia wielkości kolejki.\n", argv[0]);
       exit(1);
   }
    int create_result = pthread_create(&thread1, NULL, server_off, (void *)is_server_alive);     //tworzenie watku z obsluga bledu
    if (create_result){
    printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
    exit(-1);
    }
   while(*is_server_alive)
   {
       connection_socket_descriptor = accept(server_socket_descriptor, NULL, NULL);
       if (connection_socket_descriptor == -1)
       {
           if (errno == EWOULDBLOCK)
           {
               //printf("Brak nadchodzacych polaczen, usypiam nasluchiwanie na sekunde!\n");
               sleep(1);
           }
           else{
                fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda dla połączenia.\n", argv[0]);
                exit(1);
           }
       }
       else{
            printf("Nastąpiło połączenie na sockecie: %d\n",connection_socket_descriptor);
            handleConnection(connection_socket_descriptor,command,room_list,room_list_mutex,is_server_alive,csd_array_mutex,connection_descriptor_array);
       }
   }
   pthread_mutex_lock(&(csd_array_mutex));
   for(int i = 0; i < MAX_USERS_CONNECTED_TO_CHANNEL*MAX_ROOMS;i++){
        if(connection_descriptor_array[i]!=-1){
            close(connection_descriptor_array[i]);
        }
    }
   pthread_mutex_unlock(&(csd_array_mutex));
   for (int i = 0; i<COMMAND_ARRAY_SIZE;i++)
        delete command[i];
   delete []command;                            //uwalnianie pamieci
   delete []room_list;
   delete []connection_descriptor_array;
   close(server_socket_descriptor);
   pthread_mutex_destroy(&csd_array_mutex);
   pthread_mutex_destroy(&room_list_mutex);
   printf("Serwer pomyslnie wylaczono!\n");
   return(0);
}
