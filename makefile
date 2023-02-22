TARGET=gm

# Paths
SRC_DIR=src
LIB_DIR=lib
OBJ_DIR=obj

# File lists
C_SOURCES=$(wildcard $(LIB_DIR)/*.c)
CPP_SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
SOURCES=$(C_SOURCES) $(CPP_SOURCES)
$(info SOURCES FOUND: $(SOURCES))

OBJECTS=$(C_SOURCES:$(LIB_DIR)/%.c=$(OBJ_DIR)/%.o) $(CPP_SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS=$(OBJECTS:$(OBJ_DIR)/%.o=$(OBJ_DIR)/%.d)

# Main rule
all: $(OBJ_DIR) $(TARGET)

# Directory preparation
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Including dependency files
-include $(DEPS)

# Compilation flags
CFLAGS=-g -Wall -pedantic `sdl2-config --cflags`
# Linking flags
LFLAGS=-lSDL2_gfx `sdl2-config --libs` -lm

# Compilation recipes
$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c
	gcc -c -MMD $< $(CFLAGS) -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ -c -MMD $< $(CFLAGS) -o $@

# Linking rule
$(TARGET): $(OBJECTS)
	g++ $^ -o $@ $(LFLAGS)

# Cleaning rules
.PHONY: clean

clean:
	-rm $(TARGET) $(OBJECTS) $(DEPS)
	-rm -rf $(OBJ_DIR)
