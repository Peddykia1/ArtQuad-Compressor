#include "qtcount.h"

bool qtcount::prunable(Node* node, const int tol) const {
    int avgRed = node->avg.r;
    int avgGreen = node->avg.g;
    int avgBlue = node->avg.b;

  
    return prunableHelper(node, tol, avgRed, avgGreen, avgBlue);

    
}

bool qtcount::prunableHelper(Node* node, const int tol, int avgRed, int avgGreen, int avgBlue) const {
    
    if (isLeaf(node)) {
        int colorDistance = pow(avgRed - node->avg.r, 2) + pow(avgGreen - node->avg.g, 2) + pow(avgBlue - node->avg.b, 2);
        if (colorDistance > tol) {
            return false;
        } else {
            return true;
        }  
    } else {
        return (prunableHelper(node->NW, tol, avgRed, avgGreen, avgBlue) && prunableHelper(node->NE, tol, avgRed, avgGreen, avgBlue) && prunableHelper(node->SW, tol, avgRed, avgGreen, avgBlue) && prunableHelper(node->SE, tol, avgRed, avgGreen, avgBlue));
    }

}