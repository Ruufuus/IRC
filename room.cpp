    #include "room.h"

    void room::set_room_name(char* room_name)
    {
        this->room_name=room_name;
    }
    void room::remove_user(int socket_descriptor){
        for (int i=0;i<MAX_ROOMS;i++){
            if(this->user_list[i].get_socket_description_id()==socket_descriptor)
                {
                    this->user_list[i].set_socket_description(-1);
                }
        }
    }
    void room::add_user(user user){
        for (int i=0;i<MAX_ROOMS;i++){
            if(this->user_list[i].get_socket_description_id()==-1)
                {
                    this->user_list[i]=user;
                }
        }
    }
    void room::show_users(int socket_descriptor){
       char * buffor; 
       for (int i=0;i<MAX_ROOMS;i++){
            if(this->user_list[i].get_socket_description_id()!=-1)
                {
                    buffor = new char [BUFF_SIZE];
                    strcpy(buffor,this->user_list[i].get_username());
                    strcat(buffor,"@");
                    strcat(buffor,this->user_list[i].get_color());
                    write(socket_descriptor,buffor,strlen(buffor));
                    delete(buffor);

                }
        } 
    }
    int room::get_user_sd(int index){
        return this->user_list[index].get_socket_description_id();
    }
    room::room(char * room_name){
        this->room_name=room_name;
        this->user_list = new user [MAX_USERS_CONNECTED_TO_CHANNEL];
    }
    room::room(){
        strcpy(this->room_name,"default_name");
        this->user_list = new user [MAX_USERS_CONNECTED_TO_CHANNEL];
    };
    room::~room()
    {
        delete [] this->user_list;
    }