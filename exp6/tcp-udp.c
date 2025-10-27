#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#define TCP_PORT 8080
#define UDP_PORT 9090
#define BUF 1024
void tcp_chat(int is_server)
{
    int sock, conn;
    struct sockaddr_in addr;
    char buf[BUF];
    sock = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = is_server ? INADDR_ANY : inet_addr("127.0.0.1");
    addr.sin_port = htons(TCP_PORT);
    if (is_server)
    {
        bind(sock, (struct sockaddr *)&addr, sizeof(addr));
        listen(sock, 1);
        conn = accept(sock, (struct sockaddr *)NULL, NULL);
        while (1)
        {
            bzero(buf, BUF);
            read(conn, buf, BUF);
            printf("Client: %s", buf);
            printf("Server: ");
            fgets(buf, BUF, stdin);
            write(conn, buf, strlen(buf));
        }
    }
    else
    {
        connect(sock, (struct sockaddr *)&addr, sizeof(addr));
        while (1)
        {
            printf("Client: ");
            fgets(buf, BUF, stdin);
            write(sock, buf, strlen(buf));
            bzero(buf, BUF);
            read(sock, buf, BUF);
            printf("Server: %s", buf);
        }
    }
    close(sock);
}
void udp_chat(int is_server)
{
    int sock;
    struct sockaddr_in addr, cli;
    char buf[BUF];
    socklen_t len = sizeof(cli);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = is_server ? INADDR_ANY : inet_addr("127.0.0.1");
    addr.sin_port = htons(is_server ? UDP_PORT : UDP_PORT);
    if (is_server)
        bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    while (1)
    {
        if (is_server)
        {
            bzero(buf, BUF);
            recvfrom(sock, buf, BUF, 0, (struct sockaddr *)&cli, &len);
            printf("Client: %s", buf);
            printf("Server: ");
            fgets(buf, BUF, stdin);
            sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&cli, len);
        }
        else
        {
            printf("Client: ");
            fgets(buf, BUF, stdin);
            sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&addr, len);
            bzero(buf, BUF);
            recvfrom(sock, buf, BUF, 0, (struct sockaddr *)&addr, &len);
            printf("Server: %s", buf);
        }
    }
    close(sock);
}
int main()
{
    int mode, role;
    printf("Select mode: 1-TCP 2-UDP: ");
    scanf("%d", &mode);
    printf("Role: 1-Server 0-Client: ");
    scanf("%d", &role);
    getchar(); // consume newline
    if (mode == 1)
        tcp_chat(role);
    else
        udp_chat(role);
    return 0;
}
