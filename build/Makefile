CXX = g++
CFLAGS = -std=c++17 -O2 -Wall -g

TARGET = compress
OBJS = ../code/*.cpp

all: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o ../bin/$(TARGET)

clean:
	rm -rf ../bin/$(OBJS) $(TARGET)




