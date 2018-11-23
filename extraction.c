#include "extraction.h"


/**
 * UDR을 생성해주는 함수
 *
 * @param 		*fname UDR을 생성하려는 멜로디 파일
 */
char udr_list[4096];
char* 
getUdr(char* fname)
{
	int i;
	int delta;
	int pitch;
	int pre_pitch = -1000;				/* 이전의 pitch 값을 저장할 변수 */
	
	char* tok;
	char buffer[128];
	
	FILE *stream;
	
	for(i=0; i<(int)sizeof(udr_list); i++)
		udr_list[i] = 0;

	stream = fopen(fname, "rt");
	if(stream == NULL) {
		printf("file open error!!!\n");	
		exit(0);
	}

	while( fgets(buffer, sizeof(buffer), stream) != NULL) {	
		tok = strtok(buffer, " ");		/* Delta Time */
		delta = atoi(tok);
		
		tok = strtok(NULL, " ");		/* Pitch */
		pitch = atoi(tok);

		if(pre_pitch != -1000) {		/* 이전 음표와 비교하여 UDR 문자로 치환 */
			int udr = pre_pitch - pitch;

			if (udr > 0) {
				strcat(udr_list, "D");	
			} else if (udr < 0) {
				strcat(udr_list, "U");
			} else if (udr == 0) {
				strcat(udr_list, "R");
			}
		}
		pre_pitch = pitch;				/* 현재 pitch 값을 저장. */
	}

	fclose(stream);

	return udr_list;
}


/**
 * TYPE별로 확장 UDR을 생성해주는 함수. 
 * U1, U2, Un ... D1, D2, Dn ... R 와 같이 UDR을 세분화 시킨 표현방법.
 *
 * @param 		fname 멜로디 파일
 */
int melody_list[4096];

