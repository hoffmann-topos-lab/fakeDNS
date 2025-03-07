/*
 * Servidor DNS Spoof 
 *
 * Compilar (estático para roteador MIPS, por exemplo):
 *   mipsel-linux-gnu-gcc -static -O2 -o dns_spoof dns_spoof.c
 *
 * Uso:
 *   sudo ./dns_spoof <IP_SPOOF>
 *
 * Exemplo:
 *   sudo ./dns_spoof 192.168.1.100
 *
 * O programa escuta na porta 53 UDP e, para cada consulta DNS recebida,
 * registra o nome consultado e envia uma resposta com o IP informado.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


/*
 * Função: main
 * ------------
 * Inicializa e executa o servidor DNS spoofing, processando consultas DNS e 
 * enviando respostas forjadas para um IP especificado.
 *
 * argc: número de argumentos passados na execução.
 * argv: vetor de strings contendo os argumentos.
 *
 * Fluxo da função:
 *  - Lê os argumentos da linha de comando (IP spoof e opções adicionais).
 *  - Configura o socket UDP para escutar requisições DNS.
 *  - Entra em um loop infinito para:
 *    - Receber pacotes DNS.
 *    - Extrair e registrar o domínio consultado.
 *    - Exibir o conteúdo bruto do pacote, se ativado (`--full-dump`).
 *    - Construir e enviar uma resposta DNS forjada apontando para o IP spoof.
 *
 * O programa executa indefinidamente até ser encerrado manualmente.
 */

#include "cli.h"
#include "socket.h"
#include "dns.h"

int main(int argc, char *argv[]) {
    char *spoof_ip;
    int full_dump;




    parse_arguments(argc, argv, &spoof_ip, &full_dump);
    printf("Servidor DNS spoof rodando na porta %d com resposta: %s\n", DNS_PORT, spoof_ip);

    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    setup_socket(&sockfd, &server_addr);

    unsigned char buffer[MAX_PACKET_SIZE];

    while (1) {
        socklen_t client_len = sizeof(client_addr);
        int recv_len = recvfrom(sockfd, buffer, MAX_PACKET_SIZE, 0, 
                                (struct sockaddr*)&client_addr, &client_len);
        if (recv_len < 0) continue;

        char domain_name[MAX_DOMAIN_LEN];
        if (parse_domain(buffer, recv_len, 12, domain_name, sizeof(domain_name)) >= 0) {
            log_query(domain_name);
            if (full_dump) {
                printf("Pacote bruto:");
                hexdump(buffer, recv_len);
            }

            // Construção e envio da resposta DNS spoof
            int response_size = build_dns_response(buffer, recv_len, spoof_ip);
            if (response_size > 0) {
                sendto(sockfd, buffer, response_size, 0, 
                       (struct sockaddr*)&client_addr, client_len);
            }
        }
    }

    close(sockfd);
    return 0;
}
