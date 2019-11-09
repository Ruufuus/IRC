#ifndef ROOM_H
#define ROOM_H
#include "user.h"
class room{
    private:
        char * room_name;
        user * user_list;
    public:
        void set_room_name(char* room_name);
        void remove_user(int socket_descriptor);
        void add_user(user user);
        void show_users(int socket_descriptor);
        int get_user_sd(int index);
        room();
        room(char * room_name);
        ~room();
};

#endif