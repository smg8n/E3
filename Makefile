CC = gcc 
CFLAGS = -g -w -std=c99
TARGET = master
TARGET1 = bin_adder
TARGET2 = genRandomData
OBJS = master.o
OBJS1 = bin_adder.o
OBJS2 = genRandomData.o
LOG = adder_log
.SUFFIXES: .c .o

all : $(TARGET) $(TARGET1) $(TARGET2)

$(TARGET2): $(OBJS2)
	$(CC) -o $@ $(OBJS2) 

$(TARGET1): $(OBJS1)
	$(CC) -o $@ $(OBJS1) -lpthread

$(TARGET): $(OBJS)
	$(CC)-o $@ $(OBJS) -lpthread -lm

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o *.log *~ *.txt $(LIB) $(TARGET) $(TARGET1) $(TARGET2) $(LOG) 

