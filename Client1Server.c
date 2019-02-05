#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/wait.h>



#define BUFLEN 256	/* buffer length */
#define dataSize 100
#define SERVER_TCP_PORT 3000	/* well-known port */

//----------------------------------

  int echod(int);
  void reaper(int);
  

  //-------------------------------------
struct dataudp
	{
		char type;
		char data[dataSize];		
		
		
	};


struct dataudp udp(struct dataudp txu) 
// function accepts the dataudp struct and performs user requests
// so clear data in main, but we return a full struct
{	int port=10000;
	char peername[20],contentname[20],ipnumber[20],portnum[20];
	char c;
	sprintf(portnum,"%d",port);
	
	
	//set up portnum, this value cannot change and remember to close sockets
	
	
  struct dataudp rxu;  //check this txu
	//----------THIS PART CREATES THE UDP SOCKET----------------\\
  
 int udpsock, nBytes,i,n;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  
  udpsock = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&serverAddr, '\0', sizeof (serverAddr)); 

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  //------------------------------------------------------------\\
  
 

  
  while(1){
  
  	recvfrom(udpsock, &rxu, dataSize + 1, 0, (struct sockaddr *)&serverAddr,&addr_size);
  	if(rxu.type=='V'){// continuously check for the download request
  	break;//when you break from this while loop you will have the port num for the connection
  	}else{
  	continue;// else continue with the program
  	}
	  //if download complete TCP should send rxu.type of R-register
	  if(txu.type!='R'){ // there is no char assigned to type to we take input, this rxu is from MAIN
	  printf("R-register content, S-Search for Content to download, T-De-reg content, L-LIST, Q-DELETE ALL\n"); //got to add in Q
	  scanf("%c",&txu.type); // shouldbe char
	  }
	  
	  if(txu.type=='R'){
		//register pdu
			printf("Connection to index server. Enter Peer Name:\n");
			scanf("%s",peername);
			
					strcpy(txu.data, peername);
					printf("Enter Content Name:\n");
					scanf("%s",contentname);
					strcat(txu.data," ");
					strcat(txu.data,contentname);
					///////  
					printf("enter IP #\n"); 
					scanf("%s",ipnumber);
					strcat(txu.data," ");
					strcat(txu.data,ipnumber);
					
			sendto(udpsock,&txu, dataSize+ 1,0,(struct sockaddr *)&serverAddr,sizeof(serverAddr)); 
			recvfrom(udpsock, &rxu, dataSize + 1, 0, (struct sockaddr *)&serverAddr,&addr_size);
			
			if(rxu.type=='E'){			
					//print rx.data to show error message 
					printf(rxu.data);
					printf("\n returning to main menu\n");
			
					}
		
			if(rxu.type=='A'){ 
					printf("%c\n", rxu.type);
					printf("\nContent and PeerName registered, returning to main menu\n");
			
				}
		 
		  txu.type='E';
		
	  }
	  if(txu.type=='S'){
		  //search for download, if successful will break out of loop and go to TCP
			printf("Search: Enter Peer Name:\n");
			scanf("%s",peername);
			strcpy(txu.data, peername);
			
			printf("Search: Enter Content Name:\n");
			scanf("%s",contentname);
			strcat(txu.data," ");
			strcat(txu.data,contentname);
			
			
			sendto(udpsock,&txu, dataSize+ 1,0,(struct sockaddr *)&serverAddr,sizeof(serverAddr)); 
			recvfrom(udpsock, &rxu, dataSize + 1, 0, (struct sockaddr *)&serverAddr,&addr_size);
			
			if(rxu.type=='E'){			
					//print rx.data to show error message 
					printf(rxu.data);
					printf("\n returning to main menu \n");
			
					}
		
			if(rxu.type=='S'){ 
					// rxu is supposed to return to the main fxn should be able to have return rx; at end of while loop
					/*here rxu should change to rxu.type=D for the TCP header*/
					printf("%c\n", rxu.type);//rxu supposed to hold, content name, address and port, this gets passed
					printf("\n MATCH IN INDEX!\n");
					printf(rxu.data);
					printf("\n");
					rxu.type= 'D'; 
					 
					break; // should break from here to enter TCP cxn, should have a return r; 
				//////// NEED TO MEMSET SOMEWHERE
				}
			
		
		  
	  }
	  if(txu.type=='T'){//dereg specific content belonging to this server from index 
	  
			printf("Enter Content Name:\n");
			scanf("%s",contentname);
			strcpy(txu.data, contentname);

			
			strcat(txu.data," ");
			
			strcat(txu.data,portnum);
			
		    sendto(udpsock,&txu, dataSize+ 1,0,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
			// here the index server is to check for the port number which is the last string after a null or space
			//any value in the text document matching that port number inside will be removed from the index server and deleted
			
			
			recvfrom(udpsock, &rxu, dataSize + 1, 0, (struct sockaddr *)&serverAddr,&addr_size);
			// index server should send an Ack to make sure it we deleted the file 
			
			if(rxu.type=='E'){			
					//print rx.data to show error message 
					
					printf(rxu.data);
					printf("\n");
			
					}
		
			if(rxu.type=='A'){ 
					printf(rxu.data);
					printf("\n");
					printf("De-registered");
					printf("\n");
				
				}
		  
	  }
	  if(txu.type=='L'){
			int n=0;
		    	
			sendto(udpsock,&txu, dataSize+1,0,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
			printf("available content");
	while ((n = recvfrom(udpsock, &rxu, dataSize + 1, 0, (struct sockaddr *)&serverAddr,&addr_size)) >0){ // gotta CHECK THIS WHILE LOOP
		
			
			printf(rxu.data);// this isnt the same rxu as the input, this displays what list there is
			printf("\n"); // in the index server insert \n for new contents available
		
		printf("%d\n",n);
		memset(&rxu.data[0], '\0', dataSize); // check this out, this is needed to continuously read the data
				}
		
			
		  
		  
	  }
	  if(txu.type=='Q'){
		  // dereg ALL content belonging to this server
		  //delete everything in index
						
			strcpy(txu.data, "content");
			strcat(txu.data," ");
			

			strcat(txu.data,portnum);
			
		  sendto(udpsock,&txu, dataSize+1,0,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
		  recvfrom(udpsock, &rxu, dataSize + 1, 0, (struct sockaddr *)&serverAddr,&addr_size);
				if(rxu.type=='E'){			
					//print rx.data to show error message 
					
					printf(rxu.data);
					printf("\n");
			
					}
		
				if(rxu.type=='A'){ 
				//ack pdu for deletion of all content to specific address
					printf(rxu.data);
					printf("\n");
					printf("De-registered");
					printf("\n");
				
				}
		
		    
		  
	  }
	  
	  memset(&rxu.data[0], '\0', dataSize); 
	  // reset the data size every time u restart this function in the while loop
	 
  }
  close(udpsock);

  // should be a return here for the S pdu, we pass the address and contents back to TCP cxn
  return rxu;
	  
  }
	
	
	


//PEER 1
int main(int argc, char **argv){
	
//--------------------------------------------------------------------------------	

//--------------------------------------------------------------------------------
	
	

	struct dataudp tx, rx;
	char c;
		
	while(1){
	
		//call to the UDP function for udp setup
		//will return back the information for TCP connection
		tx=udp(tx);	
		
	
		char contentname[20],ipname[20],portname[20],help[100];
		sscanf(tx.data, "%s %s %s", contentname, ipname, portname); // this is the portname and ipname of the located server
	
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
	  //this part should create the TCP SOCKET
			
	int 	sd, new_sd, client_len, port;
	struct	sockaddr_in server, client;

	switch(argc){
	case 1:
		port = portname;
		break;
	case 2:
		port = atoi(argv[1]);//line does nothing
		
		break;
	default:
		fprintf(stderr, "Usage: %d [port]\n", argv[0]);
		exit(1);
	}

	/* Create a stream socket	*/	
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Can't creat a socket\n");
		exit(1);
	}

	/* Bind an address to the socket	*/
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl("127.0.0.1");//INADDR_ANY
	if (bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1){
		fprintf(stderr, "Can't bind name to socket\n");
		exit(1);
	}

	/* queue up to 5 connect requests  */
	listen(sd, 5);

	(void) signal(SIGCHLD, reaper);
//------------------------------------------------------------------------
//------------------------------------------------------------------------
		memset(&tx.data[0], '\0', dataSize); // should be zero or null
		strcpy(tx.data, contentname);
		printf("type %c\n",tx.type);
	//if returning type is D commence download
	if(tx.type=='D'){
			send(sd,&tx,101,0);
		 	//Successful connection, start transmitting C PDUs
			int n;
			int fp;
			fp=open(contentname, O_WRONLY);///
				FILE * fpp;
				printf("%s",contentname);
				fpp = fopen(contentname, "a");
			while ((n = recv(sd, &rx, 101, 0)) > 0){
				printf("here %c\n", rx.type);
				printf("%s",rx.data);
				fprintf(fpp,"%s",rx.data);
				write(fp,rx.data,100);///
					if (rx.type == 'F'){
						printf("%d",n);
						memset(&tx.data[0], '\0', dataSize);
						printf("File transfer complete!\n");
						break;
					}
				memset(&tx.data[0], '\0', dataSize);
				
			}
			//setting up parameters for the register of new file content
			tx.type='R';
			// put down the portnum and ipname for this file(peer) ip name should be the same, same with the port
			strcpy(contentname, tx.data);
			strcat(tx.data, " ");
			strcat(tx.data, ipname);
			strcat(tx.data, " ");
			strcat(tx.data,"10001");
			}
			
			else { 
				//This is to transmit the file
				recv(sd, &rx,101, 0); // info what file to send
				if(rx.type=='D'){ 
					int i = 0;
		
					sscanf(rx.data,"%s",contentname); // read filename
		
		int fp;
		fp=open(contentname, O_RDONLY);
		if(fp>0){ // open file and transmit
			while((i = read(fp, tx.data, 100))>0){
				tx.type = strlen(tx.data) * sizeof(char) < 100 ? 'F' : 'C';
				printf("%c",tx.type);
				send(sd,&tx,101,0);
				memset(&tx.data[0], '\0', dataSize);
			}
			close(fp); // close the file
		}else{// no such file exists
			printf("No file exists\n");
		}
		
	}		
  }
  close(sd);
}
	}				
		
void	reaper(int sig)
{
	int	status;
	while(wait3(&status, WNOHANG, (struct rusage *)0) >= 0);
}
