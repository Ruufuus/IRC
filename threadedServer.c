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

//funkcja wysylajaca wiadomosci z ochrona przeciw fragmentacji
void bezpieczne_wysylanie(struct thread_data_t * t_data, char * tresc){
    /*
    *funkcja w petli wysyla kolejne fragmenty tresci wiadmosci,
    az nie wysle sumarycznie tyle bajtow, co ma tresc wiadomosci.
    W kazdej iteracji petli while, w zmiennej temp zapisujemy
    tymczasowo coraz to mniejsza czesc tresci wiadomosci
    */
    int write_result=0;
    int dlugosc_wiadomosci=sizeof(tresc);
    char * temp;
    do{
        temp = malloc(dlugosc_wiadomosci-write_result);
        strncpy(temp,tresc+write_result,dlugosc_wiadomosci-write_result);
        write_result+=write(t_data->connection_socket_descriptor,temp,strlen(temp));
        free(temp);
    }while(write_result!=dlugosc_wiadomosci);

}

//funkcja odbierajaca wiadomosci z ochrona przeciw fragmentacji
char * bezpieczne_odczytywanie(struct thread_data_t * t_data,int * czy_polaczony){
    /*
    Funkcja ta w kolejnych iteracjach petli while,
    odczytuje po jednym znaku, az nie natrafi na znak nowej linii
    Funkcja dodatkowo wykrywa czy uzytkownik sie nie rozlaczyl
    */
    char * buffor = malloc(sizeof(char)*BUFF_SIZE);
    char * temp = malloc(sizeof(char));
    int read_result;
    do{
        read_result=read(t_data->connection_socket_descriptor,temp,1);
        if(read_result<0)
        {
            czy_polaczony=0;
            printf("Nastapil blad przy odczycie!\n");
            break;
        }
        else if(read_result==0){
            czy_polaczony=0;
            break;
        }
        strcat(buffor,temp);
    }while(strcmp(temp,"\n"));
    free(temp);
    return buffor;
}

//funkcja wykrywa czy wiadomosc zaczyna sie od ciagu znakow zawartych w komendzie
int wykrywanie_komendy(char * wiadomosc, char * komenda){
    return !strncmp(wiadomosc, komenda,strlen(komenda));
}

//funkcja opisującą zachowanie wątku - musi przyjmować argument typu (void *) i zwracać (void *)
void *ThreadBehavior(void *t_data)
{
    pthread_detach(pthread_self());
    int czy_polaczony=1;
    struct thread_data_t *th_data = (struct thread_data_t*)t_data;
    char * buffor;
    while(czy_polaczony)
    {
        buffor=bezpieczne_odczytywanie(th_data,&czy_polaczony);
        if(wykrywanie_komendy(buffor,"$leave"))
        {
            czy_polaczony=0;
        }
        if(czy_polaczony)
        {
            printf("Serwer otrzymal wiadomosc o tresci: %s\n",buffor);
            for(int i=0;i<DESCRIPTION_ARRAY_SIZE;i++)
                {
                if(th_data->descriptor_array[i]!=th_data->connection_socket_descriptor && th_data->descriptor_array[i]!=-1)
                bezpieczne_wysylanie(th_data,buffor);
                }
        }
        else
        {
            printf("Uzytkownik sie rozlaczyl!\n");
            for(int i=0;i<DESCRIPTION_ARRAY_SIZE;i++)
                {
                    if(t_data->descriptor_array[i]==t_data->connection_socket_descriptor){
                        t_data->descriptor_array[i]=-1;
                        break;
                    }
                }
        }
        free(buffor);
    }
    free(th_data);
    pthread_exit(NULL);
}

//funkcja obsługująca połączenie z nowym klientem
void handleConnection(int connection_socket_descriptor,int * descriptor_array) {
    //wynik funkcji tworzącej wątek
    int create_result = 0;

    //uchwyt na wątek
    pthread_t thread1;
    struct thread_data_t* t_data=malloc(sizeof(struct thread_data_t));
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
