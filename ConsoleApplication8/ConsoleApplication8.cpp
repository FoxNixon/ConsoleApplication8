// ConsoleApplication8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;

#define FIELD_SIZE 3

enum gameState {
    IN_PROGRESS,
    AI_WON,
    PAYER_WON,
    DRAW    
};

enum TState : char {
    ZERO = 'O',
    CROSS = 'X',
    EMPTY = ' '
};

enum gameMode {
    GM_EASY,
    GM_NORMAL,
    GM_HARD
};


struct TGame {
    int mode;
    int status;
    TState** ppField;
    int gameMode;
    TState signAI;
    TState signHuman;
    size_t step = 0;

};

void startGame(TGame& game);

void printLegend(TGame & game){
    if (game.mode != GM_HARD) {
        cout << endl;
        cout << "  7 | 8 | 9 " << endl;
        cout << " -----------" << endl;
        cout << "  4 | 5 | 6 " << endl;
        cout << " -----------" << endl;
        cout << "  1 | 2 | 3 " << endl;
        cout << endl;
    }
}



void printField(TGame & game) {
    cout << endl;
    for (int y = FIELD_SIZE-1; y > -1; y--){
        cout << " ";
        for (size_t x = 0; x < FIELD_SIZE; x++){
            cout << " " << game.ppField[y][x];
            if (x < FIELD_SIZE - 1) {
                cout << " |";
            }
        }
        if (y > 0) {
            cout << endl << " -----------" << endl;
        }
    }
    cout << endl;
    cout << endl;
}
void selectFirst(TGame& game) {
    srand(time(NULL));
    if (rand() % 2 == 0) {           // 1
        game.signAI = ZERO;
        game.signHuman = CROSS;
        game.step = 0;
    }
    else {                      // 0
        game.signAI = CROSS;
        game.signHuman = ZERO;
        game.step = 1;
    }
}

void initGame(TGame& game) {
    selectFirst(game);
    game.status = IN_PROGRESS;
    game.ppField = new TState * [FIELD_SIZE];
    for (size_t y = 0; y < FIELD_SIZE; y++) {
        game.ppField[y] = new TState[FIELD_SIZE];
    }
    for (size_t y = 0; y < FIELD_SIZE; y++) {
        for (size_t x = 0; x < FIELD_SIZE; x++) {
            game.ppField[y][x] = EMPTY;
        }
    }
}

void deinitGame(TGame& game){
    for (size_t i = 0; i <FIELD_SIZE; i++){
        delete [] game.ppField[i];
        
    }
    delete[] game.ppField;
    game.ppField = nullptr;
}


void humanTurn(TGame& game) {
    
    int playerInput = NULL;
    int x, y = NULL;
    bool wrongInput = true;
    while (wrongInput) {
        system("cls");
        printField(game);
        std::cout << "Your turn!" <<endl;
        cin >> playerInput;
        if (game.mode != GM_HARD) {
            if (playerInput > 0 && playerInput < 10) {
                if (playerInput % FIELD_SIZE == 0) {
                    x = FIELD_SIZE;
                }
                else {
                    x = playerInput % FIELD_SIZE;
                }
                y = (playerInput - 1) / FIELD_SIZE;
                if (game.ppField[y][x-1] == EMPTY) {
                    game.ppField[y][x-1] = game.signHuman;
                    wrongInput = false;
                }
                
            }
            else {
                cout << "Wrong Input!!!" << endl;
            }
            
        }
        game.step++;
    }
}
int computerTurn(TGame& game){
    system("cls");
    printField(game);
    game.step++;
    srand(time(NULL));

    if (game.mode >= GM_EASY){
        //Center
        if (game.ppField[(FIELD_SIZE / 2)][(FIELD_SIZE / 2)] == EMPTY) {
            game.ppField[(FIELD_SIZE / 2)][(FIELD_SIZE / 2)] = game.signAI;
            return 0;
        }

        //Check corners
        if (game.ppField[0][0] == EMPTY && game.ppField[FIELD_SIZE - 1][FIELD_SIZE - 1] == EMPTY) {
            //srand(time(NULL));
            ((rand() & 2) == 0) ? game.ppField[0][0] = game.signAI : game.ppField[FIELD_SIZE - 1][FIELD_SIZE - 1] = game.signAI;
            return 0;
        }
        if (game.ppField[FIELD_SIZE - 1][0] == EMPTY && game.ppField[0][FIELD_SIZE - 1] == EMPTY) {
            //srand(time(NULL));
            ((rand() & 2) == 0) ? game.ppField[FIELD_SIZE - 1][0] = game.signAI : game.ppField[0][FIELD_SIZE - 1] = game.signAI;
            return 0;
        }

        //check non-corners
        int rand_y = rand() % (FIELD_SIZE -1);
        int rand_x = rand() % (FIELD_SIZE -1);
        while (game.ppField[rand_y][rand_x] != EMPTY) {
            rand_y = rand() % (FIELD_SIZE);
            rand_x = rand() % (FIELD_SIZE);
        }
        game.ppField[rand_y][rand_x] = game.signAI;
        return 0;
    }
    

}

