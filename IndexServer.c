

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
#include <stddef.h>



#define BUFLEN 256 	/* buffer length */
#define dataSize 100 	
struct dataudp
	{
		char type;
		char data[dataSize];		
		
		
	};
	int portnum=10000;
// remember to declare fxns at top
struct dataudp  RandS(struct dataudp rx){ // RX should be S(download we want to find a match so we read only first while loop) or R 
	struct dataudp sx,rrx;
	char peername[20],contentname[20],ipname[20],portname[20];
	char peername1[20],contentname1[20],ipname1[20],portname1[20];
	char string[100];
	int empty;
			FILE* fp =fopen("IndexServer.txt","r");
			
			empty = 1;
			printf(rx.data);
		
		if(fp != NULL){
			while(!feof(fp)){
				
				fgets(string,100,fp);
				empty = 0;
				//scan fgets for the existing peer, content, and address
				sscanf(string,"%s %s %s %s",peername,contentname,ipname,portname);
		
				if(rx.type=='R'){
					//scan the received data
					sscanf(rx.data,"%s %s %s",peername1,contentname1,ipname1);
				}
				if(rx.type=='S'){
					sscanf(rx.data,"%s %s",peername1,contentname1);
					
				}	
				
				//compare what is in file to received data
				if(strcmp(contentname,contentname1)==0){
					if(strcmp(peername,peername1)==0){
						//if content and peer name exist is an error for R type
						//if content and peer name exist is good for S type
						if(rx.type=='R'){
						rrx.type='E';
						strcpy(rrx.data, "Content/Peer Exist Choose other ");
						break; // found a match break out of the loop error
						}
						if(rx.type =='S'){ // transmit the address back and port for search
						sx.type='S';
						strcpy(sx.data, contentname);
						strcat(sx.data, " ");
						strcat(sx.data, ipname);
						strcat(sx.data, " ");
						strcat(sx.data, portname);
						FILE*fd=fopen("DownloadRequest.txt","w");
						fprintf(fp,"1 %s",portname);// 1 requests a download
						break;	//break out of loop to continue download
						}
					}
					else{ // same content diff peer
						if(rx.type=='R'){
							rrx.type='A';
							strcpy(rrx.data, "\n Success, same content different peer valid, peer that has that content ");
							strcat(rrx.data, peername);
							strcat(rrx.data, "\n");
						}
						if(rx.type=='S'){
							sx.type='E';
							strcpy(sx.data, "\n SEARCH DNE \n");

							
						}
					}
					
				}else{ //if content doesn't exist then can register
						if(rx.type=='R'){
							rrx.type='A';
							strcpy(rrx.data, "\n Success, content DNE \n");
						}
						if(rx.type=='S'){
							//if search doesn't exist return error
							sx.type='E';
							strcpy(sx.data, "\n SEARCH DNE \n");							
						}
				}
				
				
				
			}
		}else{printf("couldnt open file");}
			if (empty == 1){
				printf("File Empty\n");
				if(rx.type=='R'){
					rrx.type='A';
					
				}
				if(rx.type=='S'){
					sx.type='E';
					strcpy(sx.data, "\n SEARCH DNE \n");							
				}
			}
			
			fclose(fp);
			printf("%c",rrx.type);
		
			if(rx.type=='R' && rrx.type=='E'){// THIS IS FOR THE REGISTER return back E-type with ERRMSG
				return rrx;
				
			}
			if(rx.type=='S' && sx.type=='S'){//THIS is to return portnum and address immediately 
				return sx;
			}
			if(rx.type=='S' && sx.type=='E'){//THIS is to return error on download request 
				return sx;
			}
			if(rx.type=='R' && rrx.type=='A'){// SENDS acknowledge PDU after we register to index file 
				
			FILE* fp =fopen("IndexServer.txt","a");
					strcat(rx.data," ");
					sprintf(portname1,"%d",portnum);
					strcat(rx.data,portname1);
			fprintf(fp,"\n %s",rx.data); 
					
			fclose(fp);
			printf(rrx.data);
			strcpy(rrx.data, "\n Success, content DNE \n");
			portnum++; //increment the global counter for port number
				return rrx; // return back acknowledge pdu
			}
			
			
	
	
}

struct dataudp QandT(struct dataudp rx){
	char peername[20],contentname[20],ipname[20],portname[20];
	char peername1[20],contentname1[20],ipname1[20],portname1[20];
	char string[100];
	struct dataudp tx;
	char *buffer;
	char * ptr;
	buffer= (char *)malloc(1000*sizeof(char)); //Create buffer to hold non deleted data
	ptr = buffer;
	FILE* fp =fopen("IndexServer.txt","r"); // read and write 
	while(fgets(string,100,fp)){	//this while loop is meant to read the data from the file
				
				
				
				/////////////
				sscanf(string,"%s %s %s %s",peername,contentname,ipname,portname);

				///////////////////////////
				sscanf(rx.data,"%s %s",contentname1,portname1);
			   	////////////////////////////////
				if(rx.type== 'Q'){
					if(strcmp(portname,portname1)==0){ // if they same port number don't copy into buffer
					
					}else{
						strcpy(ptr,string);	// different ports copy into buffer	
					
				   }
				}
				
				if(rx.type== 'T'){ // if content the same and port num the same don't copy into buffer
				if((strcmp(contentname,contentname1)==0) && (strcmp(portname,portname1)==0)){
					//not copying into the buffer deletes that line
				}else{
					strcpy(ptr,string);				
					
				}
				}
				
				
			}
			
