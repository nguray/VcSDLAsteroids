CC = gcc
LDFLAGS = $(shell sdl2-config --libs | sed 's/ -I/ -isystem /g' ) -lm
CFLAGS = $(shell sdl2-config --cflags | sed 's/ -I/ -isystem /g') -std=c11

PROGRAM = asteroids
SRC := main.c 

.PHONY : debug release clean_debug clean_release gen_resources res obj
OBJDIR = obj
# Create Objects list from SRCs list
OBJ := $(SRC:.c=.o)
# Add OBJDIR path to OBJs list
OBJS = $(addprefix $(OBJDIR)/,$(OBJ))

all: debug release

debug: CFLAGS += -g 
debug: debug/$(PROGRAM)
release: release/$(PROGRAM)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

release/$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o release/$(PROGRAM)

debug/$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o debug/$(PROGRAM)

clean:
	rm -f *~
	rm -f *.o
	rm -f obj/*.o
	rm -f debug/*.o
	rm -f release/*.o
	rm -f debug/$(PROGRAM)
	rm -f release/$(PROGRAM)