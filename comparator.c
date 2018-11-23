/**
 *
 * @name	comparator.c
 * @author	Nam hyunwoo
 * @date 	2007-08-01
 *
 */

#include "comparator.h"
#include "humming_data.h"

static int target_music_idx = 0;
static char* qbh_list[QBH_LIST_SIZE][128];
static int qbh_list_size;

static char* humming_list[QBH_LIST_SIZE][128];
static int humming_list_size;

static int music_diff_list[QBH_LIST_SIZE][2]; 	// [2] = [index][music diff value]


/*
 * TYPE 별로 모든 허밍음의 순위값을 구한다.
*/
char* 
__search_music(char *humming, int option)
{
	int i, j;
	int min;
	int song_simility;
	int same_song_nums;

	/* music_data 디렉토리에서 음악 파일들의 리스트를 가져온다. */
	readQbhList();
	
    int search_rank_num = 0;
    int search_avg = 0;
	
    for (i=0; i<humming_list_size; i++) {		
		min = 9999;

		for (j=0; j<qbh_list_size; j++) {
			char qbh_path[128] = "./music_data/";
			strcat(qbh_path, (char*)qbh_list[j]);	

			char humming_path[128] = "./humming_data/";
			strcat(humming_path, (char*)humming_list[i]);	

			//printf("%s %s ", humming_path, qbh_path);
			//printf("%d\n", getEditDistance(humming_path, qbh_path));
			
			song_simility = getEditDistance(humming_path, qbh_path);
			if(min > song_simility) {
				min = song_simility;
				same_song_nums = j;
			}
		}

		target_music_idx = same_song_nums;
		search_music((char*)humming_list[i], option);    


	    for (j=0; j<qbh_list_size; j++) {
		    if ( !strcmp((char*)qbh_list[target_music_idx], (char*)qbh_list[ music_diff_list[j][1] ] )) {
                //printf("=== %d : %d\n", j+1, music_diff_list[j][1]);
                int curr_rank = j+1;
                if(j<=10) {
                //    printf("below 10 : %d\n", music_diff_list[j][1]);   
                    search_rank_num++;
                }
                search_avg += curr_rank;
                //printf("curr total sum : %d\n", search_avg);
            }
    	}
    }
	
    printf("Rank(~10) : %d\nAvg Rank : %d\n", search_rank_num, search_avg/humming_list_size);

	return humming;	
}


/**
 * 실제 음악을 찾으려고 할 때 맨 처음 호출하게 되는 함수.
 * ps) 미구현 부분 : 검색된 결과를 리턴한다.
 *
 * @return		유사도에 따라 순위가 정해진 음악 리스트
 * @param 		*humming		허밍파일(*.wav)의 경로.
 */
char* 
search_music(char *humming, int option)
{
	int i;
	char* ret_music;

	/* music_data 디렉토리에서 음악 파일들의 리스트를 가져온다. */
	readQbhList();
	
	if (option == UDR) {

		//for (i=0; i<qbh_list_size; i++)	{
		for (i=0; i<100; i++)	{
			char qbh_path[128] = "./music_data/";
			strcat(qbh_path, (char*)qbh_list[i]);	
			
			// Edit Distance 알고리즘을 사용하여 비교.
			music_diff_list[i][1] = i;											// IndexI
//			music_diff_list[i][2] = getMinEditDistance(humming, qbh_path);		// Music Difference Value
			music_diff_list[i][2] = getEditDistance(humming, qbh_path);			// Music Difference Value

		}
		
		sort_music_list( humming );

	} else if (option == EXPAND_UDR) {

		for (i=0; i<qbh_list_size; i++)	{
			char qbh_path[128] = "./music_data/";
			strcat(qbh_path, (char*)qbh_list[i]);	

			music_diff_list[i][1] = i;												// IndexI
			music_diff_list[i][2] = getMinMelodyDiff(humming, qbh_path, UDR_TYPE);	// Music Difference Value
		}

		sort_music_list(humming);

	} else if (option == HISTOGRAM) {

		init_histogram();
		for (i=0; i<qbh_list_size; i++)	{
			char qbh_path[128] = "./music_data/";
			strcat(qbh_path, (char*)qbh_list[i]);	

			histogram(qbh_path);
		}
		// 음악들을 순위에 따라 정렬시킨다.
		view_histogram();
	} else if (option == RANKING) {
	
		for (i=0; i<qbh_list_size; i++)	{
			char qbh_path[128] = "./music_data/";
			strcat(qbh_path, (char*)qbh_list[i]);	

			music_diff_list[i][1] = i;												// IndexI
			music_diff_list[i][2] = getMinMelodyDiff(humming, qbh_path, UDR_TYPE);	// Music Difference Value
		}
		
		get_ranking_list(humming);
	}
	UDR_TYPE = UDR_TYPE;

	return ret_music;
}


