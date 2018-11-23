#include "humming.h"


int main(int argc, char* argv[])
{
	int i;
	FILE* src_fd;
	FILE* dest_fd;

	long chunk_size;
	char chunk_size_buf[4];
	
	char header_buffer[44];
	char buffer16[2];
	char empty_buf[2];	

	//memset(empty_buf, 0, 2);
	empty_buf[0] = 0x01;
	empty_buf[0] = 0x00;

	if(argc != 2)
		exit(1);


	// bakup file
	char command[128];
	sprintf(command, "mv %s %s.bak", argv[1], argv[1]);
	system(command);

	// open wav file
	sprintf(command, "%s.bak", argv[1]);
	src_fd = fopen(command, "rb+");
	if (src_fd == NULL)
		printf("src_fd file open error!!!\n");

	dest_fd = fopen(argv[1], "wb+");
	if (dest_fd == NULL)
		printf("dest_fd file open error!!!\n");

	// copy header to dest file
	fread(header_buffer, 1, 44, src_fd);
	fwrite(header_buffer, 1, 44, dest_fd);

	// get chunk_size in wave file
	fseek(src_fd, 40, SEEK_SET);
	fread(chunk_size_buf, 1, 4, src_fd);
	chunk_size = to32bit(chunk_size_buf[3], chunk_size_buf[2], chunk_size_buf[1], chunk_size_buf[0]);	

	// copy to dest file
	fseek(src_fd, 44, SEEK_SET);
	for (i=0; i<(chunk_size/2); i++) {
		short int pitch = 0;
	
		fread(buffer16, 1, 2, src_fd);
		pitch = to16bit(buffer16[1], buffer16[0]);

//		printf("%d \n", pitch);
		
		if ( pitch < 1000 && pitch > -1000 ) {
			fwrite(empty_buf, 1, 2, dest_fd);
		} else {
			fwrite(buffer16, 1, 2, dest_fd);
		}
	}

	printf("wave file's errors was cleaned!!!\n");

	fclose(src_fd);
	fclose(dest_fd);

	return 0;
}

static long to32bit(int c1, int c2, int c3, int c4)
{
	long value = 0L;

	value = (c1 & 0xff);
	value = (value<<8) + (c2 & 0xff);
	value = (value<<8) + (c3 & 0xff);
	value = (value<<8) + (c4 & 0xff);
	return (value);
}

static short int to16bit(int c1, int c2)
{
	short int ret;
	
	ret =  ((c1 & 0xff ) << 8) + (c2 & 0xff);
 
	if (c1 & 80) {
		ret = ~ret + 1;
	}

	return ret ;
}

