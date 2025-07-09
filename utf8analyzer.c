#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }
    utf8(argv[1]);
}

void utf8(char str[]) {
	p1(str);
}

void p1(char str[]) {
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] >> 7 != 0) {
			printf("Valid ASCII: False\n");
			return;
		}
	}
	printf("Valid ASCII: True\n");
}
