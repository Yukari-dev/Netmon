CC = gcc
CFLAGS = -g -fsanitize=address -fsanitize=leak -Wall
LIBS = -lpcap -lncurses
INCLUDES = -Iinclude

# final binary — depends on all object files
bin/netmon: obj/packet.o obj/device.o obj/decoder.o obj/stats.o obj/ui.o obj/netmon.o
	$(CC) $(CFLAGS) -o bin/netmon obj/packet.o obj/device.o obj/decoder.o obj/stats.o obj/ui.o obj/netmon.o $(LIBS)

# each .c file gets its own rule
obj/packet.o: src/packet.c include/packet.h
	$(CC) $(CFLAGS) $(INCLUDES) -c src/packet.c -o obj/packet.o

obj/device.o: src/device.c include/device.h
	$(CC) $(CFLAGS) $(INCLUDES) -c src/device.c -o obj/device.o

obj/decoder.o: src/decoder.c include/decoder.h
	$(CC) $(CFLAGS) $(INCLUDES) -c src/decoder.c -o obj/decoder.o

obj/stats.o: src/stats.c include/stats.h
	$(CC) $(CFLAGS) $(INCLUDES) -c src/stats.c -o obj/stats.o

obj/ui.o: src/ui.c include/ui.h
	$(CC) $(CFLAGS) $(INCLUDES) -c src/ui.c -o obj/ui.o

obj/netmon.o: src/netmon.c
	$(CC) $(CFLAGS) $(INCLUDES) -c src/netmon.c -o obj/netmon.o

run: bin/netmon
	sudo ./bin/netmon

clean:
	rm -f bin/netmon obj/*.o