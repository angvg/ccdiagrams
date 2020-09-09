EXECDIR = app
SOURCESDIR = modules
OBJDIR = bin
DOTDIR = dot
PDFDIR = diagrams

MAINS = creatediagrams.cpp demo.cpp

SOURCESCXX = $(wildcard $(SOURCESDIR)/*.cpp)
SOURCESCXX := $(filter-out $(addprefix $(SOURCESDIR)/, $(MAINS)), $(SOURCESCXX))

OBJECTS = $(SOURCESCXX:.cpp=.o)
OBJECTS := $(filter-out $(MAINS), $(OBJECTS))
OBJECTS := $(patsubst $(SOURCESDIR)/%.cpp, $(OBJDIR)/%.o, $(filter-out $(MAINS), $(OBJECTS)))

DOTFILES = $(wildcard $(DOTDIR)/*.dot)
PDFFILES = $(DOTFILES:.dot=.pdf)
PDFFILES := $(subst $(DOTDIR),$(PDFDIR),$(PDFFILES))

CXX = g++
CXXFLAGS = -g -Wall -Werror -std=c++2a -I.

.PHONY: clean remrender remdot

all: main demo 

demorun: demo | remrender remdot
	$(EXECDIR)/demo

main: $(OBJDIR)/creatediagrams.o $(OBJECTS) | $(EXECDIR) $(DOTDIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $< -o $(EXECDIR)/ccdg

demo: $(OBJDIR)/demo.o $(OBJECTS) | $(EXECDIR) $(DOTDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) $< -o $(EXECDIR)/$@

#$(OBJECTS): $(SOURCESCXX) | $(OBJDIR)
#	$(CXX) $(CXXFLAGS) -c $< -o $@
$(OBJDIR)/%.o: $(SOURCESDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

render: $(PDFFILES)

$(OBJDIR):
	mkdir $(OBJDIR)

$(DOTDIR):
	mkdir $(DOTDIR)

$(PDFDIR)/%.pdf: $(DOTDIR)/%.dot | $(PDFDIR)
	dot $< -Tpdf -o $@

$(PDFDIR):
	mkdir $(PDFDIR)

$(EXECDIR):
	mkdir $(EXECDIR)

clean:
	rm -f $(OBJDIR)/* $(EXECDIR)/* $(DOTDIR)/* $(PDFDIR)/*

remrender:
	rm -f $(PDFDIR)/*

remdot:
	rm -f $(DOTDIR)/*
