all:
	g++ `pkg-config --cflags gtk+-3.0` -o Main -Iinclude/ -I../Include/ *.cpp `pkg-config --libs gtk+-3.0` -std=c++11 -rdynamic
	./Main
