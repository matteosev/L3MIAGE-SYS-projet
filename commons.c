#include <unistd.h>

#include "commons.h"

void write_size_then_msg(int socket, void *msg) {
    int size = sizeof(msg);
    write(socket, &size, sizeof(size));
    write(socket, msg, size);
}

void read_size_then_msg(int socket, void *msg) {
    int size;
    read(socket, &size, sizeof(size));
    read(socket, msg, size);
}