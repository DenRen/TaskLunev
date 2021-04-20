#include "unp.h"

int diag_echo (int sockfd, SA* pcliaddr, socklen_t clilen) {
    
    char msg[BUF_SIZE];
    while (true) {
        socklen_t len = clilen;

        int num = recvfrom (sockfd, msg, sizeof (msg), 0, pcliaddr, &len);
        CHECK_ERR (num, );
        CHECK_ERR (sendto (sockfd, msg, num, 0, pcliaddr, len), );
    }
}

int main (int argc, char* argv[]) {

    struct sockaddr_in servaddr;
    bzero (&servaddr, sizeof (servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    servaddr.sin_port = htons (SERV_PORT);

    int sockfd = socket (AF_INET, SOCK_DGRAM, 0);
    CHECK_ERR (sockfd, );

    #define ERR_ACT close (sockfd);

    CHECK_ERR (bind (sockfd, (SA*) &servaddr, sizeof (servaddr)), ERR_ACT);

    struct sockaddr_in cliaddr;
    CHECK_ERR (diag_echo (sockfd, (SA*) &cliaddr, sizeof (cliaddr)), ERR_ACT);

    #undef ERR_ACT
    close (sockfd);
}