int* 
getExtendUdr(char *fname, int type)
{
	float duration;
	float pre_duration = -99999;
    int pitch;
	int pre_pitch = -99999;				// 이전의 pitch 값을 저장할 변수
    

	char* tok;
	char buffer[64];
	
	FILE *stream;

	stream = fopen(fname, "rt");
	if (stream == NULL) {
		printf("file open error(%d)!!!\n", (int)stream);	
	}

	int melody_idx = 0;
	while (fgets(buffer, sizeof(buffer), stream) != NULL) {	
		tok = strtok(buffer, " ");		// Duration Time
		duration = atoi(tok);

		tok = strtok(NULL, " ");		// Pitch
		pitch = atoi(tok);

        //priint duration, pitch        
        //printf("duration,pitch : %d, %d\n", duration, pitch);

		if (pre_pitch != -99999)			
		{
			int udr = pitch - pre_pitch;
            int slr = duration - pre_duration;

			if(type ==  0)	{		//기본적인 UDR
				if (udr >= 1){
					melody_list[melody_idx] = 1;
                    notelist->pitch = 1;
				} else if (udr <= -1) {
					melody_list[melody_idx] = -1;
                    notelist->pitch = -1;
				} else if (udr == 0) {
					melody_list[melody_idx] = 0;
                    notelist->pitch = 0;
				}

                if(slr >= 1){
                    notelist->duration = 1;
                } else if(slr <= -1){
                    notelist->duration = -1;
                } else if(slr == 0){
                    notelist->duration = 0;
                }

			}
			else if(type ==  1)	{		// 대칭적인 멜로디 표현

				if (udr >= 1 && udr <= 2){
					melody_list[melody_idx] = 1;
				} 
				else if (udr >= 3 && udr <= 5)
				{
					melody_list[melody_idx] = 2;
				} 
				else if (udr >= 6 && udr <= 9)
				{
					melody_list[melody_idx] = 3;
				} 
				else if (udr >= 10)
				{
					melody_list[melody_idx] = 4;
				} 
				else if (udr <= -1 && udr >= -2)
				{
					melody_list[melody_idx] = -1;
				} 
				else if (udr <= -3 && udr >= -5) 
				{
					melody_list[melody_idx] = -2;
				} 
				else if (udr <= -6 && udr >= -9) 
				{
					melody_list[melody_idx] = -3;
				} 
				else if (udr <= -10) 
				{
					melody_list[melody_idx] = -4;
				} 
				else if (udr == 0) 
				{
					melody_list[melody_idx] = 0;
				}
			

			} else if (type == 2) {		// 미디파일의 음변화에 대한 히스토그램을 고려한 히스토그램

				if(udr >= 1 && udr <= 2){
					melody_list[melody_idx] = 1;
				} 
				else if(udr >= 3 && udr <= 4)
				{
					melody_list[melody_idx] = 2;
				} 
				else if(udr >= 5 && udr <= 8)
				{
					melody_list[melody_idx] = 3;
				} 
				else if(udr >= 9)
				{
					melody_list[melody_idx] = 4;
				} 
				else if(udr <= -1 && udr >= -2)
				{
					melody_list[melody_idx] = -1;
				} 
				else if(udr <= -3 && udr >= -5) 
				{
					melody_list[melody_idx] = -2;
				} 
				else if(udr <= -6 && udr >= -9) 
				{
					melody_list[melody_idx] = -3;
				} 
				else if(udr <= -10) 
				{
					melody_list[melody_idx] = -4;
				} 
				else if(udr == 0) 
				{
					melody_list[melody_idx] = 0;
				}

			} else if (type == 3) {  	// 9이하의 음차이를 보이는 음들은 개별적으로 구분.
				

				if ((udr < 10) && (udr > -10)){
					melody_list[melody_idx] = udr;
				} 

			} else if (type == 4) {		// 대푯값으로 가중치 설정
	
				if(udr == 1 ){
					melody_list[melody_idx] = 1;
				} 
				else if(udr >= 2 && udr <= 3)
				{
					melody_list[melody_idx] = 2;
				} 
				else if(udr >= 4 && udr <= 6)
				{
					melody_list[melody_idx] = 4;
				} 
				else if(udr >= 7 && udr <= 9)
				{
					melody_list[melody_idx] = 7;
				} 
				else if(udr >= 10)
				{
					melody_list[melody_idx] = 10;
				} 
				else if(udr == -1)
				{
					melody_list[melody_idx] = -1;
				} 
				else if(udr <= -2 && udr >= -3) 
				{
					melody_list[melody_idx] = -2;
				} 
				else if(udr <= -4 && udr >= -6) 
				{
					melody_list[melody_idx] = -4;
				} 
				else if(udr <= -7 && udr >= -9) 
				{
					melody_list[melody_idx] = -7;
				} 
				else if(udr <= -10) 
				{
					melody_list[melody_idx] = -10;
				} 
				else if(udr == 0) 
				{
					melody_list[melody_idx] = 0;
				}

			} else if (type == 5) {
	
				if(udr == 0){
					melody_list[melody_idx] = 0;
				} 
				else if(udr >= 1 && udr <= 2)
				{
					melody_list[melody_idx] = 1;
				} 
				else if(udr >= 3 && udr <= 6)
				{
					melody_list[melody_idx] = 2;
				} 
				else if(udr >= 7)
				{
					melody_list[melody_idx] = 3;
				} 
				else if(udr <= -1 && udr >= -2) 
				{
					melody_list[melody_idx] = -1;
				} 
				else if(udr <= -3 && udr >= -6) 
				{
					melody_list[melody_idx] = -2;
				} 
				else if(udr <= -7) 
				{
					melody_list[melody_idx] = -3;
				} 

			} else if (type == 6) {
	
				if(udr <= 1 && udr >= -1 ){
					melody_list[melody_idx] = 0;
				} 
				else if(udr >= 2 && udr <= 5)
				{
					melody_list[melody_idx] = 3;
				} 
				else if(udr >= 6)
				{
					melody_list[melody_idx] = 6;
				} 
				else if(udr <= -2 && udr >= -5) 
				{
					melody_list[melody_idx] = -3;
				} 
				else if(udr >= -6) 
				{
					melody_list[melody_idx] = -6;
				} 
			} else if (type == 7) {

                // 음높이 가중치 계산
                if(udr <= 1 && udr >= -1 ){
					melody_list[melody_idx] = 0;
				} 
				else if(udr >= 2 && udr <= 5)
				{
					melody_list[melody_idx] = 3;
				} 
				else if(udr >= 6)
				{
					melody_list[melody_idx] = 6;
				} 
				else if(udr <= -2 && udr >= -5) 
				{
					melody_list[melody_idx] = -3;
				} 
				else if(udr >= -6) 
				{
					melody_list[melody_idx] = -6;
				} 
               


                // 음길이 비율 계산
                int duration_diff;
                if(duration > pre_duration) {
                    duration_diff = (int)(duration/pre_duration); 
                } else {
                    duration_diff = (int)(pre_duration/duration);
                    duration_diff = duration_diff*-2;
                }

                // 음길이 가중치? 적용
                if(duration_diff >= 0 && duration_diff <= 1)
                    notelist[melody_idx].duration = 0; 
                else if(duration_diff >= 2 && duration_diff <= 3)
                    notelist[melody_idx].duration = 1; 
                else if(duration_diff >= 4 && duration_diff <= 8)
                    notelist[melody_idx].duration = 1; 
                else if(duration_diff >= 9)
                    notelist[melody_idx].duration = 1;

                if(duration_diff >= -1 && duration_diff <= 0)
                    notelist[melody_idx].duration = 0; 
                else if(duration_diff >= -3 && duration_diff <= -2)
                    notelist[melody_idx].duration = -1; 
                else if(duration_diff >= -8 && duration_diff <= 4)
                    notelist[melody_idx].duration = -1; 
                else if(duration_diff <= -9)
                    notelist[melody_idx].duration = -1;

      /*        if (slr >= 1 && slr <= 100){
                    notelist[melody_idx].duration = 1;
				} 
				else if (slr >= 101 && slr <= 250)
				{
                    notelist[melody_idx].duration = 2;
				} 
				else if (slr >= 251 && slr <= 500)
				{
                    notelist[melody_idx].duration = 3;
				} 
				else if (slr >= 501)
				{
                    notelist[melody_idx].duration = 4;
				} 
				else if (slr <= -1 && slr >= -100)
				{
                    notelist[melody_idx].duration = -1;
				} 
				else if (slr <= -101 && slr >= -250) 
				{
                    notelist[melody_idx].duration = -2;
				} 
				else if (slr <= -251 && slr >= -500) 
				{
                    notelist[melody_idx].duration = -3;
				} 
				else if (slr <= -501) 
				{
                    notelist[melody_idx].duration = -4;
				} 
				else if (slr == 0) 
				{
                    notelist[melody_idx].duration = 0;

                }
    */

	/*
                if(slr >= 1){
                    notelist[melody_idx].pitch = 1;
                    notelist[melody_idx].duration = 1;
                } else if(slr <= -1){
                    notelist[melody_idx].pitch = -1;
                    notelist[melody_idx].duration = -1;
                } else if(slr == 0){
                    notelist[melody_idx].pitch = 0;
                    notelist[melody_idx].duration = 0;
                }
	*/
            }
		}
		pre_pitch = pitch;			    	// 현재 pitch 값을 저장.
		pre_duration = duration;			// 현재 duration 값을 저장.
		melody_idx++;
	}

	// 99999 is end sign of melody
	melody_list[melody_idx] = 99999;	

    notelist[melody_idx].duration = 99999;
    notelist[melody_idx].pitch = 99999;


	fclose(stream);

	return melody_list;
}

