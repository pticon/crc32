#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>


#include "crc32.h"


/* Buffer len
 */
#define BUFFER_LEN	1024


static void usage(const char * progname)
{
	fprintf(stderr, "usage: %s [options] <file>\n", progname);
	fprintf(stderr, "options:\n");
	fprintf(stderr, "\t-h: display this and exit\n");
	fprintf(stderr, "\t-s <seed>: set the seed\n");
}


int main(int argc, char * argv[])
{
	const char * progname = argv[0];
	const char * filename;
	FILE * file = NULL;
	size_t nbytes = 0;
	int ch;
	uint8_t buffer[BUFFER_LEN];
	uint32_t crc = CRC32_INIT;

	/* Parse args
	 */
	while ( (ch = getopt(argc, argv, "hs:")) != -1 )
	{
		switch(ch)
		{
			case 'h':
			usage(progname);
			return 0;

			case 's':
			crc = strtoul(optarg, NULL, 10);
			break;
		}
	}
	argc -= optind;
	argv += optind;

	/* Check args
	 */
	if ( argc < 1 )
	{
		usage(progname);
		return -1;
	}

	/* Assign the filename and open it
	 */
	filename = argv[0];
	if ( (file = fopen(filename, "rb")) == NULL )
	{
		fprintf(stderr, "Cannot open %s: %s\n", filename, strerror(errno));
		return -1;
	}

	/* Parse the file
	 */
	while ( (nbytes = fread(buffer, sizeof(unsigned char), BUFFER_LEN, file)) != 0 )
	{
		crc = crc32_update(buffer, nbytes, crc);
	}

	/* Check if we breaked with an error
	 */
	if ( feof(file) == 0 )
	{
		fprintf(stderr, "Did not process the whole file %s\n", filename);
	}

	/* Close the input file
	 */
	if ( file != NULL )
	{
		if ( fclose(file) != 0 )
		{
			fprintf(stderr, "Cannot close %s: %s\n", filename, strerror(errno));
			return -1;
		}
	}

	/* Print the CRC32
	 */
	fprintf(stdout, "%u\n", crc32_finalize(crc));

	return 0;
}
