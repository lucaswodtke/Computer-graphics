all:
	g++ -o Main -Iinclude/ -I../Include/ *.cpp -std=c++11 -rdynamic `pkg-config --libs gtk+-3.0 --cflags gtk+-3.0`
	./Main
	