CC = gcc.exe
CFLAGS = -std=c99 #-g

CPPFLAGS = \
	-Ilibs \
	-Ilibs/ObsCureFileParserHeader \
	-Ilibs/ObsCureInfoHeader \
	-Isrc \
	-Isrc/random \
	-Isrc/choose_items \
	-Isrc/config \
	-Isrc/logger \
	-Isrc/paths \
	-Isrc/ocr_config \

LDFLAGS = \
	-Llibs/ \

LDLIBS = \
	-lObsCureFileParser \
	-lObsCureInfo \

SRC = \
	src/main.c \
	src/business_ocr.c \
	src/commands_ocr.c \
	src/random/random.c \
	src/choose_items/choose_items.c \
	src/config/config.c \
	src/logger/logger.c \
	src/paths/paths.c \
	src/ocr_config/ocr_config.c \

OBJ = ${SRC:.c=.o}

all: ObsCureRandomizer

ObsCureRandomizer: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	$(RM) ObsCureRandomizer $(OBJ)