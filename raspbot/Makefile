CC = g++
OPENCV = $(shell pkg-config --cflags --libs opencv4)
WIRINGPI = -lwiringPi
PTHREAD = -lpthread

DEPS = main.h qr.h client.h
OBJ = main.o motor.o client.o qr.o

all: raspbot

main.o: main.c $(DEPS)
	$(CC) -c -o $@ $< $(WIRINGPI) $(PTHREAD)

motor.o: motor.c $(DEPS)
	$(CC) -c -o $@ $< $(WIRINGPI)

client.o: client.c $(DEPS)
	$(CC) -c -o $@ $< $(WIRINGPI)

qr.o: qr.cpp $(DEPS)
	$(CC) -c -o $@ $< $(OPENCV)

raspbot: $(OBJ)
	$(CC) -O2 -o $@ $^ $(OPENCV) $(WIRINGPI) $(PTHREAD)

clean: 
	rm -f *.o raspbot