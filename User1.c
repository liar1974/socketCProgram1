/*
** User will get the location from the database through UDP and get the description of
** each book from library through TCP.Some part of code comes from Beej's Guide book
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

#define MAXBUFLEN 100 
#define SERVERPORT "3723"	// the port users will connect with database
#define L1PORT "21723"// the port library1 will use
#define L2PORT "21823"// the port library2 will use
#define L3PORT "21923"// the port library3 will use
#define MAXDATASIZE 100

char queriesInfo[3][50];// the array will store queries information
int libraryNumber[3];// the array will store the location

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);

}

//User wants to get book information from library1 through TCP
int userToL1(char sendQueriesInfo[])
{
	int sockfd, numbytes;  // sock_fd is used for listen on and numbytes will store the received number of byte
	char buf[MAXDATASIZE];//buffer
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
        struct hostent *host;//store the IP address
        socklen_t len;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;


	if ((rv = getaddrinfo("nunki.usc.edu", L1PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		//set socket
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
        //connect function
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}
	host = gethostbyname("nunki.usc.edu"); //get the IP address

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure

    //send the book title to library1
	if (send(sockfd, sendQueriesInfo, strlen(sendQueriesInfo), 0) == -1)
		perror("send");
    //receive the description of book
	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	    perror("recv");
	    exit(1);
	}

	buf[numbytes-1] = '\0';
	//getsockname to get the port number
	getsockname(sockfd,p->ai_addr, &len);
        printf("<User1> has TCP port %d and IP address %s\n",ntohs(((struct sockaddr_in *)(p->ai_addr))->sin_port),inet_ntoa(*((struct in_addr*)host->h_addr_list[0])));
        printf("<User 1> is now connected to <Library 1>\n");
	printf("Send a query for <%s> to <Library 1>\n", sendQueriesInfo);
        printf("<%s> in <Library 1> with description <%s>\n", sendQueriesInfo,buf);
    //"ab" means user1 
    if (send(sockfd, "ab", strlen("ab"),0) == -1)
	  perror("send");
	
	close(sockfd);

	return 0;
}
//User wants to get book information from library2 through TCP
int userToL2(char sendQueriesInfo[])
{
	int sockfd, numbytes; // sock_fd is used for listen on and numbytes will store the received number of byte 
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	struct hostent *host;//Store the IP address
        socklen_t len;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo("nunki.usc.edu", L2PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
   	//set socket
		if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
    //connect function
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	host = gethostbyname("nunki.usc.edu"); 


	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}
              
	freeaddrinfo(servinfo); // all done with this structure

    //send the book title to library2
	if (send(sockfd, sendQueriesInfo, strlen(sendQueriesInfo), 0) == -1)
		perror("send");
    //receive the description of book
	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	    perror("recv");
	    exit(1);
	}

	buf[numbytes-1] = '\0';
	// getsockname to get the port number
	getsockname(sockfd,p->ai_addr, &len);
        printf("<User1> has TCP port %d and IP address %s\n",ntohs(((struct sockaddr_in *)(p->ai_addr))->sin_port),inet_ntoa(*((struct in_addr*)host->h_addr_list[0])));
        printf("<User 1> is now connected to <Library 2>\n"); 
	printf("Send a query for <%s> to <Library 2>\n", sendQueriesInfo);
        printf("<%s> in <Library 2> with description <%s>\n", sendQueriesInfo,buf);
    //"ab" means user1 
    if (send(sockfd, "ab", strlen("ab"),0) == -1)
	       perror("send");	
	close(sockfd);

	return 0;
}
//User wants to get book information from library3 through TCP
int userToL3(char sendQueriesInfo[])
{
	int sockfd, numbytes; // sock_fd is used for listen on and numbytes will store the received number of byte  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
        struct hostent *host;//store the IP address
	socklen_t len;	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo("nunki.usc.edu", L3PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		//set socket
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
        //connect function
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	host = gethostbyname("nunki.usc.edu"); //get the IP address

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

          
	freeaddrinfo(servinfo); // all done with this structure


   
    //send the book title to library3
	if (send(sockfd, sendQueriesInfo, strlen(sendQueriesInfo), 0) == -1)
		perror("send");
    //receive the description of book
	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	    perror("recv");
	    exit(1);
	}

	buf[numbytes-1] = '\0';
	getsockname(sockfd,p->ai_addr, &len);//getsockname to get the port number
        printf("<User1> has TCP port %d and IP address %s\n",ntohs(((struct sockaddr_in *)(p->ai_addr))->sin_port),inet_ntoa(*((struct in_addr*)host->h_addr_list[0])));
        printf("<User 1> is now connected to <Library 3>\n");
        printf("Send a query for <%s> to <Library 3>\n", sendQueriesInfo);
        printf("<%s> in <Library 3> with description <%s>\n", sendQueriesInfo,buf);
    //"ab" means user1 
    if (send(sockfd, "ab", strlen("ab"),0) == -1)
	  perror("send");	
	close(sockfd);

	return 0;
}

//User get the location from database
int main(int argc, char *argv[])
{
	int sockfd;// sock_fd is used for listen on
	struct addrinfo hints, *servinfo, *p;
	int rv,len;
	int numbytes;// received byte number
        struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
        socklen_t addr_len;
        struct hostent *host;//used for gethostbyname function
	int loopJump;//used for jumping out the loop
	struct sockaddr_in sin;
        int i,j;//Counter for loop
        int sentCounter,recvCounter, printCounter, connectCounter;//Counter for loop
    //open the queries1.txt
	FILE *fp1;
	host =  gethostbyname("nunki.usc.edu");
    if((fp1 = fopen("queries1.txt", "r")) == NULL){
		printf("Could not open queries1.txt\n");
		exit(1);
	}
    //get the each line information in queries1.txt
	for(i = 0; i <= 2; i++){
	   fgets(queriesInfo[i], 50, fp1);
       fseek(fp1,0,SEEK_CUR);
	}

    //close the file
    fclose(fp1);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo("nunki.usc.edu", SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		//set the socket
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
    //send the book title to the database 
	for(sentCounter = 0; sentCounter <= 2; sentCounter++){
	if ((numbytes = sendto(sockfd, queriesInfo[sentCounter], strlen(queriesInfo[sentCounter]), 0, p->ai_addr, p->ai_addrlen)) == -1) {
		perror("talker: sendto");
		exit(1);
	}//getsockname to get the port number
	if(loopJump == 0){
	      getsockname(sockfd,(struct sockaddr *)&sin, &len);
          printf("<User 1> has UDP port %d and IP address %s\n",ntohs(sin.sin_port),inet_ntoa(*((struct in_addr*)host->h_addr_list[0])));
          loopJump = 1;
	  }
	 queriesInfo[sentCounter][numbytes-1]='\0';
	 printf("Checking <%s> in the database.\n",queriesInfo[sentCounter]);
	}
        
        printf("Completed book queries to the database from <User1>.\n");
        addr_len = sizeof their_addr;
    //receive the location and store the location into array  
    for(recvCounter = 0; recvCounter <= 2; recvCounter++){
       	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
		}
        libraryNumber[recvCounter] = numbytes-1;
	}
    //print some information on the screen
	for(printCounter = 0; printCounter <= 2; printCounter++)
        printf("Received location info of <%s> from the database.\n",queriesInfo[printCounter]);

        printf("End of Phase 2 for <User1>.\n");

	freeaddrinfo(servinfo);


	close(sockfd);//close the socket
	//for different to jump to different function
	for(connectCounter = 0; connectCounter <= 2; connectCounter++){
		if(libraryNumber[connectCounter] == 1)
	        userToL1(queriesInfo[connectCounter]);
		else if(libraryNumber[connectCounter] == 2)
			userToL2(queriesInfo[connectCounter]);
		else if(libraryNumber[connectCounter] == 3)
			userToL3(queriesInfo[connectCounter]);
		else
			printf("There is no book named <%s> in these three libraries.\n", queriesInfo[connectCounter]);
	}
        printf("End of Phase 3 for <User 1>\n");

	return 0;
}
