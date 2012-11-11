/*
** Library3.c has two parts. One is used for giving the book titles to the database through
** UDP connection. Another is used for providing the description of book to the users through
** TCP connection. One works as client. Another works as server.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define SERVERPORT "3823"	// Library will connect this port to giving the book titles
#define MYL3PORT "21923"// Library's static port to establish TCP connection
#define MAXDATASIZE 100// The length of buffer

#define BACKLOG 10	 // how many pending connections queue will hold

char *sendBookInfo[5];//Store book information
char *bookTitle[5];//Store book title
/*
** This part works as server. It creates the TCP connection and communicate with user
** through TCP connection
*/

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int TCPforPhase3()
{
	int sockfd, new_fd;   // sock_fd is used for listen on. new_fd is used for new connection.
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; //address information of connector.
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;
        int numbytes;// Store the received byte number
        char buf[MAXDATASIZE];
        char recvBookTitle[50];//store the received book title
        int compareCounter;
        char recvInfo[50];//store the received book information
        char recvNumberOfUser[2];//store the number of user who does the request
        struct hostent *host;//store the IP address
        host = gethostbyname("nunki.usc.edu");//get the IP address
        printf("<Library3> has TCP port 21923 and IP address %s for phase 3\n", inet_ntoa(*((struct in_addr*)host -> h_addr_list[0])));   
	//the code of TCP creation comes from Beej's Guide book
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // it means use my own IP

	if ((rv = getaddrinfo("nunki.usc.edu", MYL3PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop will help through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
				//set socket
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
        //bind function
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
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

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
    //this is accept() loop
	while(1) {  
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
			if (new_fd == -1) {
			continue;
			}
                
		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener
	       	if ((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) {
	             perror("recv");
		         exit(1);
			}


        	buf[numbytes] = '\0';
               strcpy(recvBookTitle,buf);
               recvBookTitle[numbytes] = '\0';//it has already received the book title
            //find the information of received book and send it                
                
	        for(compareCounter = 0; compareCounter <= 4; compareCounter++){
				 if(strcmp(recvBookTitle,bookTitle[compareCounter]) == 0){
			
                    if(send(new_fd,sendBookInfo[compareCounter],strlen(sendBookInfo[compareCounter]), 0) == -1)
		                perror("send");
                    //receive the number of user
                    if((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1){
		               perror("recv");
                       exit(1);
                    }
                     printf("<Library 3> received query for <%s> from <User %d>\n",recvBookTitle, numbytes-1);
		    printf("<Library 3> sent the description of <%s> to <User %d>\n", bookTitle[compareCounter], numbytes-1);
		    }
		 }
						   
			close(new_fd);
			exit(0);
		}
		close(new_fd);  // parent doesn't need this
	}

	return 0;
}
/*
**The main function is phase 1 for sending the title to Database through UDP
*/

int main(int argc, char *argv[])
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	struct hostent *host;//used for gethostbyname function
        struct sockaddr_in sin;
	int rv,len;
	int numbytes;// received bytes number
	char bookInfo[5][300];//store the book information
        int i,j;//counter
        int sentCounter;// used for counting the send time
	int loopJump;
    //Read the library file to send the title to database.
	FILE *fp1;
	//get the IP address
	host =  gethostbyname("nunki.usc.edu");
	//Open the file		
    if((fp1 = fopen("library3.txt", "r")) == NULL){
		printf("Could not open library3.txt\n");
		exit(1);
	}
    //get each line information in the file
	for(i = 0; i <= 4; i++){
	fgets(bookInfo[i], 300, fp1);
        fseek(fp1,0,SEEK_CUR);
	}
    //store the book title into array
	for(j = 0; j <=4; j++){
           bookTitle[j]=strtok(bookInfo[j],"#");
	   sendBookInfo[j]=strtok(NULL,"#");
	}

    fclose(fp1);
    //the code of establishing the UDP comes from the Beej's Guide book
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo("nunki.usc.edu", SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop is help to make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	}
        //bind function
        bind(sockfd,(struct sockaddr *)&sin,sizeof(sin));
        len = sizeof(sin);

	if (p == NULL) {
		fprintf(stderr, "talker: failed to bind socket\n");
		return 2;
	}

        loopJump = 0;
		//Send the book title to the database through the UDP
	for(sentCounter = 0; sentCounter <= 4; sentCounter++){
	if ((numbytes = sendto(sockfd, bookTitle[sentCounter], strlen(bookTitle[sentCounter]), 0,
			 p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}//use getsockname to get the port number.
        if(loopJump == 0){
	      getsockname(sockfd,(struct sockaddr *)&sin, &len);
              printf("<Library 3> has UDP port %d and IP address %s for Phase 1\n",ntohs(sin.sin_port),inet_ntoa(*((struct in_addr*)host->h_addr_list[0])));
              loopJump = 1;
	  }
        printf("<Library3> has sent <%s> to the database.\n",bookTitle[sentCounter]);
	}
        printf("Updating the database is done for <Library3>\n");
        printf("End of Phase1 for <Library3>.\n");

	freeaddrinfo(servinfo);

	close(sockfd);

	TCPforPhase3();//Jump to the function for phase 3

	return 0;
}
