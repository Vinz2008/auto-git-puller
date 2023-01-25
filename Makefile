CC=gcc
CFLAGS=-c -g -Wall
DESTDIR := /usr

ifeq ($(OS),Windows_NT)
OUTPUTBIN = auto-git-pull.exe
else
OUTPUTBIN = auto-git-pull
endif

OBJS=\
file_util.o \
config_file.o \
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

install:
	cp auto-git-pull $(DESTDIR)/bin

