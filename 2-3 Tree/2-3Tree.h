#ifndef TREE_H
#define TREE_H

struct node {
  int size;            // Count of the filled keys.
  int key[3];          // Keys.
  struct node *first;  // *first <= key[0];
  struct node *second; // key[0] <= *second < key[1];
  struct node *third;  // key[1] <= *third < key[2];
  struct node *fourth; // kye[2] <= *fourth.
  struct node *parent;
};

int is_leaf(struct node *node);
int find(struct node *node, int k);

void swap(int *x, int *y);
void sort(struct node *node);
void insert_to_node(struct node *node, int k);
void remove_from_node(struct node *node, int k);
void become_node2(struct node *node, int k, struct node *first_, struct node *second_);

struct node *split(struct node *item);
struct node *insert(struct node *root, int k);
struct node *search(struct node *root, int k);
struct node *search_min(struct node *root);
struct node *remove_elem(struct node *root, int k);
struct node *fix(struct node *leaf);
struct node *redistribute(struct node *leaf);
struct node *merge(struct node *leaf);

#endif