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

#define BUFLEN 256	/* buffer length */
#define dataSize 100
#define SERVER_TCP_PORT 3000	/* well-known port */	

struct dataudp
	{
		char type;
		char data[dataSize];		
		
		
	};

	
struct dataudp udp(struct dataudp txu) 
{
	 char peername[20],contentname[20],ipnumber[20],portnum[20]="10011";
	char c;
	int port;
	
	port=atoi(portnum);
	//set up portnum, this value cannot change and remember to close sockets
	
	
  struct dataudp rxu;  //check this txu
	//----------THIS PART CREATES THE UDP SOCKET----------------\\
  
  int udpsock, nBytes,i,n;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  
  udpsock = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&serverAddr, '\0', sizeof (serverAddr)); 

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);// port num???
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  //------------------------------------------------------------\\
  
 
  

  
  while(1){
	  //if download complete TCP should send rxu.type of R-register
	  if(txu.type!='R'){ // there is no char assigned to type to we take input, this rxu is from MAIN
	  printf("R-register content, S-Search for Content to download, T-De-reg content, L-LIST, Q-DELETE ALL\n"); //got to add in Q
	  scanf("%c",&txu.type); 
	  }
	  
	  if(txu.type=='R'){
		
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
					///////
					
					strcat(txu.data," ");
				
					strcat(txu.data,portnum);
			
		
			sendto(udpsock,&txu, dataSize+ 1,0,(struct sockaddr *)&serverAddr,sizeof(serverAddr)); //check for n, size
			recvfrom(udpsock, &rxu, dataSize + 1, 0, (struct sockaddr *)&serverAddr,&addr_size);
			
			if(rxu.type=='E'){			
					
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
		  
			printf("Search: Enter Peer Name:\n");
			scanf("%s",peername);
			strcpy(txu.data, peername);
			///////
			printf("Search: Enter Content Name:\n");
			scanf("%s",contentname);
			strcat(txu.data," ");
			strcat(txu.data,contentname);
			//////// NEED TO MEMSET SOMEWHERE, did it at end of while loop
			
			sendto(udpsock,&txu, dataSize+ 1,0,(struct sockaddr *)&serverAddr,sizeof(serverAddr)); //check for n, size
			recvfrom(udpsock, &rxu, dataSize + 1, 0, (struct sockaddr *)&serverAddr,&addr_size);
			
			if(rxu.type=='E'){			
					
					printf(rxu.data);
					printf("\n returning to main menu \n");
			
					}
		
			if(rxu.type=='S'){ // rxu is supposed to return to the main fxn should be able to have return rx; at end of while
					/*here rxu should change to rxu.type=D for the TCP header*/
					printf("%c\n", rxu.type);//rxu supposed to hold, content name, address and port, this gets passed
					printf("\n MATCH IN INDEX!\n");
					printf(rxu.data);
					printf("\n");
					rxu.type= 'D'; 
					//not to sure about this memset, cause we wanna keep the data we pass to main
					break; // should break from here to enter TCP cxn, should have a return r; 
			
				}
			
		
		  
	  }
	  if(txu.type=='T'){//dereg specific content belonging to this server from index 
	  
			printf("Enter Content Name:\n");
			scanf("%s",contentname);
			strcpy(txu.data, contentname);

			
			strcat(txu.data," ");
			
			strcat(txu.data,portnum);
			
		    sendto(udpsock,&txu, dataSize+ 1,0,(struct sockaddr *)&serverAddr,sizeof(serverAddr)); //check for n, size
			// here the index server is to check for the port number which is the last string after a null or space
			//any text document matching that port num inside will be removed from the index server and deleted
			
			
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
	  if(txu.type=='Q'){// dereg ALL content belonging to this server
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
					printf(rxu.data);
					printf("\n");
					printf("De-registered");
					printf("\n");
				//////// NEED TO MEMSET SOMEWHERE
				}
		
		    
		  
	  }
	  
	  memset(&rxu.data[0], '\0', dataSize); // reset the data size every time u restart this function in the while loop
	  //outside the while loop i dont think it matter because u will exit function and data lost 
  }

  close(udpsock);
  // should be a return here for the S pdu, we pass the address and contents back to TCP cxn
  return rxu;
	  
  }
	
	
int main(int argc, char **argv)
{
	
	printf("TCP Connection Established\n");
	struct dataudp tx, rx;
	char c;
	flag=1;
	while(1){

	
	tx=udp(tx);		
	
						
						char contentname[20],ipname[20],portname[20];
		sscanf(tx.data, "%s %s %s", contentname, ipname, portname); // this is the portname and ipname of the located server
		
//--------------------------------------------------------------------------------
int 	n, i, bytes_to_read;
	int 	sd, port;
	struct	hostent		*hp;
	struct	sockaddr_in server;
	char	*host, *bp, rbuf[BUFLEN], sbuf[BUFLEN], filename[20];

	switch(argc){
	case 2:
		host = "127.0.0.1";
		port = port1;
		break;
	case 3:
		host = "127.0.0.1";
		port = port1;
		break;
	default:
		fprintf(stderr, "Usage: %s host [port]\n", argv[0]);
		exit(1);
	}
//////////////////////////////////////////////////////
	/* Create a stream socket	*/	
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "Can't creat a socket\n");
		exit(1);
	}

	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if (hp = gethostbyname(host)) 
	  bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);
	else if ( inet_aton(host, (struct in_addr *) &server.sin_addr) ){
	  fprintf(stderr, "Can't get server's address\n");
	  exit(1);
	}

	/* Connecting to the server */
	if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1){
	  fprintf(stderr, "Can't connect \n");
	  exit(1);
	}
/////////////////////////////////////////////////////////////



//--------------------------------------------------------------------------------

		

		
		memset(&tx.data[0], '\0', dataSize); // should be zero or null
		strcpy(tx.data, contentname);
		
	if(tx.type=='D'){
			send(sd,&tx,101,0);
		 	//Successful connection, start transmitting C PDUs
			int n;
			while ((n = recv(sd, &rx, 101, 0)) > 0){
				printf("%c\n", rx.type);
				FILE * fpp;
				fpp = fopen(contentname, "a");
				fprintf(fpp, "%s", rx.data);
					if (tx.type == 'F'){
						memset(&tx.data[0], '\0', dataSize);
						printf("File transfer complete!\n");
						break;
					}
				memset(&tx.data[0], '\0', dataSize);
				//Reconnect to index server here, send TX.type R type and TX.data content/ip/port
				//here you are writing to the file and clear mem everytime so u dont have overwriting data
			}
			//setting up parameters for the register
			tx.type='R';
			/**/// put down the portnum and ipname for this file(peer) ip name should be the same, same with the port
			strcpy(contentname, tx.data);
			strcat(tx.data, " ");
			strcat(tx.data, ipname);
			strcat(tx.data, " ");
			strcat(tx.data,"10011");
			
			tx=udp(tx);
			
	}else { //Downloading
	
	recv(sd, &rx,101, 0); // info what file to send
	if(rx.type=='D'){
		int i = 0;
	
		sscanf(rx.data,"%s",contentname); // read filename
		
		int fp;
		fp=open(contentname, O_RDONLY);
		if(fp>0){ // open file and transmit
			while((i = read(fp, tx.data, 100))>0){
				tx.type = strlen(tx.data) * sizeof(char) < 100 ? 'F' : 'C';
				printf("%s",tx.data);
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
}
return 0;
}		
