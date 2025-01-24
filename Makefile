TARGET = a
FOLDER = src/
SRC = $(shell find $(FOLDER) -type f -name '*.c')

CC = gcc
CFLAGS = -Wall -std=c99 -g

LIBS_UNIX = -lGL -lm -lpthread -ldl -lrt -lX11 
LIBS_WIN = -lopengl32 -lgdi32 -lwinmm
LIBS = -lraylib -lflecs

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
    LIBS += $(LIBS_UNIX)
else ifeq ($(UNAME_S), Darwin)
    LIBS += $(LIBS_UNIX)
else
    LIBS += $(LIBS_WIN)
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
