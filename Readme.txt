****Readme File****

Name :  Ankit Bhatnagar

In this Socket Programming project we have built on the client server programs to realize a Health center server. This enables 2 patients to book appointments with any of the 2 doctors and any of the 6 possible slots. As soon as a slot is reserved, patient can only choose among the available ones. On reserving a slot, patient gets info about the doctor and sends it's own insurance info to the designated doctor. Doctor finally sends the co-pay amount according to the insurance info provided. Health center server is a TCP server.
Doctor1 and Doctor2 are UDP servers. Patient1 and Patient2 employ TCP connection in Phase 1 and Phase 2, and UDP connection in Phase 3.

Code Files:

1. healthcenterserver.c : The healthcenterserver.c is the source file for the Health Center Server (TCP server) to which Patients(client) contact and request for appointments. The communication between the client and server takes place over TCP socket. It waits for a patient to connect endlessly. I have assigned static TCP port number 21927 to the Health center server.

2. doctor1.c : The doc1.c is the source file for Doctor 1 (UDP server) which responds to the Patient's(client) query about cost of Insurance over a UDP connection. This is implemented in the Phase 3 of the project. I have assigned the static UDP port number 41927.

3. doctor2.c : The doc2.c is the source for file for Doctor 2 (UDP server) which responds to the Patient's(client) query about cost of Insurance over a UDP connection. This is implemented in the Phase 3 of the project. I have assigned the static UDP port number 42927.

4. patient1.c : The patient1.c is the source file for the Patient(client). It includes setting up of two connections: TCP for Phase 1 and 2 and UDP for Phase 3. 

5. patient2.c : The patient2.c is the source file for the Patient(client). It includes setting up of two connections: TCP for Phase 1 and 2 and UDP for Phase 3. 


Compiling the program: 

I have attached a makefile in the submission. The command 'make' can be used t compile the project in one go. 
For individual compilation, the following commands may be used:

healthcenterserver: gcc -o healthcenterserver healthcenterserver.c -lsocket -lnsl -lresolv
doctor1: gcc -o doctor1 doctor1.c -lsocket -lnsl -lresolv
doctor2: gcc -o doctor2 doctor2.c -lsocket -lnsl -lresolv
patient1: gcc -o patient1 patient1.c -lsocket -lnsl -lresolv
patient2: gcc -o patient2 patient2.c -lsocket -lnsl -lresolv

Running the project:The executables for the above files should be run in the following order:
1. healthcenterserver
2. doctor1
3. doctor2
4. patient1
5. patient2

All the messages exchanged are in a character array format.

Reused Code:
I have used 'Beej's Code' as the base code for the project for only setting up TCP connections. I have referred Google for the funcationality of certain functions used like: getsockname(), getpeername() and gethostname().
I have used 'Rutgers cs dept' UDP code as the basis for setting up UDP connections.

Note: I had updated the doctor port numbers in availibilities.txt to 41927 and 42927 to run my program as i had changed the port numbers of doctor1 to 41927 and doctor2 to 42927 as told in project description/=.
