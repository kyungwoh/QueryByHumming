#include "extraction.h"
#include "editdistance.h"
#include "notediff.h"
#include "histogram.h"
#include "udrdiff.h"

#define 	QBH_LIST_SIZE	512
#define 	UDR				1
#define		EXPAND_UDR		2
#define		HISTOGRAM		3	
#define		RANKING			4

int UDR_TYPE;

char* __search_music(char* humming, int option);
char* search_music(char* humming, int option);
void sort_music_list( char* humming );
void readQbhList();
int get_ranking_list( char* humming );
