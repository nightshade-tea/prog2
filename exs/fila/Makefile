#--- CONFIG ---
MAIN    = teste
LIBS    = fila
CC      = gcc
CFLAGS  = -Wall -Wextra -g -std=c99
#LDLIBS  = -lm

#--- OBJECTS ---
LIB_OBJS = $(addsuffix .o, $(LIBS))
MAIN_OBJ = $(MAIN).o

#--- BUILD RULES ---
$(MAIN): $(MAIN_OBJ) $(LIB_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#--- PHONY ---
valgrind: $(MAIN)
	valgrind --leak-check=full --track-origins=yes ./$(MAIN)

clean:
	rm -f $(LIB_OBJS) $(MAIN_OBJ) $(MAIN)

.PHONY: valgrind clean
