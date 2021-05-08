#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "get.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    std::cout<<"penis"<<std::endl;

    field.clear();
    getGui();
    initGui();
    orig = field;
    on_button_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_button_clicked()
{

    correction();

    updateClues();
    std::cout<<std::endl;

    //p2d(field);

    std::cout<<std::endl;

    //p2d( fills );




}

void MainWindow::updateClues(){
    int i = 0;
    for(int y=0; y < 9;y++){
        for(int x=0; x < 9;x++){
            if( field[y][x] != 0 ){
                clues[i]->setText( QString( std::to_string( field[y][x]).c_str() ) );
                clues[i]->setAlignment(Qt::AlignCenter);
                pencil[i]->hide();

            }
            i++;
        }
    }
}

void MainWindow::updatePencil(){
    for(int i=0; i < 81;i++){
        QString fill;
        for(auto i: fills[i])
            fill += std::to_string(i).c_str();

        pencil[i]->setText( QString( fill ) );
        pencil[i]->setStyleSheet( "QLineEdit{ border-width: 1px; border-style: solid; border-color: #BEBEBE #BEBEBE #323232 #BEBEBE; }" );
    }
}

void MainWindow::untilFind_8(){
    ussv state = field;
    field = find_8();

    while( state != field ){
        field = state;
        state = find_8();

        std::cout<<"hallo123"<<std::endl;
    }
    fills = negative( fills );
}

void MainWindow::untilOverFly(){
    ussv state = field;
    field = overFly();

    while( state != field ){
        field = state;
        state = overFly();

        std::cout<<"hallo123"<<std::endl;
    }
}

void MainWindow::correction(){
    untilFind_8();

    pUssv( fills );
    pUssv(orig);
    std::cout<<std::endl;

    untilOverFly();
    untilFind_8();
    updateClues();
    updatePencil();
}


ussv MainWindow::negative(ussv options){
    ussv positive;
    for(uint32_t i=0; i < options.size();i++){
        positive.push_back( {} );
        int f=1;
        for(; f < 10 ; f++)
            if(std::find(options[i].begin(), options[i].end(), f) == options[i].end() && options[i][0] != 0) positive[i].push_back( f );
            //else if(options[i][0] == 0) positive[i].push_back( 0 );
    }

    return positive;
}

void MainWindow::pUssv(ussv vector){
    for(uint32_t i=0; i < vector.size();i++){
        for(uint32_t ii=0; ii < vector[i].size() ;ii++){
            std::cout<< vector[i][ii];
            if(ii == vector[i].size() - 1) std::cout<<std::endl;
        }
    }
}

void MainWindow::pBbv(bbv vector){
    for(uint32_t i=0; i < vector.size();i++){
        for(uint32_t ii=0; ii < vector[i].size() ;ii++){
            std::cout<< vector[i][ii];
            if(ii == vector[i].size() - 1) std::cout<<std::endl;
        }
    }
}

void MainWindow::pUsv(usv vector){
    for(auto i: vector){
        std::cout<<i;
    }
    std::cout<<std::endl;
}

ussv MainWindow::find_8(){
    fills.clear();

    for(int i=0; i < 9;i++){
        for(int ii=0; ii < 9;ii++){
            usv fill;

            for(int y=0; y < 9 ; y++)
                if ( std::find(fill.begin(), fill.end(), field[y][ii]) == fill.end() && field[y][ii] != 0 ) fill.push_back( field[y][ii] );

            for(int x=0; x < 9 ; x++)
                if ( std::find(fill.begin(), fill.end(), field[i][x]) == fill.end() && field[i][x] != 0 ) fill.push_back( field[i][x] );

            int gridx=0;
            int gridy=0;

            //std::cout<<fill.size()<<" pennis"<<std::endl;

            for(; gridx < 3 ; gridx++)
                if ( std::find(xbox[gridx].begin(), xbox[gridx].end(), ii) != xbox[gridx].end()) break;

            for(; gridy < 3 ; gridy++)
                if ( std::find(ybox[gridy].begin(), ybox[gridy].end(), i) != ybox[gridy].end()) break;

            for(int yy=0; yy < 3;yy++){
                for(int xx=0; xx < 3;xx++){
                    if ( std::find(fill.begin(), fill.end(), field [ybox[gridy][yy]] [xbox[gridx][xx]]) == fill.end()
                    && field [ybox[gridy][yy]] [xbox[gridx][xx]] != 0 )

                        fill.push_back( field [ybox[gridy][yy]] [xbox[gridx][xx]] );
                }
            }


            //std::cout<<fill.size()<<" pennis2 "<<ii + 1<<" "<<i + 1<<std::endl;

            if(field[i][ii] == 0 && fill.size() == 8){
                int f=1;
                for(; f < 10 ; f++)
                    if(std::find(fill.begin(), fill.end(), f) == fill.end()) break;

                if(field[i][ii] == 0) field[i][ii] = f;

                //std::cout<<fill.size()<<" penis2 "<<f<<"  "<<field[i][ii]<<std::endl;
            }

            if(orig[i][ii] == 0) fills.push_back( fill );

            else fills.push_back( usv() = {0} );
        }
    }

        return field;
}


