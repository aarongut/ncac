CFLAGS=--std=c18 -Wall -Wextra -pedantic
LDLIBS=-lncurses -lcurl

.phony: clean

objects := $(patsubst %.c,%.o,$(wildcard **/*.c))

ncac: $(objects)

clean:
	rm -f **/*.o ncac
