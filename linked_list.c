#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct node {
    char data;
    struct node *prev, *next;
} node;

node* new_node() {return (node*)malloc(sizeof(node));}

// create new linked list and return head node of it
node* new_list() {
    node *head = new_node();
    head->data = 0;
    head->prev = head;
    head->next = head;
    return head;
}

node* front(node *head) {return head->next;}
node* back(node *head) {return head->prev;}

void push_front(node *head, char val) {
    node *v = new_node();
    v->data = val;
    head->next->prev = v;
    v->next = head->next;
    v->prev = head;
    head->next = v;
}
void push_back(node *head, char val) {
    node *v = new_node();
    v->data = val;
    head->prev->next = v;
    v->prev = head->prev;
    v->next = head;
    head->prev = v;
}

bool empty(node *head) {return head->next==head;}
size_t size(node *head) {
    size_t ret = 0;
    for (node *it=head->next; it!=head; it=it->next) ++ret;
    return ret;
}

void pop_front(node *head) {
    if (empty(head)) return;
    node *to_del = head->next;
    head->next = head->next->next;
    head->next->prev = head;
    free(to_del);
}
void pop_back(node *head) {
    if (empty(head)) return;
    node *to_del = head->prev;
    head->prev = head->prev->prev;
    head->prev->next = head;
    free(to_del);
}

void insert_next(node *p, char val) {
    node *v = new_node();
    v->data = val;
    v->prev = p;
    v->next = p->next;
    p->next = v;
    v->next->prev = v;
}
void delete_node(node *p) {
    if (p==p->next) return;
    p->prev->next = p->next;
    p->next->prev = p->prev;
    free(p);
}

void clear(node *head) {
    node *cur, *nxt;
    cur = head->next;
    while (cur != head) {
        nxt = cur->next;
        free(cur);
        cur = nxt;
    }
    head->prev = head->next = head;
}
void free_list(node *head) {clear(head); free(head);}

// !warning! must store it in variable before using it. don't pass directly as fuction arguments.
node* copy(node *head) {
    node *ret = new_list();
    for (node *it=front(head); it!=head; it=it->next) push_back(ret, it->data);
    return ret;
}
void reverse(node *head) {
    node *cur, *nxt;
    cur = head->next;
    while (cur != head) {
        nxt = cur->next;
        cur->next = cur->prev;
        cur->prev = nxt;
        cur = nxt;
    }
    nxt = head->next;
    head->next = head->prev;
    head->prev = nxt;
}

// TESTCODE
/*
int main() {
    node *a = new_list();
    push_back(a, 'S');
    push_back(a, 'A');
    push_back(a, 'N');
    push_front(a, 'S');
    pop_back(a);

    insert_next(a->next->next, 'X');
    delete_node(a->next->next);

    for (node *it=a->next; it!=a; it=it->next) {
        putchar(it->data);
    }
    putchar('\n');
    clear(a);
    push_front(a, '8');
    push_back(a, '9');
    push_front(a, '7');
    reverse(a);
    for (node *it=a->prev; it!=a; it=it->prev) {
        putchar(it->data);
    }

    free_list(a);
}
*/