#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (head)
        INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    element_t *li, *safe;
    list_for_each_entry_safe (li, safe, head, list) {
        free(li->value);
        free(li);
    }

    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *ele = malloc(sizeof(element_t));
    char *es = strdup(s);
    if (!ele || !es) {
        free(ele);
        free(es);
        return false;
    }
    ele->value = es;

    list_add(&ele->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *ele = malloc(sizeof(element_t));
    char *es = strdup(s);
    if (!ele || !es) {
        free(ele);
        free(es);
        return false;
    }
    ele->value = es;

    list_add_tail(&ele->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *node = list_first_entry(head, element_t, list);

    if (sp != NULL) {
        strncpy(sp, node->value, bufsize - 1);
        sp[bufsize - 1] = 0;
    }
    list_del(head->next);
    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *node = list_last_entry(head, element_t, list);

    if (sp != NULL) {
        strncpy(sp, node->value, bufsize - 1);
        sp[bufsize - 1] = 0;
    }
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
    if (!head || list_empty(head))
        return NULL;

    struct list_head *slow = head->next, *fast = head->next;
    while (fast->next != head && fast->next->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }
    list_del(slow);

    element_t *ele = list_entry(slow, element_t, list);
    free(ele->value);
    free(ele);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head)
        return false;

    struct list_head *li, *safe;
    element_t *e1, *e2;

    for (li = head->next; li != head; li = safe) {
        e1 = list_entry(li, element_t, list);
        bool flag = false;
        while (li->next != head) {
            e2 = list_entry(li->next, element_t, list);
            if (strcmp(e1->value, e2->value) == 0) {
                flag = true;
                list_del(&e2->list);
                free(e2->value);
                free(e2);
            } else {
                break;
            }
        }
        safe = li->next;
        if (flag) {
            list_del(&e1->list);
            free(e1->value);
            free(e1);
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *l1, *l2;
    for (l1 = head->next, l2 = head->next->next; l1 != head && l2 != head;
         l1 = l1->next, l2 = l2->next->next->next) {
        struct list_head *prev = l1->prev, *next = l2->next;
        prev->next = l2;
        l2->prev = prev;
        l2->next = l1;
        l1->prev = l2;
        l1->next = next;
        next->prev = l1;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;


    struct list_head *li = head;

    do {
        struct list_head *tmp = li->next;
        li->next = li->prev;
        li->prev = tmp;
        li = tmp;
    } while (li != head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    LIST_HEAD(rev);

    int cnt = 0;

    struct list_head *li, *safe;
    list_for_each_safe (li, safe, head) {
        cnt++;
        if (cnt == k) {
            cnt = 0;
            LIST_HEAD(batch);
            list_cut_position(&batch, head, li);
            q_reverse(&batch);
            list_splice_tail(&batch, &rev);
        }
    }
    q_reverse(head);
    list_splice(&rev, head);
}

void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *fast = head->next, *slow = head;
    for (; fast != head && fast->next != head; fast = fast->next->next)
        slow = slow->next;

    LIST_HEAD(left);
    list_cut_position(&left, head, slow);

    q_sort(&left, descend);
    q_sort(head, descend);

    queue_contex_t left_queue = {.q = &left};
    queue_contex_t right_queue = {.q = head};

    LIST_HEAD(q_head);
    list_add_tail(&right_queue.chain, &q_head);
    list_add_tail(&left_queue.chain, &q_head);

    q_merge(&q_head, descend);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return q_size(head);

    for (struct list_head *li = head->prev->prev, *safe = li->prev; li != head;
         li = safe, safe = li->prev) {
        element_t *cur = list_entry(li, element_t, list),
                  *pre = list_entry(li->next, element_t, list);
        if (strcmp(cur->value, pre->value) > 0) {
            list_del(&cur->list);
            free(cur->value);
            free(cur);
        }
    }
    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return q_size(head);

    for (struct list_head *li = head->prev->prev, *safe = li->prev; li != head;
         li = safe, safe = li->prev) {
        element_t *cur = list_entry(li, element_t, list),
                  *pre = list_entry(li->next, element_t, list);
        if (strcmp(cur->value, pre->value) < 0) {
            list_del(&cur->list);
            free(cur->value);
            free(cur);
        }
    }
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    if (!head || list_empty(head))
        return 0;

    int cnt = 0;

    LIST_HEAD(sorted_list);

    struct list_head *qi;

    for (;;) {
        struct list_head *target = 0;
        list_for_each (qi, head) {
            queue_contex_t *ctx = list_entry(qi, queue_contex_t, chain);
            if (list_empty(ctx->q))
                continue;
            if (!target)
                target = ctx->q->next;
            else {
                int cmp =
                    strcmp(list_entry(ctx->q->next, element_t, list)->value,
                           list_entry(target, element_t, list)->value);
                if (descend ^ (cmp < 0))
                    target = ctx->q->next;
            }
        }
        if (!target)
            break;

        list_del(target);
        list_add_tail(target, &sorted_list);
        cnt++;
    }
    list_splice(&sorted_list, list_first_entry(head, queue_contex_t, chain)->q);
    return cnt;
}

void q_shuffle(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    int size = q_size(head);
    struct list_head *old;
    for (int i = size; i > 1; i--) {
        old = head;
        int r = rand() / (RAND_MAX / (i - 1) + 1) + 1;
        while (r > 0) {
            old = old->next;
            r--;
        }
        struct list_head *tmp = old->prev;
        list_del(old);
        list_move(head->prev, tmp);
        list_add_tail(old, head);
    }
}