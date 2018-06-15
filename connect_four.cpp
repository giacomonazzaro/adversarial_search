/*
o  o  o  o  o  o  o
o  o  o  o  o  o  o
o  o  o  o  o  o  o
o  o  o  o  o  o  o
o  o  o  o  o  o  o
o  o  o  o  o  o  o
*/
#include <vector>
#include <cassert>
#define array std::vector
using GridConnect4 = array<int>;

void insert(GridConnect4& grid, int column, int color) {
    int cell = column;
    while(1) {
        if(cell >= 6*7) break;
        if(grid[cell] == 0) cell += 7;
        if(grid[cell] != 0) break;
    }
    grid[cell - 7] = color;
    return;
}

void print_grid(const GridConnect4& grid) {
    for (int i = 0; i < 8; ++i) 
        printf("___");

    printf("\n\n");
    for (int i = 0; i < 6; ++i) {
        for (int k = 0; k < 7; ++k) {
            if(grid[i*7 + k] == 0)  printf("  •");
            if(grid[i*7 + k] == 1)  printf("  X");
            if(grid[i*7 + k] == -1) printf("  O");
        }
        printf("\n");
    }
    for (int i = 0; i < 8; ++i) printf("___");
    printf("\n");
    for (int k = 0; k < 7; ++k) printf("  %d", k+1);
    printf("\n");
}

bool check_horizontal(const GridConnect4& grid, int index) {
    int player = grid[index];
    for(int i : {1,2,3}){
        int k = index + i;
        if(k > 6*7) return false;
        if(grid[k] != player) return false;
    }

    return true;
}

bool check_vertical(const GridConnect4& grid, int index) {
    int player = grid[index];
    for(int i : {1,2,3}){
        int k = index + 7*i;
        if(k > 6*7) return false;
        if(grid[k] != player) return false;
    }
    return true;
}

bool check_diagonal_pos(const GridConnect4& grid, int index) {
    int player = grid[index];
    for(int i : {1,2,3}){
        int k = index + 7*i + i;
        if(k%7 == 0) return false;
        if(k > 6*7) return false;
        if(grid[k] != player) return false;
    }
    return true;
}

bool check_diagonal_pos(const GridConnect4& grid) {
    for (int col = 0; col < 3; ++col) {
        int val = grid[col];
        for(int row = 0; row < 6; row++) {
            int count = 1;
            if(val != grid[col + row*(7+1)]) {
                val = grid[col + row*(7+1)];
                count = 1;
            }
            else {
                count += 1;
            }
            if(count == 4) return val;
        }
    }
    return 0;
}

bool check_diagonal_neg(const GridConnect4& grid) {
    for (int col = 3; col < 7; ++col) {
        int val = grid[col];
        int count = 1;
        for(int row = 1; row < 6; row++) {
            if(val != grid[col + row*(7-1)]) {
                val = grid[col + row*(7-1)];
                count = 1;
            }
            else {
                count += 1;
            }
            if(count == 4) return val;
        }
    }
    return 0;
}


bool check_diagonal_neg(const GridConnect4& grid, int index) {
    int player = grid[index];
    for(int i : {1,2,3}){
        int k = index + 7*i - i;
        if((k+1)%7 == 0) return false;
        if(k > 6*7) return false;
        if(grid[k] != player) return false;
    }
    return true;
}

int get_utility(const GridConnect4& grid) {
    for (int index = 0; index < 6*7; ++index) {
            int row = index / 7;
            int col = index % 7;
            if(grid[index] == 0) continue;
            if(row <= 2)
                if(check_vertical(grid, index)) return grid[index];
            if(col <= 3)
                if(check_horizontal(grid, index)) return grid[index];
            if(row <= 2 and col <=3)
                if(check_diagonal_pos(grid, index)) return grid[index];
            if(row <= 3 and col >= 3)
                if(check_diagonal_neg(grid, index)) return grid[index];
    }
    return 0;
}

array<std::pair<GridConnect4, int>> make_children(const GridConnect4& grid, int player) {
    array<std::pair<GridConnect4, int>> result;
    for (int i = 0; i < 7; ++i) {
        if(grid[i] != 0) continue;
        GridConnect4 g = grid;
        insert(g, i, player);
        result.push_back({g, i});
    }

    return result;
}


