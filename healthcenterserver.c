/*
** Hospital Server
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "21927"  // the port users will be connecting to
#define Max_Queue 10	 // how many pending connections queue will hold
#define max_size 100 // max number of bytes we can get at once 

int res_flag[6];

int check_res(int res)		//This checks if appointment is reserved or not
 {
  if(res_flag[res]==0)
   return 0;
  else
   return 1;  
 }

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
	int sockfd, new_fd;  // listening on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1; char s[INET6_ADDRSTRLEN];
	int rv; char buf[max_size];
	int len; memset(buf, 0, max_size);

	int n=0;
	char server_reply[2000]; char patient_ip[10];

	FILE *fp; int i=0; char u[30]; char u1[30]; char u2[30]; char file_name[25];
	char a[30], b[30], c[30], d[30], e[30];char p1[30]; char p2[30];
	struct sockaddr_in adr_inet;
	int len_inet;
	int z; int auth,pat1succ,pat2succ,t3,t4;
	char pat_username[50]; char pat_pass[50];
	int check;
	memset(u, 0, 30); memset(u1, 0, 30); memset(u2, 0, 30); memset(file_name, 0, 25);
	memset(p1, 0, 30); memset(p2, 0, 30);
	memset(a, 0, 30); memset(b, 0, 30); memset(c, 0, 25); memset(d, 0, 30); memset(e, 0, 30);
	char avail_lines[6][30]; char avail_file[200]; char avail_all_lines[6][30];
	char avail_tokens[5][20]; char first3[6][100]; char last2[6][100]; char doc_port[100];
	int count; int tok; 
	memset(avail_file, 0, 200); memset(first3, 0, sizeof(first3)); memset(last2, 0, sizeof(last2));

	char select_word[30],select_num[10]; int selection=0; int is_avail=0;
	memset(select_word,0,30); memset(select_num,0,30);
	
	strcpy(file_name,"users.txt");
	fp = fopen(file_name,"r"); // reading the file
	
	fflush(stdout);
   if( fp == NULL )
   {
      perror("Error while opening the users file.\n");
      exit(EXIT_FAILURE);
   }
 
   fgets(u, sizeof u,fp);
   fgets(u2, sizeof u2,fp);
   len = strlen(u);
   fflush(stdout);
   if(u[len-1] == '\n')
		u[len-1] = '\0';
	
	strcpy(u1,u);
     fflush(stdout);
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt"); 
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, Max_Queue) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) 
	{
		perror("sigaction");
		exit(1);
	}
	
	len_inet = sizeof(adr_inet); 
		z = getsockname(sockfd, (struct sockaddr *)&adr_inet, (socklen_t*)&len_inet);  
		if ( z == -1 ) 
		{  
         return NULL; /* Failed   */
	    }

		struct hostent *lh = gethostbyname("nunki.usc.edu"); //get IP address
		
		printf("\nPhase 1: The Health center server has port number %u and IP address %s",(unsigned)ntohs(adr_inet.sin_port),inet_ntoa(*((struct in_addr *)lh->h_addr)));
	
	/*
		Reading file and storing stuff	*/
		
	strcpy(file_name,"availabilities.txt");
			fp = fopen(file_name,"r"); // read mode
	
			   if( fp == NULL )
			   {
				  perror("Error while opening the users.txt file.\n");
				  exit(EXIT_FAILURE);
			   }
		for(count=0; count<6;count++)	//reading the lines from Availabilities.txt and storing them 
		 {
		  fgets(avail_lines[count], sizeof avail_lines[count],fp);
		  strcpy(avail_lines[count],strtok(avail_lines[count],"\n"));
		 }
		
		for(count=0; count<6;count++) 
		 strcpy(avail_all_lines[count],avail_lines[count]);
	
	while(1) {  // main accept() loop
		
		for(count=0; count<6;count++) 
		 strcpy(avail_all_lines[count],avail_lines[count]);
		
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}
		fflush(stdout);
		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
	//	strcpy(patient_ip,s);
		
		
		fflush(stdout);

			if(recv(new_fd,buf,max_size-1,0)==-1)
			 perror("recv");
						
			strcpy(d,"authenticate");
			strcpy(a,strtok(buf," "));
			fflush(stdout);
			 strcpy(b,strtok(NULL, " "));
		     strcpy(c,strtok (NULL, " "));
			 fflush(stdout);
			
			 auth=strcmp(a,d);
			
			fflush(stdout);
			printf("\nPhase 1: The Health center server has received request from a patient with username %s and password %s.",b,c);
			strcpy(e,b);
			strcat(b," ");
			strcat(b,c);
			 
			if(auth==0)		// checking authentication
			 {
			   pat1succ=strcmp(b,u1);
			   len = strlen(u2);
   
				if(u2[len-1] == '\n')
					u2[len-1] = '\0';

				pat2succ=strcmp(b,u2);
		     
			   if((pat1succ==0) || (pat2succ ==0)) 
			    {
				fflush(stdout);
				 printf("\nPhase 1: The Health center server sends the response Success to patient with username %s",e);
				 if (send(new_fd, "Success", 10, 0) == -1)
				  perror("send");
				} 
			   else
			    {
				fflush(stdout);
				 printf("\nPhase 1: The Health center server sends the response Failure to patient with username %s\n",e);
				 if (send(new_fd, "Failure", 10, 0) == -1)
				  perror("send");
				}
			 } 
			strcpy(pat_username,e);
			strcpy(pat_pass,c);
			
