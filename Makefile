# From https://github.com/danielpinto8zz6/c-cpp-project-generator#readme

CC = gcc
CFLAGS := -Wall -Wextra
LFLAGS = 
OUTPUT := output
SRC := src
INCLUDE := include

MAIN := main
SOURCEDIRS := $(shell find $(SRC) -type d)
INCLUDEDIRS := $(shell find $(INCLUDE) -type d)
FIXPATH = $1
RM = rm -f
MD := mkdir -p

INCLUDES := $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
SOURCES := $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))
OBJECTS := $(SOURCES:.c=.o)
OUTPUTMAIN := $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing "all" complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(OUTPUT)/libceval.a
	$(RM) $(call FIXPATH,$(OBJECTS))
	@echo Cleanup complete!

.PHONY: lib
lib:
	$(RM) $(SRC)/$(MAIN).o
	ar rcs $(OUTPUT)/libceval.a $(SRC)/*.o


run: all
	./$(OUTPUTMAIN)
	@echo Executing "run: all" complete

