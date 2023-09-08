CC =g++
FLAGS =-std=c++17 -O3 -Wall -Wextra -Werror

all: build run

build:
	$(CC) $(FLAGS) TimedStorage.cpp

run:
	./a.out

