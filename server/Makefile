
CXX=g++
CXXFLAGS= -g -Wall -O2 -std=c++0x
LIBS= -levent
DEPS= panda.h
SOURCES=$(wildcard *.cpp)
OBJS=$(SOURCES:.cpp=.o)
EXEC=panda

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS) $(LIBS)

$(EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf $(EXEC)
