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
   	/* ������ �� �о����� ���. */
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

	int curr_pitch_diff;			/* note���� ���̰��� ��� �ӽú���. */
	int pre_delta = -99999;			/* ���� note�� delta_time�� �����ϴ� ����. */
	int pre_pitch = -99999;			/* ���� note�� pitch�� �����ϴ� ����. */
	int curr_delta, curr_pitch;

	FILE *stream;

	stream = fopen(fname, "rt");
	if (stream == NULL)
		printf("file open error(in histogram)!!!\n");	

    /* �� ���� ���ϰ� */
	while (fgets(buffer, sizeof(buffer), stream) != NULL)
	{
		/* ������ ó�� �κ��̶�� */
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

		note_table[curr_pitch_diff+64] += 1; 		// index�� 64�� ���ϴ� ���� ������ ���� ������ �ִ� 8��Ÿ�긦 �ѳ����� ���ҰŶ�� ����.
	}

	fclose(stream);

	return;
}

