mythread:mythread.c
	gcc -o $@ $^  -lpthread # -O2 
#	不加l也可以但最好规范使用，为什么这里不使用-I 与-L选项呢，因为该头文件与库文件在系统标准目录下

.PHONY:clean
clean:
	rm -f mythread
