#include "2-3Tree.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int find(struct node *node, int k)
{
    for (int i = 0; i < node->size; ++i)    
        if (node->key[i] == k) return 1;
    return 0;
}

void swap(int *x, int *y) 
{
    int r = *x;
    *x = *y;
    *y = r;
}

void sort(struct node *node)
{
    if (node->size == 1) return;
    if (node->size == 2) {
        if (node->key[0] > node->key[1]) swap(&node->key[0], &node->key[1]);
    }
    if (node->size == 3){
        if (node->key[0] > node->key[1]) swap(&node->key[0], &node->key[1]);
        if (node->key[0] > node->key[2]) swap(&node->key[0], &node->key[2]);
        if (node->key[1] > node->key[2]) swap(&node->key[1], &node->key[2]);
    }
}

void insert_to_node(struct node *node, int k)
{
    node->key[node->size] = k;
    node->size++;
    sort(node);
}

void remove_from_node(struct node *node, int k)
{
    if (node->size >= 1 && node->key[0] == k) {
        node->key[0] = node->key[1];
        node->key[1] = node->key[2];
        node->size--;
    } else if (node->size == 2 && node->key[1] == k) {
        node->key[1] = node->key[2];
        node->size--;
    }
}

void become_node2(struct node *node, int k, struct node *first_, struct node *second_)
{
    node->key[0] = k;
    node->key[1] = 0;
    node->key[2] = 0;
    node->first = first_;
    node->second = second_;
    node->third = NULL;
    node->fourth = NULL;
    node->parent = NULL;
    node->size = 1;
}

int is_leaf(struct node *node)
{
    return (node->first == NULL) && (node->second == NULL) && (node->third == NULL);
}

struct node *insert(struct node *root, int k)
{ 
    if(!root){
        root = malloc(sizeof(struct node));
        root->size = 1;
        root->key[0] = k;
        root->first = NULL;
        root->second = NULL;
        root->third = NULL;
        root->fourth = NULL;
        root->parent = NULL;

        return root;
    }

    if (is_leaf(root)) insert_to_node(root, k);
    else if (k <= root->key[0]) insert(root->first, k);
    else if ((root->size == 1) || ((root->size == 2) && k <= root->key[1])) insert(root->second, k);
    else insert(root->third, k);

    return split(root);
}

struct node *split(struct node *item) 
{
    if (item->size < 3) return item; 

    struct node *x = malloc(sizeof(struct node));
    x->size = 1;
    x->key[0] = item->key[0];
    x->first = item->first;
    x->second = item->second;
    x->third = NULL;
    x->fourth = NULL;
    x->parent = item->parent;

    struct node *y = malloc(sizeof(struct node));
    y->size = 1;
    y->key[0] = item->key[2];
    y->first = item->third;
    y->second = item->fourth;
    y->third = NULL;
    y->fourth = NULL;
    y->parent = item->parent;

    if (x->first)  x->first->parent = x;    
    if (x->second) x->second->parent = x;   
    if (y->first)  y->first->parent = y;    
    if (y->second) y->second->parent = y;

    if (item->parent) {
        insert_to_node(item->parent, item->key[1]);

        if (item->parent->first == item) item->parent->first = NULL;
        else if (item->parent->second == item) item->parent->second = NULL;
        else if (item->parent->third == item) item->parent->third = NULL;

        if (item->parent->first == NULL) {
            item->parent->fourth = item->parent->third;
            item->parent->third = item->parent->second;
            item->parent->second = y;
            item->parent->first = x;
        } else if (item->parent->second == NULL) {
            item->parent->fourth = item->parent->third;
            item->parent->third = y;
            item->parent->second = x;
        } else {
            item->parent->fourth = y;
            item->parent->third = x;
        }

        struct node *tmp = item->parent;
        free(item);
        return tmp;
    } else {
        x->parent = item; 
        y->parent = item;
        become_node2(item, item->key[1], x, y);
        return item;
    }
}

