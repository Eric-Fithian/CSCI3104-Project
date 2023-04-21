#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

int iter=0;

struct node {
    vector<node*> out;
    vector<node*> in;
    vector<int> nums;
    int size = 0;
    bool visited = 0;
};

struct graph {
    vector<node*> nodes;
    vector<node*> headnodes;
    int size = 0;
};

bool compareByNodeSize(node *a, node *b) {
    return a->size > b->size;
}

void printnode(node * n) {
    for (int num : n->nums) {
        cout << num << ",";
    }
    cout << endl;
}

void printnodes(graph g) {
    for (node *cur_n : g.nodes) {
        for (int num : cur_n->nums) {
            cout << num << ",";
        }
        cout << endl;
    }
}

void printgraph(graph g) {
    for (node *cur_n : g.nodes) {
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

void saveGraphToFile(graph g, string file) {

    ofstream myfile;
    myfile.open(file);

    for (node *cur_n : g.nodes) {
        for (node *out_n : cur_n->out) {
            //print cur node nums
            for (int num : cur_n->nums) {
                myfile << num << ",";
            }
            myfile << "->";
            //print out node nums
            for (int num : out_n->nums) {
                myfile << num << ",";
            }
            myfile << endl;
        }
    }

    myfile.close();
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

bool checkBranch(node * n, node * branchhead, graph g) {

    if (branchhead->visited) {
        return false;
    }

    for (int num : n->nums) {
        // (base case kinda)
        if (binary_search(branchhead->nums.begin(), branchhead->nums.end(), num) == false) {
            return false;
        }
    }

    /*
        Case 1: head has no children (base case kinda)
        - add head to the out list of the node we are adding
        - add node to the in list of the head
    */
    if (branchhead->in.empty()) {
        n->out.push_back(branchhead);
        branchhead->in.push_back(n);
        return true;
    }
    else {
        int counter = 0;
        for(node * child : branchhead->in) {
            // cout << "Recurse on node: ";
            // printnode(child);
            iter++;
            //cout << iter << endl;
            if (iter > 1000000000) {
                cout << "Failed:" << endl;
                cout << "adding node: ";
                printnode(n);
                cout << "\nrecursing on node: ";
                printnode(branchhead);
                cout << "\n\nCurrent Graph:" << endl;
                printgraphheadnodes(g);
                exit(0);
            }

            //check to see if it is recursing on itself
            if (n == child) {
                return false;
            }
            branchhead->visited = true;
            counter += checkBranch(n, child, g);
        }

        /*
            Case 2: head has no children containing the numbers
            - add head to the out list of the node we are adding
            - add node to the in list of the head
        */
        if (counter == 0) {
            n->out.push_back(branchhead);
            branchhead->in.push_back(n);
            return true;
        }
        /* 
            Case 3: when at least one child has the numbers
            - this case is accounted for when we do the recursive call
        */
    }

    return true;
}

void setGraphToUnvisited(graph g) {
    for(node * n : g.nodes) {
        n->visited = 0;
    }
}