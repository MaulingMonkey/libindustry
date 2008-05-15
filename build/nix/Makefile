CC=g++
LD=g++
CPPFLAGS=-I../../ -I/usr/local/include/ruby-1.9.0/ -I/usr/local/include/ruby-1.9.0/i686-linux/
LDFLAGS=-L. -L/usr/local/lib/ -lruby -lboost_signals
LIBRARY_LDFLAGS=$(LDFLAGS) -shared
UNITTEST_LDFLAGS=$(LDFLAGS) -lboost_unit_test_framework

default : all

%.o : %.cpp
	$(CC) $(CPPFLAGS) -c $^ -o $@

libindustry.so : $(patsubst %.cpp,%.o,$(wildcard ../../industry/*.cpp ../../industry/*/*.cpp ../../industry/*/*/*.cpp))
	$(LD) $(LIBRARY_LDFLAGS) $^ -o $@

unittest : libindustry.so $(patsubst %.cpp,%.o,$(wildcard ../../unit-tests/*.cpp ../../unit-tests/*/*.cpp ../../unit-tests/*/*/*.cpp))
	$(LD) $(UNITTEST_LDFLAGS) $(patsubst lib%.so,-l%,$(filter %.so,$^)) $(filter %.o,$^) -o $@

all : libindustry.so unittest

clean :
	rm -f libindustry.so unittest $(wildcard ../../*.o ../../*/*.o ../../*/*/*.o ../../*/*/*/*.o)
