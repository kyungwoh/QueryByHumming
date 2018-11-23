#include "comparator.h"

int 
main(int argc, char* argv[])
{
	char type[128];
	char param[128]; 

	UDR_TYPE = -1;

	if (argc == 2) {
		// histogram 데이터를 구한다면
		if(strcmp(argv[1], "histogram") == 0) {
			search_music(param, HISTOGRAM);	
		} else {
			printf("!!!parameter is not histogram!!!\n");
			printf("Usage : ./qbh type [filename]\n");
			printf("        ./qbh type all \n");
			printf("        ./qbh histogram \n");
		}

	} else if (argc > 2) {
		strcat(param, argv[2]);		// filename or all
		strcat(type, argv[1]);  	// UDR_TYPE

		// UDR_TYPE Setting.
		if(strcmp(type, "type0") == 0) {
			printf("UDR_TYPE0\n");
			UDR_TYPE = 0;
		} else if(strcmp(type, "type1") == 0) {
			printf("UDR_TYPE1\n");
			UDR_TYPE = 1;
		} else if(strcmp(type, "type2") == 0) {
			printf("UDR_TYPE2\n");
			UDR_TYPE = 2;
		} else if(strcmp(type, "type3") == 0) {
			printf("UDR_TYPE3\n");
			UDR_TYPE = 3;
		} else if(strcmp(type, "type4") == 0) {
			printf("UDR_TYPE4\n");
			UDR_TYPE = 4;
		} else if(strcmp(type, "type5") == 0) {
			printf("UDR_TYPE5\n");
			UDR_TYPE = 5;
		} else if(strcmp(type, "type6") == 0) {
			printf("UDR_TYPE6\n");
			UDR_TYPE = 6;
		} else if(strcmp(type, "type7") == 0) {
			printf("UDR_TYPE7\n");
			UDR_TYPE = 7;
		}

		// UDR_TYPE이 정해졌으면 검색. 존재하지 않는 UDR_TYPE 이라면 에러메시지.
		if(UDR_TYPE != -1) {

			if (strcmp(param, "all") == 0) {
				__search_music(param, RANKING);
	  		}
			else {
				search_music(param, EXPAND_UDR);
			}	

		} else {
			printf("This UDR_TYPE isn't supported!!!\n");
		}

	} else {
		printf("Usage : ./qbh type [filename]\n");
		printf("        ./qbh type all \n");
		printf("        ./qbh histogram \n");
	}

	return 0;
}
