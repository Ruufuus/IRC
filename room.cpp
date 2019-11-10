    #include "room.h"

    void room::set_room_name(std::string room_name)
    {
        this->room_name=room_name;
    };
    void room::remove_user(int socket_descriptor){
        for (int i=0;i<MAX_USERS_CONNECTED_TO_CHANNEL;i++){
            if(this->user_list[i].get_socket_description_id()==socket_descriptor)
                {
                    this->user_list[i].set_socket_description(-1);
                    break;
                }
        }
         for (int i=0;i<MAX_USERS_CONNECTED_TO_CHANNEL;i++){
             this->room_alive=false;
            if(this->user_list[i].get_socket_description_id()>0)
                {
                    this->room_alive=true;
                    break;
                }
        }
    };
    bool room::get_if_alive(){
        return this->room_alive;
    };
    user * room::get_user_list(){
        return this->user_list;
    };
    void room::add_user(user user){
        for (int i=0;i<MAX_USERS_CONNECTED_TO_CHANNEL;i++){
            if(this->user_list[i].get_socket_description_id()==-1)
                {
                    this->user_list[i]=user;
                    break;
                }
        }
        this->room_alive=true;
    };
    void room::send_user_list(int socket_descriptor){
       char * buffor; 
       for (int i=0;i<MAX_USERS_CONNECTED_TO_CHANNEL;i++){
            if(this->user_list[i].get_socket_description_id()!=-1)
                {
                    buffor = new char [BUFF_SIZE];
                    strcpy(buffor,this->user_list[i].get_username().c_str());
                    strcat(buffor,"@");
                    strcat(buffor,this->user_list[i].get_color().c_str());
                    strcat(buffor," ");
                    sending_message(socket_descriptor,buffor);
                    delete(buffor);

                }
        } 
    };
    int room::get_user_sd(int index){
        return this->user_list[index].get_socket_description_id();
    };
    room::room(std::string room_name){
        this->room_name=room_name;
        this->user_list = new user[MAX_USERS_CONNECTED_TO_CHANNEL];
        this->room_alive=true;
    };
    user room::get_user(int socket_descriptor){
        for(int i=0;i<MAX_USERS_CONNECTED_TO_CHANNEL;i++){
            if(this->user_list[i].get_socket_description_id()==socket_descriptor)
            {
                return this->user_list[i];
            }
        }
        user empty_user;
        return empty_user;
    };
    room::room(){
        this->room_name="Default Room";
        this->user_list = new user[MAX_USERS_CONNECTED_TO_CHANNEL];
        this->room_alive=false;
    };
    std::string room::get_room_name()
    {
        return this->room_name;
    };
    room::~room()
    {
        delete [] this->user_list;
    };