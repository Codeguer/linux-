main:main.o cal.o
	gcc $^ -o $@ 
%.o:%.c
	gcc -c $<

.PHONY:clean
clean:
	rm -f *.o main




