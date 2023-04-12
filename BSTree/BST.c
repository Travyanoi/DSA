#include <stdlib.h>
#include <stdio.h>
#include "BST.h"
#include <string.h>

struct bstree *bstree_create(char *key, uint32_t value)
{
    struct bstree *node;
    node = malloc(sizeof(*node));
    if (node != NULL) 
    {
        node->key = key;
        node->value = value;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}
void bstree_add(struct bstree *tree, char *key, uint32_t value)
{
    struct bstree *parent, *node;
    if (tree == NULL)
        return;
    for (parent = tree; tree != NULL; )
    {
        parent = tree;
        if (strcmp(key, parent->key) < 0)
            tree = tree->left;
        else
            if (strcmp(key, parent->key) > 0)
                tree = tree->right;
            else
                return;
    }
    node = bstree_create(key, value);

    if (strcmp(key, parent->key) < 0)
    {
        parent->left = node;
    } else {
        parent->right = node;
    }
}

struct bstree *bstree_lookup(struct bstree *tree, char *key)
{
    while (tree != NULL) 
    {
        if (strcmp(key, tree->key) == 0)
            return tree;
        else if (strcmp(key, tree->key) < 0)
            tree = tree->left;
        else
            tree = tree->right;
    }
    return tree;
}

struct bstree *bstree_delete(struct bstree *tree, char *key)
{
    if (tree == NULL)
        return NULL;
    if (strcmp(key, tree->key) > 0)
        tree->right = bstree_delete(tree->right, key);
    else if (strcmp(key, tree->key) < 0)
        tree->left = bstree_delete(tree->left, key);
    else 
        {
            if((tree->left == NULL) && (tree->right == NULL))
            {
                free(tree);
                return NULL;
            }
            else if((tree->left == NULL) || (tree->right == NULL))
            {
                struct bstree *node;
                if(tree->left == NULL)
                    node = tree->right;
                else
                    node = tree->left;
                free(tree);
                return node;
            }
            else
            {
                struct bstree *node = bstree_min(tree->right);
                tree->key = node->key;
                tree->right = bstree_delete(tree->right, node->key);
            }
        }
    return tree;
}

struct bstree *bstree_min(struct bstree *tree)
{
    if (tree == NULL)
       return NULL;

    while (tree->left != NULL)
        tree = tree->left;
    return tree;
}

struct bstree *bstree_max(struct bstree *tree)
{
    if (tree == NULL)
        return NULL;

    while (tree->right != NULL)
        tree = tree->right;
    return tree;    
}