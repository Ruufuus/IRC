#ifndef ROOM_H
#define ROOM_H
#include "const.h"
#include "user.h"
#include "message_handling.h"
class room{
    private:
        pthread_mutex_t room_user_list_mutex;
        pthread_mutex_t room_sending_mutex;
        std::string room_name;
        user * user_list;
        bool room_alive;
    public:
        void change_username(int socket_descriptor,char * username);
        void change_color(int socket_descriptor,char * username);
        void send_to_everyone(char * buffor);
        void lock_user_list_mutex();
        void unlock_user_list_mutex();
        void set_room_name(std::string room_name);
        void remove_user(int socket_descriptor);
        bool add_user(user user);
        void sending_mutex_unlock();
        void sending_mutex_lock();
        char * get_user_list();
        bool get_if_alive();
        user get_user(int socket_descriptor);
        std::string get_room_name();
        int get_user_sd(int index);
        room();
        ~room();
};

#endif