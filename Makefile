CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = snake

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
    RM = del /Q
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LIBS = -lraylib -lm -lpthread -ldl -lrt
    endif
    ifeq ($(UNAME_S),Darwin)
        LIBS = -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
    endif
    RM = rm -f
endif

SRC = snake.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	$(RM) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run