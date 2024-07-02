
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"
#include <cmath>
#include <iostream>

using namespace std;

// Node constructor, given.
quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
    : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}

// quadtree destructor, given.
quadtree::~quadtree() {
    clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree& other) {
    copy(other);
}
// quadtree assignment operator, given.
quadtree& quadtree::operator=(const quadtree& rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}
/**
     * Constructor that builds a quadtree out of the given PNG.
     * Every leaf in the tree corresponds to a pixel in the PNG.
     * Every non-leaf node corresponds to a 2^k x 2^k square of pixels
     * in the original PNG, represented by an (x,y) pair for the
     * upper left corner of the square and an integer dimension, k.
     * In addition, the Node stores a pixel representing the average
     * color over the square. It also stores a measure of color
     * variability over the square.
     *
     * Every node's children correspond to a partition
     * of the node's square into four smaller squares. The
     * current node's square is split in half horizontally
     * and vertically to produce the 4 children.
     *
     * This function will also build the stats object used to compute
     * average pixel color and variability, over the squares.
     */
quadtree::quadtree(PNG& imIn) {
    int dim = log2(min(imIn.width(), imIn.height()));
    edge = pow(2,dim);
    imIn.resize(edge, edge);
    stats s(imIn);
    pair<int, int> ul(0,0);
    root = buildTree(s, ul, dim);
}

quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {
    if (dim == 0){
        return new Node(ul, dim, s.getAvg(ul, dim), s.getVar(ul, dim));
    }

    Node* root = new Node(ul, dim, s.getAvg(ul, dim), s.getVar(ul, dim));

    root->NW = buildTree(s, ul, dim-1 );
    pair<int, int> ul_SW(ul.first, ul.second+ pow(2,dim)/2);
    root->SW = buildTree(s, ul_SW, dim-1);
    pair<int, int> ul_NE(ul.first + pow(2,dim)/2, ul.second);
    root->NE = buildTree(s, ul_NE, dim-1 );
    pair<int, int> ul_SE(ul.first + pow(2,dim)/2, ul.second + pow(2,dim)/2);
    root->SE = buildTree(s, ul_SE, dim-1 );

    return root;
    
}
bool quadtree::isLeaf(Node* node) const {
    if (node->NW == nullptr && node->SW == nullptr && node->SE == nullptr && node->NE == nullptr) return true;
    else return false;
}

void quadtree::Drawleaves(PNG& output, Node* node) const {
    if(isLeaf(node)){
        pair<int, int> ul = node->upLeft;
        for (int x = ul.first; x < (ul.first + pow(2,node->dim)); x++){
            for (int y = ul.second; y < (ul.second + pow(2,node->dim)); y++){
                //std::cout<<x<<","<<y<<std::endl;
                RGBAPixel* pixel = output.getPixel(x,y);
                *pixel = node->avg; 
            }
        }
    } else {
        Drawleaves(output, node->NW);
        Drawleaves(output, node->SW);
        Drawleaves(output, node->SE);
        Drawleaves(output, node->NE);
    }
}

PNG quadtree::render() const {
    PNG output(edge, edge);
    Drawleaves(output, root); 
    return output;
}


int quadtree::idealPrune(const int leaves) const {
    int high = 1;
    while (pruneSize(high) > leaves) high = 2*high;
    int low = high/2;
    int mid;

    while (low < high) {
        mid = (low + high)/2;
        int numleaves = pruneSize(mid);
        if (numleaves > leaves) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low; 
}

int quadtree::pruneSize(const int tol) const {
    return pruneSize(root, tol);
}

int quadtree::pruneSize(Node* node, const int tol) const {
    if (node == nullptr) return 0;
    if (prunable(node, tol) || isLeaf(node)) return 1;
    return (pruneSize(node->NW, tol) + pruneSize(node->NE, tol) + pruneSize(node->SW, tol) + pruneSize(node->SE, tol));
}

/**
     *  Prune function trims subtrees as high as possible in the tree.
     *  A subtree is pruned (cleared) if prunable returns true.
     *  Pruning criteria should be evaluated on the original tree, not
     *  on a pruned subtree. (we only expect that trees would be pruned once.)
     *
     * You may want a recursive helper function for this one.
     */
void quadtree::prune(const int tol) {
    prune(root, tol);
}

void quadtree::prune(Node* node, const int tol) {
    if (node == nullptr) return; 
    else if (prunable(node, tol)) {
        clear(node->NW);
        clear(node->NE);
        clear(node->SE);
        clear(node->SW);
        node->NW = nullptr;
        node->SW = nullptr;
        node->NE = nullptr;
        node->SE = nullptr;
    } else {
        prune(node->NW, tol);
        prune(node->SW, tol);
        prune(node->NE, tol);
        prune(node->SE, tol);
    }
}

void quadtree::clear() {
    clear(root);
    root = nullptr;
    edge = 0;
}

void quadtree::clear(Node* & curr) {
    if (curr == nullptr) {
        return;
    }
    clear(curr->NW);
    clear(curr->NE);
    clear(curr->SE);
    clear(curr->SW);
    delete curr;
    curr = nullptr;
}

void quadtree::copy(const quadtree& orig) {
    clear();

    if (orig.root != nullptr) {
        root = new Node(orig.root->upLeft, orig.root->dim, orig.root->avg, orig.root->var);
    }
    edge = orig.edge;

    treecopy(root, orig.root);
}

void quadtree::treecopy(Node* copyNode, Node* origNode){
    if (origNode == nullptr){
        return;
    } 

    if (origNode->NW != nullptr){
        copyNode->NW = new Node(origNode->NW->upLeft, origNode->NW->dim, origNode->NW->avg, origNode->NW->var);
        treecopy(copyNode->NW, origNode->NW);
    }
    if (origNode->SW != nullptr){
        copyNode->SW = new Node(origNode->SW->upLeft, origNode->SW->dim, origNode->SW->avg, origNode->SW->var);
        treecopy(copyNode->SW, origNode->SW);
    }
    if (origNode->NE != nullptr){
        copyNode->NE = new Node(origNode->NE->upLeft, origNode->NE->dim, origNode->NE->avg, origNode->NE->var);
        treecopy(copyNode->NE, origNode->NE);
    }
    if (origNode->SE != nullptr){
        copyNode->SE = new Node(origNode->SE->upLeft, origNode->SE->dim, origNode->SE->avg, origNode->SE->var);
        treecopy(copyNode->SE, origNode->SE);
    }
}
