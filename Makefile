EXECDIR = build
SOURCESCXX = $(wildcard *.cpp)
#SOURCESCXX := $(filter-out output.cpp, $(SOURCESCXX))
#EXECUTABLES = ${SOURCESCXX:.cpp=}
MAINS = main.o test.o
OBJECTS = ${SOURCESCXX:.cpp=.o}
OBJECTS := $(addprefix $(EXECDIR)/,$(filter-out $(MAINS), $(OBJECTS)))
FILENAME = dg

CXX = g++
CXXFLAGS = -g -I. -Wall -Werror -std=c++1z

.PHONY: clean

all: main test

runtest: clean test
	$(EXECDIR)/test

main: $(EXECDIR)/main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $< -o $(EXECDIR)/$@

test: $(EXECDIR)/test.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $< -o $(EXECDIR)/$@

$(EXECDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(addprefix $(EXECDIR)/,*.o ${MAINS:.o=})
