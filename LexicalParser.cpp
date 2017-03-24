
/*
 * LexicalParser.cpp
 *
 *  Created on: 2017?3?22?
 *      Author: 13774
 */

#include "LexicalParser.h"
#include <cstdio>
#include <cstdlib>
#       include      <cstdlib>

#include <map>
#include <string>
#include <vector>
#include <utility>
using std::map;
using std::string;
using std::vector;
using std::pair;

LexicalParser::LexicalParser() {
	// TODO Auto-generated constructor stub

}

LexicalParser::~LexicalParser() {
	// TODO Auto-generated destructor stub
}
bool LexicalParser::isDigitType(char ch,int type)
{
	return This::tellDigitType(ch)<=type;
}
int LexicalParser::tellDigitType(char ch)
{
	if(ch=='0' || ch=='1')
		return DIGIT_BIN;
	if(ch>='2' && ch<='9')
		return DIGIT_DECIMAL;
	if(ch>='a' && ch<='f')
		return DIGIT_HEX_LOWER;
	if(ch>='A' && ch<='F')
		return DIGIT_HEX_UPPER;
	return DIGIT_UNKNOWN;
}
bool LexicalParser::isInSet(char ch, const char* buffer, size_t len)
{
	for(int i=0;i<len;i++)
	{
		if(ch==buffer[i])return true;
	}
	return false;
}

char	LexicalParser::findValue(char key,const char* buffer,size_t len)
{
	for(int i=0;i<len;i+=2)
	{
		if(buffer[i]==key)return buffer[i+1];
	}
	return 0;
}



