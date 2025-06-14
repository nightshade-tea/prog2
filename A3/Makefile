#--- CONFIG ---

MAIN		:= pato
CC			:= gcc
ALLIBS		:= allegro-5 allegro_main-5 allegro_font-5 allegro_image-5 \
			   allegro_primitives-5
CFLAGS		:= -Wall -Wextra -g $$(pkg-config --cflags $(ALLIBS))
LDLIBS		:= $$(pkg-config --libs $(ALLIBS))

SRC_DIRS	:= . lib
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
