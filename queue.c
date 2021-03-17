#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* Free queue structure */
    if (!q)
        return;

    for (list_ele_t *ptr = q->head; ptr;) {
        list_ele_t *tmp = ptr;
        ptr = ptr->next;
        free(tmp->value);
        free(tmp);
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);
    newh->next = q->head;
    q->head = newh;
    if (q->size == 0)
        q->tail = q->head;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)  // O(1)!
{
    if (!q || q->size == 0)
        return q_insert_head(q, s);
    list_ele_t *ptr = malloc(sizeof(list_ele_t));
    if (!ptr)
        return false;

    char *ss = malloc((strlen(s) + 1) * sizeof(char));
    if (!ss) {
        free(ptr);
        return false;
    }
    ptr->next = NULL;
    ptr->value = ss;
    strncpy(ss, s, strlen(s) + 1);
    q->tail->next = ptr;
    q->tail = ptr;
    if (!q->head)
        q->head = ptr;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || q->size == 0)
        return false;
    strncpy(sp, q->head->value, bufsize);
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q)
        return q->size;
    else
        return 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size <= 1)
        return;
    list_ele_t *pre, *current;
    for (pre = q->head, current = pre->next; current;) {
        list_ele_t *tmp = current->next;
        current->next = pre;
        pre = current;
        current = tmp;
    }
    q->head->next = NULL;
    list_ele_t *tmp = q->head;
    q->head = q->tail;
    q->tail = tmp;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

void mergesort(list_ele_t **a, list_ele_t **b, int n)
{
    if (n <= 1)
        return;
    list_ele_t *mid = *a;
    for (int i = 1; i < n / 2; i++)
        mid = mid->next;
    list_ele_t *tmp = mid->next;
    mid->next = NULL;
    (*b)->next = NULL;
    mergesort(a, &mid, n / 2);
    mergesort(&tmp, b, n - n / 2);
    list_ele_t *newhead = NULL, *newtail = NULL, *tmpa = *a, *tmpb = tmp;
    if (strcmp((*a)->value, tmp->value) < 0) {
        newhead = *a;
        tmpa = tmpa->next;
    } else {
        newhead = tmp;
        tmpb = tmpb->next;
    }
    newtail = newhead;
    while (tmpa && tmpb) {
        if (strcmp(tmpa->value, tmpb->value) < 0) {
            newtail->next = tmpa;
            tmpa = tmpa->next;
        } else {
            newtail->next = tmpb;
            tmpb = tmpb->next;
        }
        newtail = newtail->next;
    }
    while (tmpa) {
        newtail->next = tmpa;
        tmpa = tmpa->next;
        newtail = newtail->next;
    }
    while (tmpb) {
        newtail->next = tmpb;
        tmpb = tmpb->next;
        newtail = newtail->next;
    }
    newtail->next = NULL;
    *a = newhead;
    *b = newtail;
}

void q_sort(queue_t *q)
{
    if (!q || q->size <= 1)
        return;
    mergesort(&q->head, &q->tail, q->size);
}
