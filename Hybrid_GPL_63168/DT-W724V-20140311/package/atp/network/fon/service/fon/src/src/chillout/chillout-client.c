/*
 * chillout-client <identifier> <string> [expected answer]
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "chillout.h"

#define OKSTR "ok\n"

chillout_t chillout;

int main(int argc, char **argv)
{
	char buffer[256], error_string[256];
	char *expected_answer, *fail_prefix;

	if (argc < 3) {
		fprintf(stderr, "usage: chillout-client <identifier> <string> [fail prefix]\n");
		return 1;
	}
	chillout = chillout_init("/tmp/chillout", argv[1], NULL);
	if (chillout == NULL) {
		fprintf(stderr, "Error connecting to chillout socket\n");
		return -1;
	}
	chillout_write(chillout, CHILLOUT_WRITE_SOCK, argv[2]);	
	if (argc > 3) {
		// expected_answer = argv[3];
		fail_prefix = argv[3];
		if (chillout_read(chillout, CHILLOUT_WRITE_SOCK, buffer)) {
			fprintf(stderr, "Error calling chillout_read\n");
			chillout_finish(chillout);
			printf("chillout_read\n");
			return -1;
		} else {
			if(strncmp(buffer, OKSTR, sizeof(OKSTR)))
			{
				if (strlen(buffer) < strlen(fail_prefix)) {
					fprintf(stderr, "Too short answer received: %s", buffer);
					printf("UnKnown\n");
					return -1;
				}
				strcpy(error_string, buffer + 6);
				if (error_string[strlen(error_string) - 1] == '\n') {
					error_string[strlen(error_string) - 1] = '\0';
					fprintf(stderr, "error string: %s\n", error_string);
					return -1;
				}
			}
		}
	}
	chillout_finish(chillout);
	return 0;
}

