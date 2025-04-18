
/*MP-5  ahmad habibi*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <netinet/udp.h>

#include <netinet/tcp.h>
#include <netinet/ip.h>

#include <net/ethernet.h>
#include <unistd.h>
#define BUFFER_SIZE 65536
#define ETHERNET_HEADER_LEN 14

#define MAX_DATA_SIZE 256
#define FRAME_START_DELIMITER 0x7E

//#define __FAVOR_BSD

typedef struct {
    char startDelimiter;
    unsigned char destinationAddress[6];
    unsigned char sourceAddress[6];
    unsigned short length;
    char data[MAX_DATA_SIZE];
    unsigned int fcs; // Frame Check Sequence
} Frame;
/*
struct ethhdr {
	unsigned char	h_dest[ETH_ALEN];	
	unsigned char	h_source[ETH_ALEN];	
	__be16		h_proto;		
} __attribute__((packed));*/



void printFrame(const char* buffer,int size) {
    const Frame* frame = (Frame*)buffer;


   // const uint8_t *payload = buffer + ETHERNET_HEADER_LEN ;
   // size_t payload_len = size - ETHERNET_HEADER_LEN ;

   // printf("payload (%zu bytes):\n",payload_len);

    for (int i = 0; i < size; i++) {
        if (i != 0 && i % 16 == 0) {
            printf("\n");
        }
        printf(" %02X", buffer[i]);
    }
    printf("\n");

    printf("Start Delimiter: %c\n", frame->startDelimiter);
   
    if (frame->startDelimiter == FRAME_START_DELIMITER)
    {
           printf("START \n");
    }

   // printf("   |-Source Address      : %02X:%02X:%02X:%02X:%02X:%02X\n", frame[1], frame[2], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);

    printf("\nSource Address: ");
    for (int i = 0; i < 6; i++) {
        printf("%02X ",(unsigned char)frame->sourceAddress[i]);
    }

    printf("\nDestination Address: ");
    for (int i = 0; i < 6; i++) {
        printf("%02X ", (unsigned char)frame->destinationAddress[i]);
    }
    printf("\nLength: %u\n", frame->length);
    
    for (int i = 0; i < frame->length; i++) {
        if (frame->data[i] >= 32 && frame->data[i] <=127 ){
           // printf("%c", frame->data[i]);
        }
       
    }
    printf("\nFrame Check Sequence: %08X\n", frame->fcs);
}






/*


void prnFrame(const unsigned char *buffer, int size) {

    const uint8_t *payload = buffer + ETHERNET_HEADER_LEN ;
    size_t payload_len = size - ETHERNET_HEADER_LEN ;

    printf("payload (%zu bytes):\n",payload_len);

    for (int i = 0; i < payload_len; i++) {
        if (i != 0 && i % 16 == 0) {
            printf("\n");
        }
        printf(" %02X", payload[i]);
    }
    printf("\n");
    for (int i = 0; i < payload_len; i++) {
        if (i != 0 && i % 16 == 0) {
            printf("\n");
        }
        if (payload[i] >= 32 && payload[i] <=127 )
        {
            printf("%c", payload[i]);
        }
    }
    printf("\n");
}
    */


void prnIpPacket(unsigned char* buffer)
{
    char protocol_type[10] ;
    memset(protocol_type,0,10);
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    if (iph->protocol == 6)
    {
        strcpy(protocol_type,"TCP");
    }
    else if (iph->protocol == 17)
    {
        strcpy(protocol_type,"UDP");
    }
    else{
        strcpy(protocol_type,"Other");
    }
    printf("\nIP Header\n");
    printf("   |-Src  IP         : %s\n", inet_ntoa(*(struct in_addr*)&iph->saddr));
    printf("   |-Dest IP         : %s\n", inet_ntoa(*(struct in_addr*)&iph->daddr));
    printf("   |-Protocol        : %d %s\n", iph->protocol,protocol_type);
}
void prnUdpPacket(unsigned char* buffer)
{
    struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    struct udphdr *udp = (struct udphdr*)(buffer + sizeof(struct ethhdr) + ip->ihl * 4);
    printf("UDP Packet \n");
    printf("   |- Src Port: %u\n", ntohs(udp->source));
    printf("   |- Dest Port: %u\n", ntohs(udp->dest));
    printf("   |- Lenght: %u\n", ntohs(udp->len));
}




void prnEthFrame(unsigned char* buffer, int size)
{ 
    struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    struct udphdr *udp = (struct udphdr*)(buffer + sizeof(struct ethhdr) + iph->ihl * 4);
    struct tcphdr *tcp = (struct tcphdr*)(buffer + sizeof(struct ethhdr) + iph->ihl * 4);


    for (int i = 0; i < size; i++) {
        if (i != 0 && i % 16 == 0) {
            printf("\n");
        }
        printf(" %02X", buffer[i]);
    }
    printf("\n");
    for (int i = 0; i < size; i++) {
        if (i != 0 && i % 16 == 0) {
            printf("\n");
        }
        if (buffer[i] >= 32 && buffer[i] <= 126){
            printf(" %c", buffer[i]);
        }
    }
    printf("\n");
    unsigned srcPort ;
    unsigned destPort ;
    if (iph->protocol == 6)
    {
       // srcPort = tcp->
       // destPort = tcp->dest;
    }
    else if (iph->protocol == 17)
    {
        srcPort = udp->source;
        destPort = udp->dest;
    }
    printf("   |- Src Port: %u\n", ntohs(srcPort));
    printf("   |- Dest Port: %u\n", ntohs(destPort));
//    printf("   |- Lenght: %u\n", ntohs(udp->len));
    struct ethhdr *eth = (struct ethhdr *)buffer;
    printf("\nEthernet Header\n");
    printf("   |-Source Address      : %02X:%02X:%02X:%02X:%02X:%02X\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
    printf("   |-Destination Address : %02X:%02X:%02X:%02X:%02X:%02X\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    printf("   |-Protocol            : IP4 \n"); // (unsigned short)eth->h_proto);
    printf("\n");
    printf("-----------------------------------------------------------------------------\n");
}

int main() {
    int sockfd;
    unsigned char buffer[BUFFER_SIZE];
    struct sockaddr saddr;
    int saddr_len = sizeof(saddr);
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    while (1) {   
        int data_size = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, &saddr, (socklen_t *)&saddr_len);
        prnEthFrame(buffer,data_size);
        //prnFrame(buffer,data_size);
        //prnEthPacket(buffer,data_size);
       // prnIpPacket(buffer);
       // prnUdpPacket(buffer);
       
    }
}