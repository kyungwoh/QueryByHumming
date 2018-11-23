CC = gcc
CFLAGS = -W -Wall -g 
TARGET = qbh
TARGET1 = humming

all : $(TARGET) $(TARGET1)

$(TARGET) : main.o extraction.o editdistance.o notediff.o comparator.o histogram.o udrdiff.o
	$(CC) $(CFLAGS) -lm -o qbh main.o extraction.o editdistance.o notediff.o comparator.o histogram.o udrdiff.o

$(TARGET1) : humming.o
	$(CC) $(CFLAGS) -o humming humming.o

humming.o :
	$(CC) $(CFLAGS) -c humming.c

main.o :
	$(CC) $(CFLAGS) -c main.c

extraction.o :
	$(CC) $(CFLAGS) -c extraction.c

editdistance.o :
	$(CC) $(CFLAGS) -c editdistance.c

notediff.o :
	$(CC) $(CFLAGS) -c notediff.c

comparator.o :
	$(CC) $(CFLAGS) -c comparator.c

histogram.o :
	$(CC) $(CFLAGS) -c histogram.c

udrdiff.o:
	$(CC) $(CFLAGS) -c udrdiff.c

clean :
	rm -rf qbh; rm -rf *.o
	rm -rf humming
