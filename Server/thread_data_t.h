#ifndef thread_data_t_H
#define thread_data_t_H
//struktura zawierająca dane, które zostaną przekazane do wątku
class room;
struct thread_data_t{
char ** command;
int connection_socket_descriptor; 
int * connection_descriptor_array;
room * room_list;
int room_index;
bool * is_server_alive;
pthread_mutex_t csd_array_mutex;
pthread_mutex_t room_list_mutex;
};
#endif