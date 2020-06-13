CFLAGS=--std=c18 -O2 -Wall -Wextra -pedantic
LDLIBS=-lncurses -lcurl

.phony: clean

srcs := $(wildcard **/*.c)
objects := $(patsubst %.c,%.o,$(srcs))

ncac: $(objects)

tags: $(srcs)
	ctags -w -t $^

clean:
	rm -f **/*.o ncac
