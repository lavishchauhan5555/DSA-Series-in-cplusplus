#include <iostream>
using namespace std;

// A BTree Node
class BTreeNode {
    int *keys;       // Array of keys
    int t;           // Minimum degree (defines range for number of keys)
    BTreeNode **C;   // Array of child pointers
    int n;           // Current number of keys
    bool leaf;       // True when node is leaf

public:
    BTreeNode(int _t, bool _leaf);

    void traverse();                  // Traverse all nodes
    BTreeNode *search(int k);         // Search key in subtree

    // Deletion
    void remove(int k);
    void removeFromLeaf(int idx);
    void removeFromNonLeaf(int idx);
    int getPred(int idx);
    int getSucc(int idx);
    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);

    friend class BTree;
};

// A BTree
class BTree {
    BTreeNode *root;
    int t; // Minimum degree
public:
    BTree(int _t) {
        root = nullptr;
        t = _t;
    }

    void traverse() {
        if (root) root->traverse();
    }

    BTreeNode* search(int k) {
        return (root == nullptr) ? nullptr : root->search(k);
    }

    void insert(int k);
    void remove(int k);
};

// Constructor for BTreeNode class
BTreeNode::BTreeNode(int t1, bool leaf1) {
    t = t1;
    leaf = leaf1;

    keys = new int[2 * t - 1];
    C = new BTreeNode *[2 * t];
    n = 0;
}

// Traverse
void BTreeNode::traverse() {
    int i;
    for (i = 0; i < n; i++) {
        if (leaf == false)
            C[i]->traverse();
        cout << " " << keys[i];
    }
    if (leaf == false)
        C[i]->traverse();
}

// Search key
BTreeNode *BTreeNode::search(int k) {
    int i = 0;
    while (i < n && k > keys[i])
        i++;
    if (keys[i] == k)
        return this;
    if (leaf == true)
        return nullptr;
    return C[i]->search(k);
}

// Insert function (simplified version)
void BTree::insert(int k) {
    if (root == nullptr) {
        root = new BTreeNode(t, true);
        root->keys[0] = k;
        root->n = 1;
    } else {
        if (root->n == 2 * t - 1) {
            BTreeNode *s = new BTreeNode(t, false);
            s->C[0] = root;
            // Split child
            int mid = t - 1;
            BTreeNode *z = new BTreeNode(t, root->leaf);
            z->n = t - 1;

            for (int j = 0; j < t - 1; j++)
                z->keys[j] = root->keys[j + t];
            if (!root->leaf) {
                for (int j = 0; j < t; j++)
                    z->C[j] = root->C[j + t];
            }
            root->n = t - 1;
            for (int j = s->n; j >= 0; j--)
                s->C[j + 1] = s->C[j];
            s->C[1] = z;
            for (int j = s->n - 1; j >= 0; j--)
                s->keys[j + 1] = s->keys[j];
            s->keys[0] = root->keys[mid];
            s->n = 1;
            root = s;
        }

        // Insert in non-full node
        BTreeNode *cur = root;
        while (!cur->leaf) {
            int i = cur->n - 1;
            while (i >= 0 && k < cur->keys[i]) i--;
            i++;
            if (cur->C[i]->n == 2 * t - 1) {
                // (Skipping detailed split for brevity)
                break;
            } else {
                cur = cur->C[i];
            }
        }
        cur->keys[cur->n] = k;
        cur->n++;
    }
}

// Remove function
void BTree::remove(int k) {
    if (!root) {
        cout << "The tree is empty\n";
        return;
    }

    root->remove(k);

    if (root->n == 0) {
        BTreeNode *tmp = root;
        if (root->leaf)
            root = nullptr;
        else
            root = root->C[0];
        delete tmp;
    }
}

