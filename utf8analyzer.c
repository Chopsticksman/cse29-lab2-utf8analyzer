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

void encode_utf8(uint32_t num, char dest[]) {
	//TODO: Complete this function
	int size = 0;
	uint32_t header;
	if(0 <= num && num <= 127) {
		dest[0] = num;
		return;
	} else if (num <= 0x0fff) { // 2 bytes
		size = 2;
		header = 0xc0;
	} else if (num <= 0xffff) { // 3 bytes
		size = 3;
		header = 0xe0;
	} else if (num <= 0x1fffff) { // 4 bytes
		size = 4;
		header = 0xf0;
	} else {
		puts("Size error");
		return;
	}

	uint32_t mask = 0x3f;
	mask = mask << (6 * (size - 1));

	uint32_t byte = num & mask;
	//printf("Before masking: 0x%x, After masking: 0x%x\n", num, byte);

	//printf("Header: 0x%x\n", header);
	byte = byte >> (6 * (size - 1));
	byte = byte | header;

	dest[0] = byte;

	header = 0x80;
	for(int i = 1; i < size; i++) {
		mask = mask >> 6;
		byte = num & mask;

		byte = byte >> (6 * (size - 1 - i));
		byte = byte | header;
		dest[i] = byte;
	}
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }
    utf8(argv[1]);
}

    
void upper_case(char input[])
{
	for (int i = 0; input[i] != '\0'; i++)
	{
		if (input[i] >= 'a' && input[i] <= 'z')
		{
			input[i] -= 32;
		}
	}
	printf("Uppercased ASCII: %s\n", input);
}    
    
void utf8(char str[]) {
	p1(str);
	upper_case(str);
	p3(str);
	p4(str);
	// p5
	uint32_t buf[1024];
	int size = decode_utf8(str, buf);
	printf("Code points as decimal numbers:");
	for(int i = 0; i < size; i++) {
		printf(" %d", buf[i]);
	}
	puts(" ");
	p6(str);
	p9(str);
	p7(str);
	// p8
	char strbuf[1024];
	printf("Animal Emojis:");
	for(int i = 0; i < size; i++) {
		if(is_animal_emoji_at(str, i)) {
			char animal[1024];
			encode_utf8(buf[i], animal);
			printf(" %s", animal);
		}
	}
	puts(" ");
	// p9
	char index3up1[1024];
	encode_utf8(buf[2] + 1, index3up1);
	printf("Next character of Codepoint at index 3: %s\n", index3up1);
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

void p4(char str[]) {
	int32_t sum = 0;
        for (int i = 0; i < strlen(str); i++) {
                if ((str[i] >> 6) != -2) {
                        sum++;
                }
        }
        printf("Number of code points: %d\n", sum);
}

void p6(char str[]) {
	printf("Bytes per code point: ");
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] >> 7 == 0) {
			printf("1 ");
		} else if (str[i] >> 5 == -2) {
			printf("2 ");
			i++;
		} else if (str[i] >> 4 == -2) {
			printf("3 ");
			i += 2;
		} else if (str[i] >> 3 == -2) {
			printf("4 ");
			i += 3;
		} else {
			printf("-1 ");
		}
	}
	printf("\n");
	
}

void p7(char str[])
{
	uint32_t buf[1024];
	int size = decode_utf8(str, buf);
	int byte_count = 0;
	char substring[1024];
	int i;
	
	for (i = 0; i < size && i < 6; i++)
	{
		uint32_t cp = buf[i];
		int byte_len;

		if (cp <= 0x7F)
		{
			byte_len = 1;
		}
		else if (cp <= 0x7FF)
		{
			byte_len = 2;
		}
		else if (cp <= 0xFFFF)
		{
			byte_len = 3;
		}
		else if (cp <= 0x10FFFF)
		{
			byte_len = 4;
		}
		else
		{
			break;
		}
	
		memcpy(substring + byte_count, str + byte_count, byte_len);

		byte_count += byte_len;
	}
	substring[byte_count] = '\0';

	printf("Substring of the first 6 code points: %s\n", substring);
}

void p9(char str[]) {
	
}
