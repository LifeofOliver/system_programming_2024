#ifndef MOTOR_H
#define MOTOR_H

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <float.h>

#define RIGHT_sensor1 0
#define RIGHT_sensor2 7
#define LEFT_sensor1 2
#define LEFT_sensor2 3

extern int fd;
extern int addr;
extern float value[5][5];
extern unsigned int Bomb;
extern float map_matrix[7][7];
extern int car_row;
extern int car_col;
extern char car_d;


void write_array(int reg, unsigned char* data, int length);
void tracking();
void turnRIGHT();
//void turnBACK();
void turnFRONT();
void turnLEFT();
void add_matrices(float matrix);
void set_bomb(int row, int col);
void set_score(float score, int row, int col);
void spread_value(float value, int row, int col);
char get_direction(int row, int col, char car_d, char loop);
void value_update();
void print_our_map();
void camera_motor(int index);

#endif
