# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Icommon/ -Iserver/lib -Iclient/lib
LDFLAGS := 

# Directories
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/bin
SERVER_BUILD_DIR := $(BUILD_DIR)/server
CLIENT_BUILD_DIR := $(BUILD_DIR)/client

# Source directories
SERVER_SRC_DIR := server/src server/lib
CLIENT_SRC_DIR := client/src client/lib
COMMON_SRC_DIR := common

# Find all .cpp files dynamically
SERVER_SRC := $(shell find $(SERVER_SRC_DIR) -name "*.cpp") $(shell find $(COMMON_SRC_DIR) -name "*.cpp")
CLIENT_SRC := $(shell find $(CLIENT_SRC_DIR) -name "*.cpp") $(shell find $(COMMON_SRC_DIR) -name "*.cpp")

# Create object files list
SERVER_OBJ := $(SERVER_SRC:%.cpp=$(OBJ_DIR)/%.o)
CLIENT_OBJ := $(CLIENT_SRC:%.cpp=$(OBJ_DIR)/%.o)

# Targets
SERVER_TARGET := $(SERVER_BUILD_DIR)/server
CLIENT_TARGET := $(CLIENT_BUILD_DIR)/client

# Rules
.PHONY: all clean server client

all: server client

# Server rules
server: $(SERVER_TARGET)

$(SERVER_TARGET): $(SERVER_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Client rules
client: $(CLIENT_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)
