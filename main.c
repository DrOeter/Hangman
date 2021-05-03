#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "main.h"

#define MAX 100
#define SS (&stat)->
#define WS (&words[ SS mode ])->

typedef struct{
    char user[MAX][MAX];
    int pos;
}data;

struct letters{
    char alphabet[26];
    char c_letters[26];
    char used_letters[26];
}words[2];

struct stat{
    size_t tries[2] ,c_tries[2], letter[2], mode, stage[2], pl_count, complete[2], correct[2], signal;
    double time_limit;
    char player1[MAX], player2[MAX], word[26];
    FILE *fUserdata;
}stat;

DWORD WINAPI Thread(void* data) {
    clock_t now = clock();

    while(1){
        clock_t later = clock();
        char used_letters[26] = "";
        for(int i=0,ii=0; i < strlen(WS used_letters);i++,ii+=2){
            used_letters[ii] = toupper(WS used_letters[i]);
            used_letters[ii + 1] = ' '; 
        }      
        for(int i=0; i < strlen(WS used_letters);i++){
            if(WS c_letters[i] != '_') WS c_letters[i] = toupper(WS c_letters[i]);
        }

        system("cls");
        if(SS mode == 0)printf("Spieler %d %s\n", SS mode + 1, SS player1);
        if(SS mode == 1)printf("Spieler %d %s\n", SS mode + 1, SS player2);
        printHangman(SS stage[ SS mode ]);
        printf("__________________________\n\n");
        printf("%s\n", WS c_letters);
        printf("__________________________\n\n");
        printf("Benutzte Buchstaben: %s\nVersuche: %d\nKorrekte Versuche: %d\n", used_letters, SS tries[ SS mode ], SS c_tries[ SS mode ]);
        printf("Vergangene Zeit: %ld\n", (later - now));
        if(SS pl_count == 1) printf("Mache einen Tipp\n> ");
        if(SS pl_count == 2 && SS mode == 0) printf("%s mache einen Tipp\n> ", SS player1);
        if(SS pl_count == 2 && SS mode == 1) printf("%s mache einen Tipp\n> ", SS player2);

        if(SS signal == 0 && SS time_limit > 0 && (SS time_limit * 60000) < (later - now)) {
            if(SS pl_count == 1) printf("\n\nZeitlimit ueberschritten\nVerloren :(");
            if(SS pl_count == 2) printf("\n\nZeitlimit ueberschritten\n");
            if(SS pl_count == 1) fprintf(SS fUserdata, "%s;%s;%d;Ueberschritten: %d s\n", SS player1, SS word, SS tries[ 0 ], (later - now) / 1000);
            if(SS pl_count == 2){
               // printf("werte: %d %d %d\n",SS mode, SS complete[0], SS complete[1]);
                fprintf(SS fUserdata, "%s;%s;%d;Spieler 1 Ueberschritten: %d s\n", SS player1, SS word, SS tries[ 0 ], (later - now) / 1000);
                fprintf(SS fUserdata, "%s;%s;%d;Spieler 2 Ueberschritten: %d s\n", SS player2, SS word, SS tries[ 1 ], (later - now) / 1000);
                if(SS correct[1] < SS correct[0]) printf("\n\nSpieler %s gewinnt!!!\n", SS player1);
                if(SS correct[0] < SS correct[1]) printf("\n\nSpieler %s gewinnt!!!\n", SS player2);
                if(SS correct[0] == SS correct[1] ) printf("\n\nUnentschieden\n");
            }
            fclose(SS fUserdata);
            exit(0);
        }        
        if(SS signal == 1) {
            if(SS pl_count == 1) fprintf(SS fUserdata, "%s;%s;%d;%d s\n", SS player1, SS word, SS tries[ 0 ], (later - now) / 1000);
            if(SS pl_count == 2){
               // printf("werte: %d %d %d\n",SS mode, SS complete[0], SS complete[1]);
                if(SS complete[0] == 1 ) fprintf(SS fUserdata, "%s;%s;%d;%d s\n", SS player1, SS word, SS tries[ 0 ], (later - now) / 1000);
                if(SS complete[1] == 1 ) fprintf(SS fUserdata, "%s;%s;%d;%d s\n", SS player2, SS word, SS tries[ 1 ], (later - now) / 1000);
                if(SS complete[0] == 0 ) fprintf(SS fUserdata, "%s;%s;%d;Spieler 1 verloren\n", SS player1, SS word, SS tries[ 0 ]);
                if(SS complete[1] == 0 ) fprintf(SS fUserdata, "%s;%s;%d;Spieler 2 verloren\n", SS player2, SS word, SS tries[ 1 ]);
            }
            break;
        }
        Sleep(500);
    }
    return 0;
}

