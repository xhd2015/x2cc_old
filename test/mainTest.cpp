/*
 * testParseNumber.cpp
 *
 *  Created on: 2017?3?22?
 *      Author: 13774
 */

#include <LexicalParser.h>
#include <SemanticParser.h>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <fstream>
#include <iostream>

#include <macros/all.h>
using namespace std;

#define BUFLEN 1024
char buffer[BUFLEN];

void testFile();
void testParseWord();
void testString();
void testNumber();
void testNote();
void testRecursiveMacro();


int main()
{
  testFile();
//  testParseWord();
//    testString();
//    testNumber();
//  testNote();

}
#define A(i,j) i() k
void testRecursiveMacro()
{

}
void testNote()
{
  LexicalParser lp;
  const char *wordbuf="//0x4e5fz*\n0x\\/";
  size_t index=0;
  int type;
  std::string s;
//  lp.parseNote(wordbuf, index, strlen(wordbuf),s,type);
//  cout << LexicalParser::isInWord(' ')<<endl;
//  return;
  auto hmap=LexicalParser::getHumanReadable();

  cout << "("<<hmap[type]<<","<<s<<")"<<endl;
}

void testNumber()
{
  LexicalParser lp;
  const char *wordbuf="0x4e5fz0x";
  size_t index=0;
  int type;
  std::string s;
//  lp.parseNumber(wordbuf, index, strlen(wordbuf),s,type);
//  cout << LexicalParser::isInWord(' ')<<endl;
//  return;
  auto hmap=LexicalParser::getHumanReadable();

  cout << "("<<hmap[type]<<","<<s<<")"<<endl;
}
void testString()
{
  LexicalParser lp;
  const char *wordbuf="\"eu\\x68asdf \" d";
  size_t index=0;
  int type;
  std::string s;
//  lp.parseString(wordbuf, index, strlen(wordbuf),s,type);
//  cout << LexicalParser::isInWord(' ')<<endl;
//  return;
  auto hmap=LexicalParser::getHumanReadable();

  cout << "("<<hmap[type]<<","<<s<<")"<<endl;
}

void testParseWord()
{
  LexicalParser lp;
  const char *wordbuf="xU_hand23ed ll0";
  size_t index=0;
  int type;
  std::string s;
//  lp.parseId(wordbuf, index, strlen(wordbuf),s,type);
//  cout << LexicalParser::isInWord(' ')<<endl;
//  return;
  auto hmap=LexicalParser::getHumanReadable();

  cout << "("<<hmap[type]<<","<<s<<")"<<endl;
}

void testFile()
{
	LexicalParser lp;
//	const char* str="0x899aad";
//	size_t n=strlen(str);
//	lp.parseNumber(str,n);
//	const char* strbuf="\"ab\\b\\\nadafd\"";
//	size_t index=0;
//	size_t len=strlen(strbuf);
//	lp.parseString(strbuf, index, len);
//	const char *file="examples/normal_test1.cpp";
	const char *file="examples/define_test1.cpp";
	FILE *fp=fopen(file,"r+");
	assert(fp!=NULL);
	size_t readlen=fread(buffer,sizeof(char),BUFLEN,fp);
//	std::string t(buffer,readlen);
	size_t index=0;
	LexicalParser::WordStream stream;
	LexicalParser::TransMap		humanInfo=LexicalParser::getHumanReadable();
	lp.parseWords(stream,buffer, index, BUFLEN);
	printf("As a result,we get stream :\n<start>\n");
	LexicalParser::WordStream::iterator it=stream.begin(),itend=stream.end();
	while(it!=itend)
	{
//	    if(it->second!=LexicalParser::TYPE_SPACE && it->first.size()==1 && LexicalParser::isInSpace(it->first[0]))
//	      printf("%s ",it->first.c_str());
//	    else
	      printf("%s ",humanInfo[it->second].c_str());
	    it++;
	}
	printf("\n<end>\n");
	printf("is that proper for you to do syntax analyze?\n");

	//===Do define macro expansions
	LL1Macro llm(stream,0);
	llm.S();

}