struct node *search(struct node *root, int k)
{
    if (!root) return NULL;

    if (find(root, k)) return root;
    else if (k < root->key[0]) return search(root->first, k);
    else if (((root->size == 2) && (k < root->key[1])) || (root->size == 1)) return search(root->second, k);
    else if (root->size == 2) return search(root->third, k);

    return 0;
}

struct node *search_min(struct node *root)
{ 
    if (!root) return root;
    if (!(root->first)) return root;
    else return search_min(root->first);
}

struct node *remove_elem(struct node *root, int k)
{
    struct node *item = malloc(sizeof(struct node));
    item = search(root, k);

    if (!item) return root;

    struct node *min = NULL;
    if (item->key[0] == k) min = search_min(item->second);
    else min = search_min(item->third);

    if (min) {
        int z = (k == item->key[0] ? item->key[0] : item->key[1]);
        swap(&z, &min->key[0]);
        item = min;
    }

    remove_from_node(item, k);
    return fix(item);
}

struct node *fix(struct node *leaf) 
{
    if (leaf->size == 0 && leaf->parent == NULL) {
        free(leaf);
        return NULL;
    }
    if (leaf->size != 0) {
        if (leaf->parent) return fix(leaf->parent);
        else return leaf;
    }

    struct node *parent = leaf->parent;
    if (parent->first->size == 2 || parent->second->size == 2 || parent->size == 2) leaf = redistribute(leaf);
    else if (parent->size == 2 && parent->third->size == 2) leaf = redistribute(leaf);
    else leaf = merge(leaf);

    return fix(leaf);
}

struct node *redistribute(struct node *leaf) 
{
    struct node *parent = leaf->parent;
    struct node *first = parent->first;
    struct node *second = parent->second;
    struct node *third = parent->third;

