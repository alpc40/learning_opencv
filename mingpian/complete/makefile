path=-I /usr/local/include/opencv2 -L /usr/local/lib -lm -lopencv_core -lopencv_highgui

all:complete

complete:complete.o proc.o proc.hpp
	g++ -o complete complete.o proc.o $(path)
complete.o:complete.cpp proc.hpp
	g++ -c complete.cpp $(path)
proc.o:proc.cpp proc.hpp
	g++ -c proc.cpp $(path)
clean:
	rm *.o
