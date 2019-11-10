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
            connected=false;
        }
        if(connected)
        {
            printf("Serwer otrzymal message o tresci: %s\n",buffor);
            for(int i=0;i<MAX_USERS_CONNECTED_TO_CHANNEL;i++)
                {
                    if(th_data->room_list[th_data->room_index].get_user_sd(i)!=-1 && th_data->room_list[th_data->room_index].get_user_sd(i)!=th_data->connection_socket_descriptor){
                        sending_message(th_data->room_list[th_data->room_index].get_user_sd(i),buffor);
                    }
                }
        }
        else
        {
            printf("Uzytkownik sie rozlaczyl!\n");
            close(th_data->connection_socket_descriptor);
            th_data->room_list[th_data->room_index].remove_user(th_data->connection_socket_descriptor);
        }
        delete buffor;
    }
    delete th_data;
    pthread_exit(NULL);
}

//funkcja obsługująca połączenie z nowym klientem
void handleConnection(int connection_socket_descriptor,char ** command,room * room_list) {
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
   int server_socket_descriptor;
   int connection_socket_descriptor;
   int bind_result;
   int listen_result;
   char reuse_addr_val = 1;
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

       handleConnection(connection_socket_descriptor,command,room_list);
   }
   for (int i = 0; i<COMMAND_ARRAY_SIZE;i++)
        delete command[i];
   delete []command;
   delete []room_list;
   close(server_socket_descriptor);
   return(0);
}
