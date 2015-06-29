/*
	Doctor 2 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define max_size 1024
#define SERVICE_PORT 42927

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char **argv)
{
	struct sockaddr_in myaddr;	/* our address */
	struct sockaddr_in remaddr;	/* remote address */
	socklen_t addrlen = sizeof(remaddr);		
	int recv_size; 
	int fd;	char s[INET6_ADDRSTRLEN];
	unsigned char buf[max_size];	
	struct sockaddr_storage their_addr;

	FILE *fp; char file_name[25]; int len1,len2;
	char insur_1[30];char insur_2[30];char insur_3[30];
	memset(insur_1,0,30);memset(insur_2,0,30);memset(insur_3,0,30);
	char insur_1name[20],insur_1no[10],insur_2name[20],insur_2no[10],insur_3name[20],insur_3no[10];
	int ch1,ch2,ch3; char new_buf[100];
	
	strcpy(file_name,"doc2.txt");
	fp = fopen(file_name,"r"); // read mode
	
	struct hostent *lh = gethostbyname("nunki.usc.edu"); //get IP address
	
	printf("\nPhase 3: Doctor 2 has a static UDP port %d and IP address %s",SERVICE_PORT,inet_ntoa(*((struct in_addr *)lh->h_addr)));
	
	fflush(stdout);
   if( fp == NULL )
   {
      perror("Error while opening the doctor file.\n");
      exit(EXIT_FAILURE);
   }
 
   fgets(insur_1, sizeof insur_1,fp);
   fgets(insur_2, sizeof insur_2,fp);
   fgets(insur_3, sizeof insur_3,fp);
   len1 = strlen(insur_1);
   len2 = strlen(insur_2);
   fflush(stdout);
   if(insur_1[len1-1] == '\n')
		insur_1[len1-1] = '\0';
	
	if(insur_2[len2-1] == '\n')
		insur_2[len2-1] = '\0';
	
	fflush(stdout);
	
	strcpy(insur_1name,strtok(insur_1," "));
	strcpy(insur_1no,strtok(NULL, " "));
	
	strcpy(insur_2name,strtok(insur_2," "));
	strcpy(insur_2no,strtok(NULL, " "));
	
	strcpy(insur_3name,strtok(insur_3," "));
	strcpy(insur_3no,strtok(NULL, " "));

	/* create a UDP socket */

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}

	/* bind the socket to any valid IP address and a specific port */

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(SERVICE_PORT);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}
	
	while(1)
  {
		recv_size = recvfrom(fd, buf, max_size, 0, (struct sockaddr *)&remaddr, &addrlen);
		
		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&remaddr),
			s, sizeof s);
		
		(int)ntohs(((struct sockaddr_in *)(&remaddr))->sin_port);
		
		if (recv_size > 0) {
			buf[recv_size] = 0;
			printf("\nPhase 3: Doctor 2 receives the request from the patient with port number %d and insurance plan  %s",(int)ntohs(((struct sockaddr_in *)(&remaddr))->sin_port), buf);
		}
		else
			printf("uh oh - something went wrong!\n");
		
		ch1=strcmp(buf,insur_1name);
		ch2=strcmp(buf,insur_2name);
		ch3=strcmp(buf,insur_3name);
		
		if(ch1==0)
			  strcpy(new_buf,insur_1no);
		if(ch2==0)
			  strcpy(new_buf,insur_2no);
		if(ch3==0)
			  strcpy(new_buf,insur_3no);
			
		if (sendto(fd,new_buf, strlen(new_buf), 0, (struct sockaddr *)&remaddr, addrlen) < 0)
			perror("sendto");
		printf("\nPhase 3: Doctor 2 has sent estimated price %s$ to patient with port number %d",new_buf,(int)ntohs(((struct sockaddr_in *)(&remaddr))->sin_port));	
  }	
	close(fd);
	printf("\n");
	return 0;
}