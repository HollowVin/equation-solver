CC = g++
CCFLAGS = -c
SRCS = src/
OBJS = obj/
BINS = bin/

solv-eq: equation.o solv-eq.o 
	$(CC) $(OBJS)equation.o $(OBJS)solv-eq.o -o $(BINS)solv-eq

equation.o: $(SRCS)equation.cpp 
	$(CC) $(SRCS)equation.cpp -o $(OBJS)equation.o $(CCFLAGS)

solv-eq.o: $(SRCS)solv-eq.cpp 
	$(CC) $(SRCS)solv-eq.cpp -o $(OBJS)solv-eq.o $(CCFLAGS)

.PHONY: clean

clean: 
	rm -f $(OBJS)*.o $(BINS)solv-eq

$(shell mkdir -p $(BINS))
$(shell mkdir -p $(OBJS))