#include <stdio.h>

void printHangman(size_t stage){
    if( stage == 1){
        printf("                        \n"
               "                        \n" 
               "                        \n"
               "                        \n"
               "                        \n"
               "                        \n"
               "                        \n"
               "                        \n"
               "                        \n"
               "                        \n"
               "                        \n\n\n");
    }
    else if( stage == 2){
        printf("                        \n"
               "    |                   \n" 
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n\n\n");
    }
    else if( stage == 3){
        printf("     _________          \n"
               "    |                   \n" 
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n\n\n");
    }
    else if( stage == 4){
        printf("     _________          \n"
               "    | /                 \n" 
               "    |/                  \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n\n\n");
    }
    else if( stage == 5){
        printf("     _________          \n"
               "    | /      |          \n" 
               "    |/                  \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n\n\n");
    }
    else if( stage == 6){
        printf("     _________          \n"
               "    | /      |          \n" 
               "    |/     _\\|/^        \n"
               "    |       (_oo        \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n\n\n");
    }
    else if( stage == 7){
        printf("     _________          \n"
               "    | /      |          \n" 
               "    |/     _\\|/^        \n"
               "    |       (_oo        \n"
               "    |        |          \n"
               "    |        |          \n"
               "    |        |          \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n\n\n");
    }
    else if( stage == 8){
        printf("     _________          \n"
               "    | /      |          \n" 
               "    |/     _\\|/^        \n"
               "    |       (_oo        \n"
               "    |        |          \n"
               "    |       /|          \n"
               "    |        |          \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n\n\n");
    }
    else if( stage == 9){
        printf("     _________          \n"
               "    | /      |          \n" 
               "    |/     _\\|/^        \n"
               "    |       (_oo        \n"
               "    |        |          \n"
               "    |       /|\\         \n"
               "    |        |          \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n\n\n");
    }
    else if( stage == 10){
        printf("     _________          \n"
               "    | /      |          \n" 
               "    |/     _\\|/^        \n"
               "    |       (_oo        \n"
               "    |        |          \n"
               "    |       /|\\         \n"
               "    |        |          \n"
               "    |       /           \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n\n\n");
    }
    else if( stage == 11){
        printf("     _________          \n"
               "    | /      |          \n" 
               "    |/     _\\|/^        \n"
               "    |       (_oo        \n"
               "    |        |          \n"
               "    |       /|\\         \n"
               "    |        |          \n"
               "    |       / \\         \n"
               "    |                   \n"
               "    |                   \n"
               "    |                   \n\n\n");
    }
}