#include "graphsort.h"

using namespace std;




int main(int argc, char* argv[]) {

    //read input file
    if (argc != 3) {
        cout << "Usage: <inputfilename.txt> <outputfilename.txt>" << endl;
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

    int numnodes = initg.nodes.size();
    int nodecount = 1;

    //loop through and add each node to the new graph according to the following algorithm
    cout << "Building Graph..." << endl;
    for (node * n : initg.nodes) {
        cout << "Node " << nodecount << "/" << numnodes << endl;
        nodecount++;
        //cout << "Headnodes: " << g.trees.size() << endl;

        //set graph to unvisited
        setGraphToUnvisited(g);

        //add node to graph
        g.nodes.push_back(n);

        // Go through each tree in the graph, and then through each leaf
        for (tree * t : g.trees) {
            bool changedAllLeafs = true;

            int leafindex = 0;
            for (node * leaf : t->leafs) {
                if (n != leaf) {
                    for (int num : n->nums) {
                        // check if node is in leaf
                        if (binary_search(leaf->nums.begin(), leaf->nums.end(), num) == true) {
                            //connect them
                            n->out.push_back(leaf);
                            leaf->in.push_back(n);
                            t->leafs[leafindex] = n;
                        }
                        else {
                            changedAllLeafs = false;
                            break;
                        }
                    }
                    if (changedAllLeafs == false) {
                        break;
                    }
                }
                leafindex++;
            }

            //if node wasn't connect to any leafs, then start from head of tree and work downward
            if (changedAllLeafs == false) {
                checkBranch(n, t->head, g);
            }
        }
        
        /*
        start at each head node:
            if the nums in n are all in cur

        */

        // for (node * cur : g.headnodes) {
        //     checkBranch(n, cur, g);
        // }

        //check if the node that we added made any connections: if it didn't then add it to the trees of the graph
        if(n->out.empty()) {
            tree * t = new tree;
            t->head = n;
            t->leafs.push_back(n);
            g.trees.push_back(t);
        }
        //cout << "Node added Successfully" << endl;
    }
    

    // printgraph(g); 
    cout << "finished building graph in " << iter << " Node Checks." << endl;
    saveGraphToFile(g, argv[2]);

    return -1;
}