
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
#define MAX_DATA_SIZE 256

void prnEthFrame(unsigned char* buffer, int size)
{ 
    struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    struct udphdr *udp = (struct udphdr*)(buffer + sizeof(struct ethhdr) + iph->ihl * 4);
   // struct tcphdr *tcp = (struct tcphdr*)(buffer + sizeof(struct ethhdr) + iph->ihl * 4);

   //struct tcphdr *tcph = (struct tcphdr*)buffer
   struct tcphdr *tcph = (struct tcphdr *) (buffer + sizeof (struct iphdr));
   


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
    printf("   |- Lenght: %u\n", ntohs(udp->len));
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
    }
}