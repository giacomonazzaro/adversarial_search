#include <vector>

// Utilities functions.
#define array std::vector
#define add(v, x) v.push_back(x)
#define remove(v, i) v.erase(v.begin() + i)
#define contains(v, x) (std::find(v.begin(),v.end(),x) != v.end())
// #define min(v) *std::min_element(v.begin(),v.end());
#define append(v, w) v.reserve(v.size()+w.size()); v.insert( v.end(), w.begin(), w.end() )

struct Node {
    int value;
    int parent;
    array<int> children;
};

using GameTree = array<Node>;

Node make_node(int value, int parent = -1) {
    Node node;
    node.value = value;
    node.parent = parent;
    return node;
}

void add_node(GameTree& graph, int value, int parent) {
    auto node = make_node(value, parent);
    int index = graph.size();
    graph.push_back(node);
    graph[node.parent].children.push_back(index);
    graph[node.parent].value = -1;
}

void print_tree(const GameTree& graph, bool print_leaf) {
    printf("digraph G {");
    for (int i = 0; i < graph.size(); ++i) {
        // printf("%d [label=\"%d\nval: %d\"]", i, i, (int)graph[i].value); // print indices too.
        if(graph[i].children.size() and print_leaf)
            printf("%d [label=\" \"]", i);
        else
            printf("%d [label=\"%d\"]", i, graph[i].value);
        
        for (int child : graph[i].children) {
        // if nodes[child].edge_data != None:
            // graph += "%s -> %s [label=\"%s\"]\n" % (i, child, nodes[child].edge_data)
        // else:
            printf("%d -> %d \n", i, child);
        }
    }
    printf("}\n");
}

int get_utility(const Node& node) { return node.value; }

int minimax(GameTree& graph, int index, bool max) {
    Node& node = graph[index];
    if(node.children.size() == 0)
        return get_utility(node);

    int value = minimax(graph, node.children[0], not max);
    for (int i = 1; i < node.children.size(); ++i) {
        int child = node.children[i];
        if(max)     value = std::max(value, minimax(graph, child, not max));
        if(not max) value = std::min(value, minimax(graph, child, not max));
    }
    node.value = value;
    return value;
}

int minimax(GameTree& graph) {
    return minimax(graph, 0, true);
}

int alpha_beta_search(GameTree& graph, int index, bool max, int alpha, int beta) {
    Node& node = graph[index];
    if(node.children.size() == 0)
        return get_utility(node);

    int value = alpha_beta_search(graph, node.children[0], not max, alpha, beta);
    for (int i = 1; i < node.children.size(); ++i) {
        int child = node.children[i];
        if(max) {
            value = std::max(value, alpha_beta_search(graph, child, not max, alpha, beta));
            if(value >= beta) {
                node.value = value;
                return value;
            }
            alpha = std::max(alpha, value);
        }
        if(not max) {
            value = std::min(value, alpha_beta_search(graph, child, not max, alpha, beta));
            if(value <= alpha) {
                node.value = value;
                return value;
            }
            beta = std::min(beta, value);
        }
    }
    
    node.value = value;
    return value;
}

int alpha_beta_search(GameTree& graph) {
    return alpha_beta_search(graph, 0, true, 999999, -999999);
}

GameTree make_random_game_tree(int num_nodes) {
    GameTree tree = {make_node(-1)};

    for (int i = 0; i < num_nodes; ++i) {
        int parent = rand() % tree.size();
        int value = (rand() % 2) *2 - 1;
        add_node(tree, value, parent);
    }
    return tree;
}

int main() {
    // add_node(graph, -1, 0);
    // add_node(graph, -1, 0);
    // add_node(graph, 13.0, 1);
    // add_node(graph, 15.0, 1);
    // add_node(graph, 16.0, 1);
    // add_node(graph, 22.0, 2);
    // add_node(graph, 7.0, 2);
    // add_node(graph, 35.0, 2);

    // add_node(graph, 2.0, 3);
    // add_node(graph, 19.0, 3);
    // add_node(graph, 8.0, 3);
    srand(-190);
    auto tree = make_random_game_tree(50);
    minimax(tree);
    // alpha_beta_search(graph);
    print_tree(tree, false);
    // print_tree(tree, false);
}
