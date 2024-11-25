# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17
LDFLAGS := 

# Directories
BIN_DIR := bin
BUILD_DIR := $(BIN_DIR)/build
OBJ_DIR := $(BIN_DIR)/obj
SERVER_OBJ_DIR := $(OBJ_DIR)/server
CLIENT_OBJ_DIR := $(OBJ_DIR)/client
SERVER_BUILD_DIR := $(BUILD_DIR)
CLIENT_BUILD_DIR := $(BUILD_DIR)

# Include paths
COMMON_INCLUDES := -Icommon
SERVER_INCLUDES := $(COMMON_INCLUDES) $(addprefix -I, $(shell find server/lib -type d))
CLIENT_INCLUDES := $(COMMON_INCLUDES) $(addprefix -I, $(shell find client/lib -type d))

# Source directories
SERVER_SRC_DIR := server/src server/lib
CLIENT_SRC_DIR := client/src client/lib
COMMON_SRC_DIR := common

# Find all .cpp files dynamically
SERVER_SRC := $(shell find $(SERVER_SRC_DIR) -name "*.cpp") $(shell find $(COMMON_SRC_DIR) -name "*.cpp")
CLIENT_SRC := $(shell find $(CLIENT_SRC_DIR) -name "*.cpp") $(shell find $(COMMON_SRC_DIR) -name "*.cpp")

# Create object files list
SERVER_OBJ := $(patsubst %.cpp, $(SERVER_OBJ_DIR)/%.o, $(SERVER_SRC))
CLIENT_OBJ := $(patsubst %.cpp, $(CLIENT_OBJ_DIR)/%.o, $(CLIENT_SRC))

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

$(SERVER_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(SERVER_INCLUDES) $(CXXFLAGS) -c $< -o $@

# Client rules
client: $(CLIENT_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $^ -o $@

$(CLIENT_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CLIENT_INCLUDES) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)
