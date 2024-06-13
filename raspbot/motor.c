#include "main.h"

void tracking();
void turnRIGHT();
void turnLEFT();
void turnFRONT();
void add_matrices(float matrix);
void set_bomb(int row, int col);
void set_score(float score, int row, int col);
void spread_value(float value, int row, int col);
char get_direction(int row, int col, char car_d);
void update_pos();
void camera_motor(int index);

unsigned char leftbuffer[4] = {0, 100, 1, 100};
unsigned char rightbuffer[4] = {1, 100, 0, 100};
unsigned char stopbuffer[4] = {1, 0, 1, 0 };
unsigned char semi_right[4] = {1, 90, 1, 50};
unsigned char semi_left[4] = {1, 50, 1, 90};
unsigned char frontbuffer[4] = {1, 80, 1, 80};
unsigned char backbuffer[4] = {0, 80, 0, 80};
unsigned char strong_right[4] = {1, 50, 0, 50};
unsigned char strong_left[4] = {0, 50, 1, 50};
unsigned char cam_right[2] = {1, 70};
unsigned char cam_left[2] = {1, 110};

char car_d = 'e';
char d = 'f';

int car_row = 0;
int car_col = 0;
int servo = 0x03;

int right_1 = digitalRead(RIGHT_sensor1);
int right_2 = digitalRead(RIGHT_sensor2);
int left_1 = digitalRead(LEFT_sensor1);
int left_2 = digitalRead(LEFT_sensor2);

float map_matrix[7][7] = {
        {-FLT_MAX, -FLT_MAX, -FLT_MAX,-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX},
        {-FLT_MAX,    0,    0,    0,    0,    0, -FLT_MAX},
        {-FLT_MAX,    0,    0,    0,    0,    0, -FLT_MAX},
        {-FLT_MAX,    0,    0,    0,    0,    0, -FLT_MAX},
        {-FLT_MAX,    0,    0,    0,    0,    0, -FLT_MAX},
        {-FLT_MAX,    0,    0,    0,    0,    0, -FLT_MAX},
        {-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX}
};

void write_array(int reg, unsigned char* data, int length){
	unsigned char buffer[length + 1];
	buffer[0] = reg;
	for(int i = 0; i<length; i++){
		buffer[i+1] = data[i];
	}
	write(fd, buffer, length+1);
}

void update_pos(){
	printf("update\n");
	if (car_d == 'e'){
		if (d == 'f'){
			car_d = 'e';
			car_col += 1;
		}else if(d == 'r'){
			car_d = 's';
			car_row += 1;
		}else if(d == 'l'){
			car_d = 'n';
			car_row -= 1;
		}else if(d == 'b'){
			car_d = 'w';
			car_col -= 1;
		}
	}
	else if(car_d == 'w'){
		if (d == 'f'){
			car_d = 'w';
			car_col -= 1;
		}else if(d == 'r'){
			car_d = 'n';
			car_row -= 1;
		}else if(d == 'l'){
			car_d = 's';
			car_row += 1;
		}else if(d == 'b'){
			car_d = 'e';
			car_col += 1;
		}
	}
	else if(car_d == 's'){
		if (d == 'f'){
			car_d = 's';
			car_row += 1;
		}else if(d == 'r'){
			car_d = 'w';
			car_col -= 1;
		}else if(d == 'l'){
			car_d = 'e';
			car_col += 1;
		}else if(d == 'b'){
			car_d = 'n';
			car_row -= 1;
		}
	}
	else if(car_d == 'n'){
		if (d == 'f'){
			car_d = 'n';
			car_row -= 1;
		}else if(d == 'r'){
			car_d = 'e';
			car_col += 1;
		}else if(d == 'l'){
			car_d = 'w';
			car_col -= 1;
		}else if(d == 'b'){
			car_d = 's';
			car_row += 1;
		}
	}
	return;
	
}

void turnRIGHT(){
	write_array(0x01, rightbuffer, 4);
	delay(400);
	while(1){
		right_1 = digitalRead(RIGHT_sensor1);
		right_2 = digitalRead(RIGHT_sensor2);
		left_1 = digitalRead(LEFT_sensor1);
		left_2 = digitalRead(LEFT_sensor2);
		if (left_2 == 0 &&  right_2 == 1){
			write_array(0x01, stopbuffer, 4);
			delay(50);
			break;
			};
		};
	write_array(0x01, frontbuffer, 4);
	return;
}

void turnLEFT(){
	write_array(0x01, leftbuffer, 4);
	delay(400);
	while(1){
		right_1 = digitalRead(RIGHT_sensor1);
		right_2 = digitalRead(RIGHT_sensor2);
		left_1 = digitalRead(LEFT_sensor1);
		left_2 = digitalRead(LEFT_sensor2);
		if (right_1 == 0 &&  left_1 == 1){
			write_array(0x01, stopbuffer, 4);
			delay(50);
			break;
			};
		};
	write_array(0x01, frontbuffer, 4);
	return;
}

void turnFRONT(){
	write_array(0x01, frontbuffer, 4);
	return;
}

