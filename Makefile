# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g 

# Files and directories
SERVER_SRC = server.cpp Graph.cpp MST_algorithm.cpp Reactor.cpp Tree.cpp UnionFind.cpp
CLIENT_SRC = Client.cpp
OBJ = $(SERVER_SRC:.cpp=.o) $(CLIENT_SRC:.cpp=.o)
SERVER_TARGET = server
CLIENT_TARGET = client

# Default target
all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Linking the final server binary
$(SERVER_TARGET): $(SERVER_SRC:.cpp=.o)
	$(CXX) $(CXXFLAGS) -o $(SERVER_TARGET) $(SERVER_SRC:.cpp=.o)

# Linking the final client binary
$(CLIENT_TARGET): $(CLIENT_SRC:.cpp=.o) 
	$(CXX) $(CXXFLAGS) -o $(CLIENT_TARGET) $(CLIENT_SRC:.cpp=.o)

# Compile each source file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean the build
clean:
	rm -f $(OBJ) $(SERVER_TARGET) $(CLIENT_TARGET)

# Rebuild the project
rebuild: clean all
