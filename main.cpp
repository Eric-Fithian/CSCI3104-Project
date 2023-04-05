#include "graphsort.h"

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

    printnodes(initg);

    graph g;

    //loop through and add each node to the new graph according to the following algorithm
    
    for (node * n : initg.nodes) {
        // cout << "Adding node: ";
        // printnode(n);
        // printgraph(g);
        //add node to graph
        g.nodes.push_back(n);

        /*
        start at each head node:
            if the nums in n are all in cur

        */

        for (node * cur : g.headnodes) {
            checkBranch(n, cur);
        }

        //check if the node that we added made any connections: if it didn't then add it to the headnodes of the graph
        if(n->out.empty()) {
            g.headnodes.push_back(n);
        }
    }
    

    printgraph(g);
    //printgraphheadnodes(g);


    return -1;
}