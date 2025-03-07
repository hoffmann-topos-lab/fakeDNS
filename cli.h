#ifndef CLI_H
#define CLI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void parse_arguments(int argc, char *argv[], char **spoof_ip, int *full_dump);
void print_help();

#endif
