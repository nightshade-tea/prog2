#--- CONFIG ---

MAIN			:= pato
CC				:= gcc
CFLAGS		:= -Wall -Wextra -g -I.
LDLIBS		:=

SRC_DIRS	:= .
OBJ_DIR		:= obj

HEADERS		:= $(wildcard $(addsuffix /*.h,$(SRC_DIRS)))
SRCS			:= $(wildcard $(addsuffix /*.c,$(SRC_DIRS)))
OBJS			:= $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))


#--- BUILD RULES ---

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


#--- PHONY ---

format:
	clang-format -i -style=gnu \
		$(SRCS) $(HEADERS)

valgrind: $(MAIN)
	valgrind --leak-check=full --track-origins=yes \
		./$(MAIN)

clean:
	rm -rf $(OBJ_DIR) $(MAIN)

.PHONY: format valgrind clean
