CC=gcc
CFLAGS=-c -g -Wall
CFLAGS += $(shell pkg-config --cflags libgit2)
LDFLAGS=$(shell pkg-config libgit2 --libs)

ifeq ($(OS),Windows_NT)
OUTPUTBIN = auto-git-pull.exe
else
OUTPUTBIN = auto-git-pull
endif

OBJS=\
file_util.o \
git.o \
main.o \

all: $(OUTPUTBIN)

$(OUTPUTBIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

clean-build:
ifeq ($(OS),Windows_NT)
	rmdir .\*.o /s /q
else
	rm -f ./*.o
endif


clean: clean-build
	rm -rf $(OUTPUTBIN)

