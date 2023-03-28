#include<iostream>
#include"declaration.h"
#include"funtions.h"
using namespace std;


int charType;
char nextChar;
int lexLen;
int token;
int nextToken;
bool ignoreFlag = false;
int state;
int numOfVariables = 0;
string systemkeep[20] = { "int","float","double","char","if","else" };
char  sentenceStore[1005];
symboltable* variables[10000];
char lexeme[100];
double currentOutput = NOT_DEF;
int exceptionFlag = 0;
int depth = 0;
FILE* in_fp, * fopen();
bool storeChar = false;
int storeCount = 0;

int main()
{
    fopen_s(&in_fp, "in.txt", "r");
    if (in_fp == NULL)
        cout << "error: file cannot open " << endl;
    else {
        getChar();
        do {
            do {
                int _ = lex();

                handle();

            } while (nextToken != SEM_END && nextToken != EOF && nextToken != RIGHT_BRACKET);
            cout << "end of sentence." << endl;
        } while (nextToken != EOF);
    }

}
