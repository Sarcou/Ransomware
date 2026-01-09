#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "c2_client.h"

void* handle_client(void *arg) {
    int sock = *(int*)arg;
    free(arg);
    
    C2Command cmd;
    C2Response resp;
    
    printf("[C2] 🟢 Client connecté\n");
    
    while (recv(sock, &cmd, sizeof(cmd), 0) > 0) {
        memset(&resp, 0, sizeof(resp));
        resp.status = 0;
        
        if (strcmp(cmd.command, "STATUS") == 0) {
            strcpy(resp.message, "Server OK - Ready");
        } else if (strcmp(cmd.command, "ENCRYPT") == 0) {
            snprintf(resp.message, sizeof(resp.message),
                     "Encrypted: %s (mode %d)", cmd.target, cmd.mode);
        } else {
            strcpy(resp.message, "Unknown command");
            resp.status = -1;
        }
        
        send(sock, &resp, sizeof(resp), 0);
        printf("[C2] 📨 Commande: %s\n", cmd.command);
    }
    
    printf("[C2] 🔴 Client déconnecté\n");
    close(sock);
    return NULL;
}

int main() {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(C2_SERVER_PORT);
    
    bind(server_sock, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_sock, 5);
    
    printf("╔═══════════════════════════════════════╗\n");
    printf("║  🖥️  SERVEUR C2 - PORT %d           ║\n", C2_SERVER_PORT);
    printf("╚═══════════════════════════════════════╝\n\n");
    printf("⏳ En attente de connexions...\n\n");
    
    while (1) {
        int *client_sock = malloc(sizeof(int));
        *client_sock = accept(server_sock, NULL, NULL);
        
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, client_sock);
        pthread_detach(thread);
    }
    
    return 0;
}
