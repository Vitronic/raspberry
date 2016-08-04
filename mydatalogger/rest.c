#include <stdio.h>
#include <stdlib.h>
//#include <usr/mynetwork.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/*
 * 
 */
int
main(int argc, char** argv) {

    int sockfd, n;
    int MAXLINE =32;
    char buff[MAXLINE],recvline[MAXLINE];
    struct sockaddr_in servaddr;
    
    if(argc != 3){
        printf("usage1: httppostconnect <ipaddress> <relative_url>");
        exit(0);
    }
    
    if( (sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == -1 ){
        printf("socket: error");
        exit(0);
    }

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);  //connect to http server
    
    if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr) < 0){
        printf("port: assigned invalid");
        exit(0);
    }
    if( connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1){
        printf("connect: error");
        exit(0);
    }

    
    //Sending Http header
    bzero(&buff,sizeof(buff));
    
    //char postdata[] = "param1=value1&param2=value";  /* Change param and value here */
   char postdata[] = "{{\"type_ev\":\"Pilatis-Minuten\",\"userName\":\"Rafael\",\"points_ev\":\"1000\",\"date_ev\":\"2017-01-01\"}}";  

    sprintf(buff,"POST %s HTTP1.1\r\nAccept: */*\r\nReferer: <REFERER HERE>\r\nAccept-Language: en-us\r\nContent-Type: application/json\r\nAccept-Encoding: gzip,deflate\r\nUser-Agent: Mozilla/4.0\r\nContent-Length: %d\r\nPragma: no-cache\r\nConnection: keep-alive\r\n\r\n%s",argv[2],strlen(postdata),postdata);
    
    if(write(sockfd,buff,strlen(buff)+1) == 1)
	{  printf( "contacting %s on port %d\n", buff, servaddr.sin_port );
	exit(0);
	}
    
    while ((n =read(sockfd,recvline,sizeof(recvline))) > 0){
        recvline[n] = 0;
        if(fputs(recvline,stdout) == EOF)
             printf("read error");            
    }
    return (EXIT_SUCCESS);
}