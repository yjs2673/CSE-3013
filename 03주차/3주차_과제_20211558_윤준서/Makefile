objects = main.o Str.o
src = main.cpp Str.cpp

cc = g++
cppflags = -g -c
target = main

$(target) : $(objects) Str.h
	$(cc) -o $(target) $(objects)
.PHONY : clean
clean :
	rm $(target) $(objects)
