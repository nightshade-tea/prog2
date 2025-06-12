#--- CONFIG ---

MAIN		:= pato
CC			:= gcc
CFLAGS		:= -Wall -Wextra -g -I. \
			   $$(pkg-config --cflags allegro-5 allegro_main-5 allegro_font-5)
LDLIBS		:= $$(pkg-config --libs allegro-5 allegro_main-5 allegro_font-5)

SRC_DIRS	:= .
OBJ_DIR		:= obj

HEADERS		:= $(wildcard $(addsuffix /*.h,$(SRC_DIRS)))
SRCS		:= $(wildcard $(addsuffix /*.c,$(SRC_DIRS)))
OBJS		:= $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))


#--- BUILD RULES ---

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


#--- PHONY ---

format:
	clang-format -i -style=gnu $(SRCS) $(HEADERS)

clean:
	rm -rf $(OBJ_DIR) $(MAIN)

.PHONY: format clean
