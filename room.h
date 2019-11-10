#ifndef ROOM_H
#define ROOM_H
#include "const.h"
#include "user.h"
class room{
    private:
        std::string room_name;
        user * user_list;
        bool room_alive;
    public:
        void set_room_name(std::string room_name);
        void remove_user(int socket_descriptor);
        void add_user(user user);
        void show_users(int socket_descriptor);
        user * get_user_list();
        std::string get_room_name();
        int get_user_sd(int index);
        room();
        room(std::string room_name);
        ~room();
};

#endif