CC = g++
SRC_S = serial_main.cpp
OUT_S = serial

serial:
	$(CC) $(SRC_S) -o $(OUT_S)
