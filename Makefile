CXX		=	oshcxx
CONFORM		=	-std=c++11
STRICT		=	-Wall -Wextra -pedantic
DEBUG		=	-ggdb
OPT		=	-O3
CXXFLAGS	=	$(CONFORM) $(STRICT) $(DEBUG) $(OPT)
LD		=	$(CXX)
LDFLAGS		=	$(DEBUG) $(OPT)
LIBS		=

SOURCES		=	$(wildcard *.cc)
OBJECTS		=	$(SOURCES:.cc=.o)
TARGET		=	mut

.PHONY:	all	tidy	clean

all:	$(TARGET)

$(TARGET):	$(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

tidy:
	rm -f $(OBJECTS)

clean:	tidy
	rm -f $(TARGET)
