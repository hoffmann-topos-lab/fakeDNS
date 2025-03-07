#ifndef DNS_H
#define DNS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_PACKET_SIZE 512
#define MAX_DOMAIN_LEN 512
#define MAX_POINTERS 10  // Limite para evitar loops infinitos na compressão

typedef struct {
    unsigned short id;
    unsigned short flags;
    unsigned short qdcount;
    unsigned short ancount;
    unsigned short nscount;
    unsigned short arcount;
} __attribute__((packed)) DNSHeader;

// Funções relacionadas a DNS
int parse_domain(const unsigned char *buffer, int buf_len, int offset, char *domain, int max_len);
int build_dns_response(unsigned char *buffer, int packet_len, const char *spoof_ip);
void log_query(const char *domain_name);
void hexdump(const unsigned char *buffer, size_t length);

#endif // DNS_H
