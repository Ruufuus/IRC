#ifndef thread_data_t_H
#define thread_data_t_H
//struktura zawierająca dane, które zostaną przekazane do wątku
struct thread_data_t{
char ** command;
int connection_socket_descriptor; 
int * descriptor_array;
};
#endif