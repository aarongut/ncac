CFLAGS=--std=c18 -O2 -Wall -Wextra -pedantic
LDLIBS=-lncurses -lcurl

ncac: ui/base.o asana/fetch.o ncac.o
