    #include "room.h"

    void room::set_room_name(std::string room_name)
    {
        this->room_name=room_name;
    };
    void room::remove_user(int socket_descriptor){
        pthread_mutex_lock(&(this->room_user_list_mutex));
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
        pthread_mutex_unlock(&(this->room_user_list_mutex));
    };
    bool room::get_if_alive(){
        return this->room_alive;
    };
    bool room::add_user(user user){
        pthread_mutex_lock(&(this->room_user_list_mutex));
        bool add_result = false;
        for (int i=0;i<MAX_USERS_CONNECTED_TO_CHANNEL;i++){
            if(this->user_list[i].get_socket_description_id()==-1)
                {
                    this->user_list[i]=user;
                    this->room_alive=true;
                    add_result = true;
                    break;
                }
        }
        pthread_mutex_unlock(&(this->room_user_list_mutex));
        return add_result;
    };
    void room::send_user_list(int socket_descriptor){
       char *  buffor = new char [BUFF_SIZE];
       strcpy(buffor,"$user_list ");
       pthread_mutex_lock(&(this->room_user_list_mutex));
       for (int i=0;i<MAX_USERS_CONNECTED_TO_CHANNEL;i++){
            if(this->user_list[i].get_socket_description_id()!=-1)
                {
                   
                    strcat(buffor,this->user_list[i].get_username().c_str());
                    strcat(buffor,"@");
                    strcat(buffor,this->user_list[i].get_color().c_str());
                    strcat(buffor," ");
                    

                }
        } 
        pthread_mutex_unlock(&(this->room_user_list_mutex));
        strcat(buffor,"\n");
        sending_message(socket_descriptor,buffor);
        delete(buffor);
    };
    int room::get_user_sd(int index){
        pthread_mutex_lock(&(this->room_user_list_mutex));
        int temp=this->user_list[index].get_socket_description_id();
        pthread_mutex_unlock(&(this->room_user_list_mutex));
        return temp;
    };
    user room::get_user(int socket_descriptor){
        pthread_mutex_lock(&(this->room_user_list_mutex));
        for(int i=0;i<MAX_USERS_CONNECTED_TO_CHANNEL;i++){
            if(this->user_list[i].get_socket_description_id()==socket_descriptor)
            {
                pthread_mutex_unlock(&(this->room_user_list_mutex));
                return this->user_list[i];
            }
        }
        pthread_mutex_unlock(&(this->room_user_list_mutex));
        printf("nie znaleziono usera\n");
        user empty_user;
        return empty_user;
    };
    room::room(){
        int mutex_sending_init_result = pthread_mutex_init(&(this->room_sending_mutex),NULL);
        if (mutex_sending_init_result < 0)
        {
            fprintf(stderr, "Błąd przy próbie zainicjalizowania mutexa");
            exit(1);
        }
        int mutex_init_result = pthread_mutex_init(&(this->room_user_list_mutex),NULL);
        if (mutex_init_result < 0)
        {
            fprintf(stderr, "Błąd przy próbie zainicjalizowania mutexa");
            exit(1);
        }
        this->room_name="Default Room";
        this->user_list = new user[MAX_USERS_CONNECTED_TO_CHANNEL];
        this->room_alive=false;
    };
    void room::send_to_everyone(char * buffor){
        pthread_mutex_lock(&(this->room_sending_mutex));
        for(int i=0;i<MAX_USERS_CONNECTED_TO_CHANNEL;i++)
        {
            if(this->get_user_sd(i)!=-1){
                sending_message(this->get_user_sd(i),buffor);
            }
        }
        pthread_mutex_unlock(&(this->room_sending_mutex));
    };
    void room::sending_mutex_unlock(){
        pthread_mutex_unlock(&(this->room_sending_mutex));
    };
    void room::sending_mutex_lock(){
        pthread_mutex_lock(&(this->room_sending_mutex));
    };
    std::string room::get_room_name()
    {
        return this->room_name;
    };
    void room::lock_user_list_mutex(){
        pthread_mutex_lock(&(this->room_user_list_mutex));
    };
    void room::unlock_user_list_mutex(){
        pthread_mutex_unlock(&(this->room_user_list_mutex));
    };
    room::~room()
    {
        pthread_mutex_destroy(&(this->room_sending_mutex));
        pthread_mutex_destroy(&(this->room_user_list_mutex));
        delete [] this->user_list;
    };