#include <iostream>
#include <vector>
#include <array>
#include <ctime>
#include "tictactoelib.h"

std::vector<int> boards(MAX_H, -1);

void print_board(const Board& b){
    std::cout << " -----\n";
    for(int i = 0; i < 3; ++i){
        std::cout << '|'; 
        for(int j = 0; j < 3; ++j){
            std::cout << (b[i][j] == 2 ? 'O' : (b[i][j] == 1 ? 'X' : ' ') );
            if(j < 2) std::cout << ' ';
        }
        std::cout << "|\n"; 
    }
    std::cout << " -----\n";
}

int Hash(const Board& b){
    int h = 0;
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
            h = h * 3 + b[i][j];
    return h;
}

Board un_hash(int n){
    Board b;
    for(int i = 2; i >= 0; --i){
        for(int j = 2; j >= 0; --j){
            b[i][j] = n % 3;
            n /= 3;
        }
    }
    return b;
}

int check_win(const Board& b){
    for(int i = 0; i <= 2; i++)
        if(b[i][0] != 0 && b[i][0] == b[i][1] && b[i][1] == b[i][2]) return 2;
    for(int i = 0; i <= 2; i++)
        if(b[0][i] != 0 && b[0][i] == b[1][i] && b[1][i] == b[2][i]) return 2;
    if(b[0][0] != 0 && b[0][0] == b[1][1] && b[1][1] == b[2][2]) return 2;
    if(b[0][2] != 0 && b[0][2] == b[1][1] && b[1][1] == b[2][0]) return 2;
    
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
            if(b[i][j] == 0)
                return 0; 
    return 1;
}

void gen(Board b){
    int w = check_win(b);
    int my_hash = Hash(b);
    if(boards[my_hash] != -1)
        return;
    if(w){
        boards[my_hash] = 2 - w;
    } else {
        int xes = 0;
        int oes = 0;
        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < 3; ++j){
                if(b[i][j] == 2)
                    oes++;
                if(b[i][j] == 1)
                    xes++;
            }
        }
        boards[my_hash] = 0;
        int mini = 3;
        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < 3; ++j){
                if(b[i][j] == 0){
                    Board b2 = b;
                    if(oes > xes)
                        b2[i][j] = 1;
                    else
                        b2[i][j] = 2;
                    gen(b2);
                    mini = std::min(mini, boards[Hash(b2)]);
                }
            }
        }
        boards[my_hash] = 2 - mini;
    }
}

void TicTacToe::_init(){
    gen(un_hash(0));
}

TicTacToe::TicTacToe(bool CPU_STARTS, bool MAKES_MISTAKES){ 
        _init();
        if(MAKES_MISTAKES){
            srand(time(NULL));
            makes_mistakes = true;
        }
        if(CPU_STARTS){
            cpu_plays_O = true;
            make_move(-1, -1);
        }
    }

TicTacToe::TicTacToe(Board b) {
    _init(); _my_board = b;
}

std::pair<int, int> TicTacToe::_find_best_move(){
    std::pair<int, int> mov = {-1, -1};
    int mini = 3;
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            if(_my_board[i][j] == 0){
                Board b2 = _my_board;
                if(cpu_plays_O)
                    b2[i][j] = 2;
                else
                    b2[i][j] = 1;
                int h = Hash(b2);
                std::cout << boards[h] << ' ';
                if(boards[h] < mini || makes_mistakes && (rand () % 10 == 7)){
                    mini = boards[h];
                    mov = {i, j};
                }
            } else {
                std::cout << '9' << ' ';
            }
        }
        std::cout << '\n';
    }

    return mov;
}

std::pair<int, int> TicTacToe::make_move(int x, int y){
    if(x != -1 && y != -1){
        if(_my_board[x][y] != 0)
            return {-1, -1};
        
        // player placed
        if(cpu_plays_O)
            _my_board[x][y] = 1;
        else 
            _my_board[x][y] = 2;
    }
    print_board(_my_board);
    if(check_win(_my_board)){
        game_over = true;
        return {-2, -2};
    }

    std::pair<int, int> CPU = _find_best_move();
    if(cpu_plays_O)
        _my_board[CPU.first][CPU.second] = 2;
    else
        _my_board[CPU.first][CPU.second] = 1;
    print_board(_my_board);
    if(check_win(_my_board)){
        game_over = true;
    }
    return CPU;
}
