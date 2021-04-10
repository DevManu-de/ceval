CC = gcc
CFLAGS := -Wall -Wextra -g
SRC := src
INCLUDE := include

RM = rm -rf
MD := mkdir -p

AR = ar
ARFLAGS = rcs

CTAGS := ctags
CTAGSFLAGS := -R
CTAGSDIRECTORIES = $(SRC) $(INCLUDE)

MAIN := libceval
SOURCEDIRS := $(shell find $(SRC) -type d)
INCLUDEDIRS := $(shell find $(INCLUDE) -type d)

INCLUDES := $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
SOURCES := $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))
OBJECTS := $(SOURCES:.c=.o)

all: $(MAIN)
	@echo Compilation and library creation successful

$(MAIN): $(OBJECTS)
	$(AR) $(ARFLAGS) $(MAIN).a $(OBJECTS)
	$(CTAGS) $(CTAGSFLAGS) $(CTAGSDIRECTORIES)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: test
test:
	make -C ./test

.PHONY: clean
clean:
	$(RM) $(MAIN).a $(OBJECTS)
	make -C ./test clean
	@echo Clean successful