data *userArray(FILE *fp){
    char test[MAX] = "yee",buff[MAX] = "eey";
    data *out = malloc( sizeof *out);
    out->pos = 0;
    
    while( strcmp(test,buff) ){
        //printf("%s %d ", out->user[pos], strcmp(test,buff));

        for(int i=0;i < MAX;i++)
            test[i] = buff[i];

        fgets(buff,MAX,fp); 

        for(int i=0;i < MAX;i++)
            out->user[out->pos][i] = buff[i];

        out->pos++;
    }
    return out;
}

void nullNewline(char *in){
    for(int i=0;i < MAX;i++){
        if(in[i] == '\n') in[i] = '\0';
    }
}

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

char *rot13(char *str) {
  for (int i = 0; '\0' != str[i]; i++) {
    char c = *(str + i);
    if (('a' <= c && 'n' > c) || ('A' <= c && 'N' > c)) {
      // a-m
      *(str + i) += 13;
    } else if (('n' <= c && 'z' >= c) || ('N' <= c && 'Z' >= c)) {
      // n-z
      *(str + i) -= 13;
    }
  }
  return str;
}

void encryptFile(){
    FILE *fWords = fopen(".\\words", "r+");
    FILE *fPlain = fopen(".\\plainwords", "r");
    ssize_t read;
    size_t len = 0;
    char *line = malloc( MAX );
    if (fWords == NULL || fPlain == NULL ) return;
    fseek(fWords, 0, SEEK_END);

    while ((read = getline(&line, &len, fPlain)) != -1) {
        nullNewline( line );
        char *enc = rot13( line );
        printf("%s\n",line);
        fprintf(fWords,"%s\n", enc);
    }
    fclose(fWords);
    fclose(fPlain);
    free(line);
}

