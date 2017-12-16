CC = g++
CCFLAGS = -c

solv-eq: equation.o solv-eq.o 
	$(CC) equation.o solv-eq.o -o solv-eq

equation.o: equation.cpp 
	$(CC) equation.cpp -o equation.o $(CCFLAGS)

solv-eq.o: solv-eq.cpp 
	$(CC) solv-eq.cpp -o solv-eq.o $(CCFLAGS)

.PHONY: clean

clean: 
	rm *.o solv-eq