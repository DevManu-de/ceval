#include "doublist.h"

#include <string.h>

#include "xmemtools.h"

struct doublist *doublist_create() {

	struct doublist *doublist = xmalloc(sizeof(struct doublist));
	doublist->head = NULL;
	doublist->tail = NULL;
	doublist->size = 0;

	return doublist;
}

struct node *node_create(void *value, int type) {
	
	struct node *node = xmalloc(sizeof(struct node));
	node->next = NULL;
	node->prev = NULL;
	node->value = value;
	node->type = type;
	/* STANDALONE means that this node is not in a doublist */
	node->is_in_list = STANDALONE;
	
	return node;

}

/* Inserts a allocated node after the node */
void node_insert_after(struct doublist *doublist, struct node *node, struct node *new_node) {

	/* If node provided is NULL append it to the end of doublist */
	if (node == NULL) {
		/* If doublist has no nodes */
		if (doublist_get_size(doublist) == 0) {
			
			doublist->head = new_node;
			doublist->tail = new_node;

		} else {

			doublist->tail->next = new_node;
			new_node->prev = doublist->tail;
			doublist->tail = new_node;

		}

	} else {
		/* If new node should be appended to the last node */
		if (node->next == NULL) {
			
			node->next = new_node;
			new_node->prev = node;
			doublist->tail = new_node;

		} else {
			
			node->next->prev = new_node;
			new_node->prev = node;
			new_node->next = node->next;
			node->next = new_node;

		}

	}
	
	++doublist->size;
	new_node->is_in_list = INLIST;

}

/* Inserts a allocated node before the node */
void node_insert_before(struct doublist *doublist, struct node *node, struct node *new_node) {

	/* If node provided is NULL insert it to the start of doublist */
	if (node == NULL) {
		/* If doublist has no nodes */
		if (doublist_get_size(doublist) == 0) {
			
			doublist->head = new_node;
			doublist->tail = new_node;

		} else {
			
			doublist->head->prev = new_node;
			new_node->next = doublist->head;
			doublist->head = new_node;

		}

	} else {
		/* If new node should be inserted before the first node */
		if (node->prev == NULL) {
			
			node->prev = new_node;
			new_node->next = node;
			doublist->head = new_node;

		} else {
			
			node->prev->next = new_node;
			new_node->next = node;
			new_node->prev = node->prev;
			node->prev = new_node;
			
		}

	}

	++doublist->size;
	new_node->is_in_list = INLIST;

}

/* Find a node in doublist.
 * Start looking from start to the given direction. For a match value and type have to match. size is the length for comparing
 * the void *.
 * If start is NULL start from head or tal depending on the direction.
 * If size == 0 then the return will be the first node that matches the type.
 * If no node mathes NULL is returned. */
struct node *node_find(struct doublist *doublist, struct node *start, void *value, int type, unsigned long size, int direction) {

	if (start == NULL) {
		if (direction == FORWARD) {
			start = doublist->head;
		} else {
			start = doublist->tail;
		}
	}

	if (direction == FORWARD) {
		while (start != NULL) {
			if (start->type == type) {
				if (memcmp(start->value, value, size) == 0) {
					return start;
				}
				start = start->next;
			}
		}

	} else {
		while (start != NULL) {
			if (start->type == type) {
				if (memcmp(start->value, value, size)) {
					return start;
				}
				start = start->prev;
			}
		}
	}

	return NULL;
}

/* Modifies the contents of node_old.
 * size is the size of the value of the node_new. 
 * free_node_new is a boolean. If 1 free new_node otherwise dont*/
struct node *node_modify(struct doublist *doublist, struct node *node_old, struct node *node_new, unsigned long size, unsigned int free_node_new) {

	node_old->value = memmove(node_old->value, node_new->value, size);
	node_old->type = node_new->type;
	if (free_node_new == 1)
		node_free(doublist, node_new);
	return node_old;

}

/* Removes a node from a doublist but doesnt free is.
 * Returns a pointer to the isolated node. */
struct node *node_remove(struct doublist *doublist, struct node *node) {

	if (node->is_in_list != STANDALONE) {
		if (doublist->head == node && doublist->tail == node) {
			doublist->head = NULL;
			doublist->tail = NULL;
		} else if (doublist->head == node) {
			doublist->head = node->next;
			doublist->head->prev = NULL;
		} else if (doublist->tail == node) {
			doublist->tail = node->prev;
			doublist->tail->next = NULL;
		} else {
			node->next->prev = node->prev;
			node->prev->next = node->next;

		}

		node->next = NULL;
		node->prev = NULL;
		--doublist->size;

	}
	
	return node;

}

/* Frees the entire doublist with all nodes. */
void doublist_free(struct doublist *doublist) {

	struct node *node = doublist->head;
	while (node != NULL) {
		struct node *tmp = node->next;
		node_free(doublist, node);
		node = tmp;
	}
	xfree(doublist);

}

/* Frees a node.
 * Node doesnt have to be in a list. */
void node_free(struct doublist *doublist, struct node *node) {

	node = node_remove(doublist, node);
	xfree(node->value);
	xfree(node);

}

/* Returns the amount of nodes in a list. */
int doublist_get_size(struct doublist *doublist) {
	return doublist->size;

}
