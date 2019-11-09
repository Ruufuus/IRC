#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define SERVER_PORT 2222
#define QUEUE_SIZE 5
#define BUFF_SIZE 1024
#define DESCRIPTION_ARRAY_SIZE 100
#define COMMAND_ARRAY_SIZE 3

//struktura zawierająca dane, które zostaną przekazane do wątku
struct thread_data_t
{
int connection_socket_descriptor; 
int * descriptor_array;
};

//funkcja wysylajaca messagei z ochrona przeciw fragmentacji
void bezpieczne_wysylanie(struct thread_data_t * t_data, char * tresc){
    /*
    *funkcja w petli wysyla kolejne fragmenty tresci wiadmosci,
    az nie wysle sumarycznie tyle bajtow, co ma tresc messagei.
    W kazdej iteracji petli while, w zmiennej temp zapisujemy
    tymczasowo coraz to mniejsza czesc tresci messagei
    */
    int write_result=0;
    int message_length=sizeof(tresc);
    char * temp;
    do{
        temp = new char[message_length-write_result];
        strncpy(temp,tresc+write_result,message_length-write_result);
        write_result+=write(t_data->connection_socket_descriptor,temp,strlen(temp));
        delete(temp);
    }while(write_result!=message_length);

}

//funkcja odbierajaca messagei z ochrona przeciw fragmentacji
char * bezpieczne_odczytywanie(struct thread_data_t * t_data,bool * connected){
    /*
    Funkcja ta w kolejnych iteracjach petli while,
    odczytuje po jednym znaku, az nie natrafi na znak nowej linii
    Funkcja dodatkowo wykrywa czy uzytkownik sie nie rozlaczyl
    */
    char * buffor = new char[BUFF_SIZE];
    char * temp = new char;
    int read_result;
    do{
        read_result=read(t_data->connection_socket_descriptor,temp,1);
        if(read_result<0)
        {
            printf("Nastapil blad przy odczycie!\n");
            break;
        }
        else if(read_result==0){
            *connected=false;
            break;
        }
        strcat(buffor,temp);
        printf("test\n");
    }while(strcmp(temp,"\n"));
    delete(temp);
    return buffor;
}

//funkcja wykrywa czy message zaczyna sie od ciagu znakow zawartych w komendzie
int wykrywanie_komendy(char * message){
    char ** command = new char * [COMMAND_ARRAY_SIZE];
    int command_number = -1;
    for (int i = 0; i<COMMAND_ARRAY_SIZE;i++)
        command[i]=new char [15];
    strcpy(command[0],"$leave");
    strcpy(command[1],"$join ");
    strcpy(command[2],"$username ");
    for (int i=0;i<COMMAND_ARRAY_SIZE;i++)
    {
        printf("%d %d\n",strncmp(message, command[i],strlen(command[i])),strlen(command[i]));
        if(!strncmp(message, command[i],strlen(command[i])))
            {
                char * new_message=new char [BUFF_SIZE];
                strcpy(new_message,message+strlen(command[i]));
                delete(message);
                message=new_message;
                command_number=i;
                break;
            }
    }
    printf("%s\n",message);
    for (int i = 0; i<COMMAND_ARRAY_SIZE;i++)
        delete(command[i]);
    delete(command);
    return command_number;
}

//funkcja opisującą zachowanie wątku - musi przyjmować argument typu (void *) i zwracać (void *)
void *ThreadBehavior(void *t_data)
{
    pthread_detach(pthread_self());
    bool connected=true;
    struct thread_data_t *th_data = (struct thread_data_t*)t_data;
    char * buffor;
    while(connected)
    {
        buffor=bezpieczne_odczytywanie(th_data,&connected);
        int command_number = wykrywanie_komendy(buffor);
        printf("%d\n",command_number);
        if(command_number==0){
            printf("###\n");
            printf("Uzytkownik podlaczony do socketu %d rozlaczyl sie!\n",th_data->connection_socket_descriptor);
            connected=false;
        }
        if(connected)
        {
            printf("Serwer otrzymal message o tresci: %s\n",buffor);
            for(int i=0;i<DESCRIPTION_ARRAY_SIZE;i++)
                {
                if(th_data->descriptor_array[i]!=th_data->connection_socket_descriptor && th_data->descriptor_array[i]!=-1)
                bezpieczne_wysylanie(th_data,buffor);
                }
        }
        else
        {
            printf("Uzytkownik sie rozlaczyl!\n");
            close(th_data->connection_socket_descriptor);
            for(int i=0;i<DESCRIPTION_ARRAY_SIZE;i++)
                {
                    if(th_data->descriptor_array[i]==th_data->connection_socket_descriptor){
                        th_data->descriptor_array[i]=-1;
                        break;
                    }
                }
        }
        delete(buffor);
        printf("Status %d",connected);
    }
    delete(th_data);
    pthread_exit(NULL);
}

//funkcja obsługująca połączenie z nowym klientem
void handleConnection(int connection_socket_descriptor,int * descriptor_array) {
    //wynik funkcji tworzącej wątek
    int create_result = 0;

    //uchwyt na wątek
    pthread_t thread1;
    struct thread_data_t* t_data=new struct thread_data_t;
    t_data->descriptor_array=descriptor_array;
    t_data->connection_socket_descriptor=connection_socket_descriptor; 
    create_result = pthread_create(&thread1, NULL, ThreadBehavior, (void *)t_data);
    if (create_result){
    printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
    exit(-1);
    }
}

int main(int argc, char* argv[])
{
    //int * descriptor_array = malloc(sizeof(int)*DESCRIPTION_ARRAY_SIZE);
   int * descriptor_array = new int [DESCRIPTION_ARRAY_SIZE];
   int server_socket_descriptor;
   int connection_socket_descriptor;
   int bind_result;
   int listen_result;
   char reuse_addr_val = 1;
   struct sockaddr_in server_address;

   //inicjalizacja gniazda serwera
   memset(descriptor_array, -1, sizeof(int)*DESCRIPTION_ARRAY_SIZE);
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
       for (int i=0;i<DESCRIPTION_ARRAY_SIZE;i++)
       {
           if(descriptor_array[i]==-1)
           {
                descriptor_array[i]=connection_socket_descriptor;
                break;
            }
       }
       handleConnection(connection_socket_descriptor,descriptor_array);
   }
   delete(descriptor_array);
   close(server_socket_descriptor);
   return(0);
}
