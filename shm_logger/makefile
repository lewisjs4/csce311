# Depends heavily on make dependency generation rules.
#

# place compile-able source here
#
consumer_srcs := consumer.cc consumer.h named_semaphore.h named_semaphore.cc
producer_srcs := producer.cc producer.h named_semaphore.h named_semaphore.cc

# place final executable name here
#
consumer_exe := consumer
producer_exe := producer

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
consumer_objs := $(patsubst %,$(obj_dir)/%.o,$(basename $(consumer_srcs)))
producer_objs := $(patsubst %,$(obj_dir)/%.o,$(basename $(producer_srcs)))

# intermediate dependency files from source files
#
consumer_deps := $(patsubst %,$(dep_dir)/%.d,$(basename $(consumer_srcs)))
producer_deps := $(patsubst %,$(dep_dir)/%.d,$(basename $(producer_srcs)))

$(shell mkdir -p $(obj_dir) >/dev/null)
$(shell mkdir -p $(dep_dir) >/dev/null)

# cpp build flags
#
cpp_flags := -std=c++17 -g -Wall -Wextra -pedantic -I $(inc_path)

# linker flags (these come before)
#
linker_flags := -pthread

# linker libraries (these come at end)
#
linker_libs := -lrt

# flags required for dependency generation; passed to compilers
#
dep_flags = -MT $@ -MD -MP -MF $(dep_dir)/$*.Td

# compile C++ source files
#
compile.cc = $(cc) $(dep_flags) $(cpp_flags) -c -o $@

# link C++ object files to binary
#
link.o = $(cc) $(linker_flags) $^ -o $@ $(linker_libs)


$(obj_dir)/%.o: src/%.cc inc/%.h
$(obj_dir)/%.o: src/%.cc inc/%.h $(dep_dir)/%.d
	$(compile.cc) $<

$(obj_dir)/%.o: src/%.cc 
$(obj_dir)/%.o: src/%.cc $(dep_dir)/%.d
	$(compile.cc) $<

$(consumer_exe) : $(consumer_objs)
	$(link.o)

$(producer_exe) : $(producer_objs)
	$(link.o)

.PRECIOUS: $(dep_dir)/%.d
$(dep_dir)/%.d: ;

-include $(deps)



.PHONY: clean
clean:
	$(RM) -r $(obj_dir) $(dep_dir)
