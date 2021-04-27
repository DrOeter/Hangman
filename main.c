#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

size_t find_c(char *in, ssize_t size, char c){
    size_t count = 0;
    int i = 0;

    for(;count < size; count++ ){
        if(in[count] == c){ 
            i = 1;
            break;
        }
    }

    if(i == 0) return 0;
    else return count + 1;
}

int main(){
    //zweispieler
    //gegen zeit
    //spielername
    size_t pl_count, time_limit, lines = 0, len = 0;
    char player1[20], player2[20], time_mode, tip;
    FILE *fp;
    char *line = NULL;
    ssize_t read;

    fp = fopen(".\\words.txt", "r");
    if (fp == NULL) exit(EXIT_FAILURE);

    srand(time(0));

    printf("Hangman Game:\n\nEin oder Zwei Spieler?\n> ");

    scanf("%d", &pl_count);

    if(pl_count == 1){
        printf("Gebe Spielernamen ein\n> ");
        scanf("%s", &player1);
    }
    else if(pl_count == 2){
        printf("Gebe Name für Spieler 1 ein\n> ");
        scanf("%s", &player1);
        printf("Gebe Name für Spieler 2 ein\n> ");
        scanf("%s", &player2);
    }
    else { 
        printf("Ungültige Spieleranzahl"); 
        exit(0);
    }

    printf("Gegen die Zeit? (j/n)\n> ");
    scanf(" %c", &time_mode);

    if( time_mode == 'j' ){
        printf("Gebe die Länge in Minuten ein\n> ");
        scanf("%d", &time_limit);
    }

    //printf("\033[2J");
    //printf("\033[%d;%dH", 0, 0);

    printf("anz: %d\npl1: %s\npl2 %s\nmode: %c\nlimit: %d\n", pl_count, player1, player2, time_mode, time_limit);



    while(!feof(fp)){
        char ch = fgetc(fp);
        if(ch == '\n'){
            lines++;
        }
    }
    fseek( fp, 0, SEEK_SET );

    int num = (rand() % (lines - 0 + 1)) + 0;
    lines = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        printf("%s\n", line);
        if(lines == num) break;
        lines++;
    }

    fclose(fp);

    line[read - 1]= '\0';

    printf("\n\nrandom: %s num: %d\n", line, num);

    printf("Mache einen Tipp\n> ");
    scanf(" %c", &tip);

    printf("\n\n%d", find_c(line, read, tip));







    return 0;
}