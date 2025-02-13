# Copyright 2025 CSCE 311

# Compiler and Flags
CXX := g++
CXXFLAGS := -std=c++2a
CXXFLAGS += -Wall -Wextra -pedantic
CXXFLAGS += -g
CXXFLAGS += -I include
CXXFLAGS += -MMD -MP

# Build directory
BUILD_DIR := build

# Source files
FORK_PIPE_SRC := src/fork_pipe.cc
FIFO_MANAGER_SRC := src/fifo_manager.cc
DOMAIN_SOCKET_CLIENT_SRC := src/test_socket_client.cc
DOMAIN_SOCKET_SERVER_SRC := src/test_socket_server.cc
DOMAIN_SOCKET_COMMON_SRC := src/domain_socket.cc  # Shared between client and server

# Object and dependency files in build
#   1. Replace .cc with .o,
#   2. Remove any directories before file name
#   3. Add build directory as directory above file
FORK_PIPE_OBJS := $(addprefix $(BUILD_DIR)/, $(notdir $(FORK_PIPE_SRC:.cc=.o)))
FIFO_MANAGER_OBJS := $(addprefix $(BUILD_DIR)/, $(notdir $(FIFO_MANAGER_SRC:.cc=.o)))
DOMAIN_SOCKET_CLIENT_OBJS := $(addprefix $(BUILD_DIR)/, $(notdir $(DOMAIN_SOCKET_CLIENT_SRC:.cc=.o))) $(BUILD_DIR)/domain_socket.o
DOMAIN_SOCKET_SERVER_OBJS := $(addprefix $(BUILD_DIR)/, $(notdir $(DOMAIN_SOCKET_SERVER_SRC:.cc=.o))) $(BUILD_DIR)/domain_socket.o

# Map .o objects to .d files for dependency management
#   A .d file might look like
#   	build/file.o : src/file.cc include/file.h
DEPS := $(FORK_PIPE_OBJS:.o=.d) $(FIFO_MANAGER_OBJS:.o=.d) \
        $(DOMAIN_SOCKET_CLIENT_OBJS:.o=.d) $(DOMAIN_SOCKET_SERVER_OBJS:.o=.d)

# Executables
FORK_PIPE_EXEC := fork-pipe
FIFO_MANAGER_EXEC := fifo-manager
DOMAIN_SOCKET_CLIENT_EXEC := test-socket-client
DOMAIN_SOCKET_SERVER_EXEC := test-socket-server

# Default target
all: $(FORK_PIPE_EXEC) $(FIFO_MANAGER_EXEC) $(DOMAIN_SOCKET_CLIENT_EXEC) $(DOMAIN_SOCKET_SERVER_EXEC)

# Build executables
$(FORK_PIPE_EXEC): $(FORK_PIPE_OBJS)
	$(CXX) $(FORK_PIPE_OBJS) -o $@

$(FIFO_MANAGER_EXEC): $(FIFO_MANAGER_OBJS)
	$(CXX) $(FIFO_MANAGER_OBJS) -o $@

$(DOMAIN_SOCKET_CLIENT_EXEC): $(DOMAIN_SOCKET_CLIENT_OBJS)
	$(CXX) $(DOMAIN_SOCKET_CLIENT_OBJS) -o $@

$(DOMAIN_SOCKET_SERVER_EXEC): $(DOMAIN_SOCKET_SERVER_OBJS)
	$(CXX) $(DOMAIN_SOCKET_SERVER_OBJS) -o $@

# Build .o files inside build/
$(BUILD_DIR)/%.o: src/%.cc | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Special rule for domain_socket.o (shared by both client and server)
$(BUILD_DIR)/domain_socket.o: src/domain_socket.cc | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean target
clean:
	rm -rf $(BUILD_DIR) $(FORK_PIPE_EXEC) $(FIFO_MANAGER_EXEC) $(DOMAIN_SOCKET_CLIENT_EXEC) $(DOMAIN_SOCKET_SERVER_EXEC)

# Include auto-generated dependency files and do not announce errors when
# missing (on first build)
-include $(DEPS)

.PHONY: all clean
