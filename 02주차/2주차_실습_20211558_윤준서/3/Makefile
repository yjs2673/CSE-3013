cc = gcc
cflags = -W -g
target = fmt
# .SUFFIXES : .c .o
objects = main.o stringM.o output.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

%.o : %.c
	$(cc) $(cflags) -c -o $@ $<

main.o stringM.o output.o : Header.h
# $(objects) :
.PHONY : clean
clean :
	rm $(target) $(objects)
