#define SERVER_PORT 1234
#define QUEUE_SIZE 5
#define BUFF_SIZE 1024
#define COMMAND_ARRAY_SIZE 6
#define MAX_USERS_CONNECTED_TO_CHANNEL 20
#define MAX_ROOMS 20

#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <ctime>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <netdb.h>
#include <signal.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <cstring>
#include <fcntl.h>