			fclose(fp);// close the old file with all the port/content
			
			int ret= remove("IndexServer.txt"); // delete the file 
			 //open new file with same name
			fp =fopen("IndexServer.txt","w"); // now opening same file to rewrite over, without removed ports/contents
			fprintf(fp,"%s",buffer); // write buffer to file
			fclose(fp);
			
			
			//acknowledgment pdu's
			if(rx.type=='Q'){
					tx.type='A'; 
					strcpy(tx.data, "\n Removed All \n");

			}
			if(rx.type=='T'){
					tx.type='A'; 
					strcpy(tx.data, "\n Removed Specific Content \n");

			}
		
			return tx;
	
	
	
}

int main(){
	int port=0;
	char c[10];
	char c1;
	printf("Enter the udp port num:");
	scanf("%d",&port);
	// sestup connection to specific port
	///////////////////////////////////////////
	int udpsock;
	char string[100];
	struct sockaddr_in server, client;
	socklen_t addr_size; 
	
	udpsock=socket(AF_INET,SOCK_DGRAM,0);
	memset(&server, '\0', sizeof(server)); // declare it in the other functions too, may have to pass size of server as a parameter
	
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(port);// connect to the first peer at address 10001 //atoi(argv[1])
	
	bind(udpsock,(struct sockaddr*)&server, sizeof(server)); 
	////////////////////////////////////////////////
	struct dataudp tx, rx;

	addr_size=sizeof(client);
	
	
	while(1){
	
	
			// continuously check the file for the download request,if its 1 send pdu with desired 
			//portnum found in the file description 2nd word	
		FILE*fdown;
		fdown=fopen("DownloadRequest.txt","r");
		fgets((c,10,fdown)!=NULL);
		sscan(c,"%c %s",c1,tx.data);
			if(c1=='1'){
				printf("%s",tx.data);
				tx.type='V';// send a false type to avoid D 
				sendto(udpsock,&tx, dataSize+ 1,0,(struct sockaddr *)&client,addr_size);
				fdown=fopen("DownloadRequest.txt","w");
				fprintf(fdown,"0");// reset the line to 0, no download request
		}else {
		tx.type='B';// dummy pdu to continue functions on client server side
		sendto(udpsock,&tx, dataSize+ 1,0,(struct sockaddr *)&client,addr_size);
		
		}
		fclose(fdown);
		memset(&tx.data[0], '\0', dataSize);
		
		recvfrom(udpsock, &rx, dataSize+1, 0, (struct sockaddr *)&client,&addr_size);
			
		
		
		
	if(rx.type=='R'){
			//check the file first then write to it, check if the file exists first 	
			//3) 1 file and each row of file contains the parameters for that one content, need to read file and read row by row
			tx=RandS(rx);
			sendto(udpsock,&tx, dataSize+ 1,0,(struct sockaddr *)&client,addr_size); //check for n, size
			
	}
	if(rx.type=='S'){
			//1) search and return the address and content name for download, an either send E or S type 
			tx=RandS(rx);
			sendto(udpsock,&tx, dataSize+ 1,0,(struct sockaddr *)&client,addr_size); //check for n, size
			
	}
	if(rx.type=='T'){
		
			//check the index server for the address available
			//if equals port/address number,delete accordingly 
			//if port num matches then it will delete the file
			tx= QandT(rx);
			sendto(udpsock,&tx, dataSize+ 1,0,(struct sockaddr *)&client,addr_size); //check for n, size
			
	}
	if(rx.type=='L'){

			
			// openes the indexservfile and prints out all available content
			FILE* fo =fopen("IndexServer.txt","r"); // read 
				
			while(fgets(string,100,fo)){
				
				strcpy(tx.data,string);
			
				sendto(udpsock,&tx, dataSize+ 1,0,(struct sockaddr *)&client,addr_size); //check for n, size
				
				memset(&tx.data[0], 0, dataSize); // NULL values?
				
						
			}
			
			fclose(fo); 

			
	}
	if(rx.type=='Q'){
			// just delete every file in directory belonging to the specific user address
			tx= QandT(rx);
			sendto(udpsock,&tx, dataSize+ 1,0,(struct sockaddr *)&client,addr_size); //check for n, size
			
	}
	
	memset(&rx.data[0], '\0', dataSize); // clear rx everytime a function ends to fresh new data
	}
	return 0;
	
}
