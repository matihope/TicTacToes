#include <vector>
#include <array>

#define MAX_H 19683

typedef long long LL;
typedef std::vector<int> VI;

typedef std::array<std::array<int, 3>, 3> Board;

void print_board(const Board& b);
int Hash(const Board& b);
Board un_hash(int n);
int check_win(const Board& b);
void gen(Board b);

struct TicTacToe{
    void _init();
    TicTacToe(bool CPU_STARTS=false, bool MAKES_MISTAKES=false);
    TicTacToe(Board b);
    Board _my_board = un_hash(0);
    bool cpu_plays_O = false; // defaults to player starting
    bool makes_mistakes = false;
    bool game_over = false;

    std::pair<int, int> _find_best_move();
    std::pair<int, int> make_move(int x, int y);
};