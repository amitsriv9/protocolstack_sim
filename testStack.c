#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>

   typedef struct header_s{
	char senderID;
	char receiverID;
	unsigned short type;
	unsigned short datalen;
	unsigned short seqno;
	unsigned short ackno;
	unsigned short checksum;
           }header_t;
   typedef enum Control{
			GOBKN,
			SELREP
			} Control_e;

   #define DATA_SIZE  96
   #define PKT_SIZE   DATA_SIZE + 12
   //char data[DATA_SIZE];

   char message[] ="Today, we don’t need Macaulay’s detailed illustrations to see buildings being unbuilt. We call it “destructoporn” (since 2007, according to Urban Dictionary) and it comes, unbidden, via digital media. Where did I see that Tod Williams and Billie Tsien’s Folk Art Museum, just thirteen years old, was down to steel and rubble? The art critic Jerry Saltz’s Instagram. How did I follow the destruction of Bertrand Goldberg’s Prentice Women’s Hospital (1975), in Chicago, made of poured-in-place concrete that took weeks to demolish last fall? Tweets from the Windy City’s flock of architecture observers" ;


  int calculateChecksum(char *packet){
  return 0;
  }
  int checkType(char* packet){
  return 0;
  }
 /*
  *
  *
  */
   void initializeHeader(header_t *headerInfo, int datalen, int type){

   sprintf(headerInfo->senderID, "1"); 
   sprintf(headerInfo->receiverID, "2"); 
   headerInfo->type = 0x02;
   headerInfo->datalen = datalen;
   headerInfo->seqno=0;
   headerInfo->checksum=0;

   }

 /*
  *
  *
  */
   void sendToPhysicalLayer(char *buffer, int size, int errorrate, int droprate){
   int readFromBuffer(char *buffer, int size);
   char * buff=NULL;
   buff = (char*)calloc(size+1, sizeof(char));
   strncpy(buff, buffer, size);
   
   readFromBuffer(buff, size);
   /*
 *    check if packet is begin
 *    if yes
 *    	allocate memory
 *    	read packets and copy data in loop
 *    	after doing checksum in loop
 *    	if checksum fails send an ACK
 *    	if end of packets
 *    	send upstairs
 */    	
   }


 /*
  *
  *
  */
   int readFromBuffer(char *buffer, int size){
   int i,packetsFound=0, result, type=0, bytesReceived=0 ;
   char *data=NULL;

   packetsFound = size / PKT_SIZE; 
   data = (char*)calloc(packetsFound*DATA_SIZE, sizeof(char));

   for( i=0 ; i < packetsFound; i++){

      if(!calculateChecksum(buffer + i*PKT_SIZE)){
         printf("Nice\n");
         type = checkType(buffer + i*PKT_SIZE + 16);

         switch(type){
 	 case 1: 
		bytesReceived=0;
		break;
	 case 2:
		bytesReceived+=DATA_SIZE;
		strncpy(data, buffer + i*PKT_SIZE + 12, DATA_SIZE);
		break;
	 case 3:
		bytesReceived=0; return 0;
		break;
	 default: printf("undefined\n");         
         }
 	 return 0;
      }
      else
	 return 1;
   }
   return 1;
   }


  /*
  * the return value of -99 indicates checksum failed
  * resend all packets again after timer expires
  *
  */
   void waitForAcknowledgement(int min, int max, int *ack){
 /*header_t *ackHeader=NULL; 
 * output = recv(socket, buffer, strlen(buffer), 0);
 * if output> 0 
 *   if strlen(buffer >= PKT_SIZE)
 *      if(!calculateChecksum(buffer))
 *          ackHeader = (header_t*)buffer;
 *          if (ackHeader->ackno < = max) 
 *	        *ack = ackHeader->ackno;
*/  

   }
/*
*  Sends data to the remote host 1000 bytes at a time
*  1. Creates packets 
*  2. Calculates checksum 
*  3. And handles error messages
*/
  int sendToDataLink(char* message, int messagelen, Control_e ctrl){

   int i, j, frameCount, packetRequired=0, smallPacketFlag=1, size=0, type =0;
   header_t head;
   char *buffer=NULL;
   int min_no=0, max_no=0, ack=0;
   
   memset(&head, 0, sizeof(header_t));

   if(packetRequired < 10){
   	buffer = (char*)calloc(packetRequired*PKT_SIZE, sizeof(char));
	size = packetRequired;
        frameCount = 1;
   }
   else{
        //buffer should not be more than 1k bytes
   	buffer = (char*)calloc(10*PKT_SIZE, sizeof(char));
	size =10; // size is number of packets
        frameCount = packetRequired / 10;
        if(packetRequired%10 != 0){
	     frameCount+=1;	
	}
   }

   initializeHeader(&head, messagelen, type);
   for(j=0; j < frameCount; j++){

     // last frame-set has lass than 10 frames 
     if(frameCount > 1 && (j == frameCount-1)){
        size = packetRequired%10;	
     }
   
     //fill 10 packets using loop
     for(i=0; i< size; i++){
        //head.seqno = i*DATA_SIZE + j*10*DATA_SIZE; 
        head.seqno = i + j*10; 
        head.ackno = 0;

        if(i==0){
	    min_no = head.seqno;
            if(j==0)
	        head.type=1;  
        }
        else
	    head.type = 2;
   
        strncpy(buffer + i*PKT_SIZE, (char*)&head, sizeof(head));
        strncpy(buffer + i*PKT_SIZE + 12, message + ((i+j*10)*DATA_SIZE), DATA_SIZE);
        calculateChecksum(buffer+ i*PKT_SIZE);
        //track the sent data for ack
        //how many bytes sent
        //expected ack
        //in case of error use ENUM GOBKN SLREP 
        //if GOBKN call the sendToPhy again   
        //else call sendToPhy with single packet
    }

    max_no = head.seqno;
    sendToPhysicalLayer(buffer, 0, 0, size*PKT_SIZE);
    //waitForAcknowledgement(min_no, max_no, &ack);

    if(0/*ack < max_no*/){
        sendToPhysicalLayer(buffer, 0, 0, size*PKT_SIZE);
    }

   }

   return 0;
  }

  int main(){
  Control_e ctrl;
  char *buffer=NULL;
  int i,messagelen, packetRequired=0, smallPacketFlag=1, size=0, type =0;
  messagelen = strlen(message); 
  packetRequired = messagelen / DATA_SIZE; 

  if(messagelen % DATA_SIZE != 0){
     packetRequired+=1;
     smallPacketFlag=0;
  }
  sendToDataLink(message, messagelen, ctrl);
  //sendFileToLink(file);
  //read 1000bytes from file in for loop 
  //call sendFileToLink()
  //when it returns 0 read the next 1k
  return 0;
  } 
    
