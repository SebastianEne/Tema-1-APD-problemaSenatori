CC = g++
SRC_S = serial_main.cpp
OUT_S = serial

build:
	g++ serial_main.cpp -o serial
	g++ paralel_main.cpp -o o paralel

optim:
	g++ o_serial_main.cpp -o o_serial
clean:
	rm -f serial
