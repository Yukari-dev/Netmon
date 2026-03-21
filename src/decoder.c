#include "decoder.h"
#include "packet.h"
#include "ui.h"
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void decoder(unsigned char *user, const struct pcap_pkthdr *header, const unsigned char *packet){
    unsigned short type = (packet[12] << 8) | packet[13];

    if(type != 0x0800) return;

    CaptureContext *ctx = (CaptureContext*)user;

    ctx->buffer->packets[ctx->buffer->count] = create_packet(packet, header->caplen);

    unsigned char *packet_data = ctx->buffer->packets[ctx->buffer->count]->data;

    struct iphdr *ip = (struct iphdr*)(packet_data + 14);

    ctx->buffer->count++;
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
    int bytes = header->caplen;
    /*printf("source Ip %s -> destination Ip %s Protocol %s, Length %d bytes\n", 
        src_ip, 
        inet_ntoa(destIp), 
        protocol, 
        bytes
    ); */



    update_stats(ctx->stats, src_ip, bytes, ip->protocol);

    char dest_ip[16];
    strncpy(dest_ip, inet_ntoa(destIp), 15);
    update_stats(ctx->stats, dest_ip, bytes, ip->protocol);
    ui_update_feed(src_ip, dest_ip, protocol, bytes);
}
