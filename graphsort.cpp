#include "graphsort.h"

int iter;

bool compareByNodeSize(node *a, node *b)
{
    return a->size > b->size;
}

void printnode(node *n)
{
    for (int num : n->nums)
    {
        cout << num << ",";
    }
}

void printnodes(graph *g)
{
    for (node *cur_n : g->nodes)
    {
        for (int num : cur_n->nums)
        {
            cout << num << ",";
        }
        cout << endl;
    }
}

void printleafs(graph *g)
{
    cout << "----LEAVES----" << endl;
    for (node *cur_n : g->leafs)
    {
        if (cur_n != nullptr)
        {
            for (int num : cur_n->nums)
            {
                cout << num << ",";
            }
            cout << endl;
        }
    }
}

void printgraph(graph *g)
{
    for (node *cur_n : g->nodes)
    {
        for (node *out_n : cur_n->out)
        {
            if (out_n == nullptr)
            {
                continue;
            }

            // print cur node nums
            for (int num : cur_n->nums)
            {
                cout << num << ",";
            }
            cout << "\b->";
            // print out node nums
            for (int num : out_n->nums)
            {
                cout << num << ",";
            }
            cout << "\b \n";
        }
    }
}

void saveGraphToFile(graph *g, string file)
{

    cout << "----Saving Graph to File----" << endl;

    ofstream myfile;
    myfile.open(file);

    for (node *cur_n : g->nodes)
    {
        for (node *out_n : cur_n->out)
        {
            if (out_n == nullptr)
            {
                continue;
            }

            // print cur node nums
            bool isfirst = true;
            for (int num : cur_n->nums)
            {
                if (isfirst)
                {
                    myfile << num;
                    isfirst = false;
                }
                else
                {
                    myfile << "," << num;
                }
            }
            myfile << " -> ";
            // print out node nums
            isfirst = true;
            for (int num : out_n->nums)
            {
                if (isfirst)
                {
                    myfile << num;
                    isfirst = false;
                }
                else
                {
                    myfile << "," << num;
                }
            }
            myfile << "\n";
        }
    }

    myfile.close();
}

void printgraphheadnodes(graph *g)
{
    for (tree *cur_t : g->trees)
    {
        cout << "Head Node: ";
        for (int num : cur_t->head->nums)
        {
            cout << num << ",";
        }
        cout << endl;
    }
}

bool checkNodeMatch(node *n1, node *n2)
{
    if (n1 == nullptr || n2 == nullptr)
    {
        cout << "NULL COMPARISON" << endl;
    }

    iter++;
    bool match = true;
    for (int num : n1->nums)
    {
        // check if they match
        if (binary_search(n2->nums.begin(), n2->nums.end(), num) == false)
        {
            match = false;
            break;
        }
    }

    return match;
}

void setParentsVisisted(node *n)
{
    for (node *parent : n->out)
    {
        if (parent == nullptr)
        {
            continue;
        }
        parent->visited = true;
        setParentsVisisted(parent);
    }
}
void setChildrenVisisted(node *n)
{
    for (node *child : n->in)
    {
        if (child == nullptr)
        {
            continue;
        }
        child->visited = true;
        setParentsVisisted(child);
    }
}

void setGraphToUnvisited(graph *g)
{
    for (node *n : g->nodes)
    {
        n->visited = 0;
    }
}

void buildGraphTopDown(graph *initg, graph *g)
{
    int numnodes = initg->nodes.size();
    int nodecount = 1;

    // loop through and add each node to the new graph according to the following algorithm
    cout << "Building Graph..." << endl;
    for (node *n : initg->nodes)
    {
        cout << "Node " << nodecount << "/" << numnodes << endl;
        nodecount++;
        // cout << "Headnodes: " << g.trees.size() << endl;

        // set graph to unvisited
        setGraphToUnvisited(g);

        // add node to graph
        g->nodes.push_back(n);

        // Go through each tree in the graph
        for (tree *t : g->trees)
        {
            checkBranchTopDown(n, t->head, g, t);
        }

        // check if the node that we added made any connections: if it didn't then add it to the trees of the graph
        if (n->out.empty())
        {
            tree *t = new tree;
            t->head = n;
            g->trees.push_back(t);
        }
    }
}

