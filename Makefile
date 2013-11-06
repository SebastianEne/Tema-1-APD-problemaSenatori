CC = g++
SRC_S = serial_main.cpp
OUT_S = serial

build:
	$(CC) $(SRC_S) -o $(OUT_S)
clean:
	rm -f serial
