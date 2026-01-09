#ifndef C2_CLIENT_H
#define C2_CLIENT_H

#include <stdint.h>
#include <string.h>

#define C2_SERVER_IP "127.0.0.1"
#define C2_SERVER_PORT 4444

typedef struct {
    char command[64];
    char target[256];
    char key[128];
    int mode;
    int shift;
} C2Command;

typedef struct {
    int status;
    char message[512];
    long bytes_processed;
} C2Response;

int c2_connect(const char *ip, int port);
C2Response c2_send_command(int sockfd, const C2Command *cmd);
void c2_disconnect(int sockfd);

#endif
