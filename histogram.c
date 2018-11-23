#include "histogram.h"

static int note_table[128];

void 
init_histogram()
{
	int i;
	for (i=0; i<128; i++)
		note_table[i] = 0;
}

void 
view_histogram()
{
	int i;
   	/* 파일을 다 읽었으면 출력. */
	for (i=0; i<128; i++) {
		if (note_table[i] != 0) {
			printf("%d %d\n",i-64, note_table[i]);
		}
	}
}

void 
histogram(char* fname)
{
	char* tok;
	char buffer[128];

	int curr_pitch_diff;			/* note들의 차이값을 담는 임시변수. */
	int pre_delta = -99999;			/* 이전 note의 delta_time을 저장하는 변수. */
	int pre_pitch = -99999;			/* 이전 note의 pitch을 저장하는 변수. */
	int curr_delta, curr_pitch;

	FILE *stream;

	stream = fopen(fname, "rt");
	if (stream == NULL)
		printf("file open error(in histogram)!!!\n");	

    /* 음 높이 구하고 */
	while (fgets(buffer, sizeof(buffer), stream) != NULL)
	{
		/* 음악의 처음 부분이라면 */
		if (pre_delta == -99999 && pre_pitch == -99999)
		{
			tok = strtok(buffer, " ");	// Delta Time
			pre_delta = atoi(tok);
		
			tok = strtok(NULL, " ");	// Pitch
			pre_pitch = atoi(tok);
			
			continue;
		}		

		tok = strtok(buffer, " ");		// Delta Time
		curr_delta = atoi(tok);
		
		tok = strtok(NULL, " ");		// Pitch
		curr_pitch = atoi(tok);

		curr_pitch_diff = pre_pitch - curr_pitch;

		note_table[curr_pitch_diff+64] += 1; 		// index에 64를 더하는 것은 음들의 분포 범위가 최대 8옥타브를 넘나들지 못할거라는 추축.
	}

	fclose(stream);

	return;
}

