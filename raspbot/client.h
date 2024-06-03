#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 2
#define MAP_ROW 5
#define MAP_COL 5

static int bomb = 4;



typedef enum {
    move,
    setBomb  
} Action;

typedef struct {
    int row;
    int col;
    Action action;
} ClientAction;

typedef enum {
    nothing,
    item,
    trap
} Status;

typedef struct {
    Status status;
    int score;
} Item;

typedef struct {
    int row;
    int col;
    Item item;
} Node;

typedef struct {
    int socket;
    struct sockaddr_in address;
    int row;
    int col;
    int score;
	int bomb;
} client_info;

typedef struct {
    client_info players[MAX_CLIENTS];
    Node map[MAP_ROW][MAP_COL];
} DGIST;

typedef struct {
	int qr_data;
	pthread_mutex_t mutex;
    Action action;
    ClientAction client;
    int sockfd;
} SharedData;

#endif
