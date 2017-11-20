
LIBRARY_NAME = libCureFaceParser.so
INCLUDE = -I. -I/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.151-1.b12.el6_9.x86_64/include  -I/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.151-1.b12.el6_9.x86_64/include/linux
CFLAGS = -Wall $(INCLUDE)

LIBS = 
OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

CC = g++

all: $(OBJS) 
	$(CC) -shared $(CFLAGS) $(OBJS) $(LIBS) -o $(LIBRARY_NAME)

%.o: %.cpp
	$(CC) -fPIC $(CFLAGS) -c $*.cpp

clean:
	rm -rf $(OBJS)
	rm -rf $(LIBRARY_NAME)
	rm -rf *.class

java:
	javac test.java


