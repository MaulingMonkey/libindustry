CC=g++
LD=g++
CPPFLAGS=-I../../
LDFLAGS=

UNITTEST_ROOT=../../unit-tests/
%.o : %.cpp
	$(CC) $(CPPFLAGS) -c $^ -o $@

unit-tests : $(patsubst %.cpp,%.o,$(wildcard ../../unit-tests/*.cpp ../../unit-tests/*/*.cpp ../../unit-tests/*/*/*.cpp))
	$(LD) $(LDFLAGS) $^ -o $@