void LexicalParser::parseNumber(const char * buffer,size_t n)
{
	char bin_digit[]={'0','1'};
	char dec_digit[]={'0','1','2','3','4','5','6','7','8','9'};
	char hex_digit[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','A','B','C','D','E','F'};
	enum{
		STATE_START,
		STATE_BIN,
		STATE_DECIMAL,
		STATE_HEX,
		STATE_BIN_HEX_DEC
	};
	int curState=STATE_START;
	int nextState;
	char curInput;
	int val=0;
	int i;
	for(i=0;i<n;i++)
	{
		curInput=buffer[i];
		if(curState==STATE_START)
		{
			if(curInput=='0')
			{
				nextState=STATE_BIN_HEX_DEC;
			}else if(curInput>='0' && curInput<='9'){
				val = 10*val + curInput - '0';
				nextState=STATE_DECIMAL;
			}else
				goto error;
		}else if(curState==STATE_DECIMAL){
			nextState=STATE_DECIMAL;
			if(curInput>= '0' && curInput<= '9')
			{
				val = 10*val + curInput - '0';
			}else
				goto error;
		}else if(curState==STATE_HEX){
			nextState=STATE_HEX;
			if(This::isDigitType(curInput, DIGIT_BIN)||
					This::isDigitType(curInput, DIGIT_DECIMAL)
			)
			{
				val = val*16 + curInput - '0';
			}else if(This::isDigitType(curInput, DIGIT_HEX_LOWER))
			{
				val = val*16 + curInput - 'a'+10;
			}else if(This::isDigitType(curInput,DIGIT_HEX_UPPER))
			{
				val=val*16+curInput - 'A'+10;
			}else
				goto error;
		}else if(curState==STATE_BIN){
			nextState=STATE_BIN;
			if(This::isDigitType(curInput,DIGIT_BIN))
			{
				val = 2*val + curInput - '0';
			}
		}else if(curState==STATE_BIN_HEX_DEC){
			if(curInput=='x')
			{
				nextState=STATE_HEX;
			}else if(curInput=='b'){
				nextState=STATE_BIN;
			}else if(curInput>='0' || curInput<='9'){
				val = val*10 + curInput-'0';
				nextState=STATE_DECIMAL;
			}
		}
		curState=nextState;
	}
	printf("input is %s,parsed is %d\n",buffer,val);

	return;
	error:
		printf("error: i = %d , state = %d\n",i,curState);
		This::error("error in parseNumber");
}
/**
 * General purposed automata
 *
 *		typedef int (*GET_NEXT_STATE)(int,char)
 * 		int getNextState(int state,char ch);
 *
 * 		typedef void (*PROCESS_OUTPUT)(int state,char ch,char *buffer,size_t &bufindex)
 * 		void processOutput(int state,char ch,char *buffer,size_t &bufindex);
 *
 * 		void errorProcess(int errState,char ch);
 *
 *
 * 		parseElement(const char *buffer,size_t &index,size_t len,char *buf,size_t &bufi,
 * 		int &curState,
 * 		int errState,int endState,GET_NEXT_STATE getNextState,PROCESS_OUTPUT processOutput)
 * 		{
 * 			int i;
 * 			char ch;
 * 			int nextState;
 *			for(i=index;i<size_t;i++)
 *			{
 *				if(curState==errState)
 *				{
 *					return;
 *				}else if(curState==endState){
 *					return;
 *				}
 *				ch=buffer[i];
 *				nextState=getNextState(curState,ch);
 *				processOutput(curState,ch,buf,bufi);
 *				curState=nextState;
 *			}
 *
 *
 * 		}
 */
void LexicalParser::parseString(const char* buffer, size_t& index, size_t len)
{
	const char transfer[]={'n','\n','b','\b','\\','\\','t','\t'};
	enum{
		STATE_START,STATE_SWALLOW,STATE_TRANSFER,STATE_END
	};
	char ch;
	int i;
	int curState=STATE_START,nextState;
	char *buf=new char[100];
	int bufi=0;
	for(i=index;i<len;i++)
	{
		ch=buffer[i];
		if(curState==STATE_START)
		{
			if(ch=='"')
			{
				nextState=STATE_SWALLOW;
			}else{
				goto error;
			}
		}else if(curState==STATE_SWALLOW){
			if(ch=='"'){
				buf[bufi++]=0;
				nextState=STATE_END;
			}
			else if(ch=='\\')
			{
				nextState=STATE_TRANSFER;
			}else{
				buf[bufi++]=ch;
			}
		}else if(curState==STATE_TRANSFER){
			if(ch=='\n'){
				nextState=STATE_SWALLOW;
			}else if(ch=='n' || ch=='t' || ch=='b'||ch=='\\'){
				buf[bufi++]=This::findValue(ch, transfer,ARRSIZE(transfer));
				nextState=STATE_SWALLOW;
			}else{
				goto error;//There is an error about transferring
			}
		}else if(curState==STATE_END){
			break;
		}
		curState=nextState;
	}
	index +=i;
	if(curState!=STATE_END)
	{
		This::error("input is not a valid string");
	}else{
		printf("input : %.*s, output \"%s\"(\" in the border is not part of it) ,index is at %d\n",len,buffer,buf,index);
	}
	delete [] buf;
	return;
	error:
		This::error("error in parseString");
}
/**
 * first loop,merge '\\' '\n' to NOTHING
 * 			 	merge [\s\t]* to NOTHING
 * 			 	merge \n to NOTHING if start is not #[\s\t\n]*define
 * 			 		else merge it into MACROEND
 * 			 	recognise '..'	to CHAR,take care of \'
 *
 * 			 	recognise "..." to STRING,take care of \"
 *
 */
void LexicalParser::parseWords(const char* buffer, size_t& index, size_t len)
{
	enum{
		STATE_START,
		STATE_S1_START,
		STATE_S1_DIRECTIVE,
		STATE_S1_DEFINE_DIRECTIVE,
		STATE_S1_DEFINE_NAME,
		STATE_S1_DEFINE_BODY,
		STATE_S1_INCLUDE,
		STATE_S1_DIRECTIVE_BODY,
		STATE_S1_END,
		STATE_DEFINE,
		STATE_LESS,
		STATE_GREATER,
		STATE_EQUAL,
		STATE_DIVID,
		STATE_STAR,
		STATE_LINE_NOTE,
		STATE_MULTILINE_NOTE,
		STATE_MULTILINE_GOING_END,
		STATE_SWALLOW_SPACE,
		STATE_TRANSFER,
		STATE_TRANSFER_CARRIGE,
		STATE_ID,
		STATE_STRING,
		STATE_CHAR,
		STATE_NUMBER,
		STATE_HEX_NUMBER,
		STATE_DECILMAL_NUMBER,
		STATE_BIN_NUMBER,
		STATE_NO_CARE,
	};
	enum{
		DONE_ONE_WORD,
		DOING_ONE_WROD
	};
	enum{
		TYPE_OP=0,
		TYPE_SPLIT=1,
		TYPE_NUMBER_DECIMAL=2,
		TYPE_NUMBER_BIN=23,
		TYPE_NUMBER_HEX=24,
		TYPE_ID=3,
		TYPE_STRING=4,
		TYPE_CHAR=5,
		TYPE_TRANSFER=6,
		TYPE_S1_START=16,
		TYPE_S1_END=7,
		TYPE_LESS=17,
		TYPE_GREATER=18,
		TYPE_S1_DEFINE=19,
		TYPE_S1_INCLUDE=22,
		TYPE_S1_BODY=20,
		TYPE_S1_DIRECTIVE=21,
		TYPE_NOTE=8,
		TYPE_LBRACE=9,
		TYPE_RBRACE=10,
		TYPE_LHBRACE=11,/*{ huge brace*/
		TYPE_RHBRACE=12,
		TYPE_POINTER=13,
		TYPE_NONE=14,
		TYPE_UNKNOW=15,

		SUBTYPE_COMP_MULTIPLE,

		SUBTYPE_DLESS,
		SUBTYPE_LESS,

		SUBTYPE_BIN,
		SUBTYPE_DECILMAL,
		SUBTYPE_HEX,
	};
	map<int,string> humanInfo;
	humanInfo[TYPE_OP]="OP";
	humanInfo[TYPE_SPLIT]="SPLIT";
	humanInfo[TYPE_NUMBER_DECIMAL] = "NUMBER_DECIMAL",
	humanInfo[TYPE_NUMBER_HEX] = "NUMBER_HEX",
	humanInfo[TYPE_NUMBER_BIN] = "NUMBER_BIN",
	humanInfo[TYPE_ID] = "ID";
	humanInfo[TYPE_STRING] = "STRING";
	humanInfo[TYPE_CHAR] = "CHAR";
	humanInfo[TYPE_TRANSFER] = "TRANSFER";
	humanInfo[TYPE_S1_END] = "S1_END";
	humanInfo[TYPE_NOTE] = "NOTE";
	humanInfo[TYPE_LBRACE] = "LBRACE";
	humanInfo[TYPE_RBRACE] = "RBRACE";
	humanInfo[TYPE_LHBRACE] = "LHBRACE";
	humanInfo[TYPE_RHBRACE] = "RHBRACE";
	humanInfo[TYPE_POINTER] = "POINTER";
	humanInfo[TYPE_NONE] = "NONE";
	humanInfo[TYPE_UNKNOW] = "UNKNOW";
	humanInfo[TYPE_S1_START] = "S1_START";
	humanInfo[TYPE_LESS] = "LESS";
	humanInfo[TYPE_GREATER] = "GREATER";
	humanInfo[TYPE_S1_DEFINE] = "S1_DEFINE";
	humanInfo[TYPE_S1_INCLUDE] = "S1_INCLUDE";
	humanInfo[TYPE_S1_BODY] = "S1_BODY";
	humanInfo[TYPE_S1_DIRECTIVE] = "S1_DIRECTIVE";

	char ch;
	int i;
	char *buf=new char[100];
	int bufi=0;
	int curState=STATE_START,nextState,lastState;
	bool	prepP=false;
	int wordP=DOING_ONE_WROD;
	int curType,subType;
	vector<pair<string,int> > output;
	string currentOutput;
	for(i=index;i<len;i++)
	{
		ch=buffer[i];
		switch(curState)
		{
		case STATE_START:
			if(ch=='<')
			{
				nextState=STATE_LESS;
				currentOutput.push_back(ch);
			}else if(ch=='>'){
				nextState=STATE_GREATER;
				currentOutput.push_back(ch);
			}else if(ch=='\t'||ch==' '||ch=='\r'||ch=='\n'){
				if(prepP==true && ch=='\n')
				{
					curType=TYPE_S1_END;
					wordP=DONE_ONE_WORD;
					prepP=false;
				}
				nextState=STATE_START;
			}else if(ch=='#'){// # include <c  std io> is fine.,spaces are included
				prepP=true;
				wordP=DONE_ONE_WORD;
				curType=TYPE_S1_START;
				nextState=STATE_S1_START;
			}else if( (ch>='a' && ch<='z') || (ch>='A' && ch<='Z') || ch =='_'){
				currentOutput.push_back(ch);
				nextState=STATE_ID;
			}else if(ch=='0'){
				currentOutput.push_back(ch);
				nextState=STATE_NUMBER;
			}else if(ch>='1' && ch<='9'){
				currentOutput.push_back(ch);
				nextState=STATE_DECILMAL_NUMBER;
			}else if(ch=='='){
				currentOutput.push_back(ch);
				nextState=STATE_EQUAL;
			}else if(ch=='"'){
				nextState=STATE_STRING;
			}else if(ch=='\\'){
				nextState=STATE_TRANSFER;
			}else if(ch=='\''){
				nextState=STATE_CHAR;
			}else if(ch==';'){
				wordP=DONE_ONE_WORD;
				curType=TYPE_SPLIT;
				nextState=STATE_START;
			}else if(ch=='/'){
				nextState=STATE_DIVID;
			}else if(ch=='('){
				wordP=DONE_ONE_WORD;
				curType=TYPE_LBRACE;
			}else if(ch==')'){
				wordP=DONE_ONE_WORD;
				curType=TYPE_RBRACE;
			}else if(ch=='{'){
				wordP=DONE_ONE_WORD;
				curType=TYPE_LHBRACE;
			}else if(ch=='}'){
				wordP=DONE_ONE_WORD;
				curType=TYPE_RHBRACE;
			}else if(ch=='*'){
				currentOutput.push_back(ch);
				nextState=STATE_STAR;
			}
			break;
		case STATE_ID:
			if(
					(ch>='a' && ch<='z') ||
					(ch >='A' && ch<='Z') ||
					(ch =='_') ||
					(ch >='0' && ch<='9')
					){
				currentOutput.push_back(ch);//state not changed
			}else{
				i--;
				nextState=STATE_START;
				wordP=DONE_ONE_WORD;
				curType=TYPE_ID;
			}
			break;
		case STATE_LESS:
			if(ch=='<' || ch=='=')
			{
				curType=TYPE_OP;
				currentOutput.push_back(ch);
				nextState=STATE_START;
			}else{
				i--;
				curType=TYPE_LESS;
				nextState=STATE_START;
				wordP=DONE_ONE_WORD;
			}
			break;
		case STATE_GREATER:
			if(ch=='>'||ch=='=')
			{
				curType=TYPE_OP;
				currentOutput.push_back(ch);
			}else{
				i--;
				curType=TYPE_GREATER;
				wordP=DONE_ONE_WORD;
				nextState=STATE_START;
			}
			break;
		case STATE_S1_START:
			if( (ch>='a' && ch<='z')||
				(ch>='A' && ch<='Z')||ch=='_')
			{
				currentOutput.push_back(ch);
				nextState=STATE_S1_DIRECTIVE;
			}
			else if(ch==' '||ch=='\t'){
				//continue
			}else if(ch=='\n')
			{
				i--;
				wordP=DONE_ONE_WORD;
				curType=TYPE_NONE;
				nextState=STATE_S1_END;
			}else if(ch>='0' && ch<='9'){
				i--;
				wordP=DONE_ONE_WORD;
				curType=TYPE_NONE;
				nextState=STATE_NUMBER;
			}
			break;
		case STATE_S1_DIRECTIVE:
			if( (ch>='a' && ch<='z')||
					(ch>='A' && ch<='Z') || ch=='_'||
					(ch>='0' && ch<='9')
			)
			{
				currentOutput.push_back(ch);
			}
			else{
				i--;
				wordP=DONE_ONE_WORD;
				if(currentOutput.compare("define")==0)
				{
					curType=TYPE_S1_DEFINE;
					nextState=STATE_S1_DEFINE_NAME;
				}else if(currentOutput.compare("include")==0)
				{
					curType=TYPE_S1_INCLUDE;
					nextState=STATE_S1_INCLUDE;
				}
//				else if(currentOutput.compare("if")==0)
				else{
					curType=TYPE_S1_DIRECTIVE;
					nextState=STATE_S1_DIRECTIVE_BODY;
				}

				if(ch=='\n')
				{
						nextState=STATE_S1_END;
				}
			}
			break;
		case STATE_S1_INCLUDE:
			i--;
			nextState=STATE_S1_END;
			break;
		case STATE_S1_DEFINE_NAME:
			i--;
			nextState=STATE_S1_END;
			break;
		case STATE_EQUAL:
			if(ch=='='||ch=='>')
			{
				curType=TYPE_OP;
				currentOutput.push_back(ch);
			}else{
				i--;
				wordP=DONE_ONE_WORD;
				curType=TYPE_OP;
				nextState=STATE_START;
			}
			break;
		case STATE_S1_END:
			if(ch=='\n')
			{
				curType=TYPE_S1_END;
				wordP=DONE_ONE_WORD;
			}else{
				i--;
			}
			nextState=STATE_START;
			break;
		case STATE_NUMBER:
//			printf("in number ch='%c',out=\"%s\"\n",ch,currentOutput.c_str());
			if(ch=='x' || ch=='X'){
				nextState=STATE_HEX_NUMBER;
			}else if(ch=='b' || ch=='B'){
				nextState=STATE_BIN_NUMBER;
			}else if(ch>='0' && ch<='9'){
				currentOutput.push_back(ch);
				nextState=STATE_DECILMAL_NUMBER;
			}else{
				i--;
				nextState=STATE_START;
				curType=TYPE_NUMBER_DECIMAL;
				wordP=DONE_ONE_WORD;
			}
			break;
		case STATE_DECILMAL_NUMBER:
			if(ch>='0' && ch<='9'){
				currentOutput.push_back(ch);
				nextState=STATE_DECILMAL_NUMBER;
			}else{
				i--;
				wordP=DONE_ONE_WORD;
				curType=TYPE_NUMBER_DECIMAL;
				subType=SUBTYPE_DECILMAL;
				nextState=STATE_START;
			}
			break;
		case STATE_HEX_NUMBER:
			if( (ch>='0' && ch<='9') || (ch>='a' && ch<='z')||(ch>='A'&&ch<='Z'))
			{
				currentOutput.push_back(ch);
			}else{
				i--;
				nextState=STATE_START;
				curType=TYPE_NUMBER_HEX;
				wordP=DONE_ONE_WORD;
			}
			break;
		case STATE_BIN_NUMBER:
			if(ch=='0' || ch=='1'){
				currentOutput.push_back(ch);
			}else if(ch=='_'){
					//do nothing,as an extension, we support number like 0b0011_0100
			}else{
				i--;
				wordP=DONE_ONE_WORD;
				curType=TYPE_NUMBER_BIN;
				subType=SUBTYPE_BIN;
				nextState=STATE_START;
			}
			break;
		case STATE_STRING:
//			printf("in string,current is '%c'\n",ch);
			if(ch=='"'){
				wordP=DONE_ONE_WORD;
				curType=TYPE_STRING;
				nextState=STATE_START;

			}
			else if(ch=='\\')
			{
//				printf("go here '\\'\n");
				lastState=curState;
				nextState=STATE_TRANSFER;
			}
			else
			{
				currentOutput.push_back(ch);
			}

			break;
		case STATE_TRANSFER_CARRIGE:
			if(ch=='\n')
			{
				nextState=lastState;
			}else{
				currentOutput.push_back(ch);
				nextState=lastState;
			}
			break;
		case STATE_TRANSFER:
//			printf("in transfer,ch is '%d'\n",ch);
			if(ch=='\r')
			{
//				printf("going STATE_TRANSFER for \\n\n");
				nextState=STATE_TRANSFER_CARRIGE;
			}
			else if(ch=='\n')
			{
				nextState=lastState;
				lastState=STATE_TRANSFER;
			}
			else	if(lastState==STATE_STRING || lastState==STATE_CHAR)
			{
				if(ch=='\\' || '\'' || '"')
				{
					currentOutput.push_back(ch);
				}else if(ch=='n'){
					currentOutput.push_back('\n');
				}else if(ch=='b'){
					currentOutput.push_back('\b');
				}else if(ch=='t'){
					currentOutput.push_back('\t');
				}else if(ch=='r'){
					currentOutput.push_back('\r');
				}
				nextState=lastState;
				lastState=STATE_TRANSFER;
			}else{ //here is an error
				wordP=DONE_ONE_WORD;
				curType=TYPE_TRANSFER;
				nextState=STATE_START;
				lastState=STATE_TRANSFER;
			}
			break;
		case STATE_CHAR:
			if(ch=='\\')
			{
				lastState=curState;
				nextState=STATE_TRANSFER;
			}else if(ch=='\''){
				wordP=DONE_ONE_WORD;
				curType=TYPE_CHAR;
				nextState=STATE_START;
			}
			else
			{
				currentOutput.push_back(ch);
			}
			break;
		case STATE_DIVID:
			if(ch=='/'){
				nextState=STATE_LINE_NOTE;
			}else if(ch=='*'){
				nextState=STATE_MULTILINE_NOTE;
			}
			break;
		case STATE_LINE_NOTE:
			if(ch=='\n'){
				wordP=DONE_ONE_WORD;
				curType=TYPE_NOTE;
				nextState==STATE_START;//done
			}else{
				currentOutput.push_back(ch);
			}
			break;
		case STATE_MULTILINE_NOTE:
			if(ch=='*'){
				currentOutput.push_back(ch);
				nextState=STATE_MULTILINE_GOING_END;
			}else{
				currentOutput.push_back(ch);
			}
			break;
		case STATE_MULTILINE_GOING_END:
			if(ch=='/'){
				currentOutput.erase(currentOutput.size() - 1);
				wordP=DONE_ONE_WORD;
				curType=TYPE_NOTE;
				nextState=STATE_START;
			}else if(ch=='*'){
				currentOutput.push_back(ch);
			}else{
				currentOutput.push_back(ch);
				nextState=STATE_MULTILINE_NOTE;
			}
			break;
		case STATE_STAR:
			if(ch=='=')//composed operator
			{
				curType=TYPE_OP;
				subType=SUBTYPE_COMP_MULTIPLE;
				currentOutput.push_back(ch);
				wordP=DONE_ONE_WORD;
				nextState=STATE_START;
			}else{
				nextState=STATE_START;
			}
			break;
		default:
			//nothing
			break;
		}
		if(wordP==DONE_ONE_WORD)
		{
			printf("(%s,%s)\n",currentOutput.c_str(),humanInfo[curType].c_str());
			output.push_back(pair<string,int>(currentOutput,curType));
			currentOutput.clear();
			curType=TYPE_NONE;
			wordP=DOING_ONE_WROD;
		}
		curState=nextState;
	}
	printf("As a result,we get stream :\n<start>\n");
	vector<pair<string,int> >::iterator it=output.begin(),itend=output.end();
	while(it!=itend)
	{
		printf("%s ",humanInfo[it->second].c_str());
		it++;
	}
	printf("\n<end>\n");
	printf("is that proper for you to do syntax analyze?\n");

}

void LexicalParser::error(const char* msg) const
{
	printf(" [ ERROR ]: %s\n",msg);
}

//===========class : PrinterDebugger

PrintDebugger::PrintDebugger():
		doOut(true)
{
}

PrintDebugger::~PrintDebugger() {
}
void PrintDebugger::setDoOutput(bool doOut)
{
	this->doOut=doOut;
}

void PrintDebugger::info(const char* str) const {
}
