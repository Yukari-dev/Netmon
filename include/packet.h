#ifndef PACKET_H
#define PACKET_H
#include "stats.h"
#include <time.h>
#include <pcap.h>

typedef struct{
    unsigned char* data;
    int length;
} Packet;

typedef struct{
    Packet **packets;
    int count;
} PacketBuffer;

typedef struct{
    PacketBuffer *buffer;
    StatsTable *stats;
    pcap_t *handle;
    time_t start_time;
} CaptureContext;

Packet *create_packet(const unsigned char *data, int length);
void free_packet(Packet *p);

#endif