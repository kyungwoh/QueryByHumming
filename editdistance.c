#include "editdistance.h"
#include "comparator.h"

extern char* qbh_list[];

/************************************************************************************************************** 
 * 소스 문자열 길이만큼을 타켓 문자열에서 가져온다. 타켓 문자열은 쉬프트 해가면 소스 문자열과 비교.
 * EditDistance 알고리즘을 사용하여 유사도를 계산한 결과값이 가장 작은것을 리턴.
 * - 추후 EditDistance 값이 가장 작은값을 가리키는 부분의 정보도 함께 리턴하여 결과 출력시 해당 음악중
 * 어느 부분이 가장 유사도가 큰 부분인지를 알 수 있게 한다.
 ****************************************************************************************************************/
int 
getMinEditDistance(char* query1, char* query2)
{
	int i, curr_diff=0; 
	int min=99999;
	int udr1_len;
	int udr2_len;

	char* udr1;
	char* udr2;	
	char* udr_tmp;

	udr1_len = strlen(getUdr(query1));
	udr2_len = strlen(getUdr(query2));

	udr1 = (char*)malloc(udr1_len);
	udr2 = (char*)malloc(udr2_len);

	strcpy(udr1, getUdr(query1));
	strcpy(udr2, getUdr(query2));
	
	if (udr1_len >= udr2_len) 				/* 허밍 입력의 크기가 음악보다 더 클 경우  */
	{
//		printf("--- humming size is bigger than a music\n");
		return getEditDistance(udr1, udr2);
	}

	for (i=0; i<(udr2_len-udr1_len+1); i++) 
	{
		udr_tmp = (char*)malloc(udr1_len);

		strcpy(udr_tmp, strnget(getUdr(query2), i, udr1_len) );
		
		curr_diff = getEditDistance(udr1, udr_tmp);
//		printf("strnget(%d~%d) : %s %d\n", i, i+udr1_len, udr_tmp, curr_diff);

		if (curr_diff < min) 
			min = curr_diff;
	
		free(udr_tmp);
	}
	
	free(udr1);
	free(udr2);

	UDR_TYPE = UDR_TYPE;

	return min;
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
