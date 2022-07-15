output: tproc.o
	gcc tproc.o -o output -lcurl

tproc.o: tproc.c
	gcc -c tproc.c

clean: 
	rm *.o output