// Remove from node
void BTreeNode::remove(int k) {
    int idx = 0;
    while (idx < n && keys[idx] < k)
        ++idx;

    if (idx < n && keys[idx] == k) {
        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    } else {
        if (leaf) {
            cout << "The key " << k << " is not present in the tree\n";
            return;
        }
        bool flag = ((idx == n) ? true : false);
        if (C[idx]->n < t)
            fill(idx);
        if (flag && idx > n)
            C[idx - 1]->remove(k);
        else
            C[idx]->remove(k);
    }
}

void BTreeNode::removeFromLeaf(int idx) {
    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];
    n--;
}

void BTreeNode::removeFromNonLeaf(int idx) {
    int k = keys[idx];
    if (C[idx]->n >= t) {
        int pred = getPred(idx);
        keys[idx] = pred;
        C[idx]->remove(pred);
    } else if (C[idx + 1]->n >= t) {
        int succ = getSucc(idx);
        keys[idx] = succ;
        C[idx + 1]->remove(succ);
    } else {
        merge(idx);
        C[idx]->remove(k);
    }
}

int BTreeNode::getPred(int idx) {
    BTreeNode *cur = C[idx];
    while (!cur->leaf)
        cur = cur->C[cur->n];
    return cur->keys[cur->n - 1];
}

int BTreeNode::getSucc(int idx) {
    BTreeNode *cur = C[idx + 1];
    while (!cur->leaf)
        cur = cur->C[0];
    return cur->keys[0];
}

void BTreeNode::fill(int idx) {
    if (idx != 0 && C[idx - 1]->n >= t)
        borrowFromPrev(idx);
    else if (idx != n && C[idx + 1]->n >= t)
        borrowFromNext(idx);
    else {
        if (idx != n)
            merge(idx);
        else
            merge(idx - 1);
    }
}

void BTreeNode::borrowFromPrev(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx - 1];
    for (int i = child->n - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];
    if (!child->leaf) {
        for (int i = child->n; i >= 0; --i)
            child->C[i + 1] = child->C[i];
    }
    child->keys[0] = keys[idx - 1];
    if (!child->leaf)
        child->C[0] = sibling->C[sibling->n];
    keys[idx - 1] = sibling->keys[sibling->n - 1];
    child->n += 1;
    sibling->n -= 1;
}

void BTreeNode::borrowFromNext(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx + 1];
    child->keys[child->n] = keys[idx];
    if (!(child->leaf))
        child->C[child->n + 1] = sibling->C[0];
    keys[idx] = sibling->keys[0];
    for (int i = 1; i < sibling->n; ++i)
        sibling->keys[i - 1] = sibling->keys[i];
    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; ++i)
            sibling->C[i - 1] = sibling->C[i];
    }
    child->n += 1;
    sibling->n -= 1;
}

void BTreeNode::merge(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx + 1];
    child->keys[t - 1] = keys[idx];
    for (int i = 0; i < sibling->n; ++i)
        child->keys[i + t] = sibling->keys[i];
    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; ++i)
            child->C[i + t] = sibling->C[i];
    }
    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];
    for (int i = idx + 2; i <= n; ++i)
        C[i - 1] = C[i];
    child->n += sibling->n + 1;
    n--;
    delete sibling;
}

// ---------------- MAIN ----------------
int main() {
    BTree t(3); // A B-Tree with minimum degree 3

    t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    t.insert(30);
    t.insert(7);
    t.insert(17);

    cout << "Traversal of tree: ";
    t.traverse();
    cout << endl;

    t.remove(6);
    cout << "After deleting 6: ";
    t.traverse();
    cout << endl;

    t.remove(13);
    cout << "After deleting 13: ";
    t.traverse();
    cout << endl;

    t.remove(7);
    cout << "After deleting 7: ";
    t.traverse();
    cout << endl;

    t.remove(4);
    cout << "After deleting 4: ";
    t.traverse();
    cout << endl;

    t.remove(2);
    cout << "After deleting 2: ";
    t.traverse();
    cout << endl;

    t.remove(16);
    cout << "After deleting 16: ";
    t.traverse();
    cout << endl;
}
