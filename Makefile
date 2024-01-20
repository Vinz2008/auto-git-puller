CC=gcc
CFLAGS=-c -g -Wall
DESTDIR := /usr

ifeq ($(OS),Windows_NT)
OUTPUTBIN = auto-git-pull.exe
else
OUTPUTBIN = auto-git-pull
endif

SRCS := $(wildcard src/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))

all: $(OUTPUTBIN)

$(OUTPUTBIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

clean-build:
ifeq ($(OS),Windows_NT)
	rmdir .\src\*.o /s /q
else
	rm -f ./src/*.o
endif


clean: clean-build
	rm -rf $(OUTPUTBIN)

install:
	cp auto-git-pull $(DESTDIR)/bin/

