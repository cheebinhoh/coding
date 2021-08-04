/* Copyright © 2021 Chee Bin HOH. All rights reserved.
 *
 * Given two strings A and B, the task is to convert A to B if possible.
 * The only operation allowed is to put any character from A and insert it at front.
 * Find if it’s possible to convert the string. If yes, then output minimum no. of
 * operations required for transformation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int   debug = 0;
char *programName = NULL;


int distanceFromPivot(char string[], char pivot)
{
    int i = 0;


    while ( string[i] != '\0'
            && ( string[i] != pivot
                 || ( string[i + 1] != '\0'
                      && string[i + 1] == pivot )
               ) 
          )
    {
        i++;
    }

    return i;
}

int isTransformable(char source[], char target[])
{
    int i = 0;
    int j = 0;
    int sH[256] = { 0 };
    int tH[256] = { 0 };


    while ( source[i] != '\0' )
    {
        sH[source[i]]++;

        i++;
    }

    while ( target[j] != '\0' )
    {
        tH[target[j]]++;

        j++;
    }

    if ( j != i )
    {
        return 0;
    }

    for ( i = 0; i < ( sizeof( sH ) / sizeof( sH[0] ) ); i++ )
    {
        if ( sH[i] != tH[i] )
        {
            return 0;
        }
    }

    return 1;
}

int transform(char source[], char target[])
{
    int i;
    int move  = 0;
    int count = 0;


    if ( ! isTransformable(source, target) )
    {
        return -1;
    }

    i = 0;
    while ( source[i] != '\0' 
            && count < 5000  // to prevent infinite loop
          )
    {
        if ( source[i] == target[i] )
        {
            i++;
        }
        else
        {
           int j     = i;
           int pivot = 0;
           int pos;
           int tmp;

           // we want to move the furthest source[j] that its counterpart on target is
           // on left side of the source[j].
           while ( source[j] != '\0' )
           {
               pos = distanceFromPivot(target + i, source[j]) + i;
               if ( pos < j)
               {
                   pivot = j;
               }

               j++;
           }

           if ( debug )
           {
               printf("---- Move %d, pick %c and insert at the front, %s => ", move + 1, source[pivot], source);
           }

           j   = pivot;
           tmp = source[j];
           while ( j > 0 )
           {
               source[j] = source[j - 1];
               j--;
           }

           source[0] = tmp;
           i = 0;

           if ( debug )
           {
               printf("%s\n", source);
           }


           move++;
        } // if ( source[i] == target[i] ) ... else
 
        count++;
    } //  while ( source[i] != '\0' )

    return move;
}

void printHelp(void)
{
     fprintf(stderr, "%s [-d] [-h]\n", programName);
     fprintf(stderr, "\n");
     fprintf(stderr, " -d : debug mode to demonstate the move\n");
     fprintf(stderr, " -h : print this help message\n");
}


int main(int argc, char *argv[])
{
    char str1[]    = "EACBD";
    char str2[]    = "CABED";
    char str3[]    = "EACBD";
    char str4[]    = "ABECD";
    char str5[]    = "ACEBD";
    char str6[]    = "ABCDE";
    char str7[]    = "DEABC";
    char target[]  = "EABCD";
    char str8[]    = "OHH";
    char str9[]    = "HHO";
    char target2[] = "HOH";
    char str10[]   = "HCEE";
    char str11[]   = "HEEC";
    char str12[]   = "ECEH";
    char target3[] = "CHEE";
    int  moveCnt = 0;
    int  c;


    programName = argv[0];

    while ( ( c = getopt(argc, argv, "dh") ) != -1 )
    {
        switch ( c )
        {
            case 'd':
                debug = 1;
                break;

            case '?':
            case 'h':
                printHelp();
                exit(1);
        }
    }

    printf("target string = %s\n\n", target);

    // we do not do printf(...., str1, transform(str1, target), str1 );
    // because comma separator on function arguments are not sequencing point, there
    // is no guarantee that 1st str1 is passed to printf before it is transformed by nested method transform

    printf("Transofmr string1 = %s to %s\n", str1, target);
    moveCnt = transform(str1, target);
    printf("after %d move, string1 = %s\n\n", moveCnt, str1);

    printf("Transofmr string2 = %s to %s\n", str2, target);
    moveCnt = transform(str2, target);
    printf("after %d move, string2 = %s\n\n", moveCnt, str2);

    printf("Transofmr string3 = %s to %s\n", str3, target);
    moveCnt = transform(str3, target);
    printf("after %d move, string3 = %s\n\n", moveCnt, str3);

    printf("Transofmr string4 = %s to %s\n", str4, target);
    moveCnt = transform(str4, target);
    printf("after %d move, string4 = %s\n\n", moveCnt, str4);

    printf("Transofmr string5 = %s to %s\n", str5, target);
    moveCnt = transform(str5, target);
    printf("after %d move, string5 = %s\n\n", moveCnt, str5);

    printf("Transofmr string6 = %s to %s\n", str6, target);
    moveCnt = transform(str6, target);
    printf("after %d move, string6 = %s\n\n", moveCnt, str6);

    printf("Transofmr string7 = %s to %s\n", str7, target);
    moveCnt = transform(str7, target);
    printf("after %d move, string7 = %s\n\n", moveCnt, str7);

    printf("target string = %s\n\n", target2);

    printf("Transofmr string8 = %s to %s\n", str8, target2);
    moveCnt = transform(str8, target2);
    printf("after %d move, string8 = %s\n\n", moveCnt, str8);

    printf("Transofmr string9 = %s to %s\n", str9, target2);
    moveCnt = transform(str9, target2);
    printf("after %d move, string9 = %s\n\n", moveCnt, str9);

    printf("target string = %s\n\n", target3);

    printf("Transofmr string10 = %s to %s\n", str10, target3);
    moveCnt = transform(str10, target3);
    printf("after %d move, string10 = %s\n\n", moveCnt, str10);

    printf("Transofmr string11 = %s to %s\n", str11, target3);
    moveCnt = transform(str11, target3);
    printf("after %d move, string11 = %s\n\n", moveCnt, str11);

    printf("Transofmr string12 = %s to %s\n", str12, target3);
    moveCnt = transform(str12, target3);
    printf("after %d move, string12 = %s\n\n", moveCnt, str12);

    return 0;
}
