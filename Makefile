
CXX     = g++
CXXFLAGS = -g -Wall -O0 -std=c++11

HW04:	main.o hex_game.o	
	${CXX} -o HW04  ${CXXFLAGS} main.cpp hex_game.cpp

clean:	
	rm -f *o
