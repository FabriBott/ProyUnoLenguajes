# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -g -fPIE

# Linking flags
LDFLAGS = -pie

# Libraries
LIBS = -lpthread

# Include directories
INCLUDES = -I./include

# Source directories
SERVER_SRC_DIR = src/server
CLIENT_SRC_DIR = src/client
QT_PROJECT_DIR = /home/ani/Documents/Lenguajes\ de\ Programación/ProyUnoLenguajes/src/gui/gui  # Qt project directory

# Source files
SERVER_SRCS = $(SERVER_SRC_DIR)/main_server.cpp $(SERVER_SRC_DIR)/server.cpp
CLIENT_SRCS = $(CLIENT_SRC_DIR)/main_client.cpp $(CLIENT_SRC_DIR)/client.cpp

# Object files
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)

# Output executables
SERVER_TARGET = server
CLIENT_TARGET = client
GUI_TARGET = gui

# Qt build commands (using cmake for Qt)
MAKE = make
GUI_EXECUTABLE = "$(QT_PROJECT_DIR)./gui" 

# Default target
all: directories $(SERVER_TARGET) $(CLIENT_TARGET) $(GUI_TARGET)

# Directories
directories:
	mkdir -p include
	mkdir -p src/server
	mkdir -p src/client
	mkdir -p config
	mkdir -p data

# Build server
$(SERVER_TARGET): $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDFLAGS)

# Build client
$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) $(LDFLAGS)

# Build and run GUI
$(GUI_TARGET):
	cd $(QT_PROJECT_DIR) && cmake . && $(MAKE)  # Build Qt project
	"$(GUI_EXECUTABLE)" & 

# Compile object files from cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

# Clean the build
clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_TARGET) $(CLIENT_TARGET)
	cd $(QT_PROJECT_DIR) && $(MAKE) clean  # Clean Qt build

.PHONY: all clean directories
