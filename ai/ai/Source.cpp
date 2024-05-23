#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <string>

using namespace std;


struct State {
    vector<string> board;

    bool operator==(const State& other) const {
        return board == other.board;
    }

    bool operator!=(const State& other) const {
        return !(*this == other);
    }

    struct Hash {
        size_t operator()(const State& state) const {
            size_t hash = 0;
            for (const auto& row : state.board) {
                for (char c : row) {
                    hash = hash * 31 + c;
                }
            }
            return hash;
        }
    };

    bool operator<(const State& other) const {
        
        return board < other.board;
    }
};


int manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int heuristic(const State& state, const State& goalState) {
    int totalDistance = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (state.board[i][j] != '-') {
                char letter = state.board[i][j];
                bool found = false;
                for (int m = 0; m < 4; ++m) {
                    for (int n = 0; n < 4; ++n) {
                        if (goalState.board[m][n] == letter) {
                            totalDistance += manhattanDistance(i, j, m, n);
                            found = true;
                            break;
                        }
                    }
                    if (found) break;
                }
            }
        }
    }
    return totalDistance;
}

vector<pair<State, char>> getMoves(const State& state) {
    vector<pair<State, char>> moves;
    const int dx[] = { 1, -1, 0, 0 };
    const int dy[] = { 0, 0, 1, -1 };
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (state.board[i][j] != '-') {
                for (int k = 0; k < 4; ++k) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    if (ni >= 0 && ni < 4 && nj >= 0 && nj < 4 && state.board[ni][nj] == '-') {
                        State nextState = state; 
                        nextState.board[i][j] = '-';
                        nextState.board[ni][nj] = state.board[i][j];
                        moves.push_back({ nextState, state.board[i][j] });
                    }
                }
            }
        }
    }
    return moves;
}


bool depthFirstSearch(const State& startState, const State& goalState, int& statesExpanded) {
    stack<State> stack;
    unordered_set<State, State::Hash> visited;
    unordered_map<State, State, State::Hash> parent; 

    stack.push(startState);
    visited.insert(startState);
    while (!stack.empty()) {
        State currentState = stack.top();
        stack.pop();
        statesExpanded++;

        if (currentState == goalState) {
          
            State current = currentState;
            vector<State> path;
            while (current != startState) {
                path.push_back(current);
                current = parent[current];
            }
            path.push_back(startState);
            reverse(path.begin(), path.end());

            for (const auto& state : path) {
                for (const auto& row : state.board) {
                    cout << row << endl;
                }
                cout << "----------" << endl;
            }
            return true; //  found
        }

        for (const auto& move : getMoves(currentState)) {
            const State& nextState = move.first;
            if (visited.find(nextState) == visited.end()) {
                stack.push(nextState);
                visited.insert(nextState);
                parent[nextState] = currentState; 
            }
        }
    }
    return false; //  not found
}


bool bestFirstSearch(const State& startState, const State& goalState, int& statesExpanded) {
    priority_queue<pair<int, State>, vector<pair<int, State>>, greater<pair<int, State>>> pq;
    unordered_set<State, State::Hash> visited;
    unordered_map<State, State, State::Hash> parent; // To reconstruct the path

    pq.push({ heuristic(startState, goalState), startState });
    visited.insert(startState);
    while (!pq.empty()) {
        State currentState = pq.top().second;
        pq.pop();
        statesExpanded++;

        if (currentState == goalState) {
            
            State current = currentState;
            vector<State> path;
            while (current != startState) {
                path.push_back(current);
                current = parent[current];
            }
            path.push_back(startState);
            reverse(path.begin(), path.end());

            for (const auto& state : path) {
                for (const auto& row : state.board) {
                    cout << row << endl;
                }
                cout << "----------" << endl;
            }
            return true; 
        }

        for (const auto& move : getMoves(currentState)) {
            const State& nextState = move.first;
            if (visited.find(nextState) == visited.end()) {
                pq.push({ heuristic(nextState, goalState), nextState });
                visited.insert(nextState);
                parent[nextState] = currentState; 
            }
        }
    }
    return false; 
}

int main() {
    string filename;
   
        cout << "Enter the name of the text file containing the initial and goal states: ";
    cin >> filename;

    ifstream file(filename);
    State startState, goalState;
    if (file.is_open()) {
        for (int i = 0; i < 4; ++i) {
            string line;
            getline(file, line);
            startState.board.push_back(line);
        }
        for (int i = 0; i < 4; ++i) {
            string line;
            getline(file, line);
            goalState.board.push_back(line);
        }
        file.close();
    }



    int statesExpandedDFS = 0;
    if (depthFirstSearch(startState, goalState, statesExpandedDFS)) {
        cout << "Number of states expanded using depth-first search: " << statesExpandedDFS << endl;
    }
    else {
        cout << "Goal state is not reachable from the initial state using depth-first search." << endl;
    }


    cout << "\n\n\n\n\n\n\n\n\n\n\n";



    int statesExpandedBFS = 0;
    if (bestFirstSearch(startState, goalState, statesExpandedBFS)) {
        cout << "Number of states expanded using best-first search: " << statesExpandedBFS << endl;
    }
    else {
        cout << "Goal state is not reachable from the initial state using best-first search." << endl;
    }

    return 0;
}
