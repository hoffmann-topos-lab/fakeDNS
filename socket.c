
/*
 * Função: setup_socket
 * ---------------------
 * Cria e configura um socket UDP para escutar requisições DNS na porta especificada.
 *
 * sockfd: ponteiro para armazenar o descritor do socket criado.
 * server_addr: estrutura que será preenchida com as configurações do servidor.
 *
 * A função:
 *  - Cria um socket UDP (AF_INET, SOCK_DGRAM).
 *  - Define o socket para aceitar conexões em qualquer interface de rede (INADDR_ANY).
 *  - Associa o socket à porta DNS (53) usando bind().
 *
 * Em caso de erro na criação ou associação do socket, exibe uma mensagem e encerra o programa.
 */
 
#include "socket.h"

void setup_socket(int *sockfd, struct sockaddr_in *server_addr) {
    *sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (*sockfd < 0) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(DNS_PORT);
    if (bind(*sockfd, (struct sockaddr*)server_addr, sizeof(*server_addr)) < 0) {
        fprintf(stderr, "Erro ao fazer bind na porta %d. Verifique se já existe um serviço usando essa porta ou se possui permissões adequadas.\n", DNS_PORT);
        perror("Detalhes do erro");

        close(*sockfd);
        exit(EXIT_FAILURE);
    }
}
