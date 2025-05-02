PACK = xpasekj00

LIBNAME = htab
LIBBASE = $(LIBNAME)
LIB_EXTENSION = so
LIB_TARGET = lib$(LIBBASE).$(LIB_EXTENSION)
LIB_STATIC_EXTENSION = a
LIB_STATIC_TARGET = lib$(LIBBASE).$(LIB_STATIC_EXTENSION)

OBJ_DIR = obj
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Werror -g
CC = gcc
CFLAGS = -std=gnu11 -O2 -pedantic -Wall -Wextra -Werror -g
CFLAGS_LIB = -fPIC $(CFLAGS)
LDFLAGS = -shared

LIB_SRC_FILES := $(wildcard ./libhash/*.c)
LIB_OBJECTS := $(LIB_SRC_FILES:./libhash/%.c=$(OBJ_DIR)/libhash/%.o)

SRC_FILES_TAIL = tail.c
SRC_FILES_MAX = maxwordcount.c io.c
OBJ_FILES_TAIL = $(SRC_FILES_TAIL:%.c=$(OBJ_DIR)/%.o)
OBJ_FILES_MAX = $(SRC_FILES_MAX:%.c=$(OBJ_DIR)/%.o)
OBJ_FILES_MAX-CPP = $(SRC_FILES_MAX-CPP:%.cc=$(OBJ_DIR)/%.o)
EXECUTABLE_TAIL = tail
EXECUTABLE_MAX-DYN = maxwordcount-dynamic
EXECUTABLE_MAX-STA = maxwordcount
EXECUTABLE_MAX-CPP = maxwordcount-cpp

.PHONY: all clean pack
all: $(EXECUTABLE_TAIL) $(EXECUTABLE_MAX-DYN) $(EXECUTABLE_MAX-STA)

$(LIB_TARGET): $(LIB_OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(LIB_STATIC_TARGET): $(LIB_OBJECTS)
	ar crs $@ $^

$(OBJ_DIR)/libhash/%.o: libhash/%.c | $(OBJ_DIR)/libhash
	$(CC) $(CFLAGS_LIB) -c $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXECUTABLE_TAIL): $(OBJ_FILES_TAIL)
	$(CC) $(CFLAGS) $^ -o $@

$(EXECUTABLE_MAX-DYN): $(OBJ_FILES_MAX) $(LIB_TARGET)
	$(CC) $(CFLAGS) $^ -o $@ -L. -lhtab -Wl,-rpath=.

$(EXECUTABLE_MAX-STA): $(OBJ_FILES_MAX) $(LIB_STATIC_TARGET)
	$(CC) $(CFLAGS) -static $^ -o $@ -L. -lhtab
$(EXECUTABLE_MAX-CPP): $(OBJ_FILES_MAX-CPP)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/libhash:
	mkdir -p $@

pack: $(PACK).zip

$(PACK).zip: $(SRC_FILES_MAX) $(SRC_FILES_TAIL) Makefile libhash/* maxwordcount-cpp.cc *.h
	zip $@ $^
clean:
	rm -rf $(OBJ_DIR) *.so *.a $(EXECUTABLE_TAIL) $(EXECUTABLE_MAX-DYN) $(EXECUTABLE_MAX-STA)
