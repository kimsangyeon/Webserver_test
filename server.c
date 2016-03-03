#include "stems.h"
#include "request.h"

// 
// server.c: A very, very simple web server
//
// To run:
//  server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

typedef struct {
  int fd;
  long size, arrival, dispatch;
} request;

long server_start;

enum {FIFO, HPSC, HPDC};

request arr_Req[1000];
thread Workerthread = {0,0,0,0};

void getargs(int *port, int *threads, int *buffers, int *alg, int argc, char *argv[])
{
  if (argc != 2) { /* You will change 2 to 5 */
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(1);
    }
  *port = atoi(argv[1]);
  *threads = 1;
  *buffers = 1;
  *alg = FIFO;
}

void consumer(request *req) {
  struct timeval dispatch;

  gettimeofday(&dispatch, NULL);
  req->dispatch = ((dispatch.tv_sec) * 1000 + dispatch.tv_usec/1000.0) + 0.5;
  requestHandle(req->fd, req->arrival, req->dispatch);
  Close(req->fd);
}

int main(int argc, char *argv[])
{
  int i,j,index;
  int listenfd, port, threads, buffers, alg, clientlen;
  int connfd[1000];
  struct sockaddr_in clientaddr;
  struct timeval arrival;
  struct timeval start;
  request req[1000];

  fd_set readSet;
  fd_set tempSet;
  int fd_max;

  getargs(&port, &threads, &buffers, &alg, argc, argv);

  listenfd = Open_listenfd(port);
  
  FD_ZERO(&readSet);
  FD_ZERO(&tempSet);
  index = 0;

  FD_SET(listenfd, &readSet);
  fd_max = listenfd;
  

  while (1) {
	tempSet = readSet;
	
	Select(fd_max+1,&readSet,NULL,NULL,NULL);
	
	for(i=0;i<fd_max+1;i++){
		if(FD_ISSET(i,&tempSet)){
			if(i==listenfd){
    			clientlen = sizeof(clientaddr);
    			connfd[index] = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
    			
				gettimeofday(&arrival, NULL);
    			req[index].fd = connfd[index];
    			req[index].arrival = ((arrival.tv_sec) * 1000 + arrival.tv_usec/1000.0) + 0.5;
				FD_SET(req[index].fd,&readSet);
				index++;
			}
			
			else{
				for(j=0;j<1000;j++){
					if(req[j].fd == i)
						consumer(&req[index]);
				}
			}
			
		}
	}		
    }
}


    


 
