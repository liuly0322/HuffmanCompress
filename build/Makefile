CXX = g++
CFLAGS = -std=c++17 -O2 -Wall -g

TARGET = compress
OBJS = ../code/io/*.cpp ../code/adt/*.cpp \
       ../code/main.cpp

all: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o ../bin/$(TARGET)

clean:
	rm -rf ../bin/$(OBJS) $(TARGET)




