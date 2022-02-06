# Depends heavily on make dependency generation rules.
#

# place compile-able source here
#
srcs := producer_consumer.cc semaphore_manager.cc

# place final executable name here
#
exe := producer-consumer

inc_path := ..

# compiler
#
cc := g++

# locations for intermediate files
#
obj_dir := .o
dep_dir := .d

# intermediate object files from source files
#
objs := $(patsubst %,$(obj_dir)/%.o,$(basename $(srcs)))

# intermediate dependency files from source files
#
deps := $(patsubst %,$(dep_dir)/%.d,$(basename $(srcs)))

$(shell mkdir -p $(dir $(objs)) >/dev/null)
$(shell mkdir -p $(dir $(deps)) >/dev/null)

# cpp build flags
#
cpp_flags := -std=c++17 -g -Wall -Wextra -pedantic -I $(inc_path)

# linker flags 
#
linker_flags :=

# linker libraries
#
linker_libs := -pthread

# flags required for dependency generation; passed to compilers
#
dep_flags = -MT $@ -MD -MP -MF $(dep_dir)/$*.Td

# compile C++ source files
#
compile.cc = $(cc) $(dep_flags) $(cpp_flags) -c -o $@

# link object files to binary
#
link.o = $(cc) $(linker_libs) -o $@

all: $(exe)

.PHONY: clean
clean:
	$(RM) -r $(obj_dir) $(dep_dir)

$(exe): $(objs)
	$(link.o) $^

$(obj_dir)/%.o: src/%.cc
$(obj_dir)/%.o: src/%.cc $(dep_dir)/%.d
	$(compile.cc) $<

.PRECIOUS: $(dep_dir)/%.d
$(dep_dir)/%.d: ;

-include $(deps)

