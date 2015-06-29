/*
** Patient 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "21927" // the port client will be connecting to 
#define maxbuf 2048
#define max_size 200 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;  
	char buf[max_size];
	struct addrinfo hints, *servinfo, *p;
	int rv; char s[INET6_ADDRSTRLEN]; int len_inet; int z;
	fflush(stdout); struct sockaddr_in adr_inet;	
	char ch; char file_name[25]; char k[25]; char u1[60]; char u2[60];
    FILE *fp; int i=0; char a[25],b[25],y[25]; char avail[25]; char select[100];
	int check; int app_choice=0; int loop_flag; char sel_num[10]; char doc_port[100];
	memset(select,0,100); memset(buf,0,100);
	char msg[100]; char msg_1[20]; int avail_check=0;
	memset(msg,0,100); memset(msg_1,0,20);
	char doc_info[30]; memset(doc_info,0,30); char ipp[30];
	
	if (argc != 2) {
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	fflush(stdout);
	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}
	fflush(stdout);
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);

	freeaddrinfo(servinfo); // all done with this structure

     len_inet = sizeof(adr_inet); 
		z = getsockname(sockfd, (struct sockaddr *)&adr_inet, (socklen_t*)&len_inet);  
		if ( z == -1 ) 
		{  
         return NULL; /* Failed   */
	    }

	struct hostent *lh = gethostbyname("nunki.usc.edu"); //get IP address
	
	printf("\nPhase 1: Patient 1 has TCP port number %u and IP address %s",(unsigned)ntohs(adr_inet.sin_port),inet_ntoa(*((struct in_addr *)lh->h_addr)));
	strcpy(ipp,inet_ntoa(*((struct in_addr *)lh->h_addr)));
    
	fp = fopen("patient1.txt","r"); 		// read mode
	fflush(stdout);
   if( fp == NULL )
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
 
  fgets(k,25,fp); 
  strcpy(y,k);
  strcpy(a,strtok(y," "));
  strcpy(b,strtok(NULL, " ")); 
  fflush(stdout);
  strcpy(u2,"authenticate ");
  strcat(u2,k);
  fclose(fp);
  
    if( send(sockfd,u2,60,0) == -1)
    {
        puts("Send failed");
        return 1;
    }
    printf("\nPhase 1: Authentication request from Patient 1 with username %s and password %s has been sent to the Health center server.",a,b);
	fflush(stdout);
	if ((numbytes = recv(sockfd, buf, max_size-1, 0)) == -1) 
	    perror("recv");
	buf[numbytes] = '\0';
		printf("\nPhase 1: Patient 1 authentication result: %s",buf);
	int succ,fail;
	succ=strcmp("Success",buf);
	fail=strcmp("Failure",buf);	
	
	if(fail==0)
	 {
	  printf("\nExiting....");
	  return 0;
	 } 
	else 
	 printf("\nEnd of Phase 1 for Patient 1");	
	fflush(stdout);
	
	/*
	Start of Phase 2
	*/
	
	check=strcmp(buf,"Success");
	if(check==0)
	{
		strcpy(avail,"available");
		 if( send(sockfd,avail,25,0) == -1)
		{
			puts("Send failed");
			return 1;
		}
		
		if ((numbytes = recv(sockfd, buf, max_size-1, 0)) == -1) 
			perror("recv");
		buf[numbytes] = '\0';
		
		if(buf[0]=='\0')
		 {
		  printf("\nSorry All Appointments taken!! Exiting.....");
		  close(sockfd);
		  exit(1);
		 }
		else
		 printf("\nPhase 2: The following appointments are available for Patient 1:\n%s",buf);
		
		
		memset(buf,0,100);
		
		
		do{ 
		   printf("\nPlease enter the preferred appointment index and press enter: ");
		   scanf("%d",&app_choice);
		   if(app_choice>=1 && app_choice<=6)
			{
			 loop_flag=0;
			 strcpy(select,"selection ");
			 sprintf(select,"%s%d",select,app_choice);
			 
			sprintf(sel_num,"",app_choice);
			strcat(select,sel_num);
			
			 if( send(sockfd,select,25,0) == -1)
			 {
				puts("Send failed");
				return 1;
			 }
			 fflush(stdout); recv(sockfd, buf, max_size-1, 0); fflush(stdout);    
		  
			 if ((numbytes = recv(sockfd, buf, max_size-1, 0)) == -1) {
			 perror("recv");
			}
			buf[numbytes] = '\0';
			
			fflush(stdout);
			strcpy(msg,buf);
			strcpy(msg_1,strtok(msg," "));
			avail_check=strcmp(msg_1,"Not");
			
			if(avail_check!=0)
			{
			 printf("\nPhase 2: The requested appointment is available and reserved for Patient 1. The assigned doctor port number is %s",buf);
			 strcpy(doc_info,buf);
			}
			else
			{
			 printf("\nPhase 2: The requested appointment from Patient 1 is not available. Exiting...");
			 close(sockfd);
			 return 0;
			}
		  }
		  
		   else
		    {
			 printf("\nEnter again between 1 and 6");
			 loop_flag=1;
			} 
		   
		}while(loop_flag==1);
	}
	close(sockfd);
	
	/*
		UDP starts now
	*/
	
	struct sockaddr_in myaddr, remaddr;
	int fd, slen=sizeof(remaddr);  
	int recvlen;		
	char *server = "68.181.201.3";	/* change this to use a different server */

	char doc_name[20]; char doc_p[20]; int SERVICE_PORT;
	int len; char insur_info[30];
	
	strcpy(doc_name,strtok(doc_info," "));
	strcpy(doc_p,strtok(NULL, " "));
	SERVICE_PORT=atoi(doc_p);
	
	// Reading insurance info
	
	strcpy(file_name,"patient1insurance.txt");
	fp = fopen(file_name,"r"); // read mode
	
	fflush(stdout);
   if( fp == NULL )
   {
      perror("Error while opening the patient1 file.\n");
      exit(EXIT_FAILURE);
   }
 
   fgets(insur_info, sizeof insur_info,fp);
   len = strlen(insur_info);
   if(insur_info[len-1] == '\n')
		insur_info[len-1] = '\0';
	
	/* create a socket */

	if ((fd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
		perror("error");
	/* bind it to all local addresses and pick any port number */

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(0);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}       

	memset((char *) &remaddr, 0, sizeof(remaddr));
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(SERVICE_PORT);
	if (inet_aton(server, &remaddr.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	len_inet = sizeof(adr_inet); 
	z = getsockname(fd, (struct sockaddr *)&adr_inet, (socklen_t*)&len_inet);  //get the port address.
	if ( z == -1 ) {  
		return NULL; 
	}
	
	printf("\nPhase 3: Patient 1 has a dynamic UDP port number %u and IP address %s",(unsigned)ntohs(adr_inet.sin_port),ipp);
	
	freeaddrinfo(servinfo);
	/* insurance info sent to doctor port!! */

		if (sendto(fd, insur_info, strlen(insur_info), 0, (struct sockaddr *)&remaddr, slen)==-1) {
			perror("sendto");
			exit(1);
		}
		
		printf("\nPhase 3: The cost estimation request from Patient 1 with insurance plan %s has been sent to the doctor with port number %d and IP address %s",insur_info,SERVICE_PORT,ipp);
		
		/* now receives the copay info */
		recvlen = recvfrom(fd, buf, maxbuf, 0, (struct sockaddr *)&remaddr, &slen);
                if (recvlen >= 0) {
                        buf[recvlen] = 0;	
						
					printf("\nPhase 3: Patient 1 receives %s$ estimation cost from doctor with port number %d and name %s",buf,SERVICE_PORT,doc_name);	
                }
	printf("\nPhase 3: End of Phase 3 for Patient 1");
	close(fd);
	printf("\n");
	return 0;
}
