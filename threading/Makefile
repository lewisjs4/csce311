# Copyright 2026 CSCE 311 Spring
#
CXX      := g++
RM       := rm -f
STANDARD := c++20
INCLUDE  := ..

# Flags (put -pthread in both compile and link)
CXXFLAGS := -std=$(STANDARD) -Wall -Wextra -pedantic -g -I$(INCLUDE) -pthread
LDFLAGS  := -pthread

# Layout
SRCDIR   := src
BUILDDIR := build
BINDIR   := bin

TARGETS  := thread atomic_race
BINS     := $(addprefix $(BINDIR)/, $(TARGETS))
OBJS     := $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(TARGETS)))

.PHONY: all clean $(TARGETS)
all: $(BINS)

# allow "make thread" or "make atomic_race"
$(TARGETS): %: $(BINDIR)/% ;

# Link rule (pattern)
$(BINDIR)/%: $(BUILDDIR)/%.o | $(BINDIR)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compile rule (pattern)
$(BUILDDIR)/%.o: $(SRCDIR)/%.cc | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Cleanup
clean:
	$(RM) $(OBJS) $(BINS)

