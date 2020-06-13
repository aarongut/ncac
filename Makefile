CFLAGS=--std=c11 -O2 -Wall -Wextra -pedantic
LDLIBS=-lncurses -lcurl

.phony: clean

hdrs := $(wildcard **/*.h)
srcs := $(wildcard **/*.c)
objects := $(patsubst %.c,%.o,$(srcs))

ncac: $(objects)

tags: $(srcs) $(hdrs)
	ctags -w $^

clean:
	rm -f **/*.o ncac
