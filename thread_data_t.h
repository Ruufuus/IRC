#ifndef thread_data_t_H
#define thread_data_t_H
#include "const.h"
#include "room.h"
//struktura zawierająca dane, które zostaną przekazane do wątku
class room;
struct thread_data_t{
char ** command;
int connection_socket_descriptor; 
std::vector<room> room_list;
int room_index;
};
#endif