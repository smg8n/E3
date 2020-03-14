CC = gcc 
CFLAGS = -g -w
TARGET = master
TARGET1 = bin_adder
OBJS = master.o
OBJS1 = bin_adder.o
LOG = adder_log
.SUFFIXES: .c .o

all : $(TARGET) $(TARGET1)

$(TARGET1): $(OBJS1)
	$(CC) -o $@ $(OBJS1) -lpthread

$(TARGET): $(OBJS)
	$(CC)-o $@ $(OBJS) -lpthread -lm

genRandomData: $(genRandomData.o)

	$(CC)-o $@ genRandomData.o
.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o *.log *~ *.txt $(LIB) $(TARGET) $(TARGET1) $(LOG) 