int checkStatus(TGame& game) {
    int counterHuman = 0;
    int counterAI = 0;
    // check string for winner
    for (size_t y = 0; y < FIELD_SIZE; y++){      
        int counterHuman = 0;
        int counterAI = 0;
        for (size_t x = 0; x < FIELD_SIZE; x++){
            if (game.ppField[y][x]!= EMPTY && game.ppField[y][0] == game.ppField[y][x] ){
                if (game.ppField[y][x] == game.signAI) {
                    counterAI++;
                }
                else {
                    counterHuman++;
                }
                if (counterAI == FIELD_SIZE && game.ppField[y][x] == game.signAI) {
                    game.status = AI_WON;
                    return game.status;
                }
                else if (counterHuman == FIELD_SIZE && game.ppField[y][x] == game.signHuman) {
                    game.status = PAYER_WON;
                    return game.status;
                }
            }
            
            
        }
           
    }

    //check column for winner
    counterHuman = 0;
    counterAI = 0;
    for (size_t x = 0; x < FIELD_SIZE; x++)
    {
        for (size_t y = 0; y < FIELD_SIZE; y++) {
            counterHuman = 0;
            counterAI = 0;
            if (game.ppField[y][x] != EMPTY && game.ppField[0][x] == game.ppField[y][x]) {
                if (game.ppField[y][x] == game.signAI) {
                    counterAI++;
                }
                else {
                    counterHuman++;
                }
                if (counterAI == FIELD_SIZE && game.ppField[y][x] == game.signAI) {
                    game.status = AI_WON;
                    //cout << "AI Won!!" << endl;
                    return game.status;
                }
                else if (counterHuman == FIELD_SIZE && game.ppField[y][x] == game.signHuman) {
                    game.status = PAYER_WON;
                    //cout << "You Won!" << endl;
                    return game.status;;
                }
            }
        }
    }

    //check diagonals for winner
    //simple diagonal
    counterHuman = 0;
    counterAI = 0;
    for (size_t xy = 0; xy < FIELD_SIZE; xy++)
    {
        if (game.ppField[xy][xy] != EMPTY && game.ppField[0][0] == game.ppField[xy][xy]) {
            if (game.ppField[xy][xy] == game.signAI) {
                counterAI++;
            }
            else {
                counterHuman++;
            }
            if (counterAI == FIELD_SIZE && game.ppField[xy][xy] == game.signAI) {
                game.status = AI_WON;
                //cout << "AI Won!!" << endl;
                return game.status;
            }
            else if (counterHuman == FIELD_SIZE && game.ppField[xy][xy] == game.signHuman) {
                game.status = PAYER_WON;
                //cout << "You Won!" << endl;
                return game.status;;
            }
        }
        
    }

    //not simple diagonal
    counterAI = 0;
    counterHuman = 0;
    for (size_t x = 0; x < FIELD_SIZE; x++)
    {
        size_t y = FIELD_SIZE - x - 1;
        if (game.ppField[y][x] != EMPTY && game.ppField[FIELD_SIZE - 1][0] == game.ppField[y][x]) {
            if (game.ppField[y][x] == game.signAI) {
                counterAI++;
            }
            else {
                counterHuman++;
            }
            if (counterAI == FIELD_SIZE && game.ppField[y][x] == game.signAI) {
                game.status = AI_WON;
                //cout << "AI Won!!" << endl;
                return game.status;
            }
            else if (counterHuman == FIELD_SIZE && game.ppField[y][x] == game.signHuman) {
                game.status = PAYER_WON;
                //cout << "You Won!" << endl;
                return game.status;;
            }
        }

    }
    
    counterAI = 0;
    counterHuman = 0;
    int counter = 0;
    for (size_t y = 0; y < FIELD_SIZE; y++)
    {
        for (size_t x = 0; x < FIELD_SIZE; x++)
        {
            if (game.ppField[y][x] == EMPTY) {
                game.status = IN_PROGRESS;
                return game.status;
            }
        }
        counter++;
        if (counter == FIELD_SIZE) {
            game.status = DRAW;
            return game.status;
        }
    }
   
}
void requestNewGame(TGame& game) {
    string respond;
    printLegend(game);

    cout << endl << "Wanna start neg game? [yes/no]" << endl;
    cin >> respond;
    if (respond == "yes" || respond == "Yes" || respond == "y" || respond == "Y") {
        startGame(game);
    }
    else {
        cout << "See you later!" << endl;
        return;
    }

}

void startGame(TGame& game){
    initGame(game);
    selectFirst(game);
    while (game.status == IN_PROGRESS) {
        
        if (game.step % 2 == 0) {
            humanTurn(game);
        }
        else{
            computerTurn(game);
        }
        system("cls");
        printField(game);
        checkStatus(game);
    }
    if (game.status == AI_WON) {
        cout << "AI Wins!!" << endl;
    }
    else if (game.status == PAYER_WON) {
        cout << "Congratultations! You win!" << endl;
    }
    else {
        cout << "Draw..." << endl;
    }


    deinitGame(game);
    requestNewGame(game);
}


int main()
{
    TGame game;
    game.mode = GM_EASY;
    requestNewGame(game);
    //startGame(game);

    return 0;
}
