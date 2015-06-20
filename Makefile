INCLUDES = -I./src -I/usr/local/include -I/usr/include
LIBS = -L/usr/local/lib -L/usr/lib
LDLIBS = -lpthread
BUILD := debug

cxxflags.debug := -g -DNDEBUG
cxxflags.release := -03
CXXFLAGS := -Wall -std=c++11 ${cxxflags.${BUILD}}

SOURCE = ./src/main.cpp
OBJECTS = ./src/Monitor.o

main: ./src/lib.a $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) $(INCLUDES) $(LIBS) $(LDLIBS) -o $@ $<

./src/lib.a: $(OBJECTS)
	$(AR) rvs $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	find . -name "*.o" -delete
	find . -name "*.a" -delete
	find . -name "main" -delete

