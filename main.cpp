#include "graphsort.h"

using namespace std;

int main(int argc, char *argv[])
{

    // read input file
    if (argc != 4)
    {
        cout << "Usage: '0/1 -> topdown/bottomup' <inputfilename.txt> <outputfilename.txt>" << endl;
        return -1;
    }
    // create initilization graph
    graph *initg = new graph;
    initg->size = 0;

    // read file into graph
    string inFile = argv[2];
    ifstream inputFile(inFile);

    while (inputFile.good() && inputFile.peek() != EOF)
    {
        string line = "";
        getline(inputFile, line);
        string item = "";
        stringstream ss(line);

        // create node
        node *n = new node;

        while (!ss.eof())
        {
            // put num into item
            getline(ss, item, ' ');
            // add item to node
            n->nums.push_back(stoi(item));
            n->size += 1;
        }

        // add node to init graph
        initg->nodes.push_back(n);
        initg->size += 1;
    }

    // sort the initilization graph file
    sort(initg->nodes.begin(), initg->nodes.end(), compareByNodeSize);

    // printnodes(initg);

    graph *g = new graph;

    iter = 0;

    auto start = high_resolution_clock::now();

    if (stoi(argv[1]) == 0)
    {
        buildGraphTopDown(initg, g);
    }
    else
    {
        buildGraphFromLeaves(initg, g);
    }

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Time to build graph: " << (duration.count() / 1000.) << " seconds" << endl;

    cout << "Average time per node: " << (duration.count() / (double)initg->size) << " milliseconds" << endl;
    ;

    cout << "finished building graph in " << iter << " Node Checks." << endl;
    saveGraphToFile(g, argv[3]);

    cout << "----Checking Graph Correctness----" << endl;
    if (checkConnectionsCorrectness(g))
    {
        cout << "----Graph Correctness Check Complete----" << endl;
    }

    return -1;
}