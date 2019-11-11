#include "const.h"
#include "user.h"
#include "room.h"
#include "thread_data_t.h"
#include "message_handling.h"
#include <pthread.h>

class room;
class user;
//funkcja opisującą zachowanie wątku - musi przyjmować argument typu (void *) i zwracać (void *)
void *ThreadBehavior(void *t_data)
{
    pthread_detach(pthread_self());
    bool connected=true;
    thread_data_t *th_data = (thread_data_t*)t_data;
    char * buffor;
    while(connected)
    {
        buffor=reading_message(th_data->connection_socket_descriptor,&connected);
        int command_number = command_detection(buffor,th_data->command);
        if(command_number==0){
            printf("Uzytkownik podlaczony do socketu %d rozlaczyl sie!\n",th_data->connection_socket_descriptor);
            char* buff = new char[30];
            strcpy(buff,"connection ended");
            sending_message(th_data->connection_socket_descriptor,buff);
            connected=false;
            delete buff;
        }
        else if(command_number==1){
            bool czy_dolaczono=false;
            pthread_mutex_lock(&(th_data->room_list_mutex));
            for(int i=0;i<MAX_ROOMS;i++){
                if(!strcmp(th_data->room_list[i].get_room_name().c_str(),buffor)){
                    user temp_user = th_data->room_list[th_data->room_index].get_user(th_data->connection_socket_descriptor);
                    if(th_data->room_list[i].add_user(temp_user))
                    {
                        th_data->room_list[th_data->room_index].remove_user(th_data->connection_socket_descriptor);
                        th_data->room_index=i;
                        printf("User polaczony na sockecie %d dolaczyl do kanalu %s\n",th_data->connection_socket_descriptor,buffor);
                    }
                    else{
                        char * error = new char [100];
                        memset(error,'\0',sizeof(char)*100);
                        strcpy(error,"$error Nie udalo sie dolaczyc do kanalu");
                        th_data->room_list[th_data->room_index].sending_mutex_lock();
                        sending_message(th_data->connection_socket_descriptor,error);
                        th_data->room_list[th_data->room_index].sending_mutex_unlock();
                        delete error;
                    }
                    czy_dolaczono=true; 
                    break;
                }
            }
            pthread_mutex_unlock(&(th_data->room_list_mutex));
            if(!czy_dolaczono){
                bool czy_stworzono=false;
                pthread_mutex_lock(&(th_data->room_list_mutex));
                for(int i=1;i<MAX_ROOMS;i++){
                    if(!th_data->room_list[i].get_if_alive()){
                        user temp_user = th_data->room_list[th_data->room_index].get_user(th_data->connection_socket_descriptor);
                        th_data->room_list[i].set_room_name(std::string(buffor));
                        th_data->room_list[i].add_user(temp_user);
                        th_data->room_list[th_data->room_index].remove_user(th_data->connection_socket_descriptor);
                        th_data->room_index=i; 
                        czy_stworzono=true;
                        printf("User polaczony na sockecie %d stworzyl kanal %s\n",th_data->connection_socket_descriptor,buffor);
                        break;
                    }
                }
                pthread_mutex_unlock(&(th_data->room_list_mutex));
                if(!czy_stworzono){
                    char * error = new char [100];
                    memset(error,'\0',sizeof(char)*100);
                    strcpy(error,"$error Nie udalo sie utworzyc kanalu");
                    th_data->room_list[th_data->room_index].sending_mutex_lock();
                    sending_message(th_data->connection_socket_descriptor,error);
                    th_data->room_list[th_data->room_index].sending_mutex_unlock();
                    delete error;
                    printf("User'owi polaczonemu na sockecie %d nie udalo sie dolaczyc do kanalu llub go stworzyc\n",th_data->connection_socket_descriptor);
                }
            }
        }
        else if(command_number==2){
                user new_user = th_data->room_list[th_data->room_index].get_user(th_data->connection_socket_descriptor);
                new_user.set_username(buffor);
                th_data->room_list[th_data->room_index].remove_user(th_data->connection_socket_descriptor);
                th_data->room_list[th_data->room_index].add_user(new_user);
            }
        else if(command_number==3){
                user new_user = th_data->room_list[th_data->room_index].get_user(th_data->connection_socket_descriptor);
                new_user.set_color(buffor);
                th_data->room_list[th_data->room_index].remove_user(th_data->connection_socket_descriptor);
                th_data->room_list[th_data->room_index].add_user(new_user);
        }
        else if(command_number==4){
            char * buff = new char [BUFF_SIZE];
            memset(buff,'\0',sizeof(char)*BUFF_SIZE);
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
            sending_message(th_data->connection_socket_descriptor,buff);
            delete buff;
        }
        else if(command_number==5){
            th_data->room_list[th_data->room_index].send_user_list(th_data->connection_socket_descriptor);
        }
        if(connected && command_number==-1)
        {
            printf("Wysylanie wiadomosci o tresci %s",buffor);
            th_data->room_list[th_data->room_index].send_to_everyone(buffor);
        }
        else if(command_number==0 || !connected)
        {
            printf("Uzytkownik sie rozlaczyl!\n");
            th_data->room_list[th_data->room_index].remove_user(th_data->connection_socket_descriptor);
        }
        delete buffor;
    }
    close(th_data->connection_socket_descriptor);
    delete th_data;
    pthread_exit(NULL);
}

