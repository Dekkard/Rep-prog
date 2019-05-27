all: exe

exe: comp
	./regexin $(arq)

comp: regexin.o
	g++ -o regexin regexin.o

regexin.o: regexin.cpp
	g++ -std=c++11 -c -g regexin.cpp

clean:
	rm -rf *.o *~ regexin
