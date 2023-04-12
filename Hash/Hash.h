#ifndef HASH_H
#define HASH_H

struct listnode{

    char key[32];
    int value;
    struct listnode *next;
    
};

unsigned int hashtab_hash(char *key);
void hashtab_init(struct listnode **hashtab);
void hashtab_add(struct listnode **hashtab, char *key, int value, int hashtype, int *collisions);
struct listnode *hashtab_lookup(struct listnode **hashtab, char *key);
void hashtab_delete(struct listnode **hashtab, char *key);
unsigned int JENKINS_Hash(char *key);
unsigned int KRHash(char *key);


#endif
