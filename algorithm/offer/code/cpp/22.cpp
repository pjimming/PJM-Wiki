/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* getKthFromEnd(ListNode* head, int k) {
        int n = 0;
        ListNode* p = head;
        while (p) {
            n++;
            p = p->next;
        }

        if (k > n) return NULL;

        k = n - k;
        ListNode* q = head;
        while (k-- && q->next) {
            q = q->next;
        }
        return q;
    }
};