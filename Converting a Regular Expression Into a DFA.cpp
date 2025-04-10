#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;

struct Node {
    int position;
    char value;
    Node* left;
    Node* right;

    Node(char v, int p) : value(v), left(nullptr), right(nullptr), position(p) {}
};

unordered_set<int> setUnion(const unordered_set<int>& set1, const unordered_set<int>& set2) {
    unordered_set<int> result = set1;
    for (const auto& element : set2) {
        result.insert(element);
    }
    return result;
}

bool nullable(Node* node) {
    if (!node) return false;
    if (node->value == '@' || node->value == '*') return true;
    if (node->value == '|') return nullable(node->left) || nullable(node->right);
    if (node->value == '.') return nullable(node->left) && nullable(node->right);
    return false;
}

unordered_set<int> firstpos(Node* node) {
    unordered_set<int> fpn;
    if (!node) return fpn;

    if (!node->left && !node->right) {
        if (node->value != '@' && node->position > 0)
            fpn.insert(node->position);
        return fpn;
    }

    if (node->value == '|') {
        return setUnion(firstpos(node->left), firstpos(node->right));
    } else if (node->value == '*') {
        return firstpos(node->left);
    } else if (node->value == '.') {
        fpn = firstpos(node->left);
        if (nullable(node->left)) {
            fpn = setUnion(fpn, firstpos(node->right));
        }
        return fpn;
    }

    return fpn;
}

unordered_set<int> lastpos(Node* node) {
    unordered_set<int> lpn;
    if (!node) return lpn;

    if (!node->left && !node->right) {
        if (node->value != '@' && node->position > 0)
            lpn.insert(node->position);
        return lpn;
    }

    if (node->value == '|') {
        return setUnion(lastpos(node->left), lastpos(node->right));
    } else if (node->value == '*') {
        return lastpos(node->left);
    } else if (node->value == '.') {
        lpn = lastpos(node->right);
        if (nullable(node->right)) {
            lpn = setUnion(lpn, lastpos(node->left));
        }
        return lpn;
    }

    return lpn;
}

unordered_map<int, unordered_set<int>> followpos_table;

void computeFollowpos(Node* node) {
    if (!node) return;
    computeFollowpos(node->left);
    computeFollowpos(node->right);

    if (node->value == '.') {
        for (int i : lastpos(node->left)) {
            followpos_table[i] = setUnion(followpos_table[i], firstpos(node->right));
        }
    } else if (node->value == '*') {
        for (int i : lastpos(node->left)) {
            followpos_table[i] = setUnion(followpos_table[i], firstpos(node->left));
        }
    }
}

Node* GetTheTree() {
    char v;
    int p;
    cout << "Enter node value (use ~ to end): ";
    cin >> v;
    if (v == '~') return nullptr;

    cout << "Enter node position (0 if operator or epsilon): ";
    cin >> p;
    Node* root = new Node(v, p);

    cout << "Creating left node for: " << v << endl;
    root->left = GetTheTree();
    cout << "Creating right node for: " << v << endl;
    root->right = GetTheTree();

    return root;
}

void ConvertToDFA(Node* root, unordered_set<int> start_state, const vector<char>& alphabet) {
    vector<unordered_set<int>> DStates;
    unordered_map<string, unordered_map<char, unordered_set<int>>> transitions;
    DStates.push_back(start_state);

    size_t idx = 0;
    while (idx < DStates.size()) {
        unordered_set<int> current = DStates[idx];
        string state_repr;
        for (int s : current) state_repr += to_string(s) + ",";

        for (char a : alphabet) {
            unordered_set<int> next_state;
            for (int pos : current) {
                for (const auto& entry : followpos_table) {
                    if (entry.first == pos && root->value == a) {
                        next_state = setUnion(next_state, entry.second);
                    }
                }
            }
            if (!next_state.empty() && find(DStates.begin(), DStates.end(), next_state) == DStates.end()) {
                DStates.push_back(next_state);
            }
            transitions[state_repr][a] = next_state;
        }
        idx++;
    }

    cout << "\nDFA Transitions:\n";
    for (const auto& [state, trans] : transitions) {
        for (const auto& [ch, target] : trans) {
            cout << "From {" << state << "} with '" << ch << "' goes to {";
            for (int t : target) cout << t << ",";
            cout << "}\n";
        }
    }
}

int main() {
    cout << "Enter the regular expression (just for display, not used in logic): ";
    string regex;
    cin >> regex;
    cout << "Expression used for syntax tree: (" << regex << ")#" << endl;

    Node* syntaxTree = GetTheTree();
    unordered_set<int> start = firstpos(syntaxTree);
    computeFollowpos(syntaxTree);

    vector<char> alphabet;
    cout << "Enter the input alphabet symbols (enter ~ to end): ";
    char symbol;
    while (cin >> symbol && symbol != '~') {
        alphabet.push_back(symbol);
    }

    ConvertToDFA(syntaxTree, start, alphabet);
    return 0;
}