//funkcja obsługująca połączenie z nowym klientem
void handleConnection(int connection_socket_descriptor,char ** command,room * room_list,pthread_mutex_t room_list_mutex) {
    //wynik funkcji tworzącej wątek
    int create_result = 0;

    //uchwyt na wątek
    pthread_t thread1;
    thread_data_t* t_data=new thread_data_t;
    user new_user(connection_socket_descriptor);
    room_list[0].add_user(new_user);
    t_data->connection_socket_descriptor=connection_socket_descriptor; 
    t_data->command=command;
    t_data->room_list=room_list;
    t_data->room_index=0;
    t_data->room_list_mutex=room_list_mutex;
    create_result = pthread_create(&thread1, NULL, ThreadBehavior, (void *)t_data);
    if (create_result){
    printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
    exit(-1);
    }
}

int main(int argc, char* argv[])
{
   room * room_list = new room[MAX_ROOMS];
   char ** command = new char * [COMMAND_ARRAY_SIZE];
    for (int i = 0; i<COMMAND_ARRAY_SIZE;i++)
        {
            command[i]=new char [15];
        }
    strcpy(command[0],"$leave");
    strcpy(command[1],"$join ");
    strcpy(command[2],"$username ");
    strcpy(command[3],"$color ");
    strcpy(command[4],"$room_list");
    strcpy(command[5],"$user_list");
   int server_socket_descriptor;
   int connection_socket_descriptor;
   int bind_result;
   int listen_result;
   char reuse_addr_val = 1;
   pthread_mutex_t room_list_mutex;
   int mutex_room_list_init_result = pthread_mutex_init(&room_list_mutex,NULL);
        if (mutex_room_list_init_result < 0)
        {
            fprintf(stderr, "Błąd przy próbie zainicjalizowania mutexa");
            exit(1);
        }
   struct sockaddr_in server_address;

   //inicjalizacja gniazda serwera
   memset(&server_address, 0, sizeof(struct sockaddr));
   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = htonl(INADDR_ANY);
   server_address.sin_port = htons(SERVER_PORT);

   server_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
   if (server_socket_descriptor < 0)
   {
       fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda..\n", argv[0]);
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

   while(1)
   {
       connection_socket_descriptor = accept(server_socket_descriptor, NULL, NULL);
       printf("Nastąpiło połączenie na sockecie: %d\n",connection_socket_descriptor);
       if (connection_socket_descriptor < 0)
       {
           fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda dla połączenia.\n", argv[0]);
           exit(1);
       }

       handleConnection(connection_socket_descriptor,command,room_list,room_list_mutex);
   }
   for (int i = 0; i<COMMAND_ARRAY_SIZE;i++)
        delete command[i];
   delete []command;
   delete []room_list;
   close(server_socket_descriptor);
   return(0);
}
