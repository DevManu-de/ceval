#ifndef _DOUBLIST_H
#define _DOUBLIST_H

#define STANDALONE 0
#define INLIST 1

struct node {
	
	void *value;
	int type;
	struct node *next;
	struct node *prev;
	int is_in_list : 1;

};

struct doublist {
	
	struct node *head;
	struct node *tail;
	int size;

};

struct doublist *doublist_create();
struct node *node_create(void *value, int type);

void node_insert_after(struct doublist *doublist, struct node *node, struct node *new_node);
void node_insert_before(struct doublist *doublist, struct node *node, struct node *new_node);

#define FORWARD 0
#define BACKWARD 1
struct node *node_find(struct doublist *doublist, struct node *start, void *value, int type, unsigned long size, int direction);
struct node *node_modify(struct doublist *doublist, struct node *node_old, struct node *node_new, unsigned long size, unsigned int free_node_new);

struct node *node_remove(struct doublist *doublist, struct node *node);

void doublist_free(struct doublist *doublist);
void node_free(struct doublist *doublist, struct node *node);

int doublist_get_size(struct doublist *doublist);

#endif
