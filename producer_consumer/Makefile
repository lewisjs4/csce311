# Copyright 2026 CSCE 311 Spring

CXX      := g++
RM       := rm -f
STANDARD := c++20

# point to your include directory
INCLUDE  := ..

# Flags (put -pthread in both compile and link)
CXXFLAGS := -std=$(STANDARD) -Wall -Wextra -pedantic -g -I$(INCLUDE) -pthread -MMD -MP
LDFLAGS  := -pthread

# Layout
SRCDIR   := src
BUILDDIR := build
BINDIR   := bin

TARGET   := producer_consumer
BIN      := $(BINDIR)/$(TARGET)

SRCS     := $(wildcard $(SRCDIR)/*.cc)
OBJS     := $(patsubst $(SRCDIR)/%.cc,$(BUILDDIR)/%.o,$(SRCS))
DEPS     := $(OBJS:.o=.d)

.PHONY: all clean
all: $(BIN)

# Link the final binary from all object files
$(BIN): $(OBJS) | $(BINDIR)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compile each .cc to a corresponding .o in build/
$(BUILDDIR)/%.o: $(SRCDIR)/%.cc | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Create directories if they don't exist
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Include dependency files if they exist
-include $(DEPS)

clean:
	$(RM) $(OBJS) $(DEPS) $(BIN)

