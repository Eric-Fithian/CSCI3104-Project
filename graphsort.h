#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

extern int iter;

struct node {
    vector<node*> out;
    vector<node*> in;
    vector<int> nums;
    int size = 0;
    bool visited = 0;
};

struct tree {
    node * head;
    vector<node*> leafs;
};

struct graph {
    vector<node*> nodes;
    vector<tree*> trees;
    vector<node*> leafs;
    int size = 0;
};

bool compareByNodeSize(node *a, node *b);
void printnode(node * n);
void printnodes(graph * g);
void printnodes(graph * g);
void printgraph(graph * g);
void saveGraphToFile(graph * g, string file);
void printgraphheadnodes(graph * g);

void printleafs(graph * g);

void setGraphToUnvisited(graph * g);
bool checkNodeMatch(node * n1, node * n2);
void setParentsVisisted(node * n);

void checkBranchBottomUp(node * n, node * leaf, graph * g);
void buildGraphBottomUp(graph * initg, graph * g);

bool checkBranchTopDown(node * n, node * branchhead, graph * g, tree * t);
void buildGraphTopDown(graph * initg, graph * g);

void buildGraphFromLeaves(graph * initg, graph * g);
void checkParents(graph * g, node * n, node * child);

bool checkCorrectness(graph * g);




