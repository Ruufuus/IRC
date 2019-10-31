#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define SERVER_PORT 1234
#define QUEUE_SIZE 5
#define BUFF_SIZE 1024
#define DESCRIPTION_ARRAY_SIZE 100

//struktura zawierająca dane, które zostaną przekazane do wątku
struct thread_data_t
{
int connection_socket_descriptor; 
int * descriptor_array;
};

//funkcja opisującą zachowanie wątku - musi przyjmować argument typu (void *) i zwracać (void *)
void *ThreadBehavior(void *t_data)
{
    pthread_detach(pthread_self());
    int czy_polaczony=1;
    struct thread_data_t *th_data = (struct thread_data_t*)t_data;
    char * buffor = malloc(sizeof(char)*BUFF_SIZE);
    //dostęp do pól struktury: (*th_data).pole
    //TODO (przy zadaniu 1) klawiatura -> wysyłanie albo odbieranie -> wyświetlanie
    while(czy_polaczony)
    {
        printf("Nasluchuje połączenie o sockecie: %d\n",th_data->connection_socket_descriptor);
        int read_result=read(th_data->connection_socket_descriptor,buffor,BUFF_SIZE);
        if(read_result<0){
            if(read_result==0){
                printf("Połączenie socketu %d zakończone\n",th_data->connection_socket_descriptor);
                for(int i=0;i<DESCRIPTION_ARRAY_SIZE;i++)
                {
                    if(th_data->descriptor_array[i]==th_data->connection_socket_descriptor)
                        th_data->descriptor_array[i]=-1;
                        break;
                }
            }
            else
            {
            printf("error przy odczycie!\n");
            }
        }
        else
            {
                if(!strncmp(buffor,"\\leave",strlen("\\leave")) && strlen(buffor)-1==strlen("\\leave"))
                {
                    czy_polaczony=0;
                    printf("Uzytkownik podlaczony do socketu %d sie rozlaczyl!\n",th_data->connection_socket_descriptor);
                }
                else{
                    printf("Serwer otrzymal wiadomosc o tresci: %s\n",buffor);
                    for(int i=0;i<DESCRIPTION_ARRAY_SIZE;i++){
                        if(th_data->descriptor_array[i]!=th_data->connection_socket_descriptor && th_data->descriptor_array[i]!=-1)
                            write(th_data->descriptor_array[i],buffor,strlen(buffor));
                    }
                }
            }
    }
    free(th_data);
    free(buffor);
    pthread_exit(NULL);
}

//funkcja obsługująca połączenie z nowym klientem
void handleConnection(int connection_socket_descriptor,int * descriptor_array) {
    //wynik funkcji tworzącej wątek
    int create_result = 0;

    //uchwyt na wątek
    pthread_t thread1;
    //dane, które zostaną przekazane do wątku
    //TODO dynamiczne utworzenie instancji struktury thread_data_t o nazwie t_data (+ w odpowiednim miejscu zwolnienie pamięci)
    //TODO wypełnienie pól struktury
    struct thread_data_t* t_data=malloc(sizeof(struct thread_data_t));
    t_data->descriptor_array=descriptor_array;
    t_data->connection_socket_descriptor=connection_socket_descriptor; 
    create_result = pthread_create(&thread1, NULL, ThreadBehavior, (void *)t_data);
    if (create_result){
    printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
    exit(-1);
    }
    //TODO (przy zadaniu 1) odbieranie -> wyświetlanie albo klawiatura -> wysyłanie
}

int main(int argc, char* argv[])
{
    int * descriptor_array = malloc(sizeof(int)*DESCRIPTION_ARRAY_SIZE);
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
   free(descriptor_array);
   close(server_socket_descriptor);
   return(0);
}
