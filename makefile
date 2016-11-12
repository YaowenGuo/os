pcbcontrol: pcbcontrol.o pcb.o 
	gcc -o pcbcontrol pcbcontrol.o pcb.o
pcbcontrol.o:   pcbcontrol.c pcb.h
	gcc -c pcbcontrol.c
pcb.o:  pcb.c pcb.h
	gcc -c pcb.c
clean:
	rm -f *.o
