#include <stdio.h>
#include <stdlib.h>

#include "server.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#endif


void uni_start_server(){
    #ifdef _WIN32
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Erreur WSAStartup.\n");
        exit(EXIT_FAILURE);
    }
    #else

    #endif
}
void uni_start_client(){
    #ifdef _WIN32
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Erreur WSAStartup.\n");
        exit(EXIT_FAILURE);
    }
    #else

    #endif
}

int uni_socket(int domain, int type, int protocol){
    int server_fd = socket(domain, type, protocol);
    #ifdef _WIN32
    if (server_fd == INVALID_SOCKET) return 0;
    #else
    return server_fd;
    #endif
}

int uni_nblock(int server_fd){
    #ifdef _WIN32
    u_long mode = 1;
    if (ioctlsocket(server_fd, FIONBIO, &mode) == SOCKET_ERROR) return -1;
    #else
    return fcntl(server_fd, F_SETFL, fcntl(server_fd, F_GETFL, 0) | O_NONBLOCK);
    #endif
}

int uni_set_sockopt(int server_fd){
    int opt = 1;
    #ifdef _WIN32
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR) return -1;
    #else
    return setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    #endif
}

int uni_bind(server *s){
    #ifdef _WIN32
    if(bind(s->serveur_fd, (struct sockaddr*)s->address, s->addrlen) == SOCKET_ERROR) return -1;
    #else
    return bind(s->serveur_fd, (struct sockaddr *) &(*s->address), s->addrlen);
    #endif
}

int uni_listen(int server_fd, int mx_client){
    #ifdef _WIN32
    if (listen(server_fd, MAX_CLIENT) == SOCKET_ERROR) return -1;
    #else
    return listen(server_fd, MAX_CLIENT);
    #endif
}

void uni_stop_server(){
    #ifdef _WIN32
    WSACleanup();
    #else

    #endif
}
void uni_stop_client(){
    #ifdef _WIN32
    WSACleanup();
    #else

    #endif
}

void uni_close(int server_fd){
    #ifdef _WIN32
    closesocket(server_fd);
    #else
    close(server_fd);
    #endif
}

int uni_get_error(){
    #ifdef _WIN32
    return WSAGetLastError();
    #else
    return errno;
    #endif
}

int uni_accept(server *s){
    #ifdef _WIN32
    int tmp = accept(s->serveur_fd, (struct sockaddr *)&(s->address), &addrlen)
    if(tmp == INVALID_SOCKET){
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK) {
            return 0;
        }
        return -1;
    }
    return tmp;
    #else
    return accept(s->serveur_fd, (struct sockaddr *)&(s->address), (socklen_t*)&(s->addrlen));
    #endif
}

int uni_select(server *s, fd_set *readfds, struct timeval *timeout){
    #ifdef _WIN32
    int activity = select(0, readfds, NULL, NULL, timeout);
    if (activity == SOCKET_ERROR){
        return -1;
    }
    return activity;
    #else
    return select(s->serveur_fd + 1, readfds, NULL, NULL, timeout);
    #endif
}

ssize_t uni_write(int socket_fd, const void *buffer, size_t length) {
    #ifdef _WIN32
    return send(socket_fd, (const char *)buffer, length, 0);
    #else
    return write(socket_fd, buffer, length);
    #endif

}

ssize_t uni_read(int socket_fd, void *buffer, size_t length) {
    #ifdef _WIN32
    return recv(socket_fd, (char *)buffer, length, 0);
    #else
    return read(socket_fd, buffer, length);
    #endif
}