usv MainWindow::collectRow(ussv field, int rc, char roc){
    usv collect;
    if(roc == 'x'){
        for(int i=0; i < 9;i++)
            if( field[i][rc] != 0 ) collect.push_back( field[i][rc] );
    }
    else if(roc == 'y'){
        for(int i=0; i < 9;i++)
            if( field[rc][i] != 0 ) collect.push_back( field[rc][i] );
    }

    return collect;
}

void MainWindow::boxElim(bbv &box, sv rows, uint16_t i,uint16_t x, uint16_t y){
    if(i == 0 || i == 1){
        for(int el=0; el < 3;el++)
            box[ el ][ x + rows[i] ] = 1;
    }
    else if(i == 2 || i == 3){
        for(int el=0; el < 3;el++)
            box[ y + rows[i] ][ el ] = 1;
    }

}

void MainWindow::rowColSolve(ussv &field, sv pos_row, uint16_t x, uint16_t y, uint16_t xx, uint16_t yy){

    ussv clues;
    clues.push_back( collectRow( field, x + pos_row[0], 'x' ) );
    clues.push_back( collectRow( field, x + pos_row[1], 'x' ) );
    clues.push_back( collectRow( field, y + pos_row[2], 'y' ) );
    clues.push_back( collectRow( field, y + pos_row[3], 'y' ) );

    for (int value=1; value < 10;value++ ){
        bbv box = {{0,0,0},
                   {0,0,0},
                   {0,0,0}};
        uint16_t gridx=0;
        uint16_t gridy=0;
        bool value_isin = 0;

        for(; gridx < 3 ; gridx++)
            if ( std::find(xbox[gridx].begin(), xbox[gridx].end(), x) != xbox[gridx].end()) break;

        for(; gridy < 3 ; gridy++)
            if ( std::find(ybox[gridy].begin(), ybox[gridy].end(), y) != ybox[gridy].end()) break;

        for (int yy=0; yy < 3;yy++) {
            for (int xx=0; xx < 3;xx++) {
                if( field [ybox[gridy][yy]] [xbox[gridx][xx]] != 0) box[yy][xx] = 1;
                if( field [ybox[gridy][yy]] [xbox[gridx][xx]] == value) value_isin = 1;
            }
        }
        if( value_isin ) continue;
        for(int i=0; i < 4;i++ ){
            if ( std::find(clues[i].begin(), clues[i].end(), value) != clues[i].end()){
                boxElim(box, pos_row, i, xx, yy);
            }
        }
        uint16_t complete = 0;
        for(auto &ii: box){
            for(auto i: ii){
            if(i == 0) complete++;
            }
        }
        if( box[yy][xx] == 0 && complete == 1) {
            field[y][x] = value;
            pBbv(box);
        }
        //std::cout<<"sack: "<<value<<std::endl;
    }
}

ussv MainWindow::overFly(){
    ssv position = {{1,2,1,2},
                    {1,-1,1,2},
                    {-1,-2,1,2},
                    {1,2,1,-1},
                    {1,-1,1,-1},
                    {-1,-2,1,-1},
                    {1,2,-1,-2},
                    {1,-1,-1,-2},
                    {-1,-2,-1,-2}};
    ussv newfield = field;

    for(uint16_t y=0; y < 9;y++){
        for(uint16_t x=0; x < 9;x++){
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x + 1 & x + 2 & y + 1 & y + 2
                rowColSolve(newfield, position[0], x, y, 0, 0);
            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x + 1 & x - 1 & y + 1 & y + 2
                rowColSolve(newfield, position[1], x, y, 1, 0);
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 0 || y == 0 +3 || y == 0 +3+3) ){
                //x - 1 & x - 2 & y + 1 & y + 2
                rowColSolve(newfield, position[2], x, y, 2, 0);
            }
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x + 2 & y + 1 & y - 1
                rowColSolve(newfield, position[3], x, y, 0, 1);
            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x - 1 & y + 1 & y - 1
                rowColSolve(newfield, position[4], x, y, 1, 1);
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 1 || y == 1 +3 || y == 1 +3+3) ){
                //x + 1 & x - 2 & y + 1 & y - 1
                rowColSolve(newfield, position[5], x, y, 2, 1);
            }
            if( (x == 0 || x == 0 +3 || x == 0 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x + 1 & x + 2 & y - 1 & y - 2
                rowColSolve(newfield, position[6], x, y, 0, 2);
            }
            if( (x == 1 || x == 1 +3 || x == 1 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x + 1 & x - 1 & y - 1 & y - 2
                rowColSolve(newfield, position[7], x, y, 1, 2);
            }
            if( (x == 2 || x == 2 +3 || x == 2 +3+3) && (y == 2 || y == 2 +3 || y == 2 +3+3) ){
                //x - 1 & x - 2 & y - 1 & y - 2
                rowColSolve(newfield, position[8], x, y, 2, 2);
            }
        }
    }
    return newfield;
}


