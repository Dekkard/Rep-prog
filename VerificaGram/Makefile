all: exe

exe: comp
	./verify $(a)

comp: verify.o
	g++ -o verify verify.o
verify.o: verify.cpp
	g++ -std=c++11 -c -g verify.cpp

clean:
	rm -rf *.o *~ verify
