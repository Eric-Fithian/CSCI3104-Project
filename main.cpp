#include "graphsort.h"
#include <algorithm>

using namespace std;




int main(int argc, char* argv[]) {

    //read input file
    if (argc != 2) {
        cout << "Usage: <inputfilename.txt>" << endl;
        return -1;
    }
    //create initilization graph
    graph initg;

    //read file into graph
    string inFile = argv[1];
    ifstream inputFile(inFile);

    while(inputFile.good() && inputFile.peek() != EOF) {
        string line = "";
        getline(inputFile, line);
        string item = "";
        stringstream ss(line);

        //create node
        node * n = new node;
        
        while (!ss.eof()) {
            //put num into item
            getline(ss, item, ' ');
            //add item to node
            n->nums.push_back(stoi(item));
            n->size += 1;
        }

        //add node to init graph
        initg.nodes.push_back(n);
        initg.size += 1;
    }

    //sort the initilization graph file
    sort(initg.nodes.begin(), initg.nodes.end(), compareByNodeSize);

    int graphsize = initg.size;

    // for (int i=0; i<graphsize; i++) {
    //     cout << initg.nodes[i].size << " ";
    // }
    // cout << endl;

    // for (int i=0; i<graphsize; i++) {
    //     for (int i : initg.nodes[i].nums) {
    //         cout << i << " ";
    //     }
    //     cout << endl;
    // }

    graph g;

    //loop through and add each node to the new graph according to the following algorithm
    
    for (node * n : initg.nodes) {
        //add node to graph
        g.nodes.push_back(n);

        /*
        start at each head node:
            if the nums in n are all in cur

        */

        //check each head branch for a connection
        for (node *cur : g.headnodes) {
            //while the cur node has nodes going into it/
            node *next = nullptr;
            while(!cur->in.empty()) {
                bool curNodeContainsNums = true;

                for (int num : n->nums) {
                    if (binary_search(cur->nums.begin(), cur->nums.end(), num) == false) {
                        curNodeContainsNums = false;
                        break;
                    }
                }

                if (curNodeContainsNums) {
                    next = cur;
                }
            }
            //check if a connection was made for this head branch
            if (next != nullptr) {
                n->out.push_back(next);
            }
        }

        //check if the node that we added made any connections: if it didn't then add it to the headnodes of the graph
        if(n->out.size() == 0) {
            g.headnodes.push_back(n);
        }
    }
    

    printgraph(g);
    printgraphheadnodes(g);


    return -1;
}