char* 
getSimilityName(char* name1, char* name2)
{
	name1 = name1;
	name2 = name2;

	return 0;
}

/*
* 허밍음과의 유사도 체크가 끝난 음악리스트를 내림차순으로 정리.
*/
void 
sort_music_list( char* humming )
{
	int tmp_index, tmp_diff;
	int i, j;			

	humming = humming; // dummy

	/* 내림차순으로 정렬 */
	for (i=0; i<qbh_list_size; i++) {
		for (j=i+1; j<qbh_list_size; j++) {
			/* 다음번째 음보다 클경우 */
			if (music_diff_list[i][2] > music_diff_list[j][2])	{
				/* 현재음을 임시버퍼에 저장 */
				tmp_index = music_diff_list[i][1];	
				tmp_diff = music_diff_list[i][2];
				
				/* 음악의 차이값 교환 */
				music_diff_list[i][1] =	music_diff_list[j][1]; 
				music_diff_list[i][2] =	music_diff_list[j][2]; 

				music_diff_list[j][1] = tmp_index; 
				music_diff_list[j][2] = tmp_diff; 
			}
		}
	}

	/* 정렬된 음악 리스트 출력 */
	//for (i=0; i<qbh_list_size; i++)
	for (i=0; i<50; i++) {
		printf("[%s] = %d\n", (char*)qbh_list[ music_diff_list[i][1] ], music_diff_list[i][2]);
	}

}


/** 
 * ./qbh_data 디렉토리에 있는 파일(음악)들의 리스트를 읽어서 qbh_list에 저장.
 */
void 
readQbhList()
{
	// "./music_data" 디렉토리에 있는 음악들의 리스트를 생성. 
	system("cd ./music_data; ls -w1 ./*.qbh > ../music_list.txt");

	char buffer[128];
	FILE *stream;

	stream = fopen("./music_list.txt", "rt");
	if (stream == NULL)
		printf("file open error!!!\n");	

	// 리스트 파일을 읽고 이를 qbh_list[]에 저장. 
	int cnt=0;
	while (fgets(buffer, sizeof(buffer), stream) != NULL) {
		strcpy((char*)qbh_list[cnt], strtok(buffer, "\n"));
		cnt++;
	}

	qbh_list_size = cnt;
	
	fclose(stream);
	
	// "./humming_data" 디렉토리에 있는 허밍들의 리스트를 생성. 
	system("ls -w1 ./humming_data/*.qbh > humming_list.txt");

	stream = fopen("./humming_list.txt", "rt");
	if (stream == NULL)
		printf("file open error!!!\n");	

	// 리스트 파일을 읽고 이를 humming_list[]에 저장. 
	cnt=0;
	while (fgets(buffer, sizeof(buffer), stream) != NULL) {
		strcpy((char*)humming_list[cnt], strtok(buffer, "\n"));
	    cnt++;
	}

	humming_list_size = cnt;
	
	fclose(stream);
}


int get_ranking_list( char* humming )
{
	int tmp_index, tmp_diff;
	int i, j;			

	/* 내림차순으로 정렬 */
	for (i=0; i<qbh_list_size; i++) {
		for (j=i+1; j<qbh_list_size; j++) {
			/* 다음번째 음보다 클경우 */
			if (music_diff_list[i][2] > music_diff_list[j][2])	{
				/* 현재음을 임시버퍼에 저장 */
				tmp_index = music_diff_list[i][1];	
				tmp_diff = music_diff_list[i][2];
				
				/* 음악의 차이값 교환 */
				music_diff_list[i][1] =	music_diff_list[j][1]; 
				music_diff_list[i][2] =	music_diff_list[j][2]; 

				music_diff_list[j][1] = tmp_index; 
				music_diff_list[j][2] = tmp_diff; 
			}
		}
	}

/*
	for (i=0; i<36; i++) {
		if ( !strcmp( HUMMING_LIST[i], humming ) ) {
			break;
		}
	}
*/
	// 음악 파일명을 가지고 순위를 알아낸다.
	for (j=0; j<qbh_list_size; j++) {
		if ( !strcmp((char*)qbh_list[target_music_idx], (char*)qbh_list[ music_diff_list[j][1] ] )) {
	        printf("%s %d\n",humming, j+1);
        }
	}
 
    return j+1;

}

