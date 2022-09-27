flags = -c
sources = $(wildcard *.c)
headers = $(wildcard *.h)
objects = $(sources:.c=.o$)
exec = main

main:
	gcc $(flags) $(sources) $(headers)
	gcc $(objects) -o $(exec)

clean:
	del /f *.o
	del /f *.h.gch
	del /f $(exec)

