EXECDIR = app
SOURCESCXX = $(wildcard *.cpp)

MAINS = creatediagrams.o demo.o

OBJDIR = build
OBJECTS = $(SOURCESCXX:.cpp=.o)
OBJECTS := $(addprefix $(OBJDIR)/,$(filter-out $(MAINS), $(OBJECTS)))

DOTDIR = dot
PDFDIR = diagrams
DOTFILES = $(wildcard $(DOTDIR)/*.dot)
PDFFILES = $(DOTFILES:.dot=.pdf)
PDFFILES := $(subst $(DOTDIR),$(PDFDIR),$(PDFFILES))

CXX = g++
CXXFLAGS = -g -Wall -Werror -std=c++2a -I.

.PHONY: clean remrender remdot

all: main  

demorun: demo | remrender remdot
	$(EXECDIR)/demo

main: $(OBJDIR)/creatediagrams.o $(OBJECTS) | $(EXECDIR) $(DOTDIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $< -o $(EXECDIR)/ccdg

demo: $(OBJDIR)/demo.o $(OBJECTS) | $(EXECDIR) $(DOTDIR)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) $< -o $(EXECDIR)/$@

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
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