/*
	Start of Phase 2
						*/
						
		if((pat1succ==0) || (pat2succ ==0))
		{
			memset(avail_file, 0, 200); memset(first3, 0, sizeof(first3)); memset(last2, 0, sizeof(last2));

			for(count=0; count<6;count++)			
			  {
			   												//tokenizing now
			   strcpy(avail_tokens[0],strtok(avail_all_lines[count]," "));
			   strcpy(avail_tokens[1],strtok(NULL, " "));											
			   strcpy(avail_tokens[2],strtok(NULL, " "));
			   strcpy(avail_tokens[3],strtok(NULL, " "));
			   strcpy(avail_tokens[4],strtok(NULL, " "));
			    
			for(tok=0;tok<3;tok++)
				{
			     strcat(first3[count],avail_tokens[tok]);		//first3 has only 3 columns //last2 has only last 2 columns
				 if(tok<2)
				  strcat(first3[count]," ");
				}
				
				is_avail=check_res(count);
			  if(is_avail==0)	
			   strcat(avail_file,first3[count]);		// concatenated file only first 3 columns
			  else
				continue;
				
			if(count<5)
			   strcat(avail_file,"\n");
			   
			    for(tok=3;tok<5;tok++)
				{
			     strcat(last2[count],avail_tokens[tok]);
				 if(tok<4)
				  strcat(last2[count]," ");
				}
			  }
			
			if(recv(new_fd,buf,max_size-1,0)==-1)
			perror("recv");
			
			check=strcmp(buf,"available");
			if(check==0)
			 {
			 
			 z = getpeername(new_fd, (struct sockaddr *)&adr_inet, (socklen_t*)&len_inet);  // Get the IP address and Port Number
				if ( z == -1 ) {  
					return NULL; 
				}
			 
			  printf("\nPhase 2: The Health center server receives a request for available time slots from patient with port number %u and ip address %s",(unsigned)ntohs(adr_inet.sin_port),inet_ntoa(*((struct in_addr *)lh->h_addr)));
			  
			  if (send(new_fd, avail_file, 200, 0) == -1)
				  perror("send");
			  printf("\nPhase 2: The Health center server sends available time slots to patient with username %s",pat_username);
			 
			 if(recv(new_fd,buf,max_size-1,0)==-1)
				perror("recv");
			 
			 strcpy(select_word,strtok(buf," ")); 
			 strcpy(select_num,strtok(NULL," ")); 
			 selection=atoi(select_num);
			 
			 check=strcmp(select_word,"selection");
			 
			  if(check==0)		//if selection keyword present
			   {
			    printf("\nPhase 2: The Health center server receives a request for appointment %d from patient with port number %u and username %s",selection,(unsigned)ntohs(adr_inet.sin_port),pat_username);
																	//check if reserved
				is_avail=check_res(selection-1);	
				if(is_avail==0)										//open slot
				 {
				  res_flag[selection-1]=1;
				  printf("\nPhase 2: The Health center server confirms the following appointment %d to patient with username %s",selection,pat_username);
			  
				  strcpy(doc_port,last2[selection-1]);
				  strcpy(doc_port,strtok(doc_port,"\n"));
				  fflush(stdout);send(new_fd, doc_port, 100, 0);fflush(stdout);
				  
				  if (send(new_fd, doc_port, 100, 0) == -1)
					perror("send");
				}
				
				else												//reserved slot
				 {
				  printf("\nPhase 2: The Health center server rejects the following appointment %d to patient with username %s",selection,pat_username);
				  if (send(new_fd,"Not available", 100, 0) == -1)
					perror("send");
				 }
			   } 
			 }
		}	 
			close(new_fd);
		printf("\n");
	}
	return 0;
}
