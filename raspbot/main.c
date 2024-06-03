#include "main.h"
#include "qr.h"
#include "client.h"
#include <signal.h>
#include <time.h>
int fd;
int addr = 0x16;
unsigned char stop[1] = {0x00}; 
char direction = 'r';
float value[5][5] = {0};
unsigned int Bomb = 4;
const char* SERVER_IP;
int SERVER_PORT;

void *qr_code_reader(void *arg){
	SharedData *shared_data = (SharedData*)arg;
	int xy = 0;
	while(1){

		xy = capture();
		if(xy == -1){
			printf("There is an error\n");
			continue;
		}
		
		pthread_mutex_lock(&shared_data->mutex);
		shared_data->qr_data= xy;
		shared_data->client.row = xy / 10;
		shared_data->client.col = xy % 10;
		
		pthread_mutex_unlock(&shared_data->mutex);	
		printf("position x y: %d\n",shared_data->qr_data);
		

	}
	return NULL;
}

void* ServerCommunication(void* arg) {
    SharedData* data = (SharedData*)arg;
    Item tmpItem;
    int socket_file_disc = 0;
    DGIST game_state;
    ClientAction action;
    
    socket_file_disc = data->sockfd;
    
    while (1) {
	if (recv(socket_file_disc, &game_state, sizeof(game_state), 0) == 0) {
            perror("Error receiving data from server");
            break;
	}
        printf("Received game state from server.\n");
	
	
	
	for(int i = 0; i < MAP_ROW; i++){
		for(int j = 0; j < MAP_COL; j++){
			tmpItem = game_state.map[i][j].item;
			switch(tmpItem.status){
				case nothing:
					value[i][j] = 0;
					break;
				case item:
					value[i][j] = tmpItem.score;
					break;
				case trap:
					value[i][j] = -10;
					break;
				}
			}
		}
	pthread_mutex_lock(&data->mutex);
	value_update();
	print_our_map();
	
	int rows = data->client.row;
	int cols = data->client.col;
	
	if(bomb > 0){ 
		data->client.action = setBomb;
		bomb -= 1;
	}
	else{
		data->client.action = move;
	}
        pthread_mutex_unlock(&data->mutex);
    }

    return NULL;
}

void* MotorThread(void* arg){
    SharedData* data = (SharedData*)arg;

    int socket_file_disc = 0;
    socket_file_disc = data->sockfd;

    while (1) {
	pthread_mutex_lock(&data->mutex);
        if (send(socket_file_disc, &(data->client), sizeof(data->client), 0) < 0) {
            perror("Error sending data to server");
            break;
        }
        printf("Sent action (%d, %d, %d) to server\n", data->client.row, data->client.col, data->client.action);	    
        pthread_mutex_unlock(&data->mutex);
	delay(650);
	}
	return NULL;
}

void handler(int sig){
	write_array(0x02, stop ,1);
	exit(0);
}
int main(int argc, char *argv[]){
	if(argc != 6) {
		printf("Usage: %s <server IP> <Server Port> <row> <col> <dir>\n",argv[0]);
		exit(1);
	}
		
	SERVER_IP = argv[1];
	SERVER_PORT = atoi(argv[2]);
	car_row = atoi(argv[3]);
	car_col = atoi(argv[4]);
	car_d = (*argv[5]);
	
	signal(SIGINT, handler);
	
	pthread_t qr_thread_id, client_thread_id, motor_thread_id;
	

	
	//shared data
	SharedData shared_position;
	shared_position.qr_data = 0;
	
	pthread_mutex_init(&shared_position.mutex, NULL);	
	shared_position.client.row = car_row;
	shared_position.client.col = car_col;
	shared_position.client.action = move;
	shared_position.sockfd = 0;
	
	//socket address
	struct sockaddr_in serv_addr;
	
	delay(3000);
	
	//wiringPiSetup()
	wiringPiSetup();
	
	fd = wiringPiI2CSetup(addr);
	
	pinMode(RIGHT_sensor1, INPUT);
	pinMode(RIGHT_sensor2, INPUT);
	pinMode(LEFT_sensor1, INPUT);
	pinMode(LEFT_sensor2, INPUT);
	
	    
    
    
    
    int sock_file_disc;
    sock_file_disc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_file_disc < 0) {
	perror("Error opening socket");
	exit(1);
	}

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sock_file_disc, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
	perror("Error connecting to the server");
	exit(1);
	}

    printf("Connected to the server at %s:%d\n", SERVER_IP, SERVER_PORT);
    
    shared_position.sockfd = sock_file_disc;


	if(pthread_create(&qr_thread_id, NULL, qr_code_reader, (void*)&shared_position) != 0){
		printf("error creating qr code reader thread\n");
		return 1;
	}
	
	if(pthread_create(&client_thread_id, NULL, ServerCommunication, (void*)&shared_position) != 0){
		printf("error creating servercommunication thread \n");
		return 1;
	}
	
	if(pthread_create(&motor_thread_id, NULL, MotorThread, (void*)&shared_position) != 0){
		printf("\n");
		return 1;
	}
	while(1) {
		tracking();
		delay(10);
	}
	
	
	close(sock_file_disc);
	
	pthread_join(qr_thread_id, NULL);
	pthread_join(client_thread_id, NULL);
	pthread_join(motor_thread_id, NULL);
	pthread_mutex_destroy(&shared_position.mutex);
	
	return 0;		
}
