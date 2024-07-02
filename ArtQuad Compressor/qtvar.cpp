#include "qtvar.h"

bool qtvar::prunable(Node* node, const int tol) const {
   if (node->var < tol){
    return true;
   } else return false;       
}
