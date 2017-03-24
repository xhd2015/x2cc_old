/*
 * testParseNumber.cpp
 *
 *  Created on: 2017?3?22?
 *      Author: 13774
 */

#include "LexicalParser.h"
#include <cstring>
#include <cstdio>
#include <cassert>

#define BUFLEN 1024
char buffer[BUFLEN];
int main()
{
	LexicalParser lp;
//	const char* str="0x899aad";
//	size_t n=strlen(str);
//	lp.parseNumber(str,n);
//	const char* strbuf="\"ab\\b\\\nadafd\"";
//	size_t index=0;
//	size_t len=strlen(strbuf);
//	lp.parseString(strbuf, index, len);
	FILE *fp=fopen("test/test1.cpp","r+");
	assert(fp!=NULL);
	fread(buffer,sizeof(char),BUFLEN,fp);
	size_t index=0;
	lp.parseWords(buffer, index, BUFLEN);

}



