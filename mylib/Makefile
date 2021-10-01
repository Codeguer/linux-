libmymath.so:myadd.o mysub.o
	gcc -shared -o $@ $^
myadd.o:myadd.c
	gcc -fPIC -c $<
mysub.o:mysub.c
	gcc -fPIC -c $<

.PHONY:clean
clean:
	rm *.o libmymath,so
