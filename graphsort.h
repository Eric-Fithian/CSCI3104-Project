#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct node {
    vector<node*> out;
    vector<node*> in;
    vector<int> nums;
    int size = 0;
};

struct graph {
    vector<node*> nodes;
    vector<node*> headnodes;
    int size = 0;
};

bool compareByNodeSize(node *a, node *b) {
    return a->size > b->size;
}

void printgraph(graph g) {
    for (node *cur_n : g.nodes) {
        cout << "Node: ";
        for (int num : cur_n->nums) {
            cout << num << ",";
        }
        cout << endl;
        cout << "Edges: " << endl;
        for (node *out_n : cur_n->out) {
            //print cur node nums
            for (int num : cur_n->nums) {
                cout << num << ",";
            }
            cout << "->";
            //print out node nums
            for (int num : out_n->nums) {
                cout << num << ",";
            }
            cout << endl;
        }
    }
}

void printgraphheadnodes(graph g) {
    for (node *cur_n : g.headnodes) {
        cout << "Head Node: ";
        for (int num : cur_n->nums) {
            cout << num << ",";
        }
        cout << endl;
    }
}

void checkBranches(node * n, node * branchhead) {

}

bool checkBranchHelper()