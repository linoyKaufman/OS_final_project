# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

# Files and directories
SRC = server.cpp Graph.cpp MST_algorithm.cpp Reactor.cpp Tree.cpp UnionFind.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = server

# Default target
all: $(TARGET)

# Linking the final binary
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Compile each source file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean the build
# Clean the build
clean:
	rm -f $(OBJ) $(TARGET)

# Rebuild the project
rebuild: clean all
