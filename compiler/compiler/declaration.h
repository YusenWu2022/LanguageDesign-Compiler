#pragma once

#include <iostream>
#include<ctype.h>
#include<fstream>
#include<cstring>
using namespace std;

#define ERRO -1
#define SEN_STAR 0 
#define ASSI_STA 1


// char type
#define ALPHA 0    // a-z
#define DIGIT 1    // num
#define NO_IDEA 2

//signal type
#define INT_LIT 10
#define IDENT 11
#define OP_ASSIGN 20
#define OP_ADD 21
#define OP_SUB 22
#define OP_MULT 23
#define OP_DIV 24
#define LEFT_BRACE 25
#define RIGHT_BRACE 26
#define SEM_END 27
#define OP_EQU 28
#define OP_APO 29
#define OP_BIGGER 30
#define OP_LESS 31
#define OP_NO 32
#define OP_AND 33
#define OP_OR 34
#define OP_XOR 39
#define LEFT_BRACKET 35
#define RIGHT_BRACKET 36
#define RIGHT_SHIFT 37
#define LEFT_SHIFT 38
#define OP_BIT_OR 41
#define OP_BIT_AND 42
#define SPAC 4040
#define NOT_DEF -1

typedef union sav {
    double num;
    char ch;
    float fl;
    double db;
};

class symboltable
{
public:
    string name;
    string type;
    sav info;
    symboltable(string type_, string name_, sav p)
    {
        type = type_;
        name = name_;
        info = p;
    }
    symboltable() {}
};

extern int charType;
extern char lexeme[100];
extern char nextChar;
extern int lexLen;
extern int token;
extern int nextToken;
extern bool ignoreFlag ;
extern int state;
extern int numOfVariables;
extern double currentOutput;
extern int exceptionFlag;
extern class symboltable;
extern int depth;
extern FILE* in_fp, * fopen();
extern string systemkeep[20];
extern symboltable* variables[10000];
extern char sentenceStore[1005];
extern bool storeChar ;
extern int storeCount;

