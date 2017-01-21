#compiler
CC=g++
CFLAGS=-std=c++11 -Wall -Wno-unused-result `pkg-config --cflags gtk+-3.0`
INCLUDE=-I./pointvec/ -I./cairo_drawing/ -I./user_interface/
LIBS=`pkg-config --libs gtk+-3.0`

#files
SRC=./cairo_drawing/draw.c ./pointvec/c_figure_maker.cpp ./user_interface/main_UI.c
MAIN1=main.c
BIN1=prog

#tools
RM=rm -vf


all:
	$(CC) $(MAIN1) $(SRC) -o $(BIN1) $(INCLUDE) $(CFLAGS) $(LIBS)

clean:
	find -type f -name "*~" -execdir $(RM) '{}' \;
