#include "Hash.h"
#define HASHTAB_SIZE 200003

unsigned int hashtab_hash(char *key)
{
    unsigned int h = 0, hash_mul = 31;

    while (*key)
    {
        h = h * hash_mul + (unsigned int) *key++;
    }

    return h % HASHTAB_SIZE;

}

void hashtab_init(struct listnode **hashtab)
{
    for (int i = 0; i < HASHTAB_SIZE; i++)
        hashtab[i] = NULL;
}

void hashtab_add(struct listnode **hashtab, char *key, int value, int hashtype, int *collisions)
{
    struct listnode *node;
    int index;
    if(hashtype == 1){
        index = hashtab_hash(key);
    } else {
        index = JENKINS_Hash(key);
    }

    node = malloc(sizeof(*node));
    if (node != NULL)
    {
        if(hashtab[index] != NULL){
            *collisions += 1;
        }
        strcpy(node->key, key);
        node->next = hashtab[index];
        hashtab[index] = node;
    }
}

struct listnode *hashtab_lookup(struct listnode **hashtab, char *key)
{
    struct listnode *node;

    int index = hashtab_hash(key);
    for (node = hashtab[index]; node != NULL; node = node->next)
    {
        if (0 == strcmp(node->key, key))
            return node;
    }
    return NULL;
}

void hashtab_delete(struct listnode **hashtab, char *key)
{
    struct listnode *node, *prev = NULL;
    int index = hashtab_hash(key);
    for (node = hashtab[index]; node != NULL; node = node->next)
    {
        if (0 == strcmp(node->key, key))
        {
            if (prev == NULL)
                hashtab[index] = node->next;
            else
                prev->next = node->next;
            free(node);
            return;
        }
        prev = node;
    }
}

unsigned int JENKINS_Hash(char *key)
{
	int len = strlen(key);
	unsigned int hash, i;
	for(hash = i = 0; i < len; ++i)
	{
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash % HASHTAB_SIZE;
}
