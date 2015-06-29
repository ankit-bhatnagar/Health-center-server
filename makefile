all:healthcenterserver doctor1 doctor2 patient1 patient2

healthcenterserver:healthcenterserver.c
	gcc -o healthcenterserver healthcenterserver.c -lsocket -lnsl -lresolv

doctor1:doctor1.c
	gcc -o doctor1 doctor1.c -lsocket -lnsl -lresolv

doctor2:doctor2.c
	gcc -o doctor2 doctor2.c -lsocket -lnsl -lresolv

patient1:patient1.c
	gcc -o patient1 patient1.c -lsocket -lnsl -lresolv

patient2:patient2.c
	gcc -o patient2 patient2.c -lsocket -lnsl -lresolv

clean:
	rm healthcenterserver
	rm doctor1
	rm doctor2
	rm patient1
	rm patient2


