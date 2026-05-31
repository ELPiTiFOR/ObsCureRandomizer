CC = "C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe"
#WR = "C:\Program Files\CodeBlocks\MinGW\bin\windres.exe"
#CFLAGS = -Wall -Werror -pedantic -std=c99
CPPFLAGS = \
	-Isrc \
	-Isrc/hoe_file \

CFLAGS = -std=c99

SRC = \
	src/allitems.c \
	src/commands.c \
	src/config.c \
	src/file_debug.c \
	src/file_io.c \
	src/file_read.c \
	src/file_write.c \
	src/logger.c \
	src/main.c \
	src/path.c \
	src/room_debug.c \
	src/room_vars.c \
	src/room.c \
	src/state.c \
	src/tm_file.c \
	src/utils.c \
	src/hoe_file/hoe_file.c \
	src/business.c \
	#src/data_structures/pointers.c

OBJ = ${SRC:.c=.o}

all: ObsCureRandomizer

ObsCureRandomizer: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) ObsCureRandomizer $(OBJ)