bool checkBranchTopDown(node *n, node *branchhead, graph *g, tree *t)
{

    bool match = checkNodeMatch(n, branchhead);
    if (branchhead->visited)
    {
        if (match)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    branchhead->visited = true;

    if (match == false)
    {
        return false;
    }

    /*
        Case 1: head has no children (base case kinda)
        - add head to the out list of the node we are adding
        - add node to the in list of the head
    */
    if (branchhead->in.empty())
    {
        n->out.push_back(branchhead);
        branchhead->in.push_back(n);
        return true;
    }
    else
    {
        int counter = 0;
        for (node *child : branchhead->in)
        {
            if (n == child)
            {
                continue;
            }
            branchhead->visited = true;
            counter += checkBranchTopDown(n, child, g, t);
        }

        /*
            Case 2: head has no children containing the numbers
            - add head to the out list of the node we are adding
            - add node to the in list of the head
        */
        if (counter == 0)
        {
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

void buildGraphFromLeaves(graph *initg, graph *g)
{
    int numnodes = initg->nodes.size();
    int nodecount = 1;

    // loop through and add each node to the new graph according to the following algorithm
    cout << "Building Graph..." << endl;
    for (node *n : initg->nodes)
    {
        cout << "Node " << nodecount << "/" << numnodes << endl;
        nodecount++;

        setGraphToUnvisited(g);

        // add node to graph
        g->nodes.push_back(n);

        priority_queue<node *, vector<node *>, MyComparator> q;

        // add all leaves of the graph to the queue
        for (node *leaf : g->leafs)
        {
            q.push(leaf);
        }

        while (!q.empty())
        {
            node *cur = q.top();
            q.pop();

            if (cur->visited == true)
            {
                continue;
            }
            cur->visited = true;

            if (checkNodeMatch(n, cur))
            {
                n->out.push_back(cur);
                setParentsVisisted(cur);
                setChildrenVisisted(cur);

                // set level
                int curlevel = cur->level + 1;
                if (curlevel > n->level)
                {
                    n->level = curlevel;
                }

                // check if we are replacing leaf
                if (cur->isleaf)
                {
                    if (n->isleaf == false)
                    {
                        replace(g->leafs.begin(), g->leafs.end(), cur, n);

                        n->isleaf = true;
                        cur->isleaf = false;
                    }
                    else
                    {
                        remove(g->leafs.begin(), g->leafs.end(), cur);

                        cur->isleaf = false;
                    }
                }
                else
                {
                    if (n->isleaf == false)
                    {
                        g->leafs.push_back(n);
                    }
                }
            }
            else
            {
                for (node *parent : cur->out)
                {
                    if (parent->visited == false)
                    {
                        q.push(parent);
                    }
                }
            }
        }

        // if no connetions made then set as leaf and set level to 0
        if (n->out.empty())
        {
            n->level = 0;
            n->isleaf = true;
            g->leafs.push_back(n);
        }
    }
}

void checkParents(graph *g, node *n, node *child)
{
    if (child == nullptr)
    {
        return;
    }
    if (n == child)
    {
        return;
    }
    child->visited = true;
    for (node *parent : child->out)
    {
        if (parent == nullptr)
        {
            continue;
        }

        // if already visited then return
        if (parent->visited == true)
        {
            continue;
        }

        // set parent to visited
        parent->visited = true;

        // if match
        if (checkNodeMatch(n, parent))
        {
            // check if node->out to see if matched node is before it
            int i = 0;
            for (node *temp : n->out)
            {
                if (temp == nullptr)
                {
                    continue;
                }
                if (checkNodeMatch(parent, temp))
                {
                    // remove temp node connection

                    n->out[i] = nullptr;
                    // n->out.erase(remove(n->out.begin(), n->out.end(),temp), n->out.end());
                }

                i++;
            }

            n->out.push_back(parent);
            setParentsVisisted(parent);
        }
        // not a match, recurse on parents
        else
        {
            checkParents(g, n, parent);
        }
    }
}

bool checkConnectionsCorrectness(graph *g)
{
    bool correct = true;
    for (node *n : g->nodes)
    {
        for (node *n1 : n->out)
        {
            if (n1 == nullptr)
            {
                continue;
            }
            for (node *n2 : n->out)
            {
                if (n2 == nullptr)
                {
                    continue;
                }
                if (n1 != n2 && checkNodeMatch(n1, n2))
                {
                    cout << "----Algorithm Failure----" << endl;
                    // print n1 connection
                    printnode(n);
                    cout << "->";
                    printnode(n1);
                    cout << endl;

                    // print n2 connection
                    printnode(n);
                    cout << "->";
                    printnode(n2);
                    cout << endl;

                    correct = false;
                }
            }
        }
    }
    return correct;
}
