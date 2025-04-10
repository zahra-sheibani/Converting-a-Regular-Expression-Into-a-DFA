# Converting-a-Regular-Expression-Into-a-DFA
This C++ program constructs a syntax tree from a user-defined regular expression, computes nullable, firstpos, lastpos, and followpos, and finally generates a DFA (Deterministic Finite Automaton) transition table based on these attributes. The tree structure and input alphabet are user-driven.\
Code Structure:
1. Node Structure\
struct Node {\
    int position;\
    char value;\
    Node* left;\
    Node* right;\
};\
Each syntax tree node holds:\
•	value: character from the regex (operand/operator).\
•	position: unique index (non-zero for leaf nodes).\
•	left and right: child pointers for binary tree structure.
________________________________________
2. Set Utilities\
Performs union of two sets used in firstpos, lastpos, and followpos.
________________________________________
3. Nullable Function\
bool nullable(Node* node);\
Returns true if the subexpression rooted at node can derive the empty string (ε). Logic:\
•	'@' and '*' → nullable\
•	'|' → if any child is nullable\
•	'.' → if both children are nullable
________________________________________
4. Firstpos & Lastpos\
unordered_set<int> firstpos(Node* node);\
unordered_set<int> lastpos(Node* node);\
Used in DFA state construction:\
•	firstpos: positions that can appear first.\
•	lastpos: positions that can appear last. These are recursively computed based on node type.
________________________________________
5. Followpos Table\
unordered_map<int, unordered_set<int>> followpos_table;\
void computeFollowpos(Node* node);\
The key to DFA transition logic:\
•	For '.', each position in lastpos(left) gets firstpos(right) added to its followpos.\
•	For '*', each position in lastpos(node) gets firstpos(node) added.
________________________________________
6. Syntax Tree Construction\
Node* GetTheTree();\
Recursive function to build the syntax tree interactively:\
•	Asks for node value and position.\
•	Continues left and right until user enters ~.\
This allows dynamic, user-driven structure creation.
________________________________________
7. DFA Construction\
void ConvertToDFA(Node* root, unordered_set<int> start_state, const vector<char>& alphabet);\
Key steps:\
.	Start from firstpos(root) as initial DFA state.\
.	For each state, compute transitions for each alphabet symbol using followpos.\
.	Track already seen states and add new ones.\
.	Print DFA transitions.
________________________________________
8. Main Function\
int main();\
Flow:\
•	Gets the regex (for reference).\
•	Builds tree via GetTheTree.\
•	Computes firstpos, followpos.\
•	Accepts alphabet.\
•	Converts to DFA.

