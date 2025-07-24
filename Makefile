CXX       := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN        := .
SRC        := engine
INCLUDE    := engine
LIB        := lib
LIBRARIES  :=
EXECUTABLE := main

all: $(BIN)/$(EXECUTABLE)

run: clean all
    # must be a TAB here
    clear
    @echo "🚀 Executing..."
    ./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
    # must be a TAB here
    @echo "🚧 Building..."
    $(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
    # must be a TAB here
    @echo "🧹 Clearing..."
    -rm -f $(BIN)/$(EXECUTABLE)