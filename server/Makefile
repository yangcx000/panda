
PRJ_DIR=/home/yangchunxin/panda
INCLUDES=-I$(PRJ_DIR)/server -I$(PRJ_DIR)/include -I$(PRJ_DIR)/libs
LIBS=-levent
CXX=g++
CXXFLAGS=-g -Wall -O2 -std=c++0x -c $(INCLUDES)
SOURCES=$(wildcard $(PRJ_DIR)/*/*.cpp)
OBJS=$(SOURCES:.cpp=.o)
EXEC=panda

.PHONY: clean

$(EXEC): $(OBJS)
	$(CXX) -o $@ $< $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< 

%.o: %.cc
	$(CXX) $(CXXFLAGS) -o $@ $< 

clean:
	rm -rf *.o
	rm -rf $(EXEC)
