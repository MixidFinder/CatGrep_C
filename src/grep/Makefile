CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11
DEBUG_FLAGS=-g

SOURCES=$(shell find . -name '*.c')
OBJ=$(patsubst %.c, %.o, $(SOURCES))

TARGET=s21_grep
TESTS=tests/run_tests.sh

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	bash $(TESTS)

check_valgrind: $(TARGET)
	valgrind -s --leak-check=full --dsymutil=yes --show-leak-kinds=all --track-origins=yes bash $(TESTS)

check_cpp: $(TARGET)
	cppcheck --enable=all --suppress=missingIncludeSystem $(SOURCES)

clean:
	rm -f $(OBJ)
	rm -f *.a
	rm -f *.so

clean_trgt:
	rm -f $(TARGET)

rebuild: clean clean_trgt all