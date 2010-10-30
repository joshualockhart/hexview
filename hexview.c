#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STD_CHUNK_SIZE 16

void print_chunk(char *chunk, size_t chunk_size)
{
	int i,j;
	for (j = 0; j < chunk_size; j++)
	{
		// get rid of those unsightly leading FFs with an AND.
		printf("%.2x ", chunk[j] & 0x000000FF);
	}
	
	// If this is an incomplete chunk, pad out with spaces to keep it pretty.	
	if(STD_CHUNK_SIZE > chunk_size)
		for(i = 0; i < STD_CHUNK_SIZE - chunk_size; i++)
			printf("   ");

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
	    address = 0,
	    number_of_complete_chunks,
	    incomplete_chunk_size;

	FILE *fp;
	char c, *chunk;

	if (argc < 2)
	{
		printf("usage: hexview [file]\n");
		exit(0);
	}

	if((fp = fopen(argv[1], "rb")) == NULL)
	{
		perror("Error opening file");
		exit(1);
	}

	fseek(fp, 0L, SEEK_END);
	filesize = ftell(fp);
	rewind(fp);

	number_of_complete_chunks = filesize / STD_CHUNK_SIZE;
	incomplete_chunk_size = filesize % STD_CHUNK_SIZE;

	if((chunk = (char *)malloc(STD_CHUNK_SIZE)) == NULL)
	{
		perror("Malloc");
		exit(1);
	}

	for ( i = 0; i < number_of_complete_chunks; i++)
	{
		fread(chunk, STD_CHUNK_SIZE, 1, fp);
		printf("0x%08x : ", address);
		address += STD_CHUNK_SIZE;
		print_chunk(chunk, STD_CHUNK_SIZE);
	}

	// get the last incomplete chunk at the end of the file. (This chunk has a size of: total_file_size % chunk size)
	fread(chunk, incomplete_chunk_size, 1, fp);
	printf("0x%08x : ", address);
	address += STD_CHUNK_SIZE;

	print_chunk(chunk, (size_t) incomplete_chunk_size);
	free(chunk);
	exit(0);

}

