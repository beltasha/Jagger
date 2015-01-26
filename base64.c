#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define _CRT_SECURE_NO_WARNINGS

/* ---- Base64 Encoding/Decoding Table --- */
char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* decodeblock - decode 4 '6-bit' characters into 3 8-bit binary bytes */

static char encoding_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/' };
static char *decoding_table = NULL;
static int mod_table[] = { 0, 2, 1 };


char *base64_encode(const unsigned char *data,
	size_t input_length,
	size_t *output_length) {

	*output_length = 4 * ((input_length + 2) / 3);

	char *encoded_data = malloc(*output_length);
	if (encoded_data == NULL) return NULL;

	for (int i = 0, j = 0; i < input_length;) {

		uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
		uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
		uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

		uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

		encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
	}

	for (int i = 0; i < mod_table[input_length % 3]; i++)
		encoded_data[*output_length - 1 - i] = '=';

	return encoded_data;
}


void decodeblock(unsigned char in[], char *clrstr) {
	unsigned char out[4];
	out[0] = in[0] << 2 | in[1] >> 4;
	out[1] = in[1] << 4 | in[2] >> 2;
	out[2] = in[2] << 6 | in[3] >> 0;
	out[3] = '\0';
	strncat(clrstr, out, sizeof(out));
}

void b64_decode(char *b64src, char *clrdst) {
	int c, phase, i;
	unsigned char in[4];
	char *p;

	clrdst[0] = '\0';
	phase = 0; i = 0;
	while (b64src[i]) {
		c = (int)b64src[i];
		if (c == '=') {
			decodeblock(in, clrdst);
			break;
		}
		p = strchr(b64, c);
		if (p) {
			in[phase] = p - b64;
			phase = (phase + 1) % 4;
			if (phase == 0) {
				decodeblock(in, clrdst);
				in[0] = in[1] = in[2] = in[3] = 0;
			}
		}
		i++;
	}
}

/* encodeblock - encode 3 8-bit binary bytes as 4 '6-bit' characters */
void encodeblock(unsigned char in[], char b64str[], int len) {
	unsigned char out[5];
	out[0] = b64[in[0] >> 2];
	out[1] = b64[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
	out[2] = (unsigned char)(len > 1 ? b64[((in[1] & 0x0f) << 2) |
		((in[2] & 0xc0) >> 6)] : '=');
	out[3] = (unsigned char)(len > 2 ? b64[in[2] & 0x3f] : '=');

	b64str[len + 1] = out[0];
	b64str[len + 2] = out[1];
	b64str[len + 3] = out[2];
	b64str[len + 4] = out[3];
}

/* encode - base64 encode a stream, adding padding if needed */
void b64_encode(char *clrstr, char *b64dst) {
	unsigned char in[3];
	int i, len = 0;
	int j = 0;

	//b64dst[0] = '\0';
	while (clrstr[j]) {
		len = 0;
		for (i = 0; i<3; i++) {
			in[i] = (unsigned char)clrstr[j];
			if (clrstr[j]) {
				len++; j++;
			}
			else in[i] = 0;
		}
		if (len) {
			encodeblock(in, b64dst, len, j);
		}
	}
}