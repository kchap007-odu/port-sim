port-sim: main.o
	g++ main.o -o port-sim
	rm main.o

clean:
	rm -f port-sim *.o

main.o: main.cpp macros.h
	g++ -c main.cpp