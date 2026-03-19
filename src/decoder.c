#include "decoder.h"
#include "packet.h"
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void decoder(unsigned char *user, const struct pcap_pkthdr *header, const unsigned char *packet){
    unsigned short type = (packet[12] << 8) | packet[13];

    if(type != 0x0800) return;

    PacketBuffer *buf = (PacketBuffer*)user;

    buf->packets[buf->count] = create_packet(packet, header->caplen);

    unsigned char *packet_data = buf->packets[buf->count]->data;

    struct iphdr *ip = (struct iphdr*)(packet_data + 14);

    buf->count++;
    struct in_addr srcIp;
    srcIp.s_addr = ip->saddr;

    struct in_addr destIp;
    destIp.s_addr = ip->daddr;

    char* protocol = "";
    if(ip->protocol == PROTO_TCP) protocol = "TCP";
    if(ip->protocol == PROTO_UDP) protocol = "UDP";
    if(ip->protocol == PROTO_ICMP) protocol = "ICMP";
    char src_ip[16];
    strncpy(src_ip, inet_ntoa(srcIp), 15);
    printf("source Ip %s -> destination Ip %s Protocol %s, Length %d bytes\n", 
        src_ip, 
        inet_ntoa(destIp), 
        protocol, 
        header->caplen
    );
    
}
