#ifndef PACKET_H
#define PACKET_H

typedef struct{
    unsigned char* data;
    int length;
} Packet;

typedef struct{
    Packet **packets;
    int count;
} PacketBuffer;

Packet *create_packet(const unsigned char *data, int length);
void free_packet(Packet *p);

#endif