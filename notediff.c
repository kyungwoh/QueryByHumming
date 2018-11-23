#include "notediff.h"

/**
 * deltaTime과 pitch의 차이를 계산. 차이값의 합이 최소가 되는값을 리턴한다.
 *
 * @param		humming_file 
 * @param		music_file
 *
 * @ret			min_diff_sum
 */
int 
getMinNoteDiff(char* humming_file, char* music_file)
{
	int min_diff_sum;	
	int file1_curr=0;
	int file2_curr=0;
	
	_note note1[4096];
	_note note2[4096];
	
	char* tok;				/* token으로 분리할 때 사용하는 버퍼. */
	char buffer[128];			
	
	FILE *stream1, *stream2;

	stream1 = fopen(humming_file, "rt");
	if (stream1 == NULL)
		printf("stream1 file open error!!!\n");	

	stream2 = fopen(music_file, "rt");
	if (stream2 == NULL)
		printf("stream2 file open error!!!\n");	

	/* file1(humming)에서 delta값과 time을 읽어들여 note1배열에 note값을 채워 넣는다. */
	while (fgets(buffer, sizeof(buffer), stream1) != NULL) {	
		tok = strtok(buffer, " ");		// Delta Time
		note1[file1_curr].delta = atoi(tok);  
	
		tok = strtok(NULL, " ");		// Pitch
		note1[file1_curr].pitch = atoi(tok);

		file1_curr++;	
	}
	
	/* file2(midi)에서 delta값과 time을 읽어들여 note2배열에 note값을 채워 넣는다. */
	while (fgets(buffer, sizeof(buffer), stream2) != NULL) {	
		tok = strtok(buffer, " ");		// Delta Time
		note2[file2_curr].delta = atoi(tok);  
	
		tok = strtok(NULL, " ");		// Pitch
		note2[file2_curr].pitch = atoi(tok);
		
		file2_curr++;
	}

	/* note1과 note2 중 작은 크기를 정한다. 이 값 크기로 쉬프트해가며 비교하게 된다. */
	if (file1_curr >= file2_curr) {	
		/* 허밍음이 더 클때 */
		min_diff_sum = getNoteDiff(note1, note2, file2_curr, 1);
	} else {
		min_diff_sum = getNoteDiff(note1, note2, file1_curr, file2_curr-file1_curr);
	}

	fclose(stream1);
	fclose(stream2);

	return min_diff_sum;
}

int 
getNoteDiff(_note note1[], _note note2[], int size, int loop_nums)
{
	int i, j;
	int delta_diff, pitch_diff;

	int diff_sum;
	int min_diff_sum = 99999;

	for(i=0; i < loop_nums; i++) {
		diff_sum = 0;

		/* 쉬프트하며 노트 차이를 구한다. */
		for (j=i; j<(size+i); j++) { 
			delta_diff = note1[j-i].delta - note2[j].delta;
			pitch_diff = note1[j-i].pitch - note2[j].pitch;

		//	아직은 delta값만을 가지고 비교해 보자.
		//	diff_sum += hypot(delta_diff, pitch_diff);
			diff_sum += delta_diff; 
		}

		/* 최소값 구하기 */
		if (min_diff_sum > diff_sum) {
			min_diff_sum = diff_sum;
		}
	}

	return min_diff_sum;
}

int 
power(int base, int n)
{
	int i, p=1;

	for (i=1; i<=n; ++i)
		p = p*base;

	return p;
}

