#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include "main.h"

#define MAX 100
size_t signal = 0;

typedef struct{
    char user[MAX][MAX];
    int pos;
}data;

struct letters{
    char alphabet[26];
    char c_letters[26];
    char used_letters[26];
}words[2];

DWORD WINAPI Thread(void* data) {
    size_t limit = (size_t)data;
    clock_t now = clock();

    while(1){
        clock_t later = clock();
        //printf("Time: %ld   %d\n", now, limit);
        if(signal == 0 && limit > 0 && (limit * 60000) < (later - now)) {
            printf("Zeitlimit überschritten\nVerloren :(");
            exit(0);
        }        
        if(signal == 1 && limit == 0) {
            printf("Vergangene Zeit: %ld \n", (later - now));
            exit(0);
        }
        //Sleep(1000);
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

int main(){
    //zweispieler
    //gegen zeit
    //spielername
    char alphabet[26] = "abcdefghijklmnopqrstuvwxyz";


    for(int i=0; i < 2;i++){
        for(int ii=0; ii < 26;ii++){
            (&words[i])->c_letters[ii] = '_';
            (&words[i])->alphabet[ii] = alphabet[ii];
        }
    }

    printf("%s\n", (&words[0])->c_letters);
    printf("%s\n", (&words[0])->alphabet);
    printf("%s\n", (&words[1])->c_letters);
    printf("%s\n", (&words[1])->alphabet);

    size_t pl_count, time_limit = 0, lines = 0, len = 0, equal[2];
    char player1[20], player2[20], time_mode, tip, listuser;
    char *line = NULL;
    ssize_t read;
    equal[0]=0,equal[1]=0;

    FILE *fWords = fopen(".\\words", "r");
    FILE *fUser = fopen(".\\user", "r+");

    if (fWords == NULL || fUser == NULL) return 0;

    srand(time(NULL));

    printf("Hangman Game:\n\nEin oder Zwei Spieler?\n> ");
    scanf("%d", &pl_count);
    if(pl_count > 2) { 
        printf("Ungültige Spieleranzahl"); 
        exit(0);
    }
    printf("Benutzer anzeigen? (j/n)\n> ");
    scanf(" %c", &listuser);

    data *dat = userArray(fUser);

    if(pl_count == 1 && listuser == 'j'){
        const char user[20];

        for(int i=0;i < dat->pos - 1;i++){
            nullNewline( dat->user[i] );
            printf("%s\n" , dat->user[i]);
        }

        printf("Benutzer auswählen\n> ");
        scanf("%s", &user);

        int equal = 0;
        for(int i=0;i < dat->pos - 1 ;i++){
            if(strcmp(user, dat->user[i]) == 0){
                for(int ii=0;ii < 20;ii++)
                    player1[ii] = dat->user[i][ii];
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

        printf("Benutzer 1 auswählen\n> ");
        scanf("%s", &user1);

        printf("Benutzer 2 auswählen\n> ");
        scanf("%s", &user2);

        for(int i=0;i < dat->pos - 1 ;i++){
            if(strcmp(user1, dat->user[i]) == 0){
                for(int ii=0;ii < 20;ii++)
                    player1[ii] = dat->user[i][ii];
                equal[0] = 1;
            }
        }    
        for(int i=0;i < dat->pos - 1 ;i++){
            if(strcmp(user2, dat->user[i]) == 0){
                for(int ii=0;ii < 20;ii++)
                    player2[ii] = dat->user[i][ii];
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
        scanf("%s", &player1);
        fprintf (fUser, "%s\n", player1);
    }
    else if(pl_count == 2 && listuser == 'n'){
        if(equal[0] == 0){
            printf("Gebe Name für Spieler 1 ein\n> ");
            scanf("%s", &player1);
        }
        if(equal[1] == 0){
            printf("Gebe Name für Spieler 2 ein\n> ");
            scanf("%s", &player2);
        }
        fprintf (fUser, "%s\n", player1);
        fprintf (fUser, "%s\n", player2);
    }
    
    fclose(fUser);
    
    printf("Gegen die Zeit? (j/n)\n> ");
    scanf(" %c", &time_mode);

    if( time_mode == 'j' ){
        printf("Gebe die Länge in Minuten ein\n> ");
        scanf("%d", &time_limit);
    }

    //printf("anz: %d\npl1: %s\npl2 %s\nmode: %c\nlimit: %d\n", pl_count, player1, player2, time_mode, time_limit);

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
        //printf("%s\n", line);
        if(lines == num) break;
        lines++;
    }

    fclose(fWords);

    line[read - 1]= '\0';

    for(ssize_t i = read - 1; i < 26;i++){
        (&words[0])->c_letters[i] = '\0'; 
        (&words[1])->c_letters[i] = '\0';  
    }    

    printf("\n\nrandom: %s num: %d\n", line, num);

    size_t tries[2] ,c_tries[2], letter[2], mode = 0, stage[2];
    letter[0] = 0, letter[1] = 0;
    stage[0] = 1, stage[1] = 1; 
    c_tries[0] = 0, c_tries[1] = 0;
    tries[0] = 0, tries[1] = 0;

    system("cls");

    HANDLE thread;
    
    if(time_mode == 'j')  thread = CreateThread(NULL, 0, Thread, (void*)time_limit, 0, NULL);
    if(time_mode == 'n')  thread = CreateThread(NULL, 0, Thread, NULL, 0, NULL);
    

    while(1){
        size_t correct = 0, complete = 1;
        //buchstaben nach alphabet
        //Versuche und richtige Versuche
        if(pl_count == 1) printf("Mache einen Tipp\n> ");
        if(pl_count == 2 && mode == 0) printf("Spieler 1 mache einen Tipp\n> ");
        if(pl_count == 2 && mode == 1) printf("Spieler 2 mache einen Tipp\n> ");
        scanf(" %c", &tip);

        size_t at = find_c((&words[ mode ])->alphabet, 26, tip);
        size_t guess = find_c(line, read, tip);
        
        if( guess > 0 ) c_tries[ mode ]++;
        if( guess == 0 ) tries[ mode ]++;
        
        if( (&words[ mode ])->alphabet[ at - 1 ] == '0' || at == 0 ) {
            printf("Buchstabe wurde schon benutzt!\n");
            continue;
        }
        else if( (&words[ mode ])->alphabet[ at - 1 ] != '0' ) {
            (&words[ mode ])->used_letters[letter[ mode ]] = (&words[ mode ])->alphabet[ at - 1 ];
            for(int i=0; i < read;i++)
                if(line[i] == tip){
                    (&words[ mode ])->c_letters[i] = tip;
                    correct = 1;
                }
            
            (&words[ mode ])->alphabet[ at - 1 ] = '0';
            letter[ mode ]++;
        }
        for(int i=0; i < read;i++){
            if((&words[ mode ])->c_letters[i] == '_') complete = 0;
        }

        system("cls");
        printf("%s\n\n", (&words[ mode ])->alphabet);

        if(correct == 0) stage[ mode ]++;
        printHangman(stage[ mode ]);
        if( stage [mode] < 11 && mode == 0 && pl_count == 2){
            printf("Spieler 2 gewinnt!!!\n");
            break;
        }
        if( stage [mode] == 11 && mode == 1 && pl_count == 2){
            printf("Spieler 1 gewinnt!!!\n");
            break;
        }
        if( stage [mode] == 11 && mode == 0 && pl_count == 1){
            printf("Verloren :(\n");
            break;
        }
        if( complete == 1 && mode == 0 && pl_count == 2){
            printf("Spieler 1 gewinnt!!!\n");
            break;
        }
        if( complete == 1 && mode == 1 && pl_count == 2){
            printf("Spieler 2 gewinnt!!!\n");
            break;
        }
        if( complete == 1 && mode == 0 && pl_count == 1){
            printf("Verloren :(\n");
            break;
        }
        

        printf("\n\n%d %s  %s\n", guess, (&words[ mode ])->c_letters, (&words[ mode ])->used_letters);

        if( pl_count == 2 && correct == 0 ){
            if(mode == 0) mode = 1;
            else if(mode == 1) mode = 0;
        }
    }

    signal = 1;
    Sleep(1000);

    free( dat );
    return 0;
}
