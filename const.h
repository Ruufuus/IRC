#define SERVER_PORT 2222
#define QUEUE_SIZE 5
#define BUFF_SIZE 1024
#define COMMAND_ARRAY_SIZE 3
#define MAX_USERS_CONNECTED_TO_CHANNEL 20
#define MAX_ROOMS 5

#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <cstring>
#include "thread_data_t.h"