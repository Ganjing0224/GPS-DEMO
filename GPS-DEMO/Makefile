objs=uart.o gnss.o main.o
srcs=uart.c gnss.c main.c
 
gps_test: $(objs)
	$(CC) -o gps_test $(objs)
	@make clean
 
main.o: $(srcs) uart.h gnss.h
	$(CC) -c  $(srcs)
 
uart.o: uart.c
	$(CC) -c  uart.c
 
gnss.o: gnss.c 
	$(CC) -c  gnss.c
 
clean:
	rm *.o
