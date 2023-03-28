#include"declaration.h"
#include<iostream>
void exceptionReport(int x);
void boolLex();
void handle();
void getChar()
{
    if ((nextChar = fgetc(in_fp)) != EOF)
    {
        if (storeChar) // store for "for" reuse
        {
            sentenceStore[storeCount++] = nextChar;
            sentenceStore[storeCount] = 0;
        }
        if (isalpha(nextChar))
        {
            charType = ALPHA;

        }
        else if (isdigit(nextChar)||nextChar=='.')
        {
            charType = DIGIT;

        }
        else charType = NO_IDEA;
    }
    else charType = EOF;
}
void addChar()
{
    if (lexLen <= 99)
    {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
    {
        for (int i = 0; i < depth; i++)
            cout << "    ";
        cout << "error£º-1: expression too long" << endl;
        return;
    }
}

// if get blank just read next one
void dealBlank()
{
    while (isspace(nextChar))
    {
        getChar();
    }
}
int judgeSignal(char signal)
{
    switch (signal)
    {
    case '+':
        addChar();
        nextToken = OP_ADD;
        break;
    case '-':
        addChar();
        nextToken = OP_SUB;
        break;
    case '*':
        addChar();
        nextToken = OP_MULT;
        break;
    case '/':
        addChar();
        nextToken = OP_DIV;
        break;
    case '^':
        addChar();
        nextToken = OP_XOR;
        break;
    case '(':
        addChar();
        nextToken = LEFT_BRACE;
        break;
    case ')':
        addChar();
        nextToken = RIGHT_BRACE;
        break;
    case'{':
        addChar();
        nextToken = LEFT_BRACKET;
        break;
    case'}':
        addChar();
        nextToken = RIGHT_BRACKET;
        break;
    case'#':
        addChar();
        nextToken = LEFT_SHIFT;
        break;
    case'$':
        addChar();
        nextToken = RIGHT_SHIFT;
        break;
    case'>':
        addChar();
        nextToken = OP_BIGGER;
        break;
    case'<':
        addChar();
        nextToken = OP_LESS;
        break;
    case'!':
        addChar();
        nextToken = OP_NO;
        break;
    case'|':
        addChar();
        nextToken = OP_OR;
        break;
    case'&':
        addChar();
        nextToken = OP_AND;
        break;
    case ';':
        addChar();
        nextToken = SEM_END;
        break;
    case '=':
        addChar();
        nextToken = OP_EQU;
        break;
    case'\'':
        addChar();
        nextToken = OP_APO;
        break;
    case '?':
        addChar();
        nextToken = OP_BIT_AND;
        break;
    case '@':
        addChar();
        nextToken = OP_BIT_OR;
        break;
    default:
        addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}

void noteDeal()
{
    if (nextToken == OP_DIV)
    {
        char _tmp = getc(in_fp);
        if (_tmp == '/')
        {
            ignoreFlag = true;
            while (1)
            {
                char x = getc(in_fp);
                if (x == '\n')break;
            }
            getChar();
            return;
        }
        else if (_tmp == '*')
        {
            ignoreFlag = true;
            while (1) {
                if (getc(in_fp) == '*') {
                    char x = getc(in_fp);
                    if (x == '/')break;
                    else ungetc(x, in_fp);
                }

            }
            getChar();
            return;
        }
        else ungetc(_tmp, in_fp);
    }
}

int lex(int num=1)  // main module to read, clarify and deal with different types of token   parameter: num as lex times(read in how many exprs)
{
    for (int i = 0; i < num; i++)
    {
        lexLen = 0;
        dealBlank();
        switch (charType)
        {
        case ALPHA:   // if get a letter in the front, read until the end of name 
            addChar();
            getChar();
            while (charType == ALPHA || charType == DIGIT)  // letter object can contain number in its name
            {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;
        case DIGIT:   // indent int number
            addChar();
            getChar();
            while (charType == DIGIT)  // int number should not contain letters 
            {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
        case NO_IDEA: // get signals to use
            judgeSignal(nextChar);  // get nextToken

            // note to deal with
            noteDeal();

            getChar();
            break;
        case EOF:  // no chars left; end of sentence
            nextToken = EOF;
            strcpy_s(lexeme, "eof");
            
        }
    }
    // TODO only if notes then not jump here
    for (int i = 0; i < depth; i++)
        cout << "    ";
    if(ignoreFlag==false)
        cout << "next token is " << nextToken << " and next lexeme is " << lexeme << endl;
    ignoreFlag = false;
    return nextToken;
}

// smallest block: value
void factor()
{
    string name;
    depth++;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    cout << "enter <factor>" << endl;
    switch (nextToken)
    {
        case INT_LIT:   // num
        {
            currentOutput = atof(lexeme); // direct return num
            lex();
            break;
        }
        case IDENT:     // variable
        {// check declared variable
            bool flag_declared = false;
            name = lexeme;
            for (int i = 0; i < numOfVariables; i++)
                if (variables[i]->name == name)
                {
                    flag_declared = true;
                    break;
                }
            if (!flag_declared) // not registered
                exceptionReport(5);
            // check variable has legal value
            for (int i = 0; i < numOfVariables; i++)
                if (variables[i]->name == name) // find stored variable
                {
                    if (variables[i]->info.num == NOT_DEF)   // currently the variable do not have value
                        exceptionReport(17);
                    currentOutput = variables[i]->info.num;
                    break;
                }
            lex();
            break;
        }
        case LEFT_BRACE: // paren: (xxx) should have the highest privilege, so deal with that together with int_lit & ident
        {
            lex(); // clean ( out
            boolLex(); // base expr, because between ( and ) there can be a whole expr
            if (lexeme[0] != ')')  // check whether () can match(boolLex dont clean ) out)
                exceptionReport(6);
            else
                lex(); // clean ) out
            break;
        }
        default:
            exceptionReport(100);    

    }
    for (int i = 0; i < depth; i++)
        cout << "    ";
    depth--;
    cout << "exit <factor>" << endl;
}

void bitwise() // ^,|,&,>>,<<  bitwise operation -> superior than *,/ 
{
    depth++;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    cout << "enter <bitwise>" << endl;
    factor();
    int tmp_wise = currentOutput;
    while (nextToken == OP_BIT_AND || nextToken == OP_BIT_OR || nextToken == OP_XOR || nextToken == LEFT_SHIFT ||nextToken==RIGHT_SHIFT) // several steps
    {
        switch (nextToken)
        {
        case OP_BIT_AND:
            lex(); // &
            factor();
            tmp_wise &= (int)currentOutput;
            break;
        case OP_BIT_OR: // |
            lex();
            factor();
            tmp_wise |= (int)currentOutput;
            break;
        case OP_XOR: // ^
            lex();
            factor();
            tmp_wise ^= (int)currentOutput;
            break;
        case LEFT_SHIFT:
            if (nextChar == '#')
            {
                lex(2); // clean >>
                factor(); // y return 
                tmp_wise = tmp_wise >> (int)currentOutput; // x>>y
            }
            else 
                exceptionReport(19);
            break;
        case RIGHT_SHIFT:
            if (nextChar == '$')
            {
                lex(2); // clean <<
                factor(); // y return 
                tmp_wise = tmp_wise << (int)currentOutput; // x<<y
            }
            else
                exceptionReport(19);
            break;
        }
    }
    currentOutput = tmp_wise;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    depth--;
    cout << "exit <bitwise>" << endl;
}

void term() // */ |expr, may have several steps
{
    depth++;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    cout << "enter <term>" << endl;
    bitwise();
    int tmp_term = currentOutput;
    while (nextToken == OP_MULT || nextToken == OP_DIV) // several steps
    {
        if (nextToken == OP_MULT)
        {
            lex();
            bitwise();
            tmp_term *= currentOutput;
        }
        else
        {
            lex();
            bitwise();
            if (currentOutput == 0) // notice 0 cannot be divided, illegal
            {
                exceptionReport(8);
                tmp_term = 0;   // set current result to 0, in case latter be the same 0, deal here: set to 0 and continue
            }
            else
                tmp_term /= currentOutput;
        }
    }
    currentOutput = tmp_term;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    depth--;
    cout << "exit <term>" << endl;
}

void expr() // +- | term£¬ may have several steps
{
    depth++;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    cout << "enter <expr>" << endl;
    term();
    int  tmp_expr = currentOutput;
    while (nextToken == OP_ADD || nextToken == OP_SUB) // several steps
    {
        if (nextToken == OP_ADD)
        {
            lex(); // only clean one signal +/-
            term();
            tmp_expr += currentOutput;
        }
        else
        {
            lex();
            term();
            tmp_expr -= currentOutput;
        }
    }
    currentOutput = tmp_expr;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    depth--;
    cout << "exit <expr>" << endl;
}

void boolExpr() // x>= <= > < == != y expression, only one step(so not while)
{
    depth++;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    cout << "enter <boolExpr>" << endl;
    expr(); // get x
    int tmp_boolExpr = currentOutput;
    if (nextToken == OP_BIGGER || nextToken == OP_LESS)
    {
        int token = nextToken;
        if (nextChar == '=') // >= <=
        {
            lex(2); // clear >= signal 
            expr(); // get y
            if (token == OP_BIGGER)
                currentOutput = tmp_boolExpr >= currentOutput;
            else
                currentOutput = tmp_boolExpr <= currentOutput;
        }
        else // > <
        {
            lex(); // clear > signal
            expr();
            if (token == OP_BIGGER)
                currentOutput = tmp_boolExpr > currentOutput;
            else 
                currentOutput = tmp_boolExpr < currentOutput;
        }
    }
    else if (nextToken == OP_NO) // !=
    {
        if (nextChar == '=')
        {
            lex(2); // clear != signal
            expr();
            currentOutput = tmp_boolExpr != currentOutput;
        }
        else // single !: illegal
        {
            exceptionReport(16);
        }
    }
    else if(nextToken==OP_EQU)// ==
    {
        if (nextChar == '=')
        {
            lex(2); // clean == signal
            expr();
            currentOutput = tmp_boolExpr == currentOutput;
        }
        else
        {
            exceptionReport(15);
        }
    }
    for (int i = 0; i < depth; i++)
        cout << "    ";
    depth--;
    cout << "exit <boolExpr>" << endl;
}

void boolLex() // base operator: x&&y or x||y, x y for a>=,<=,>,<,==,!= b(as boolExpr)  can continue: a&&b&&c 
{
    depth++;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    cout << "enter <boolLex>" << endl;
    boolExpr(); // x return
    while (nextToken == OP_AND || nextToken == OP_OR)
    {
        double tmp_boolLex = currentOutput;  // result to last step
        if (nextToken == OP_AND)
        {
            if (nextChar == '&')
            {
                lex(2); // clean &&: two signals
                boolExpr(); // y return 
                currentOutput = tmp_boolLex && currentOutput; // x&&y
            }
            else // single & signal: illegal  how to cope with:just ignore this step
                exceptionReport(10);
        }
        else if (nextToken == OP_OR)
        {
            if (nextChar == '|')
            {
                lex(2); // clean || signal
                boolExpr();
                currentOutput = tmp_boolLex || currentOutput;
            }
            else exceptionReport(10);
        }
    }
    for (int i = 0; i < depth; i++)
        cout << "    ";
    depth--;
    cout << "exit <boolLex>" << endl;
}

void giveUpSentence()
{
    do
    {
        int a = lex(); // clean onw signal out
    } while (nextToken != SEM_END && nextToken != EOF);
}
void declare()
{
    depth++;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    cout << "enter <declaration>" << endl;
    bool flag = false;
    int tmp = 0;

    for (int i = 0; i < 4; i++)   // judge whether declared variable type 
    {
        if (strcmp(lexeme, systemkeep[i].c_str()) == 0)
        {
            flag = true;
            break;
        }
    }
    if (!flag)  // not legal variable type( not in systemkeep)
    {
        exceptionReport(2);  // type unclear
        exceptionFlag = 1;
        giveUpSentence();
        return;
    }
        
    // declaration sentence: two types of "int x;" or "int x=y;"
    string type = lexeme;
    tmp=lex(); // read variable name in
    string name = lexeme;
    // check whether name repeats
    for (int i = 0; i < numOfVariables; i++)
    {
        if (variables[i]->name == name)
        {
            exceptionReport(3);
            giveUpSentence();
            return;
        }

    }
    
    tmp=lex(); // read value for initialization: sem_end or equal 
    switch (tmp) 
    {
    case OP_EQU:
        lex();
        boolLex();  // get returning: a base expression with value for init in declaration
    case SEM_END:
        variables[numOfVariables] = new symboltable;
        variables[numOfVariables]->name = name;
        variables[numOfVariables]->type = type;
        variables[numOfVariables]->info.num = currentOutput;
        numOfVariables++;
        for (int i = 0; i < depth; i++)
            cout << "    ";
        cout << "user declared one " << type << " variable named " << name << " and its num is ";
        if (currentOutput == NOT_DEF)
            cout << "undefined." << endl;
        else cout << currentOutput << endl;
        break;
    default:
        exceptionReport(4);
        do
        {
            int tmp1 = lex();
        } while (nextToken != SEM_END && nextToken != EOF); //  give up the sentence
    
    }
    for (int i = 0; i < depth; i++)
        cout << "    ";
    depth--;
    cout << "exit <declaration>" << endl;
    return;

}

// give up sentences between { and }
void giveUpBracket()
{
    int countLeftBracket = 0;
    if (nextChar == '}') // else{} null sentences
    {
        lex(); // get |
    }
    else  // else{...} -> ignore all chars between { and }: } need to be settled still
    {
        while (true)
        {
            char tmp = getc(in_fp);
            if (tmp == '{') // {{...}...}
                countLeftBracket++;
            else if (tmp == '}')
            {
                if (countLeftBracket == 0)  // last } to settle
                {
                    ungetc('}', in_fp);
                    lex(); // settle }
                    break;
                }
                countLeftBracket--;
            }
        }
    }
}
// settle expr between { and }
void settleBracket()
{
    while (true)
    {
        bool endIf = false;
        while (true)
        {
            int _ = lex();
            handle();    // deal one sentence
            if (nextToken == EOF || nextToken == SEM_END || nextToken == RIGHT_BRACKET)
                break;
        }
        switch (nextToken)
        {
        case EOF:    // early terminate
            exceptionReport(14);
        case RIGHT_BRACKET: // get } and terminate
            endIf = true;
            break;
        case SEM_END:   // finish one sentence
            cout << "sentence ended here." << endl;
        default:
            break;
        }
        if (endIf) // } or EOF: finish {...} settle
            break;
    }
}
void ifstmt() // after get "if"
{
    bool ifCheck;
    depth++;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    cout << "enter <if>" << endl;  
    lex(); // get & deal "("
    if (nextToken == LEFT_BRACE) // (
    {
        lex();   
        boolLex();  // get if-sentence: bool in returning
    }
    else
        exceptionReport(12);
    if (nextToken != RIGHT_BRACE)
        exceptionReport(6);
    ifCheck = currentOutput; // 0 to take else choice , 1 to take if choice
    lex();  // get next signal
    if (nextToken == LEFT_BRACKET)  // if (...) {}
    {
        // to settle operation if {}
        if (ifCheck)
            settleBracket();
        else // give up all sentences in if(){...}
            giveUpBracket();
        lex();  // settle ;
    }
    else exceptionReport(11);
    // settle else expression (if exist)

    // store former  lexeme and token for recovery
    string storeLexeme = lexeme;
    int storeNextToken = nextToken;
    lex(); // get a expr NOTICE here may wrongly get another expr earlier
    //case 1: else->directly settle like above
    if (strcmp(lexeme, "else") == 0)
    {
        lex(); // get {
        if (nextToken == LEFT_BRACKET)
        {
            if (ifCheck)  // give up else expr
                giveUpBracket();
            else  // run else choice
                settleBracket();
            lex();
        }
        // else
        // directly get ; instead of {  -> else; is legal expr
    }
    // case 2: no else exist-> so another lexeme has been wrongly gotten earlier-> need be put back
    else
    {
        ungetc(' ', in_fp);
        ungetc(nextChar, in_fp);
        ungetc(' ', in_fp);
        // put back in a reverse turn
        for (int i = strlen(lexeme) - 1; i >= 0; i--)
        {
            ungetc(lexeme[i], in_fp);
        }
        for (int i = 0; i < storeLexeme.length(); i++)
        {
            lexeme[i] = storeLexeme[i];
        }
        lexeme[storeLexeme.length()] = '\0';
        nextToken = storeNextToken;
    }
    for (int i = 0; i < depth; i++)
        cout << "    ";
    depth--;
    cout << "exit <if>" << endl;
}

void whilestmt()  // for(5)
{
    depth++;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    cout << "enter <while>" << endl;
    storeCount = 0;
    storeChar = true;
    int count_iter = 0;
    int count_judge = 0;
    while (true)
    {
        for (int j = count_judge-1; j >= 0; j--)
        {
            ungetc(sentenceStore[j], in_fp);
        }
        if (count_iter == 0)
        {
            storeCount = 1;
            sentenceStore[0] = '(';
        }
        lex();
        //cout << sentenceStore << endl;
        if (nextToken == LEFT_BRACE) // (
        {
            lex();
            boolLex();  // get if-sentence: bool in returning
        }
        else
            exceptionReport(12);
        if (nextToken != RIGHT_BRACE)
            exceptionReport(6);
        if(count_iter==0)
            count_judge = storeCount;
        bool ifCheck = currentOutput;
        if (ifCheck == false)
            break;
        for (int j = storeCount-1; j >= count_judge; j--)
        {
            ungetc(sentenceStore[j], in_fp);
        }
        lex();
        if (nextToken == LEFT_BRACKET)
            settleBracket();
        else
            exceptionReport(20);
        count_iter++;
        storeChar = false;
        // cout << sentenceStore << endl;
    }
    storeChar = false;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    depth--;
    cout << "exit <while>" << endl;
}

void assign()
{
    depth++;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    cout << "enter <assign>" << endl;
    string tmpName = lexeme;
    // find the variable to change value
    bool found = false;
    symboltable* variable = new symboltable();
    for (int i = 0; i < numOfVariables; i++)
    {
        if (variables[i]->name == tmpName)
        {
            variable = variables[i];
            found = true;
            break;
        }
    }
    if (!found) // not found
    {
        exceptionReport(5);
        return;
    }
    lex(); // get =
    if (nextToken != OP_EQU)
    {
        exceptionReport(7);
        return;
    }
    lex();
    boolLex();
    variable->info.num = currentOutput;
    cout << variable->name << " is turned to " << currentOutput << endl;
    for (int i = 0; i < depth; i++)
        cout << "    ";
    depth--;
    cout << "exit <assign>" << endl;
}

void handle()
{
    switch (nextToken)
    {
    case IDENT: // operate on ident object
        // 1¡¢ declare a variable
        bool flag = false;
        for (int i = 0; i < 4; i++)   // judge whether declared variable type 
        {
            if (strcmp(lexeme, systemkeep[i].c_str()) == 0)
            {
                flag = true;
                break;
            }
        }
        if (flag == true)
        {
            declare();
            return;
        }
        // 2¡¢deal with "if"
        if (strcmp(lexeme, "if") == 0)
        {
            ifstmt();
            return;
        }
        // 3¡¢error: begin with else
        if (strcmp(lexeme, "else") == 0)
        {
            exceptionReport(9);
            return;
        }
        // 4¡¢deal with "for"
        if (strcmp(lexeme, "while") == 0)
        {
            whilestmt();
            return;
        }
        // 5¡¢ assignment of a variable
        assign();
        return;

        // other possible situations: report error(?
        
    }
}

void exceptionReport(int exceptionCode)
{
    for (int i = 0; i < depth; i++)
        cout << "    ";
    switch (exceptionCode)
    {
        case 1:
            cout << "error: 1 reserved test case" << endl;
            break;
        case 2:
            cout << "error£º2  type unclear" << endl;
            break;
        case 3:
            cout << "error£º3 name repeat" << endl;
            break;
        case 4:
            cout << "error£º4 ¡®;¡¯or¡®=¡¯not exist" << endl;
            break;
        case 5:
            cout << "error£º5 undeclared variable" << endl;
            break;
        case 6:
            cout << "error£º6 ')' not exist" << endl;
            break;
        case 7:
            cout << "error£º7 '='-like format error" << endl;
            break;
        case 8:
            cout << "error£º8 can not be divided by 0" << endl;
            break;
        case 9:
            cout << "error£º9 'if' not exist to match 'else'" << endl;
            break;
        case 10:
            cout << "error£º10 not support bool bitwise" << endl;
            break;
        case 11:
            cout << "error£º11 if should be successed by '{'" << endl;
            break;
        case 12:
            cout << "error£º12 '('not exist" << endl;
            break;
        case 13:
            cout << "error£º13 unexpected signal" << endl;
            break;
        case 14:
            cout << "error£º14 mismatch:'( ' and ')'" << endl;
            break;
        case 15:
            cout << "error£º15 mismatch: maybe '=='" << endl;
            break;
        case 16:
            cout << "error£º16 mismatch: maybe '!=" << endl;
            break;
        case 17:
            cout << "error£º17 variable not init" << endl;
            break;
        case 18:
            cout << "error£º18 series comparison" << endl;
            break;
        case 19:
            cout << "error 19: not support solid value '>' and '<'" << endl;
            break;
        case 20:
            cout << "error 20: while has not got }" << endl;
            break;
        default:
            cout << "unknown error: 114514, just stop" << endl;
            exit(0);
    }
}
