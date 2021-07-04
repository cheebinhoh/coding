/* Copyright © 2021 Chee Bin HOH. All rights reserved.
 *
 * A simple calculator
 */

#include <stdio.h>
#include <ctype.h>


char * getNumber(char s[], int *retValue)
{
    char *p = s;
    int  value = 0;

    while ( isdigit(*p) )
    {
        value = value * 10 + ( *p - '0' );
        p++;
    }    

    *retValue = value;

    return p;
}

char * getOperator(char s[], char *op)
{
    char *p = s;

    if ( '+' == *p 
         || '-' == *p
         || '*' == *p
         || '/' == *p )
    {
        *op = *p;
        p++;
    }

    return p;
}

char * skipWhitespace(char s[])
{
    char *p = s;

    while ( isspace(*p) )
        p++;
       
    return p;
}

int getPrecedence(char op)
{
    int pos = 0;

    switch ( op ) 
    {
        case '+' :
        case '-' :
            pos = 2;
            break;

        case '*' :
            pos = 1;
            break;

        default:
            break;
    }

    return op;
}

int performBinaryOperation(int opr1, int opr2, char op)
{
    int value;

    switch ( op )
    {
        case '*':
            value = opr1 * opr2;
            break;

        case '/':
           value = opr1 / opr2;
           break;

        case '+':
           value = opr1 + opr2;
           break;

        case '-':
           value = opr1 - opr2;
           break;
    }

    return value;
}

int main(int argc, char *argv[])
{
    char  s[] = "(2 * ( 4 + 3 + 2 ) ) * 2";
    char *p = s;
    char *tmpP;
    int   numberIndex = 0;
    int   opIndex = 0;   
    int   precedenceIndex = 0;
    int   number[100];
    char  op[100];
    int   precedence[100];
    int   total = 0;
    int   newNumber;
    char  newOp;

    while ( *p != '\0' )
    {
        p = skipWhitespace(p);
    
        tmpP = p;
        if ( '(' == *p )
        {
            precedence[precedenceIndex++] = numberIndex;
            p++;
        }
        else if ( ')' == *p )
        {
            int startNumberIndex = precedence[--precedenceIndex]; 

            if ( opIndex > 0 )
            {
                if ( ( numberIndex - startNumberIndex + 1 ) >= 2 )
                {
                    int prevOp;
                    int result;
                    int opr1;
                    int opr2;
 
                    prevOp = op[--opIndex];
                    switch ( prevOp )
                    {
                        case '+' :
                        case '-' :
                            opr1 = number[--numberIndex];
                            opr2 = number[--numberIndex];
                            result = performBinaryOperation(opr1, opr2, prevOp);
                            break;

                        case '*' :
                        case '/' :
                            opr1 = number[--numberIndex];
                            opr2 = number[--numberIndex];
                            result = performBinaryOperation(opr1, opr2, prevOp);
                            break;
                    }

                    number[numberIndex++] = result;
                }   
            }

            p++;
        }
        else if ( ( p = getNumber(p, &newNumber) ) != tmpP )
        {
            number[numberIndex++] = newNumber;
        } 
        else if ( ( p = getOperator(p, &newOp) ) != tmpP )
        {
            if ( opIndex > 0 )
            {
                int prevOp = op[opIndex - 1];
                int prevOpPos = getPrecedence(prevOp);
                int newOpPos = getPrecedence(newOp);
                int opr1;
                int opr2;
                int result;

                if ( prevOpPos <= newOpPos
                     && ( precedenceIndex <= 0
                          || ( numberIndex - precedence[precedenceIndex - 1] ) >= 2 ) )
                {
                    opIndex--;

                    switch ( prevOp )
                    {
                        case '+' :
                        case '-' :
                            opr1 = number[--numberIndex];            
                            opr2 = number[--numberIndex];
                            result = performBinaryOperation(opr1, opr2, prevOp);
                            break;

                        case '*' :
                        case '/' :
                            opr1 = number[--numberIndex];
                            opr2 = number[--numberIndex];        
                            result = performBinaryOperation(opr1, opr2, prevOp);
                            break;    
                    }

                    number[numberIndex++] = result;
                }       
            }

            op[opIndex++] = newOp;
        }
    }

    if ( opIndex > 0 )
    {
        int prevOp;
        int result;
        int opr1;
        int opr2;

        prevOp = op[--opIndex];
        switch ( prevOp )
        {
            case '+' :
            case '-' :
                opr1 = number[--numberIndex];
                opr2 = number[--numberIndex];
                result = performBinaryOperation(opr1, opr2, prevOp);
                break;

           case '*' :
           case '/' :
                opr1 = number[--numberIndex];
                opr2 = number[--numberIndex];
                result = performBinaryOperation(opr1, opr2, prevOp);
                break;
        }

        number[numberIndex++] = result;
    }


    if ( numberIndex > 0 )
    {
         printf("calculation of %s = %d\n", s, number[--numberIndex]);
    }

    return 0;
}
