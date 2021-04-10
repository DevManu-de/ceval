#ifndef _DOUBLIST_H
#define _DOUBLIST_H

#define STANDALONE  0
#define INLIST      1

#define ITER_DOUBLIST(doublist, varname, code)    for (struct node *varname = doublist->head; varname != NULL; varname = varname->next) {     \
                                                      code                                                                                    \
                                                  }                                                                                           \

enum node_types {
    
    NULL_TYPE,
    CHAR_TYPE,
    SHORT_TYPE,
    INT_TYPE,
    LONG_TYPE,
    LONG_LONG_TYPE,
    FLOAT_TYPE,
    DOUBLE_TYPE,
    STRING_TYPE,
    UNSIGNED_CHAR_TYPE,
    UNSIGNED_SHORT_TYPE,
    UNSIGNED_INT_TYPE,
    UNSIGNED_LONG_TYPE,
    UNSIGNED_LONG_LONG_TYPE,
    STRUCT_TYPE,
    UNION_TYPE,
    ENUM_TYPE

};

typedef struct node {
	
	void *value;
	enum node_types type;
	struct node *next;
	struct node *prev;
	int is_in_list : 1;

} node;

typedef struct doublist {
	
	struct node *head;
	struct node *tail;
	int size;

} doublist;

struct doublist *doublist_create();
struct node *node_create(void *value, enum node_types type);

void node_insert_after(struct doublist *doublist, struct node *node, struct node *new_node);
void node_insert_before(struct doublist *doublist, struct node *node, struct node *new_node);

#define FORWARD     0
#define BACKWARD    1
struct node *node_find(struct doublist *doublist, struct node *start, void *value, enum node_types type, unsigned long size, int direction);
struct node *node_modify(struct doublist *doublist, struct node *node_old, struct node *node_new, unsigned long size, unsigned int free_node_new);

struct node *node_remove(struct doublist *doublist, struct node *node);

void doublist_free(struct doublist *doublist);
void node_free(struct doublist *doublist, struct node *node);

int doublist_get_size(struct doublist *doublist);

/* ATTENTION THIS SHOULD NOT BE USED NORMALLY ITS DESIGNED TO DEBUG */
void doublist_print(struct doublist *doublist);

#endif
