#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>

#define MAX 100

typedef struct{
    char user[MAX][MAX];
    

    int pos;

}data;

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
    size_t pl_count, time_limit, lines = 0, len = 0, equal[2];
    char player1[20], player2[20], time_mode, tip, listuser;
    char *line = NULL;
    ssize_t read;
    equal[0]=0,equal[1]=0;

    FILE *fWords = fopen(".\\words", "r");
    FILE *fUser = fopen(".\\user", "r+");

    if (fWords == NULL || fUser == NULL) return 0;

    srand(time(0));

    printf("Hangman Game:\n\nEin oder Zwei Spieler?\n> ");
    scanf("%d", &pl_count);
    if(pl_count > 2) { 
        printf("Ungültige Spieleranzahl"); 
        exit(0);
    }
    printf("Benutzer anzeigen? (j/n)\n> ");
    scanf(" %c", &listuser);

    if(pl_count == 1 && listuser == 'j'){
        const char user[20];
        data *dat = userArray(fUser);

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
        data *dat = userArray(fUser);

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

    //printf("\033[2J");
    //printf("\033[%d;%dH", 0, 0);

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

    printf("\n\nrandom: %s num: %d\n", line, num);



    while(0){
        printf("Mache einen Tipp\n> ");
        scanf(" %c", &tip);

        printf("\n\n%d", find_c(line, read, tip));
    }






    return 0;
}
