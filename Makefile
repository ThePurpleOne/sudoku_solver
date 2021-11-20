CC       := gcc
CFLAGS := -Wall -Wextra -g -fsanitize=address -fsanitize=leak

BIN     	 := bin
SRC     	 := src
INCLUDE 	 := include
LIBRARIES    := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
EXECUTABLE1  := main

# ADD ALL THE SOURCES FOR EXECUTABLE1
SOURCES1 += $(SRC)/$(EXECUTABLE1).c
SOURCES1 += $(SRC)/sudoku.c

.PHONY: all run clean pre_build # Specify these are not files but internal terms

all: pre_build $(BIN)/$(EXECUTABLE1) 

run: clean all 
	clear
	@echo "########## EXECUTE FIRST PROGRAM ##########"
	./$(BIN)/$(EXECUTABLE1)

# # $^ -> dep
# # $@ -> target
# # $< -> premiere dep
# # Compiles directly the c files
# # Not optimal but succint and simple

# COMPILE FIRST EXECUTABLE
$(BIN)/$(EXECUTABLE1): $(SOURCES1) 
	@echo "########## COMPILING FIRST EXECUTABLE ##########"
	$(CC) $(CFLAGS) -I $(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	@echo "########## CLEANING ##########"	
	-rm $(BIN)/*

# pre_build:
# 	@echo "########## CREATING BIN DIR  ##########"
# 	-mkdir -p $(BIN)
