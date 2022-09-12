#include "llrec.h"

//*********************************************
// Provide your implementation of llpivot below
//*********************************************

void llpivot(Node *&head, Node *&smaller, Node *&larger, int pivot)
{
    if (head != NULL)
    {
        Node* tmp = head->next;
        head->next = NULL;
        if (head->val <= pivot)
        {
            smaller = head;
            llpivot(tmp, smaller->next, larger, pivot);
            head = NULL;   
        }
        else {
            larger = head;
            llpivot(tmp, smaller, larger->next, pivot);
            head = NULL;
        }
    }
}
