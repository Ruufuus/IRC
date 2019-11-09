#include "room.h"
class room{
    void set_room_name(char* room_name);
    void remove_user(int socket_descriptor);
    void add_user(user user);
    void show_users(int socket_descriptor);
    room(char * room_name,user user);
}