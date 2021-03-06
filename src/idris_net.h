// C Library for Idris Network Bindings
// SJF, started on 28/09/13...
#ifndef IDRISNET_H
#define IDRISNET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <idris_rts.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

// Raw structure, passed to Idris as a void* pointer.
// Contains connection-specific state, used in operations such as send and receive.
// TODO: Error location: so, we'll need some enum specifying which call caused the failure
typedef struct idr_conn_info {
    struct addrinfo* addr_info;
    int sockfd;
    int last_error;
    char* fetched_data;
} idr_conn_info;


typedef struct idr_udp_res {
    struct sockaddr_storage* remote_host;
    int result;
    int err;
    int addr_len;
    int data_len;
    char* fetched_data;
} idr_udp_res;

typedef struct idr_udp_send_res {
    int result;
    int err;
} idr_udp_send_res;

// Structure returned by accept(): will require 2 FFI calls to access, 
// but will support concurrency (when we get there)
typedef struct idr_accept_res {
    int result;
    int err;
    idr_conn_info* new_client;
} idr_accept_res;

// API:

// Listens on a TCP socket with the given IP and port.
// Idris will have checked IP and port at this point, and serialised to strings.
// TODO: Perhaps add other params in
int idrnet_listen(void* conn_info, char* ip, char* port); 

// Accept a new client on a bound socket
void* idrnet_accept(void* acceptor_info);
// Accessor functions for members of accept result structure
int idrnet_get_accept_res(void* accept_res_struct);
int idrnet_get_accept_err(void* accept_res_struct);
void* idrnet_get_accept_client(void* accept_res_struct);
void idrnet_free_accept_struct(void* accept_res_struct);


// Attempts to connect to a TCP socket with the given IP and port.
// Again, Idris will have checked IP and port by this point.
int idrnet_connect(void* conn_info, char* ip, char* port);

// Attempts to close the current connection
int idrnet_close(void* conn_info);


// Sends data to the given connection.
// (We assume flags are 0 atm.)
// Returns -1 on error, or number of bytes sent otherwise
int idrnet_send(void* conn_info, char* data);

// Receives data from the given connection.
// Returns NULL if there's a 
int idrnet_recv(void* conn_info);

// Allocates an idr_conn_info struct, which is passed into subsequent calls
void* idrnet_allocate_conn_info();

// Frees the idr_conn_info struct
void idrnet_deallocate_conn_info(void* conn_info);

// Returns the last error stored in the connection state
int idrnet_get_last_error(void* conn_info);

// Retrieve result of last fetch (as we can't do a null check on a string in idris...)
char* idrnet_get_fetched_data(void* conn_info);


// UDP Stuffs
int idrnet_udp_listen(void* conn_info, char* ip, char* port);
void* idrnet_udp_recv(void* conn_info);
void* idrnet_udp_send(char* ip, char* port, char* data);
int idrnet_get_udp_result(void* udp_res_struct);
int idrnet_get_udp_err(void* udp_res_struct);
char* idrnet_get_udp_data(void* udp_res_struct);
char* idrnet_get_udp_ip(void* udp_res_struct);
int idrnet_get_udp_port(void* udp_res_struct);
void idrnet_free_udp_send(void* udp_send_res);
void idrnet_free_udp_res(void* udp_res_struct);
int idrnet_get_udp_send_result(void* udp_send_res);
int idrnet_get_udp_send_err(void* udp_send_res);
void idrnet_free_udp_send(void* udp_send_res);

#endif
