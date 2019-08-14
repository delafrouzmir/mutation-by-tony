CC	=	oshcc
STRICT	=	-Wall -Wextra -pedantic
DEBUG	=	-ggdb
OPT	=	-O3
CFLAGS	=	$(STRICT) $(DEBUG) $(OPT)
LD	=	$(CC)
LDFLAGS	=	$(DEBUG) $(OPT)
LIB	=

SOURCES =	$(wildcard *.c)
OBJECTS =	$(SOURCES:.c=.o)

.PHONY:	all	tidy	clean

all:	mut

mut:	$(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

tidy:
	rm -f $(OBJECTS)

clean:	tidy
	rm -f mut
