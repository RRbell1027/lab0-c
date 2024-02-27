#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/* struct list_head {
 *     struct list_head *prev;
 *     struct list_head *next;
 * };
 */

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    head->next = head;
    head->prev = head;
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;
    /* Free elements, then free head */
    struct list_head *target = head->next;
    for (struct list_head *li = head->next->next; target != head;
         li = li->next) {
        /* Free the element whose list is target */
        element_t *ele =
            (element_t *) ((void *) target - offsetof(element_t, list));
        free(ele->value);
        free(ele);
        target = li;
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t **ele = malloc(sizeof(element_t *));
    if (!ele)
        return false;

    *ele = malloc(sizeof(element_t));
    if (!(*ele)) {
        free(ele);
        return false;
    }

    (*ele)->value = strdup(s);

    list_add(&((*ele)->list), head);
    free(ele);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t **ele = malloc(sizeof(element_t *));
    if (ele == NULL)
        return false;

    *ele = malloc(sizeof(element_t));
    if (!(*ele)) {
        free(ele);
        return false;
    }

    (*ele)->value = strdup(s);

    list_add_tail(&((*ele)->list), head);
    free(ele);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || (head->next) == head)
        return NULL;

    element_t *node =
        (element_t *) ((void *) (head->next) - offsetof(element_t, list));
    if (sp != NULL)
        strncpy(sp, node->value, bufsize);
    list_del(head->next);
    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || (head->next) == head)
        return NULL;

    element_t *node =
        (element_t *) ((void *) (head->prev) - offsetof(element_t, list));
    if (sp != NULL)
        strncpy(sp, node->value, bufsize);
    list_del(head->prev);
    return node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
