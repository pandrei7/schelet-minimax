#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

// Says if the current player is Maxi or not.
using Player = bool;

// For our use case, a "move" is just the index of a child node.
using Move = int;

// The state of the game / a node in the tree.
struct State {
    vector<State*> edges;
    int score;

    // Return a new state representing the game after making a move.
    const State* ApplyMove(const Move& move) const {
        return edges[move];
    }

    // Checks if the state is final.
    bool Final() const {
        return edges.empty();
    }

    // Computes the value of this state.
    int Evaluate(const Player& maxi) const {
        return maxi ? score : -score;
    }

    // Returns all moves you can make in this state.
    vector<Move> PossibleMoves() const {
        vector<int> moves(edges.size());
        iota(moves.begin(), moves.end(), 0);
        return moves;
    }
};

struct Test {
    vector<State> states;
    int correct_score;
    int depth;
};

constexpr int kInf = 1 << 30;

pair<Move, int> Negamax(State const*& state, int depth, const Player& player) {
    if (state->Final() || depth <= 0) {
        return {-1, state->Evaluate(player)};
    }

    int best_score = -kInf;
    Move best_move = -1;

    for (const auto& move : state->PossibleMoves()) {
        auto new_game = state->ApplyMove(move);
        auto score = -Negamax(new_game, depth - 1, !player).second;

        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }
    return {best_move, best_score};
}

pair<Move, int> AlphaBeta(
    State const*& state, int depth, const Player& player, int alpha, int beta
) {
    if (state->Final() || depth <= 0) {
        return {-1, state->Evaluate(player)};
    }

    int best_score = -kInf;
    Move best_move = -1;

    for (const auto& move : state->PossibleMoves()) {
        auto new_game = state->ApplyMove(move);
        auto score = -AlphaBeta(new_game, depth - 1, !player, -beta, -alpha).second;

        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
        if (score > alpha) {
            alpha = score;
        }
        if (alpha >= beta) {
            break;
        }
    }
    return {best_move, best_score};
}

Test ReadTest(const string& filename) {
    ifstream fin(filename);
    Test test;

    int nodes;
    fin >> nodes;
    test.states.resize(nodes);

    fin >> test.correct_score;
    fin >> test.depth;

    for (auto& state : test.states) {
        fin >> state.score;
    }
    for (int i = 1; i < nodes; i += 1) {
        int parent, son;
        fin >> parent >> son;
        test.states[parent].edges.push_back(&test.states[son]);
    }

    return test;
}

void RunTest(const string& filename) {
    auto test = ReadTest(filename);
    State const* root = &test.states[0];

    auto score_negamax = Negamax(root, test.depth, true).second;
    auto score_alphabeta = AlphaBeta(root, test.depth, true, -kInf, kInf).second;

    auto check_score = [&](const string& name, int score) {
        cout << setw(10) << name << ": ";
        if (score == test.correct_score) {
            cout << "ok";
        } else {
            cout << "got " << score << ", wanted " << test.correct_score;
        }
        cout << "\n";
    };

    cout << "Test " << filename << "\n";
    check_score("Negamax", score_negamax);
    check_score("Alpha-Beta", score_alphabeta);
    cout << "\n";
}

int main() {
    RunTest("small.in");
    RunTest("large.in");
    return 0;
}
