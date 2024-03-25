#include "list.h"
#include "sort_impl.h"

static inline void merge(void *priv,
                         list_cmp_func_t cmp,
                         struct list_head *a,
                         struct list_head *b)
{
    LIST_HEAD(res);
    for (;;) {
        if (list_empty(a)) {
            list_splice_tail(b, &res);
            break;
        }
        if (list_empty(b)) {
            list_splice_tail_init(a, &res);
            break;
        }
        struct list_head *tmp =
            cmp(priv, a->next, b->next) <= 0 ? a->next : b->next;
        list_del(tmp);
        list_add_tail(tmp, &res);
    }
    list_splice_tail(&res, a);
}

void mergesort(void *priv, struct list_head *head, list_cmp_func_t cmp)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *fast = head->next, *slow = head;
    for (; fast != head && fast->next != head; fast = fast->next->next)
        slow = slow->next;

    LIST_HEAD(left);
    list_cut_position(&left, head, slow);

    mergesort(priv, &left, cmp);
    mergesort(priv, head, cmp);

    merge(priv, cmp, head, &left);
}