    if ((parent->size == 2) && (first->size < 2) && (second->size < 2) && (third->size < 2)) {
        if (first == leaf) {
            parent->first = parent->second;
            parent->second = parent->third;
            parent->third = NULL;
            insert_to_node(parent->first, parent->key[0]);
            parent->first->third = parent->first->second;
            parent->first->second = parent->first->first;

            if (leaf->first != NULL) parent->first->first = leaf->first;
            else if (leaf->second != NULL) parent->first->first = leaf->second;

            if (parent->first->first != NULL) parent->first->first->parent = parent->first;

            remove_from_node(parent, parent->key[0]);

            free(first);

        } else if (second == leaf) {
            insert_to_node(first, parent->key[0]);
            remove_from_node(parent, parent->key[0]);
            if (leaf->first != NULL) first->third = leaf->first;
            else if (leaf->second != NULL) first->third = leaf->second;

            if (first->third != NULL) first->third->parent = first;

            parent->second = parent->third;
            parent->third = NULL;

            free(second);

        } else if (third == leaf) {
            insert_to_node(second, parent->key[1]);
            parent->third = NULL;
            remove_from_node(parent, parent->key[1]);
            if (leaf->first != NULL) second->third = leaf->first;
            else if (leaf->second != NULL) second->third = leaf->second;

            if (second->third != NULL)  second->third->parent = second;

            free(third);
        }

    } else if ((parent->size == 2) && ((first->size == 2) || (second->size == 2) || (third->size == 2))) {
        if (third == leaf) {
            if (leaf->first != NULL) {
                leaf->second = leaf->first;
                leaf->first = NULL;
            }

            insert_to_node(leaf, parent->key[1]);
            if (second->size == 2) {
                parent->key[1] = second->key[1];
                remove_from_node(second, second->key[1]);
                leaf->first = second->third;
                second->third = NULL;
                if (leaf->first != NULL) leaf->first->parent = leaf;
            } else if (first->size == 2) {
                parent->key[1] = second->key[0];
                leaf->first = second->second;
                second->second = second->first;
                if (leaf->first != NULL) leaf->first->parent = leaf;

                second->key[0] = parent->key[0];
                parent->key[0] = first->key[1];
                remove_from_node(first, first->key[1]);
                second->first = first->third;
                if (second->first != NULL) second->first->parent = second;
                first->third = NULL;
            }

        } else if (second == leaf) {
            if (third->size == 2) {
                if (leaf->first == NULL) {
                    leaf->first = leaf->second;
                    leaf->second = NULL;
                }
                insert_to_node(second, parent->key[1]);
                parent->key[1] = third->key[0];
                remove_from_node(third, third->key[0]);
                second->second = third->first;
                if (second->second != NULL) second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = NULL;
            } else if (first->size == 2) {
                if (leaf->second == NULL) {
                    leaf->second = leaf->first;
                    leaf->first = NULL;
                }
                insert_to_node(second, parent->key[0]);
                parent->key[0] = first->key[1];
                remove_from_node(first, first->key[1]);
                second->first = first->third;
                if (second->first != NULL) second->first->parent = second;
                first->third = NULL;
            }

        } else if (first == leaf) {
            if (leaf->first == NULL) {
                leaf->first = leaf->second;
                leaf->second = NULL;
            }
            insert_to_node(first, parent->key[0]);
            if (second->size == 2) {
                parent->key[0] = second->key[0];
                remove_from_node(second, second->key[0]);
                first->second = second->first;
                if (first->second != NULL) first->second->parent = first;
                second->first = second->second;
                second->second = second->third;
                second->third = NULL;
            } else if (third->size == 2) {
                parent->key[0] = second->key[0];
                second->key[0] = parent->key[1];
                parent->key[1] = third->key[0];
                remove_from_node(third, third->key[0]);
                first->second = second->first;
                if (first->second != NULL) first->second->parent = first;
                second->first = second->second;
                second->second = third->first;
                if (second->second != NULL) second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = NULL;
            }
        }

    } else if (parent->size == 1) {
        insert_to_node(leaf, parent->key[0]);

        if (first == leaf && second->size == 2) {
            parent->key[0] = second->key[0];
            remove_from_node(second, second->key[0]);

            if (leaf->first == NULL) leaf->first = leaf->second;

            leaf->second = second->first;
            second->first = second->second;
            second->second = second->third;
            second->third = NULL;
            if (leaf->second != NULL) leaf->second->parent = leaf;

        } else if (second == leaf && first->size == 2) {
            parent->key[0] = first->key[1];
            remove_from_node(first, first->key[1]);

            if (leaf->second == NULL) leaf->second = leaf->first;

            leaf->first = first->third;
            first->third = NULL;
            if (leaf->first != NULL) leaf->first->parent = leaf;

        }
    }
    return parent;
}

struct node *merge(struct node *leaf) 
{
    struct node *parent = leaf->parent;

    if (parent->first == leaf) {
        insert_to_node(parent->second, parent->key[0]);
        parent->second->third = parent->second->second;
        parent->second->second = parent->second->first;

        if (leaf->first != NULL) parent->second->first = leaf->first;
        else if (leaf->second != NULL) parent->second->first = leaf->second;

        if (parent->second->first != NULL) parent->second->first->parent = parent->second;

        remove_from_node(parent, parent->key[0]);
        free(parent->first);
        parent->first = NULL;
    } else if (parent->second == leaf) {
        insert_to_node(parent->first, parent->key[0]);

        if (leaf->first != NULL) parent->first->third = leaf->first;
        else if (leaf->second != NULL) parent->first->third = leaf->second;

        if (parent->first->third != NULL) parent->first->third->parent = parent->first;

        remove_from_node(parent, parent->key[0]);
        free(parent->second);
        parent->second = NULL;
    }

    if (parent->parent == NULL) {
        struct node *tmp = NULL;
        if (parent->first != NULL) tmp = parent->first;
        else tmp = parent->second;
        tmp->parent = NULL;
        free(parent);
        return tmp;
    }
    return parent;
}