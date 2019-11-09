#include "room.h"
class room{
    void set_room_name(char* room_name);
    {
        this.room_name=room_name;
    }
    void remove_user(int socket_descriptor){
        for (int i=0;i<sizeof(this.user_list)/sizeof(user);i++){
            if(this.user_list[i] && this.user_list[i].socket_descriptor_id()==socket_descriptor)
                {
                    this.user_list[i]=NULL;
                }
        }
    }
    void add_user(user user)
    {
        for (int i=0;i<sizeof(this.user_list)/sizeof(user);i++){
            if(!this.user_list[i])
                {
                    this.user_list[i]=user;
                }
        }
    }
    void show_users(int socket_descriptor){
       char * buffor 
       for (int i=0;i<sizeof(this.user_list)/sizeof(user);i++){
            if(this.user_list[i])
                {
                    buffor = new char [BUFF_SIZE];
                    strcpy(buffor,this.user_list[i].get_name());
                    strcat(buffor,"@");
                    strcat(buffor,this.user_list[i].get_color());
                    write(socket_descriptor,buffor,strlen(buffor));
                    delete(buffor);

                }
        } 
    }
    room(char * room_name,user user);
    {
        this.room_name=room_name;
        this.user_list = new user [MAX_USERS_CONNECTED_TO_CHANNEL];
        for (int i=0;i<sizeof(this.user_list)/sizeof(user);i++){
            this.user_list[i]=NULL;
        }
        add_user(user);
    }
}