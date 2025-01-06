objects = main.o Array.o RangeArray.o
src = main.cpp Array.cpp RangeArray.cpp

cc = g++
cppflags = -g -c
target = main

$(target) : $(objects) Array.h RangeArray.h
	$(cc) -o $(target) $(objects)
.PHONY : clean
clean :
	rm $(target) $(objects)
