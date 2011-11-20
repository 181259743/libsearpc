#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <glib.h>
#include <glib-object.h>

#include "searpc-server.h"
#include "searpc-demo-packet.h"
#define BUFLEN 256

static int
searpc_strlen(const char *str)
{
    if (str == NULL)
        return -1;
    else
        return strlen(str);
}

static void
start_rpc_service(void)
{
    searpc_server_init();

    /* The first parameter is the implementation function.
     * The second parameter is the name of the rpc function the 
     * client would call.
     * The third parameter is the signature.
     */
    searpc_server_register_function(searpc_strlen,
                                    "searpc_strlen",
                                    searpc_signature_int__string());
}


int
main(int argc, char *argv[])
{
    int listenfd, connfd;
    int ret;
    struct sockaddr_in client_addr, server_addr;
    socklen_t clilen;
    char buf[BUFLEN];
    packet *pac, *pac_ret;

    g_type_init();

    start_rpc_service();

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        fprintf(stderr, "socket failed: %s\n", strerror(errno));
        exit(-1);
    }

    int on = 1;
    if (setsockopt (listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        fprintf (stderr, "setsockopt of SO_REUSEADDR error: %s\n", strerror(errno));
        exit(-1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(12345);

    ret = bind(listenfd, (struct sockaddr *)&server_addr,
               sizeof(server_addr));

    if (ret < 0) {
        fprintf(stderr, "bind failed: %s\n", strerror(errno));
        exit(-1);
    }

    ret = listen(listenfd, 5);
    if (ret < 0) {
        fprintf(stderr, "listen failed: %s\n", strerror(errno));
        exit(-1);
    }

    while (1) {
        GError *error = NULL;

        clilen = sizeof(client_addr);
        connfd = accept(listenfd, (struct sockaddr *)&client_addr, &clilen);
        if (connfd < 0) {
            fprintf(stderr, "accept failed: %s\n", strerror(errno));
            continue;
        }

        /* read the header packet */
        pac = read_packet(connfd, buf);
        if (pac == NULL) {
            fprintf(stderr, "read packet failed: %s\n", strerror(errno));
            exit(-1);
        }                              

        gsize ret_len;
        int fcall_len = ntohs(pac->length);
        /* Execute the RPC function */
        char *res = searpc_server_call_function (pac->data, fcall_len,
                                                 &ret_len, &error);

        pac_ret = (packet *)buf;
        pac_ret->length = htons((uint16_t)ret_len);
        strncpy(pac_ret->data, res, ret_len);

        /* send the ret packet */
        if (writen (connfd, buf, PACKET_HEADER_LENGTH + ret_len) == -1) {
            fprintf (stderr, "write packet failed: %s\n", strerror(errno));
            exit(-1);
        }
    }

    return 0;
}
