final: main.o Vector.o
	g++ main.o Vector.o -o final

main.o: main.cpp Vector.h
	g++ -c main.cpp -o main.o

Vector.o: Vector.cpp Vector.h
	g++ -c Vector.cpp -o Vector.o

clean:
	rm Vector.o main.o
