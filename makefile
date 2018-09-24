# change application name here (executable output name)
TARGET=proyecto
TARGET_pending=pending
TARGET_floyd=floyd
TARGET_knapsack=knapsack
TARGET_deportivas=deportivas
 
# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall
 
PTHREAD=-pthread
 
CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe
 
GTKLIB=`pkg-config --cflags --libs gtk+-3.0`
 
# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic
 
OBJS = main.o
 
all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)	
	$(CC) -o $(TARGET_pending) src/$(TARGET_pending).c $(LDFLAGS)	
	$(CC) -o $(TARGET_floyd) src/$(TARGET_floyd).c $(LDFLAGS)
	$(CC) -o $(TARGET_knapsack) src/$(TARGET_knapsack).c $(LDFLAGS)
	$(CC) -o $(TARGET_deportivas) src/$(TARGET_deportivas).c $(LDFLAGS)
    
main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o
    
clean:
	rm -f *.o $(TARGET)	$(TARGET_pending) $(TARGET_floyd) $(TARGET_knapsack) $(TARGET_deportivas)

