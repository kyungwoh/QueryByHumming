#include "editdistance.h"
#include "comparator.h"

extern char* qbh_list[];

/************************************************************************************************************** 
 * - 허밍음과 음악간의 유사도를 구해주는 함수.
 * 허밍음과 음악에서 멜로디를 UDR로 변환. EditDistance 알고리즘을 이용해 두 스트링간의 차이값을 구함.
 * 작은 크기의 멜로디가 큰 멜로디를 쉬프트 해가면 차이값을 구하고 그중 가장 작은 값을 리턴
 *
 * @param		humming	허밍음
 * @param 		music	음악
 *
 * @return		diff	허밍음과 음악의 차이값
 ****************************************************************************************************************/
int 
getMinEditDistance(char* humming, char* music)
{
	int i;
	int curr_diff = 0; 
	int diff = 99999;

	int hm_len;
	int mu_len;

	char* hm_udr;		/* humming  UDR */
	char* mu_udr;		/* music UDR */
	char* udr_tmp;

	hm_len = strlen(getUdr(humming));
	mu_len = strlen(getUdr(music));

	/* 허밍과 음악에서 얻어진 UDR의 길이만큼 메모리 할당  */
	hm_udr = (char*)malloc(hm_len);
	mu_udr = (char*)malloc(mu_len);

	strcpy(hm_udr, getUdr(humming));
	strcpy(mu_udr, getUdr(music));
	
	/* 허밍 입력의 크기가 음악보다 더 클 경우  */
	if (hm_len >= mu_len) 				
	{
		return getEditDistance(hm_udr, mu_udr);
	}


	/* 쉬프트하며 차이값을 구해서 제일 작은 차이값만을 리턴 */
	for (i=0; i<(mu_len-hm_len+1); i++) 
	{
		udr_tmp = (char*)malloc(hm_len);
		strcpy(udr_tmp, strnget(getUdr(humming), i, hm_len) );
		
		curr_diff = getEditDistance(hm_udr, udr_tmp);

		if (curr_diff < diff) 
			diff = curr_diff;
	
		free(udr_tmp);
	}
	
	free(hm_udr);
	free(mu_udr);

	return diff;
}



/**********************************************************************
*	Edit Diatance 알고리즘에 대한 자세한 설명은 아래의 사이트를 참고. *
*	http://www.merriampark.com/ld.htm                                 *
***********************************************************************/
int 
getEditDistance(char *s, char *t)
{
	int i, j;
	int cost;
	int above, left, diagonal;						/* cost 계산시 참고할 위치의 값을 저장할 변수  */
	int s_len, t_len;								/* source 문자열과 target 문자열의 길이 */
	int** matrix;									/* s_len x t_len 크기의 행렬  */
	
	s_len = strlen(s);								/* 1. source 문자열과 target 문자열의 길이를 구한다. */
	t_len = strlen(t);

	if (s_len==0 || t_len==0)
		return -1;
	
	matrix = (int**)malloc( (s_len+1) * sizeof(int*) ); // matrix[s_len][t_len] 메모리 할당
	for (i=0; i<(s_len+1); i++) 
	{
		matrix[i] = (int*)malloc( (t_len+1) * sizeof(int*) );
	} 
	
	for (i=0; i<(s_len+1); i++) 					/* 2.초기값 설정 */
	{
		matrix[i][0] = i;
	}
	for (i=0; i<(t_len+1); i++) 
	{
		matrix[0][i] = i;
	}

	for (i=1; i<(s_len+1); i++) 
	{
		for (j=1; j<(t_len+1); j++) 
		{
			if ( s[i-1] == t[j-1] ) 				/* 5. cost 계산. s[i]와 t[j]가 같으면 0, 다르면 1. */
			{
				cost = 0;
			} else {
				cost = 1;			
			}

			above = matrix[i][j-1] + cost;			/* 6. 각 셀 위치에서 상단, 대각선(왼쪽상단), 왼쪽에 위치한 */
			left = matrix[i-1][j] + cost;			/* 셀 값들과 cost을 더하여 아래에서 최소값들을 구한다. */
			diagonal = matrix[i-1][j-1] + cost;

			matrix[i][j] = getMin( above, left, diagonal );
	
#ifdef MESSAGE_VIEW		
			printf("x:%d, y:%d, above:%d, left:%d, diagonal:%d, cost:%d, min:%d\n", \
					i, j, above, left, diagonal, cost, getMin(above, left, diagonal) ); 
#endif
		}
	}

#ifdef MESSAGE_VIEW
	printf("\t");
	for (i=0; i<(s_len); i++) 
		printf("\t%c", s[i]); 					// source string 출력

	printf("\n");

	for (j=0; j<(t_len+1); j++)  				// matrix[s_len][t_len] 출력
	{
		if (j>=1) 
			printf("%c\t", t[j-1]); 			// target string 출력
		else 
			printf("\t");		

		for (i=0; i<(s_len+1); i++) 
		{
			printf("%d\t", matrix[i][j]);
		}

		printf("\n");
	}
#endif
		
	for (i=0; i<(s_len+1); i++)					// matrix[s_len][t_len] 메모리 해제
	{
		free(matrix[i]);
	}
	free(matrix);

	
	return matrix[s_len][t_len];
}

int getMin(int a, int b, int c)
{
	int min = 999999;

	if (min > a) min = a;
	if (min > b) min = b;
	if (min > c) min = c;

	return min;
}

char* strnget(char* str, int pos, int nums)
{
	int i, cnt=0;
	static char retStr[1024];	

	for (i=pos; i<pos+nums; i++)
	{
		retStr[cnt] = str[i];	
		cnt++;
	}
	retStr[i] = 0;

	return retStr;
}
