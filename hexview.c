#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CHUNKSIZE 16

void print_chunk(char *chunk, size_t chunk_size)
{
	int j;
	for (j = 0; j < chunk_size; j++)
	{
		// get rid of those unsightly leading FFs with an AND.
		printf("%.2x ", chunk[j] & 0x000000FF);
	}
	printf(" | ");

	for (j = 0; j < chunk_size; j++)
	{
		if(isprint(chunk[j]))
			printf("%c", chunk[j]);
		else
			printf(".");
	}

	printf(" |\n");
}


int main(int argc, char **argv)
{
	int i,
	    j,
	    filesize,
	    address = 0;

	FILE *fp;
	char c;

	if (argc < 2)
	{
		printf("usage: hexview [file]\n");
		exit(0);
	}

	fp = fopen(argv[1], "rb");

	fseek(fp, 0L, SEEK_END);
	filesize = ftell(fp);
	rewind(fp);

	int number_of_complete_chunks = filesize / DEFAULT_CHUNKSIZE,
	    incomplete_chunk_size = filesize % DEFAULT_CHUNKSIZE;

	char chunk[DEFAULT_CHUNKSIZE];

	for ( i = 0; i < number_of_complete_chunks; i++)
	{
		fread(chunk, DEFAULT_CHUNKSIZE, 1, fp);
		printf("0x%08x : ", address);
		address += 16;
		print_chunk(chunk, sizeof(chunk));
	}

	// get the last incomplete chunk at the end of the file. (This chunk has a size of: total_file_size % chunk size)
	fread(chunk, incomplete_chunk_size, 1, fp);
	printf("0x%08x : ", address);
	address += 16;

	print_chunk(chunk, (size_t) incomplete_chunk_size);

}

