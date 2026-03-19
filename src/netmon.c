#include "device.h"
#include "packet.h"
#include "decoder.h"
#include "stats.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[]){
    const char *device = "wlan0";
    const char *filter = "";
    int packets_limit  = 200;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-i") == 0){
            if(i + 1 >= argc){
                fprintf(stderr, "Error: -i requires an argument\n");
                return 1;
            }
            i++;
            device = argv[i];
        } else if(strcmp(argv[i], "-f") == 0){
            if(i + 1 >= argc){
                fprintf(stderr, "Error: -f requires an argument\n");
                return 1;
            }
            i++;
            filter = argv[i];
        } else if(strcmp(argv[i], "-c") == 0){
            if(i + 1 >= argc){
                fprintf(stderr, "Error: -c requires an argument\n");
                return 1;
            }
            i++;
            packets_limit = atoi(argv[i]);
        } else if(strcmp(argv[i], "-h") == 0){
            printf("Usage: netmon [-i interface] [-f filter] [-c count]\n");
            return 0;
        }
    }

    pcap_t *handle = open_device(device, filter);
    if(handle == NULL){
        fprintf(stderr, "Couldn't open device %s\n", device);
        return 1;
    }
    
    PacketBuffer buffer;
    buffer.packets = malloc(sizeof(Packet*) * packets_limit);
    buffer.count = 0;

    CaptureContext ctx;
    ctx.buffer = &buffer;
    ctx.stats  = create_stat();
    pcap_loop(handle, packets_limit, decoder, (unsigned char*)&ctx);
    print_stats(ctx.stats);
    close_device(handle);

    for(int i = 0; i < ctx.buffer->count; i++){
        free_packet(ctx.buffer->packets[i]);
    }
    free(ctx.buffer->packets);
    free_stats(ctx.stats);

    return 0;
}