int main(){
    encryptFile();

     printf("penis\n\n");
    char alphabet[26] = "abcdefghijklmnopqrstuvwxyz";

    for(int i=0; i < 2;i++){
        for(int ii=0; ii < 26;ii++){
            (&words[i])->c_letters[ii] = '_';
            (&words[i])->alphabet[ii] = alphabet[ii];
        }
    }
    size_t pl_count, lines = 0, len = 0, equal[2];
    char time_mode, tip, listuser, *dec_line = malloc(MAX);
    char *line = NULL;
    ssize_t read;
    equal[0]=0,equal[1]=0;

   

    FILE *fWords = fopen(".\\words", "r");
    FILE *fUser = fopen(".\\user", "r+");
    SS fUserdata = fopen(".\\userdata.csv", "r+");
    fseek(SS fUserdata, 0, SEEK_END);

    if (fWords == NULL || fUser == NULL || SS fUserdata == NULL) return 0;

    srand(time(NULL));

    while(1){
        printf("Hangman Game:\n\nEin oder Zwei Spieler?\n> ");
        scanf("%d", &pl_count);
        if(pl_count == 1 || pl_count == 2) break;
    }    
    while(1){
        printf("Benutzer anzeigen? (j/n)\n> ");
        scanf(" %c", &listuser);
        if(listuser == 'j' || listuser == 'n') break;
    }
    SS pl_count = pl_count;
    data *dat = userArray(fUser);

    if(pl_count == 1 && listuser == 'j'){
        const char user[20];

        for(int i=0;i < dat->pos - 1;i++){
            nullNewline( dat->user[i] );
            printf("%s\n" , dat->user[i]);
        }

        printf("Benutzer auswaehlen\n> ");
        scanf("%s", &user);

        int equal = 0;
        for(int i=0;i < dat->pos - 1 ;i++){
            if(strcmp(user, dat->user[i]) == 0){
                for(int ii=0;ii < 20;ii++)
                    SS player1[ii] = dat->user[i][ii];
                equal = 1;
            }
        }    
        if(equal == 0){
            printf("Benutzer nicht gefunden!\n");
            listuser = 'n';
        }


    }
    else if(pl_count == 2 && listuser == 'j'){
        char user1[20], user2[20];

        for(int i=0;i < dat->pos - 1;i++){
            nullNewline( dat->user[i] );
            printf("%s\n" , dat->user[i]);
        }

        printf("Benutzer 1 auswaehlen\n> ");
        scanf("%s", &user1);

        printf("Benutzer 2 auswaehlen\n> ");
        scanf("%s", &user2);

        for(int i=0;i < dat->pos - 1 ;i++){
            if(strcmp(user1, dat->user[i]) == 0){
                for(int ii=0;ii < 20;ii++)
                    SS player1[ii] = dat->user[i][ii];
                equal[0] = 1;
            }
        }    
        for(int i=0;i < dat->pos - 1 ;i++){
            if(strcmp(user2, dat->user[i]) == 0){
                for(int ii=0;ii < 20;ii++)
                    SS player2[ii] = dat->user[i][ii];
                equal[1] = 1;
            }
        }   

        if(equal[0] == 0){
            printf("Benutzer 1 nicht gefunden!\n");
            listuser = 'n';
        } 
        else if(equal[1] == 0){
            printf("Benutzer 2 nicht gefunden!\n");
            listuser = 'n';
        }
    }

    fseek(fUser, 0, SEEK_END);

    if(pl_count == 1 && listuser == 'n'){
        printf("Gebe Spielernamen ein\n> ");
        scanf("%s", &SS player1);
        fprintf (fUser, "%s\n", SS player1);
    }
    else if(pl_count == 2 && listuser == 'n'){
        if(equal[0] == 0){
            printf("Gebe Name für Spieler 1 ein\n> ");
            scanf("%s", &SS player1);
        }
        if(equal[1] == 0){
            printf("Gebe Name für Spieler 2 ein\n> ");
            scanf("%s", &SS player2);
        }
        fprintf (fUser, "%s\n", SS player1);
        fprintf (fUser, "%s\n", SS player2);
    }
    
    fclose(fUser);
    while(1){
        printf("Gegen die Zeit? (j/n)\n> ");
        scanf(" %c", &time_mode);
        if(time_mode == 'j' || time_mode == 'n') break;
    }
    if( time_mode == 'j' ){
        while(1){
            printf("Gebe die Laenge in Minuten ein\n> ");
            scanf("%lf", &SS time_limit);
            if(SS time_limit > 0) break;
        }
    }

    //printf("anz: %d\npl1: %s\npl2 %s\nmode: %c\nlimit: %d\n", pl_count, player1, player2, time_mode, time_limit);

    A:
    fseek( fWords, 0, SEEK_SET );
    while(!feof(fWords)){
        char ch = fgetc(fWords);
        if(ch == '\n'){
            lines++;
        }
    }
    fseek( fWords, 0, SEEK_SET );

    int num = (rand() % (lines - 0 + 1)) + 0;
    lines = 0;


    while ((read = getline(&line, &len, fWords)) != -1) {
        for(int i=0; i < read;i++)
            dec_line[i] = line[i];

        dec_line = rot13( dec_line );

        for(int i=0; i < read;i++)
            line[i] = dec_line[i];

        //printf("%s\n", line);
        if(lines == num) break;
        lines++;
    }
    if(feof(fWords)) goto A;

    fclose(fWords);
    free( dec_line );

    line[read - 1]= '\0';

    for(int i=0; i < read;i++)
        SS word[i] = line[i];

    for(int i=0; i < read;i++)
        line[i] = tolower( line[i] );

    for(ssize_t i = read - 1; i < 26;i++){
        (&words[0])->c_letters[i] = '\0'; 
        (&words[1])->c_letters[i] = '\0';  
    }    

    SS mode = 0;
    SS signal = 0;
    SS letter[0] = 0, SS  letter[1] = 0;
    SS stage[0] = 1, SS stage[1] = 1; 
    SS c_tries[0] = 0, SS c_tries[1] = 0;
    SS tries[0] = 0, SS tries[1] = 0;
    SS complete[0] = 0,SS complete[1] = 0;
    SS correct[0] = 0, SS correct[1] = 0;
    size_t correct = 0, complete = 1;

    system("cls");

    HANDLE thread = CreateThread(NULL, 0, Thread, NULL, 0, NULL);
    
    while(1){
        correct = 0, complete = 1;
        scanf(" %c", &tip);

        size_t at = find_c(WS alphabet, 26, tip);
        size_t guess = find_c(line, read, tip);
        
        if( guess > 0 ) SS c_tries[ SS mode ]++;
        SS tries[ SS mode ]++;
        
        if( WS alphabet[ at - 1 ] == '0' || at == 0 ) {
            printf("Buchstabe wurde schon benutzt!\n");
            continue;
        }
        else if( WS alphabet[ at - 1 ] != '0' ) {
            WS used_letters[SS letter[ SS mode ]] = WS alphabet[ at - 1 ];
            for(int i=0; i < read;i++)
                if(line[i] == tip){
                    WS c_letters[i] = tip;
                    correct = 1;
                }
                if(correct == 1) SS correct[ SS mode ]++;
            
            WS alphabet[ at - 1 ] = '0';
            SS letter[ SS mode ]++;
        }
        for(int i=0; i < read;i++){
            if(WS c_letters[i] == '_')  complete = 0;
        }

        if(correct == 0) SS stage[ SS mode ]++;
        
        if( SS stage [SS mode] == 11 && SS mode == 0 && pl_count == 2) {
            SS complete[1] = 1; 
            break;
        }
        if( SS stage [SS mode] == 11 && SS mode == 1 && pl_count == 2) {
            SS complete[0] = 1; 
            break;
        }
        if( SS stage [SS mode] == 11 && SS mode == 0 && pl_count == 1) break;

        if( complete == 1 && SS mode == 0 && pl_count == 2) {
            SS complete[0] = 1; 
            break;
        }
        if( complete == 1 && SS mode == 1 && pl_count == 2) {
            SS complete[1] = 1; 
            break;
        }
        if( complete == 1 && SS mode == 0 && pl_count == 1) break;

        if( pl_count == 2 && correct == 0 ){
            if(SS mode == 0) SS mode = 1;
            else if(SS mode == 1) SS mode = 0;
        }
    }

    SS signal = 1;
    Sleep(1000);

     if( SS stage [SS mode] == 11 && SS mode == 0 && pl_count == 2){
        printf("\n\nSpieler %s gewinnt!!!\n", SS player2);
    }
    if( SS stage [SS mode] == 11 && SS mode == 1 && pl_count == 2){
        printf("\n\nSpieler %s gewinnt!!!\n", SS player1);
    }
    if( SS stage [SS mode] == 11 && SS mode == 0 && pl_count == 1){
        printf("\n\nVerloren :(\n");
    }
    if( complete == 1 && SS mode == 0 && pl_count == 2){
        printf("\n\nSpieler %s gewinnt!!!\n", SS player1);
    }
    if( complete == 1 && SS mode == 1 && pl_count == 2){
        printf("\n\nSpieler %s gewinnt!!!\n", SS player2);
    }
    if( complete == 1 && SS mode == 0 && pl_count == 1){
        printf("\n\nUeberlebt\n");
    }

    free( dat );
    return 0;
}
