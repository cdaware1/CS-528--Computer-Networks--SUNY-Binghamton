//Charuhas Daware
//Assignment 1 - client.cpp
//B00815646(cdaware1@binghamton.edu)

#include <iostream>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h> 
#include <sys/time.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#define PORT	 12000
using namespace std;

int main(int argc, char **argv)
{
	if(argc < 2) 	//If your input is without Server IP, it will throw an error with return -1
		{
			printf("Error. Please Enter Server IP\n");
			return -1;
		}
	if(argc > 2) 	//If you enter more than 2 arguments, it will throw an error with return -1
		{
			printf("Error. Please Enter Only IP Address\n");
			return -1;
		}
	int sockfd, n;
	int count = 1;			//To keep ping count
	int lostcount=0;		//To keep lost packets count
	int successcount= 0; 		//To keep success ping count
	int pings = 10;			//Pings Required
	socklen_t len;
	char buffer[1024];
	struct sockaddr_in servaddr, cliaddr;

	memset(&servaddr, 0, sizeof(servaddr));	//Initializing server address
	memset(&cliaddr, 0, sizeof(cliaddr));	//Initializing client address

	//Server Information
	servaddr.sin_family = AF_INET; 		// IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY; 	// localhost
	servaddr.sin_port = htons(PORT); 	// Port Number

	inet_aton(argv[1], &servaddr.sin_addr); 				//Stores Entered IP in servaddr
	cout<<"Entered IP Address: "<<inet_ntoa(servaddr.sin_addr)<<endl;	//Prints Entered IP
	cout<<"Port Number: "<<(int) ntohs(servaddr.sin_port)<<endl;		//Prints Port Number

	for(int i =0; i<pings; i++) 
	{
		sockfd = socket(AF_INET, SOCK_DGRAM, 0); //Creation of UDP socket
		struct timeval waiting;			 //The client wait up to one second for a reply from the server
		waiting.tv_sec = 1;			 //Wait set to 1 second
		waiting.tv_usec = 0; 			 //usec=microsecond

		if (setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&waiting, sizeof(waiting)) < 0)	//If receive waiting fails
		{		
			cout<<"\nWaiting failed"<<endl;
		}
		if (setsockopt (sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&waiting, sizeof(waiting)) < 0)	//If send waiting fails
     		{
			cout<<"\nWaiting failed"<<endl;
		}

		struct timeval sendping;
      		gettimeofday(&sendping, NULL);	//Running time while client sends a ping to server
		
		sendto(sockfd, (const char *)buffer, strlen(buffer),MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));//Client sends ping to the server
		n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), 0, ( struct sockaddr *) &cliaddr, &len);//Client recieves ping back from the server

		struct timeval receivepong;
      		gettimeofday(&receivepong, NULL);	//Running time while client receives a response from the server
		
		cout<<"******************************************"<<endl;
		cout<<"Ping Request Number: "<<count<<endl;	//It will Print number for which the client is pinged to the server
		count++;					//Count is incremented in each iteration to keep count
		
		//Round Trip Time(in microseconds for higher precision)
		int rtt = (receivepong.tv_sec - sendping.tv_sec) * 1000000 + ((int)receivepong.tv_usec - (int)sendping.tv_usec); 
		//tv_sec:The number of whole seconds elapsed since the starting point 
		//tv_usec:The number of microseconds elapsed since the time given by the tv_sec 

		if(n==-1) 	//n is -1, ping unsuccessful, that means packet lost
			{
				cout<<"\nPacket Lost - Pong not received"<<endl;
				lostcount++;
			}
		else 		//n is not -1, ping is received that means ping success and the round trip time for the same is in microseconds
			{
				cout<<"\nPing Success"<<endl;
				cout<<"RTT is: "<<rtt<<" microseconds"<<endl;
				successcount++;
			}
		close(sockfd);	//Close the socket within the loop
	}
	close(sockfd);		//Close the socket

	cout<<"\nTotal Ping Requests Count: "<<pings<<endl;
	cout<<"Ping Success Count: "<<successcount<<endl;
	cout<<"Packet Lost Count: "<<lostcount<<endl;
	
	return 0;
}
