/*
** This is database. It has two parts. One is used for getting the book title
** Another is used for getting the user's request and send the location of book.
** All will use UDP connection.
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

#define MYPORT "3823"	// the port library will be connecting to
#define USERPORT "3723" // the port user will be connecting to 

#define MAXBUFLEN 100// buffer size

char titleL1[5][50],titleL2[5][50],titleL3[5][50];//array will be used for storing the book title from libraries.
/*
**The following part is used for communicating with user through UDP connection
**The code comes from Beej's Guide book
*/
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
int phase2ForDatabase()
{
	int sockfd;// sock_fd is used for listen on.
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;// Store the received byte number
	struct sockaddr_storage their_addr;//address information of connector.
        struct hostent *host;//To get the IP address by gethostbyname
	char buf[MAXBUFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];
        int recvCounterUser1,recvCounterUser2;//counter for user to receive
	int setNumber;

        char user1Title[3][50],user2Title[3][50];//the user's request's book title
        int compare1,compare2,compare3,compare4;// it is used for get the location of each book 

	setNumber = 0;
 	//the code of UDP creation comes from Beej's Guide book      
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
       
        host = gethostbyname("nunki.usc.edu");// get the ip address 
        printf("The central database has UDP port 3723 and IP address %s\n",inet_ntoa(*((struct in_addr*)host -> h_addr_list[0])));//print the ip address and static port

	if ((rv = getaddrinfo(NULL, USERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		//set socket
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}
        //bind function
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	

	addr_len = sizeof their_addr;
    //get the title from the user1 and find the location in database, then send the location to the user1
	for(recvCounterUser1 = 0;recvCounterUser1 <= 2; recvCounterUser1++){
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}
	
	buf[numbytes-1] = '\0';
	//array store the book title from the user1
	strcpy(user1Title[recvCounterUser1],buf);
      
	}
	//find the location and send the location
	for(compare1=0; compare1<=2; compare1++){
	        setNumber = 0;
		for(compare2=0;compare2<=4;compare2++){
			if(strcmp(user1Title[compare1],titleL1[compare2]) == 0){
			  if ((numbytes = sendto(sockfd,"ab", strlen("ab"), 0,(struct sockaddr *)&their_addr, addr_len)) == -1) {
		                   perror("talker: sendto");
		                   exit(1);
				}
	                        setNumber = 1;
				     
			}
            if(strcmp(user1Title[compare1],titleL2[compare2]) == 0){
			   if ((numbytes = sendto(sockfd, "abc", strlen("abc"), 0,(struct sockaddr *)&their_addr, addr_len)) == -1) {
		                   perror("talker: sendto");
		                   exit(1);
				}
	                        setNumber = 1;
			}
	        if(strcmp(user1Title[compare1],titleL3[compare2]) == 0){
		       if ((numbytes = sendto(sockfd, "abcd", strlen("abcd"), 0,(struct sockaddr *)&their_addr, addr_len)) == -1) {
		                   perror("talker: sendto");
		                   exit(1);
				}
	                       setNumber = 1;
			}
		      
		}
           if(setNumber == 0){
		           if((numbytes = sendto(sockfd, "abcde", strlen("abcde"), 0, (struct sockaddr *)&their_addr, addr_len)) == -1){
		             perror("talker:sendto");
                      exit(1);
                  }
            }
                printf("Sent location info about <%s> to <User1>.\n",user1Title[compare1]);
	}
  

    //get the title from the user2 and find the location in database, then send the location to the user1
    for(recvCounterUser2 = 0;recvCounterUser2 <= 2; recvCounterUser2++){
	   if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	   }


	buf[numbytes-1] = '\0';
	//array store the book title from the user2     
	strcpy(user2Title[recvCounterUser2],buf);
	
	}
	//find the location and send the location
	for(compare3=0; compare3<=2; compare3++){
	        setNumber = 0;
		for(compare4=0;compare4<=4;compare4++){
			if(strcmp(user2Title[compare3],titleL1[compare4]) == 0){
		       if ((numbytes = sendto(sockfd,"ab", strlen("ab"), 0,(struct sockaddr *)&their_addr, addr_len)) == -1) {
		                   perror("talker: sendto");
		                   exit(1);
				}
	                        setNumber = 1;
					
			}
            if(strcmp(user2Title[compare3],titleL2[compare4]) == 0){
			 	if ((numbytes = sendto(sockfd, "abc", strlen("abc"), 0,(struct sockaddr *)&their_addr, addr_len)) == -1) {
		                   perror("talker: sendto");
		                   exit(1);
				}
	                        setNumber = 1;

			}
	        if(strcmp(user2Title[compare3],titleL3[compare4]) == 0){
			     if ((numbytes = sendto(sockfd, "abcd", strlen("abcd"), 0,(struct sockaddr *)&their_addr, addr_len)) == -1) {
		                   perror("talker: sendto");
		                   exit(1);
				}
	                       setNumber = 1;

			}
		     
		}
        if(setNumber == 0){
		   if((numbytes = sendto(sockfd, "abcde", strlen("abcde"), 0, (struct sockaddr *)&their_addr, addr_len)) == -1){
		            perror("talker:sendto");
                    exit(1);
                  }
        }
       printf("Sent location info about <%s> to <User2>.\n",user2Title[compare3]);
	}

    printf("End of Phase 2 for the database.\n");

	close(sockfd);//close the sockfd

	return 0;

}

int main(void)
{
	int sockfd;// sock_fd is used for listen on. 
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_in cliaddr;
        struct hostent *host;//get IP address
        int rv;
	int numbytes;// Store the received byte number
	struct sockaddr_storage their_addr;//address information of connector.
	char buf[MAXBUFLEN];
	socklen_t addr_len, len;
	char s[INET6_ADDRSTRLEN];
        int recvCounter1,recvCounter2,recvCounter3;//receive counter for phase1
         

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
    //get the IP address    
        host = gethostbyname("nunki.usc.edu");       
        
        printf("The central database has UDP port 3823 and IP address %s\n",inet_ntoa(*((struct in_addr*) host->h_addr_list[0]))); 
       
	if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		//set the socket
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}
        //bind function
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}
      

	freeaddrinfo(servinfo);

	addr_len = sizeof their_addr;
    //receive the book title from library1
	for(recvCounter1 = 0;recvCounter1 <= 4; recvCounter1++){
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}
	//store the book title in array
	buf[numbytes] = '\0';
	strcpy(titleL1[recvCounter1],buf);
	
	}
	printf("Received the book list from <Library1>\n");
    
	//receive the book title from library2
	for(recvCounter2 = 0;recvCounter2 <= 4; recvCounter2++){
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}
    //store the book title in array
	buf[numbytes] = '\0';
	strcpy(titleL2[recvCounter2],buf);
	}
        printf("Received the book list from <Library2>\n");

    //receive the book title from libary3
	for(recvCounter3 = 0;recvCounter3 <= 4; recvCounter3++){
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}
    //store the book title in array
	buf[numbytes] = '\0';
	strcpy(titleL3[recvCounter3],buf);
	}
        printf("Received the book list from <Library3>\n");
        printf("End of Phase 1 for the database\n");       
	close(sockfd);

	phase2ForDatabase();//Jump to phase 2

	return 0;
}
