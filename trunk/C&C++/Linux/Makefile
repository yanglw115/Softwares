

EXE_NAME = IRLinux
INCLUDE = -I. -I$(shell pwd)/include
CFLAGS = -Wall -g $(INCLUDE)
#LIBS = -L$(shell pwd)/libs -ljson -lmsc -lpthread -lasound -lrt -ldl -lc \
	#-lm -lavcodec -lavformat -lswresample -lavutil -lSDL2 -lvlc -lvlccore
LIBS = -L$(shell pwd)/libs -ljson -lmsc -lpthread -lasound -lrt -ldl -lc \
	-lm -lavcodec -lavformat -lswresample -lavutil -lSDL2  -lvlc
OBJS = $(patsubst %.c, %.o, $(wildcard *.c))
OUTPUT_WAV = output.wav convert.wav convert1.wav

CC = gcc

all: $(OBJS) 
	$(CC) -o $(EXE_NAME) $(OBJS) $(LIBS)

c.o:
	$(CC) -c $*.c $(CFLAGS)


clean:
	rm -rf $(OBJS)
	rm -rf $(EXE_NAME)
	rm -rf $(OUTPUT_WAV)