void tracking(){
	right_1 = digitalRead(RIGHT_sensor1);
	right_2 = digitalRead(RIGHT_sensor2);
	left_1 = digitalRead(LEFT_sensor1);
	left_2 = digitalRead(LEFT_sensor2);

	// 1 0 0 1	
	if (left_1 == 1 && left_2 == 0 && right_1 == 0 && right_2 == 1){
		write_array(0x01, frontbuffer, 4);
		return;
	}
	
	//main tracking
	
	// 1 1 0 1
	if (left_1 == 1 && left_2 == 1 && right_1 == 0 && right_2 == 1){
		write_array(0x01, semi_right, 4);
		delay(30);
		return;
	}
	// 1 0 1 1
	if (left_1 == 1 && left_2 == 0 && right_1 == 1 && right_2 == 1){

		write_array(0x01, semi_left, 4);
		delay(30);
		return;		
	}
	
	
	// sub tracking
	// 1 1 1 0
	if (left_1 == 1 && left_2 == 1 && right_1 == 1 && right_2 == 0){

		write_array(0x01, strong_right, 4);
		delay(50);
		return;

	}
	// 0 1 1 1
	if (left_1 == 0 && left_2 == 1 && right_1 == 1 && right_2 == 1){

		write_array(0x01, strong_left, 4);
		delay(50);
		return;
	}
	
	
	// 0 0 0 0 or 0 0 0 1 or 1 0 0 0
	if ((left_1 == 0 && left_2 == 0 && right_1 == 0 && right_2 == 0) || (left_1 == 0 && left_2 == 0 && right_1 == 0 && right_2 == 1) || (left_1 == 1 && left_2 == 0 && right_1 == 0 && right_2 == 0)){
		write_array(0x01, frontbuffer, 4);
		delay(130);
		write_array(0x01, stopbuffer, 4);
		delay(100);
		
		update_pos();
		
		d = get_direction(car_row, car_col, car_d);
		
		printf("car_direction is %c\n",car_d);
		printf("direction is %c\n",d);
		printf("position is %d, %d\n",car_row, car_col);
		
		if (d == 'r'){
			turnRIGHT();
			return;
		}else if(d == 'l'){
			turnLEFT();
			return;
		}else{
			turnFRONT();
			return;
		}
		
	}

}

void add_matrices(float matrix[5][5]) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            map_matrix[i+1][j+1] = map_matrix[i+1][j+1] + matrix[i][j];
        }
    }
};

void value_update(){
	for(int i = 1; i < 6; i++){
		for(int j = 1; j < 6; j++){
			map_matrix[i][j] = 0;
		}
	}
	for (int r = 0; r < 5; r++){
		for (int c = 0; c < 5; c++){
			float v = value[r][c];
			if (v == -10){
				set_bomb(r+1,c+1);
				continue;
			}else if (v == 0){
				continue;
			}else{
				spread_value(v, r, c);
			}
		}
	}
	return;
}


void spread_value(float value, int row, int col) {
    float sum_mat[5][5] = { 0 };

    sum_mat[row][col] = value;

    for (int i = 1; i < 5; i++) {
        for (int r = row - i; r <= row + i; r++) {
            for (int c = col - i; c <= col + i; c++) {
                if (r >= 0 && r < 5 && c >= 0 && c < 5) {
                    float distance = abs(r - row) + abs(c - col);
                    sum_mat[r][c] = sum_mat[row][col] * pow(0.01, distance);
                }
            }
        }
    }
    add_matrices(sum_mat);
    return;
}

void set_score(float score, int row, int col) {
    spread_value(score, row, col);
};

void set_bomb(int row, int col) {
    float bomb = -10;
    map_matrix[row][col] = bomb;
};

char get_direction(int row, int col, char car_d) {
    char direction = car_d;
    int index = 0;
    float com_q = -100;

    float f_q = 0; //Front Q
    float r_q = 0; //Right Q
    float l_q = 0; //Left Q
    float b_q = 0; //Back Q

    if (car_d == 'e') {
        f_q = map_matrix[row + 1][col + 2];
        r_q = map_matrix[row + 2][col + 1];
        l_q = map_matrix[row][col + 1];
        b_q = map_matrix[row + 1][col];
    }
    else if (car_d == 'w') {
        f_q = map_matrix[row + 1][col];
        r_q = map_matrix[row][col + 1];
        l_q = map_matrix[row + 2][col + 1];
        b_q = map_matrix[row + 1][col + 2];
    }
    else if (car_d == 's') {
        f_q = map_matrix[row + 2][col + 1];
        r_q = map_matrix[row + 1][col];
        l_q = map_matrix[row + 1][col + 2];
        b_q = map_matrix[row][col + 1];
    }
    else if (car_d == 'n') {
        f_q = map_matrix[row][col + 1];
        r_q = map_matrix[row + 1][col + 2];
        l_q = map_matrix[row + 1][col];
        b_q = map_matrix[row + 2][col + 1];
    };


    float q_l[3] = {f_q, r_q, l_q};

    for (int i = 0; i < 3; i++) {
        if (q_l[i] > com_q) {
            com_q = q_l[i];
            index = i;
        };
    };

    if (index == 0) {
        direction = 'f';
    }
    else if (index == 1) {
        direction = 'r';
    }
    else if (index == 2) {
        direction = 'l';
    }
    return direction;
}

void print_our_map(){
	for(int i = 0; i < 7; i++){
		for(int j = 0; j < 7; j++){
			if(map_matrix[i][j] == -FLT_MAX){
				printf("- ");
			}
			else{
				printf("%0.2f ", map_matrix[i][j]);
			}
		}
		printf("\n");
	}
}
