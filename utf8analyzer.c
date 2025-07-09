#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

uint32_t get_codepoint(const char str[], int index, int length) {
     uint32_t point = str[index];
        if (length == 2) {
	    point = point & 0x1f;
	} else {
	    point = point & 0x0f;
	}
	point = point << (6 * (length - 1));
	uint32_t byte = 0;
	for(int i = 0; i < length - 1; i++) {
	    byte = str[index + length - 1 - i];
	    byte = byte & 0x3f;
	    byte = byte << (6 * i);
	    point = point | byte;
	}
	//printf("Codepoint: 0x%x, index = %d, length = %d\n", point, index, length);
	return point;
}

int decode_utf8(const char str[], uint32_t buf[]) {
    int points = 0;
    int i = 0;
    while(str[i] != 0) {
        if ((str[i] & 0x80) == 0) {
             buf[points] = str[i];
                 i++;
        } else if ((str[i] & 0xe0) == 0xc0) {
             buf[points] = get_codepoint(str, i, 2);
                 i += 2;
        } else if ((str[i] & 0xf0) == 0xe0) {
                 buf[points] = get_codepoint(str, i, 3);
                 i += 3;
        } else if ((str[i] & 0xf8) == 0xf0) {
                 buf[points] = get_codepoint(str, i, 4);
                 i += 4;
        } else {
                 return 0;
        }
        points++;
    }
    return points;
}

int is_animal_emoji_at(const char str[], int index){
	uint32_t buf[1024];
	uint32_t s;
	if (decode_utf8(str, buf)) {
		s = buf[index];
	} else {
		puts("Buffer error");
		return -1;
	}
	//printf("Hex: 0x%x\n", s);
	if((0x1f400 <= s && s <= 0x1f43f) ||
			(0x1f980 <= s && s <= 0x1f9ae)) {
		return 1;
	}
	return 0;
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }
    utf8(argv[1]);
}

void utf8(char str[]) {
	p1(str);
	p3(str);

	// p5
	uint32_t buf[1024];
	int size = decode_utf8(str, buf);
	printf("Code points as decimal numbers:");
	for(int i = 0; i < size; i++) {
		printf(" %d", buf[i]);
	}
	puts(" ");
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

void p3(char str[]) {
	printf("Length in bytes: %lu\n", strlen(str));
}
