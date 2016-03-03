/*
 * client.c: A very, very primitive HTTP client.
 * 
 * To run, try: 
 *      client pintos.kumoh.ac.kr 80 /~choety/
 *
 * Sends one HTTP request to the specified HTTP server.
 * Prints out the HTTP response.
 *
 * For testing your server, you will want to modify this client.  
 *
 * When we test your server, we will be using modifications to this client.
 *
 */

#include "stems.h"
#include <time.h>
/*
 * Send an HTTP request for the specified file 
 */
void clientSend(int fd, char *filename)
{
  char buf[MAXLINE];
  char hostname[MAXLINE];

  Gethostname(hostname, MAXLINE);

  /* Form and send the HTTP request */
  sprintf(buf, "GET %s HTTP/1.1\n", filename);
  sprintf(buf, "%shost: %s\n\r\n", buf, hostname);
  Rio_writen(fd, buf, strlen(buf));
}
  
/*
 * Read the HTTP response and print it out
 */
void clientPrint(int fd)
{
  rio_t rio;
  char buf[MAXBUF];  
  int length = 0;
  int n;
  
  Rio_readinitb(&rio, fd);

  /* Read and display the HTTP Header */
  n = Rio_readlineb(&rio, buf, MAXBUF);
  while (strcmp(buf, "\r\n") && (n > 0)) {
    printf("Header: %s", buf);
    n = Rio_readlineb(&rio, buf, MAXBUF);

    /* If you want to look for certain HTTP tags... */
    if (sscanf(buf, "Content-Length: %d ", &length) == 1) {
      printf("Length = %d\n", length);
    }
  }

  /* Read and display the HTTP Body */
  n = Rio_readlineb(&rio, buf, MAXBUF);
  while (n > 0) {
    printf("%s", buf);
    n = Rio_readlineb(&rio, buf, MAXBUF);
  }
}

int main(int argc, char *argv[])
{
  int i,j,k,fileopt;
  char fname[20];
  char *host, *filename;
  int port;
  int clientfd[1000];
 
  srand(time(NULL));
  /*
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <host> <port> <filename>\n", argv[0]);
    exit(1);
    }
  */

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <host> <port> \n", argv[0]);
    exit(1);
    }
 
  host = argv[1];
  port = atoi(argv[2]);
  //filename = argv[3];

  

  for(i=0;i<1000;i++){
	clientfd[i] = Open_clientfd(host, port);
     }

  for(j=0;j<1000;j++){
	
	fileopt = 0 + (rand()%3);
	if(fileopt == 0)
		filename = strcpy(fname,"\"/output.cgi?1&5&0\"");
	else if(fileopt == 1)
		filename = strcpy(fname,"test.html");
	else if(fileopt == 2)
		filename = strcpy(fname,"index.html");
  	
  	clientSend(clientfd[j], filename);
  	clientPrint(clientfd[j]);
     }
     
  for(k=0;k<1000;k++)    
  	Close(clientfd[k]);

  exit(0);
}
