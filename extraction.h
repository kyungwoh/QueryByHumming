#ifndef _EXTRACTION_H_
#define _EXTRACTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* getUdr(char* fname);
int* getExtendUdr(char* fname, int type);

typedef struct _note{
    int duration;
    int pitch;
} note;

// 음표의 음길이, 음높이를 저장할 리스트
note notelist[4096];

#endif
