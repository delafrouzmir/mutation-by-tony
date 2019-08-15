CXX		=	oshcxx
CONFORM		=	-std=c++11
STRICT		=	-Wall -Wextra -pedantic
DEBUG		=	-ggdb
OPT		=	-O3
CXXFLAGS	=	$(CONFORM) $(STRICT) $(DEBUG) $(OPT)
LD		=	$(CXX)
LDFLAGS		=	$(DEBUG) $(OPT)
LIB		=

SOURCES		=	$(wildcard *.cc)
OBJECTS		=	$(SOURCES:.cc=.o)

.PHONY:	all	tidy	clean

all:	mut

mut:	$(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

tidy:
	rm -f $(OBJECTS)

clean:	tidy
	rm -f mut
