/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        if (!head) return head;
        int n = 0;
        ListNode* p = head;
        while (p) {
            n++;
            p = p->next;
        }

        k = k % n;
        if (!k) return head;

        ListNode* first = head;
        while (k-- && first) first = first->next;
        ListNode* second = head;
        while (first->next) {
            second = second->next;
            first = first->next;
        }

        first->next = head;
        head = second->next;
        second->next = nullptr;
        return head;
    }
};