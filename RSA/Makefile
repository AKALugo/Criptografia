CXX = g++						# The C++ compiler command
CXXFLAGS = -g -O0 -Wall -std=c++14	# The C++ compiler options (preserve debug symbols and No optimization)
LDFLAGS = 	# The linker options (if any)

# The all target builds all of the programs handled by the makefile.
# This target has the dependency chain:
#     all -> hello -> hello.o -> hello.cpp
all: rsa

# The hello_world target builds the hello_world executable.
rsa: rsa.cc
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Indicate that the all and clean targets do not
# correspond to actual files.
.PHONY: all clean

# The following rule is effectively built into make and
# therefore need not be explicitly specified:
# hello_world.o: hello_world.cc
#
	$(CXX) $(CXXFLAGS) -c $<

# The clean target removes all of the executable files
# and object files produced by the build process
# We can use it for additional housekeeping purposes
clean :
	rm -f rsa *.o
push:
	git pull 
	git add . 
	git commit -m "Another example" 
	git push