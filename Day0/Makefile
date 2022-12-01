
.PHONY=clean

CC := g++
CFLAGS := -std=c++2a -Wall -Wextra

day_file := $(wildcard *.cpp)

main: $(day_file)
	@ $(CC) $(CFLAGS) -o $@ $(day_file)

clean:
	@ rm -f test_input
