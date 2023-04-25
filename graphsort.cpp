#include "graphsort.h"

int iter;

bool compareByNodeSize(node *a, node *b) {
    return a->size > b->size;
}

void printnode(node * n) {
    for (int num : n->nums) {
        cout << num << ",";
    }
    cout << endl;
}

void printnodes(graph * g) {
    for (node *cur_n : g->nodes) {
        for (int num : cur_n->nums) {
            cout << num << ",";
        }
        cout << endl;
    }
}


void printleafs(graph * g) {
    cout << "----LEAVES----" << endl;
    for (node *cur_n : g->leafs) {
        if (cur_n != nullptr) {
            for (int num : cur_n->nums) {
                cout << num << ",";
            }
            cout << endl;
        }
    }
}

void printgraph(graph * g) {
    for (node *cur_n : g->nodes) {
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

void saveGraphToFile(graph * g, string file) {

    ofstream myfile;
    myfile.open(file);

    for (node *cur_n : g->nodes) {
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

void printgraphheadnodes(graph * g) {
    for (tree *cur_t : g->trees) {
        cout << "Head Node: ";
        for (int num : cur_t->head->nums) {
            cout << num << ",";
        }
        cout << endl;
    }
}

bool checkNodeMatch(node * n1, node * n2) {
    iter++;
    bool match = true;
    for (int num : n1->nums) {
        // check if they match
        if (binary_search(n2->nums.begin(), n2->nums.end(), num) == false) {
            match = false;
            break;
        }
    }

    return match;
}

void setParentsVisisted(node * n) {
    for (node * parent : n->out) {
        parent->visited = true;
    }
}

void checkBranchBottomUp(node * n, node * leaf, graph * g) {

    if (leaf->visited) {
        return;
    }
    leaf->visited = true;

    bool match = true;

    iter++;
    for (int num : n->nums) {
        // check if they match
        if (binary_search(leaf->nums.begin(), leaf->nums.end(), num) == false) {
            match = false;
            break;
        }
    }
    //if they match then connect the nodes and return
    if (match = true) {
        n->out.push_back(leaf);
        leaf->in.push_back(n);
    }
    //they don't match
    else {
        //recurse through each parent node
        for (node * parent : leaf->out) {
            checkBranchBottomUp(n, parent, g);
        }
    }
}

void setGraphToUnvisited(graph * g) {
    for(node * n : g->nodes) {
        n->visited = 0;
    }
}

void buildGraphBottomUp(graph * initg, graph * g) {
    int numnodes = initg->nodes.size();
    int nodecount = 1;

    //loop through and add each node to the new graph according to the following algorithm
    cout << "Building Graph..." << endl;
    for (node * n : initg->nodes) {
        cout << "Node " << nodecount << "/" << numnodes << endl;
        nodecount++;
        //cout << "Headnodes: " << g.trees.size() << endl;

        //set graph to unvisited
        setGraphToUnvisited(g);

        //add node to graph
        g->nodes.push_back(n);

        // Go through each tree in the graph, and then through each leaf
        for (tree * t : g->trees) {

            int leafindex = 0;
            for (node * leaf : t->leafs) {
                if (n != leaf) {
                    bool match = true;
                    iter++;
                    for (int num : n->nums) {
                        // check if node is in leaf
                        if (binary_search(leaf->nums.begin(), leaf->nums.end(), num) == false) {
                            match = false;
                            break;
                        }
                    }
                    if (match = true) {
                        //connect them
                        n->out.push_back(leaf);
                        leaf->in.push_back(n);
                        t->leafs[leafindex] = n;
                    }
                    else {
                        checkBranchBottomUp(n, leaf, g);
                    }
                }
                leafindex++;
            }
        }

        //check if the node that we added made any connections: if it didn't then add it to the trees of the graph
        if(n->out.empty()) {
            tree * t = new tree;
            t->head = n;
            t->leafs.push_back(n);
            g->trees.push_back(t);
        }
        //cout << "Node added Successfully" << endl;
    }
}

bool checkBranchTopDown(node * n, node * branchhead, graph * g, tree * t) {

    if (branchhead->visited) {
        return false;
    }

    iter++;
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
        cout << "Connection made" << endl;
        n->out.push_back(branchhead);
        branchhead->in.push_back(n);

        //add node as leaf
        t->leafs.push_back(n);

        return true;
    }
    else {
        int counter = 0;
        for(node * child : branchhead->in) {
            // cout << "Recurse on node: ";
            // printnode(child);
            iter++;
            //cout << iter << endl;
            // if (iter > 1000000000) {
            //     cout << "Failed:" << endl;
            //     cout << "adding node: ";
            //     printnode(n);
            //     cout << "\nrecursing on node: ";
            //     printnode(branchhead);
            //     cout << "\n\nCurrent Graph:" << endl;
            //     printgraphheadnodes(g);
            //     exit(0);
            // }

            //check to see if it is recursing on itself
            if (n == child) {
                return false;
            }
            branchhead->visited = true;
            counter += checkBranchTopDown(n, child, g, t);
        }

        /*
            Case 2: head has no children containing the numbers
            - add head to the out list of the node we are adding
            - add node to the in list of the head
        */
        if (counter == 0) {
            n->out.push_back(branchhead);
            branchhead->in.push_back(n);

            //add node as leaf
            t->leafs.push_back(n);
            //remove previous leaf

            return true;
        }
        /* 
            Case 3: when at least one child has the numbers
            - this case is accounted for when we do the recursive call
        */
    }

    return true;
}

void buildGraphTopDown(graph * initg, graph * g) {
    int numnodes = initg->nodes.size();
    int nodecount = 1;

    //loop through and add each node to the new graph according to the following algorithm
    cout << "Building Graph..." << endl;
    for (node * n : initg->nodes) {
        cout << "Node " << nodecount << "/" << numnodes << endl;
        nodecount++;
        //cout << "Headnodes: " << g.trees.size() << endl;

        //set graph to unvisited
        setGraphToUnvisited(g);

        //add node to graph
        g->nodes.push_back(n);

        // Go through each tree in the graph, and then through each leaf
        for (tree * t : g->trees) {

            bool changedAllLeafs = true;
            int leafindex = 0;
            for (node * leaf : t->leafs) {
                cout << "Leaf: ";
                printnode(leaf);
                if (n != leaf) {
                    bool match = true;
                    iter++;
                    for (int num : n->nums) {
                        // check if node is in leaf
                        if (binary_search(leaf->nums.begin(), leaf->nums.end(), num) == false) {
                            match = false;
                            break;
                        }
                    }
                    if (match == true) {
                        //connect them
                        n->out.push_back(leaf);
                        leaf->in.push_back(n);
                        t->leafs[leafindex] = n;
                    }
                    //if a leaf is not a match then break
                    else {
                        changedAllLeafs = false;
                        break;
                    }
                }
                leafindex++;
            }

            //if node wasn't connect to all leafs, then start from head of tree and work downward
            if (changedAllLeafs == false) {
                cout << "here" << endl;
                checkBranchTopDown(n, t->head, g, t);
            }
        }

        //check if the node that we added made any connections: if it didn't then add it to the trees of the graph
        if(n->out.empty()) {
            tree * t = new tree;
            t->head = n;
            t->leafs.push_back(n);
            g->trees.push_back(t);
        }
    }
}

void buildGraphFromLeaves(graph * initg, graph * g) {
    int numnodes = initg->nodes.size();
    int nodecount = 1;

    //loop through and add each node to the new graph according to the following algorithm
    cout << "Building Graph..." << endl;
    for (node * n : initg->nodes) {
        cout << "Node " << nodecount << "/" << numnodes << endl;
        nodecount++;

        setGraphToUnvisited(g);

        //add node to graph
        g->nodes.push_back(n);

        //add node starting from each leaf
        int leafindex = 0;
        bool isleaf = false;
        for (node * leaf : g->leafs) {
            if (leaf != nullptr) {
                leaf->visited = true;
                //they match
                if (checkNodeMatch(n, leaf)) {
                    if (!isleaf) {
                        n->out.push_back(leaf);
                        leaf->in.push_back(n);
                        g->leafs[leafindex] = n;
                        setParentsVisisted(leaf);
                        isleaf = true;
                    }
                    else {
                        n->out.push_back(leaf);
                        leaf->in.push_back(n);
                        g->leafs[leafindex] = nullptr;
                        setParentsVisisted(leaf);
                    }
                }
                //don't match
                else {
                    //check parents for match
                    checkParents(g, n, leaf);
                }
            }
            leafindex++;
        }
        //check if the node that we added made any connections: if it didn't then add it to the trees of the graph
        if(!isleaf) {
            g->leafs.push_back(n);
        }
    }
}

void checkParents(graph * g, node * n, node * child) {
    if (n == child) {
        return;
    }
    child->visited = true;
    for (node * parent : child->out) {
        //if already visited then return
        if (parent->visited == true) {
            return;
        }
        //set parent to visited
        parent->visited = true;
        //if match
        if(checkNodeMatch(n, parent)) {
            //check if node->out to see if matched node is before it
            for (node * temp : n->out) {
                if (checkNodeMatch(parent, temp)) {
                    //remove temp node connection
                    remove(temp->in.begin(),temp->in.end(),n);
                    remove(n->out.begin(),n->out.end(),temp);
                }
            }

            n->out.push_back(parent);
            parent->in.push_back(n);
            setParentsVisisted(parent);
        }
        //not a match, recurse on parent
        else {
            checkParents(g, n, parent);
        }
    }
}



