#--- CONFIG ---
MAIN    = vinac
LIBS    = lz
CC      = gcc
CFLAGS  = -Wall -Wextra -g

#--- OBJECTS ---
LIB_OBJS = $(addsuffix .o, $(LIBS))
MAIN_OBJ = $(MAIN).o

#--- BUILD RULES ---
$(MAIN): $(MAIN_OBJ) $(LIB_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

#--- PHONY ---
format:
	clang-format -i -style=gnu $(wildcard $(MAIN).c $(addsuffix .c, $(LIBS)) \
		$(addsuffix .h, $(LIBS)))

valgrind: $(MAIN)
	valgrind --leak-check=full --track-origins=yes ./$(MAIN)

clean:
	rm -f $(LIB_OBJS) $(MAIN_OBJ) $(MAIN)

.PHONY: format valgrind clean
