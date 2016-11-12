pcbcontrol: pcbcontrol.o pcb.o getTime.o
	gcc -o pcbcontrol pcbcontrol.o getTime.o pcb.o
pcbcontrol.o:   pcbcontrol.c pcb.h getTime.h
	gcc -c pcbcontrol.c
pcb.o:  pcb.c pcb.h getTime.h 
	gcc -c pcb.c
getTime.o:	getTime.c getTime.h
	gcc -c getTime.c
clean:
	rm -f *.o
