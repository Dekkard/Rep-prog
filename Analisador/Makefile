all: comp

exe: 
	./regexin

comp: regexin
	./regexin

regexin: regexin.o
	g++ regexin.o -o regexin

regexin.o: regexin.cpp
	g++ -std=c++11 -c regexin.cpp

clean:
	rm -rf *.o *~ regexin
