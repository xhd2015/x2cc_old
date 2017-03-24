/*
 * LexicalParser.h
 *
 *  Created on: 2017?3?22?
 *      Author: 13774
 */

#ifndef LEXICALPARSER_H_
#define LEXICALPARSER_H_

#include <cstddef>

#define AS_MACRO inline __attribute__((always_inline))
#define ARRSIZE(arr)	(sizeof(arr)/sizeof(arr[0]))
#define CODE64
#define BY_FULTON

class LexicalParser {
public:
	typedef LexicalParser This;
public:
	enum{
		TYPE,
		IF,
		WHILE,
		LEFT_BRACE,
	};
	enum{
		DIGIT_BIN=0,
		DIGIT_DECIMAL,
		DIGIT_HEX_LOWER,
		DIGIT_HEX_UPPER,
		DIGIT_UNKNOWN
	};
	LexicalParser();
	~LexicalParser();

	static bool isDigitType(char ch,int type);
	static int tellDigitType(char ch);
	static bool isInSet(char ch,const char* buffer,size_t len);
	static char	findValue(char key,const char* buffer,size_t len);

	/**
	 * parsers
	 */
	void parseNumber(const char *buffer,size_t len);
	void parseString(const char *buffer,size_t &index,size_t len);

	void parseWords(const char *buffer,size_t &index,size_t len);

	void error(const char *msg)const;
};

class PrintDebugger{
public:
	PrintDebugger();
	~PrintDebugger();

	void setDoOutput(bool doOut);
	void info(const char *str)const;
protected:
	bool doOut;
};

//===========function macros


#endif /* LEXICALPARSER_H_ */
