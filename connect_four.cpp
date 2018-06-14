/*
o  o  o  o  o  o  o
o  o  o  o  o  o  o
o  o  o  o  o  o  o
o  o  o  o  o  o  o
o  o  o  o  o  o  o
o  o  o  o  o  o  o
*/
#include <vector>
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
    for (int i = 0; i < 6; ++i) {
        for (int k = 0; k < 7; ++k) {
            if(grid[i*7 + k] == 0)  printf("  .");
            if(grid[i*7 + k] == 1)  printf("  X");
            if(grid[i*7 + k] == -1) printf("  O");
        }
        printf("\n");
    }
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
        if(k > 6*7) return false;
        if(grid[k] != player) return false;
    }
    return true;
}

bool check_diagonal_neg(const GridConnect4& grid, int index) {
    int player = grid[index];
    for(int i : {1,2,3}){
        int k = index + 7*i - i;
        if(k > 6*7) return false;
        if(grid[k] != player) return false;
    }
    return true;
}

int get_utility(const GridConnect4& grid) {
    for (int i = 0; i < 6; ++i) {
        for (int k = 0; k < 7; ++k) {
            int index = i*7 + k;
            if(grid[index] == 0) continue;
            if(check_vertical(grid, index)) return grid[index];
            if(check_horizontal(grid, index)) return grid[index];
            if(check_diagonal_pos(grid, index)) return grid[index];
            if(check_diagonal_neg(grid, index)) return grid[index];
        }
    }
    return 0;
}

array<GridConnect4> make_children(const GridConnect4& grid, int player) {
    array<GridConnect4> result;
    for (int i = 0; i < 7; ++i) {
        if(grid[i] != 0) continue;
        GridConnect4 g = grid;
        insert(g, i, player);
        result.push_back(g);
    }

    return result;
}

int best_move;

template <typename State>
int minimax(const State& state, bool max, int depth) {
    // if(is_terminal(state)) return get_utility(state);
    int u = get_utility(state);
    if(u != 0) return u;
    if(depth == 0) return 0;

    auto children = make_children(state, max? 1 : -1);
    int value = minimax(children[0], not max, depth-1);
    best_move = 0;
    for (int i = 1; i < children.size(); ++i)
    {
        const auto& child = children[i];
        int new_val = minimax(child, not max, depth-1);
        if(max and new_val > value) {
            value = new_val;
            best_move = i;
        }
        if(not max and new_val < value) {
            value = new_val;
            best_move = i;
        }
    }
    
    return value;
}


void start_match() {
    GridConnect4 grid (6*7, 0);
    insert(grid, 2, 1);
    insert(grid, 3, 1);

    int col;
    int player = -1;
    print_grid(grid);
    while(true){
        printf("  next move: ");
        scanf("%d", &col);
        printf("\n");
        if(col < 1 or col > 7) continue;
        insert(grid, col-1, player);
        player = -player;
        print_grid(grid);

        if(get_utility(grid) != 0) {
            printf("\nPlayer %d wins!\n", get_utility(grid));
            return;
        }
    }
}

void ai_move(GridConnect4& grid, int color) {
    minimax<GridConnect4>(grid, color>0, 7);
    insert(grid, best_move, color);
}

void beat_ai() {
    GridConnect4 grid (6*7, 0);
    insert(grid, 2, -1);
    insert(grid, 3, -1);
    // insert(grid, 4, -1);

    ai_move(grid, 1);
    print_grid(grid);

    // insert(grid, 2, -1);
    // insert(grid, 3, 1);

    // for(auto g : make_children(grid, -1)) {
    //     print_grid(g);
    //     printf("%d\n\n", get_utility(g));
    // }

    return;



    int col;
    int player = 1;
    print_grid(grid);
    while(true){
        if(player == -1)
            ai_move(grid, player);
        else {
            printf("  next move: ");
            scanf("%d", &col);
            printf("\n");   
            printf("\n");
            if(col < 1 or col > 7) continue;
            insert(grid, col-1, player);
        }

        player = -player;
        print_grid(grid);
        if(get_utility(grid) != 0) {
            printf("\nPlayer %d wins!\n\n", get_utility(grid));
            return;
        }
    }
}

int main(int argc, char const *argv[])
{
    beat_ai();
    // start_match();
    // GridConnect4 grid (6*7, 0);

    // int v = minimax<GridConnect4>(grid, true, 6);
    // printf("%d\n", v);
    // for(auto i : moves) printf("%d ", i);
    // printf("\n");
    return 0;
}