template <typename State>
int minimax(const State& state, bool max, int depth, int& best_move) {
    // if(is_terminal(state)) return get_utility(state);
    int u = get_utility(state);
    if(u != 0) return u * (depth + 1);
    if(depth == 0) return 0;

    auto children = make_children(state, max? 1 : -1);
    int dummy;
    int value = minimax(children[0].first, not max, depth-1, dummy);
    best_move = children[0].second;
    for (int i = 0; i < children.size(); ++i) {
        const auto& child = children[i].first;
        int new_val = minimax(child, not max, depth-1, dummy);
        if(max and new_val > value) {
            value = new_val;
            best_move = children[i].second;
        }
        if(not max and new_val < value) {
            value = new_val;
            best_move = children[i].second;
        }
    }

    return value;
}

template <typename State>
int alpha_beta_search(const State& state, bool max, int alpha, int beta, int depth, int& best_move) {
    int u = get_utility(state);
    if(u != 0) return u * (depth + 1);
    if(depth == 0) return 0;

    auto children = make_children(state, max? 1 : -1);
    if(children.size() == 0) return 0;
    
    int dummy;
    int start = rand() % children.size();
    int value = alpha_beta_search(children[start].first, not max, alpha, beta, depth-1, dummy);
    best_move = children[start].second;
    for (int i = 0; i < children.size(); ++i) {
        if(i == start) continue;
        const auto& child = children[i].first;
        int new_value = alpha_beta_search(child, not max, alpha, beta, depth-1, dummy);
        if(max) {
            if(new_value > value) {
                value = new_value;
                best_move = children[i].second;
            }
            if(value >= beta) {
                return value;
            }
            alpha = std::max(alpha, value);
        }
        if(not max) {
            if(new_value < value) {
                value = new_value;
                best_move = children[i].second;
            }
            if(value <= alpha) {
                return value;
            }
            beta = std::min(beta, value);
        }
    }
    
    return value;
}

template <typename State>
int alpha_beta_search(const State& state, bool max, int depth, int& best_move) {
    return alpha_beta_search(state, max, -99999, 999999, depth, best_move);
}

void one_vs_one() {
    GridConnect4 grid (6*7, 0);
    int move;
    int player = -1;
    print_grid(grid);
    while(true){
        printf("  next move: ");
        scanf("%d", &move);
        printf("\n");
        if(move < 1 or move > 7) continue;
        insert(grid, move-1, player);
        player = -player;
        print_grid(grid);

        if(get_utility(grid) != 0) {
            printf("\nPlayer %d wins!\n", get_utility(grid));
            return;
        }
    }
}

void ai_move(GridConnect4& grid, int color, int depth) {
    int move;
    // int value = minimax<GridConnect4>(grid, color>0, 8, move);
    auto value = alpha_beta_search<GridConnect4>(grid, color>0, depth, move);
    insert(grid, move, color);
    printf("(%d) ", depth);
    printf("ai move: %d\n", move+1);
    if(value * color > 0) printf("I will win in %d moves!!!\n", depth - value * color);
    if(value * color < 0) printf("I will lose in %d moves!!!\n", depth + value * color);
}

bool is_full(const GridConnect4& grid) {
    for(int i=0; i<7; i++)
        if(grid[i] == 0) return false;

    return true;
}


void beat_ai() {
    GridConnect4 grid (6*7, 0);
    srand(76);
    int player = 1;
    int human_player = 1;
    int ai_player = -human_player;
    int turn = 0;
    insert(grid, 3, ai_player);
    print_grid(grid);
    while(true) {
        if(player == ai_player) {
            int depth = 11;
            if(turn > 15) depth = 15;
            if(turn > 30) depth = 30;
            ai_move(grid, ai_player, 11);
        }
        else {
            printf("  next move: ");
            int col;
            scanf("%d", &col);
            if(col < 1 or col > 7) continue;
            insert(grid, col-1, human_player);
        }
        printf("\n");   
        printf("\n");

        turn += 1;
        player = -player;
        print_grid(grid);
        if(get_utility(grid) != 0) {
            printf("\nPlayer %d wins!\n\n", get_utility(grid));
            return;
        }
        if(is_full(grid)) {
            printf("\nTie!\n\n");
            return;
        }
    }
}

int main(int argc, char const *argv[])
{
    beat_ai();
    // one_vs_one();
    return 0;
}
