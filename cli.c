
/*
 * Função: parse_arguments
 * ------------------------
 * Analisa os argumentos da linha de comando e configura as opções do programa.
 *
 * argc: número de argumentos passados.
 * argv: array de strings contendo os argumentos.
 * spoof_ip: ponteiro para armazenar o IP de spoofing fornecido pelo usuário.
 * full_dump: flag que indica se o dump completo dos pacotes deve ser ativado.
 *
 * O primeiro argumento após o nome do programa deve ser o IP de spoofing.
 * Opções adicionais:
 *  - "-h" ou "--help": exibe a ajuda e encerra a execução.
 *  - "--full-dump": ativa o dump completo dos pacotes recebidos.
 *
 * Se os argumentos forem inválidos ou insuficientes, exibe a ajuda e encerra com erro.
 */

#include "cli.h"

void parse_arguments(int argc, char *argv[], char **spoof_ip, int *full_dump) {
    if (argc < 2) {
        print_help();
        exit(EXIT_FAILURE);
    }
    *spoof_ip = argv[1];
    *full_dump = 0;

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help();
            exit(EXIT_SUCCESS);
        } else if (strcmp(argv[i], "--full-dump") == 0) {
            *full_dump = 1;
        } else {
            print_help();
            exit(EXIT_FAILURE);
        }
    }
}

/*
 * Função: print_help
 * ---------------------
 * Exibe informações sobre o uso do programa e as opções disponíveis.
 * 
 * Uso:
 *   Chamado quando o usuário executa o programa sem argumentos válidos
 *   ou solicita ajuda com as opções `-h` ou `--help`.
 */

void print_help() {
    printf("Uso: ./dns_spoof <ip> [opções]\n");
    printf("Opções:\n");
    printf("  -h, --help       Exibe esta tela de ajuda\n");
    printf("  --full-dump      Exibe os pacotes brutos recebidos\n");
}

