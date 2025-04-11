
/*MP-5  ahmad habibi*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <net/ethernet.h>
#include <unistd.h>
#define BUFFER_SIZE 65536

void prnEthPacket(unsigned char* buffer)
{
    struct ethhdr *eth = (struct ethhdr *)buffer;
    printf("\nEthernet Header\n");
    printf("   |-Source Address      : %02X:%02X:%02X:%02X:%02X:%02X\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
    printf("   |-Destination Address : %02X:%02X:%02X:%02X:%02X:%02X\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    printf("   |-Protocol            : %u\n", (unsigned short)eth->h_proto);
}
void prnIpPacket(unsigned char* buffer)
{
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    printf("\nIP Header\n");
    printf("   |-Src  IP         : %s\n", inet_ntoa(*(struct in_addr*)&iph->saddr));
    printf("   |-Dest IP         : %s\n", inet_ntoa(*(struct in_addr*)&iph->daddr));
    printf("   |-Protocol         : %d\n", iph->protocol);
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

void prnData(const unsigned char *data, int size) {
    //char output[size];
    //char output1[size];
    //int x = 0 ;
    for (int i = 0; i < size; i++) {
        if (i != 0 && i % 16 == 0) {
            printf("\n");
        }
        printf(" %02X", data[i]);
       // output[x] = (char)data[i];
       // output1[x]=(char)output[x];
       // printf("\n%c",output1[x]);
       // x++;
    }
    printf("\n");
}

int main() {
    int sockfd;
    unsigned char buffer[BUFFER_SIZE];
    struct sockaddr saddr;
    int saddr_len = sizeof(saddr);
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    while (1) {   
        int data_size = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, &saddr, (socklen_t *)&saddr_len);
        prnData(buffer,data_size);
        prnEthPacket(buffer);
        prnIpPacket(buffer);
        prnUdpPacket(buffer);
        printf("-----------------------------------------------------------------------------\n");
    }
}