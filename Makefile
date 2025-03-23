# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -g

# Libraries
LIBS = -lpthread

# Include directories
INCLUDES = -I./include

# Source directories
SERVER_SRC_DIR = src/server
CLIENT_SRC_DIR = src/client

# Source files
SERVER_SRCS = $(SERVER_SRC_DIR)/main_server.cpp $(SERVER_SRC_DIR)/server.cpp
CLIENT_SRCS = $(CLIENT_SRC_DIR)/main_client.cpp $(CLIENT_SRC_DIR)/client.cpp

# Object files
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)

# Output executables
SERVER_TARGET = server
CLIENT_TARGET = client

all: directories $(SERVER_TARGET) $(CLIENT_TARGET)

directories:
	mkdir -p include
	mkdir -p src/server
	mkdir -p src/client
	mkdir -p config
	mkdir -p data

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_TARGET) $(CLIENT_TARGET)

.PHONY: all clean directories