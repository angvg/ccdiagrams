SOURCESCXX = $(wildcard *.cpp)
#SOURCESCXX := $(filter-out output.cpp, $(SOURCESCXX))
#EXECUTABLES = ${SOURCESCXX:.cpp=}
MAINS = main.o test.o
OBJECTS = ${SOURCESCXX:.cpp=.o}
OBJECTS := $(filter-out $(MAINS), $(OBJECTS))
FILENAME = dg

CXX = g++
CXXFLAGS = -g -I. -Wall -Werror -std=c++1z

.PHONY: clean

all: main test

main: main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $< -o $@

test: test.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o ${MAINS:.o=}
