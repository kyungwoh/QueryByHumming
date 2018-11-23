#include "udrdiff.h"

/**
 * 확장 UDR을 이용해서 허밍과 음악의 차이값을 계산하는 함수.
 *
 * @param		humming
 * @param		music
 *
 * @ret			diff
 *
 */
int 
getMinMelodyDiff(char* humming, char* music, int type)
{
	int humming_len;
	int music_len;
	
	int* music_melody;
	int* humming_tmp;
	humming_tmp = getExtendUdr(humming, type);

	int humming_melody[4096];
	
	/* get humming_melody and humming_size */
	int idx = 0;
	
	/* 99999 is end sign of melody */
	while (humming_tmp[idx] != 99999) {
		humming_melody[idx] = humming_tmp[idx];
		idx++;
	}
	humming_len = idx - 1; 

	/* get music_melody and music_size */
	music_melody = getExtendUdr(music, type);

	idx = 0;
	// 99999 is end sign of melody
	while (music_melody[idx] != 99999) {
		idx++;
	}
	music_len = idx - 1;

//  구체적인 비교 내용을 볼 때 제목을 표시
//	printf("%s\n", music);

	// calculate minimalize diff value between humming_melody and music_melody	
	int diff=99999;
	if (humming_len < music_len)
	{
		diff = melodyDiff(music_melody, music_len, humming_melody, humming_len, type);
	}
	else 
	{
		diff = melodyDiff(humming_melody, humming_len, music_melody, music_len, type);
	}

	return diff;
}


int 
melodyDiff(int* melody1, int size1, int* melody2, int size2, int type)
{
	int i, j;
	int min_index=0;
	
	int min_diff_sum = 99999;
	int diff_sum;

	for (i=0; i<(size1-size2); i++) {
		diff_sum = 0;

		// calculate difference between two melodies
		for (j=0; j<size2; j++)	{
			// diff_sum += abs(melody1[i+j] - melody2[j]);	
			diff_sum += (melody1[i+j] - melody2[j]) * (melody1[i+j] - melody2[j]);	

            ///////////////////////////////////////////////
            // type==7, 즉 음길이도 포함해서 계산할 경우 //
            ///////////////////////////////////////////////
            if(type == 7){
                int duration_diff;
                duration_diff = ((notelist[i+j].duration - notelist[j].duration) * (notelist[i+j].duration - notelist[j].duration));               

                diff_sum += (int)(duration_diff);
            }


		    //////////////////////////////////////////////////////////
    		// type5인 경우. 새로운 가중치 대입법.                  //
	    	//////////////////////////////////////////////////////////
			if(type == 5) {

				if(diff_sum == 0) diff_sum = 0;
				else if(diff_sum == 1) diff_sum = 1;
				else if(diff_sum == 4) diff_sum = 3;
				else if(diff_sum == 9) diff_sum = 6;
				else if(diff_sum == 16) diff_sum = 10;
				else if(diff_sum == 25) diff_sum = 15;
				else if(diff_sum == 36) diff_sum = 21;
				else diff_sum = 30;				
			}
		}

		if (min_diff_sum > diff_sum) {
			min_diff_sum = diff_sum;
			min_index=i;
		}
	}

	return min_diff_sum;
}


int melodyDiff_duration()
{
    int diff;
        
    diff = notelist[2].duration;
    printf("note duration : %d\n", diff);

    return diff;
}
