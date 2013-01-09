#include <assert.h>
#include <stdlib.h>

#include <stdio.h>

#include "tree.h"

/* makes double-pointers look a little less nasty */
#define SELF (*self)

static void __walk_inorder(tree_node_t *self, tree_walk_callback_t cb)
{
	if (!self)
		return;

	__walk_inorder(self->down[LEFT], cb);
	cb(self);
	__walk_inorder(self->down[RIGHT], cb);
}

static int __insert(tree_node_t **self, tree_node_t * leaf)
{
	tree_dir_t dir;

	if (!SELF) {
		SELF = leaf;
		return 0;
	}

	if (leaf->key == SELF->key)
		return 1;

	dir = leaf->key > SELF->key;

	return __insert(&(SELF->down[dir]), leaf);
}

static tree_node_t *__remove(tree_node_t **self, const int key)
{
#define SUCC (*succ)
	tree_node_t *yeah, **succ;
	tree_dir_t dir;

	/* self is pointing to parent->down[dir] */

	if (!SELF)
		return NULL;

	dir = key > SELF->key;

	if (SELF->key == key) {
		yeah = SELF;

		/* how many children do we have? */
		if (yeah->down[LEFT] && yeah->down[RIGHT]) {
			/* two kids.  we'll replace ourselves with the successor. */

			for (succ = &yeah->down[RIGHT]; SUCC->down[LEFT];
			     succ = &(SUCC->down[LEFT])) ;

			/* patch the successor into the tree */
			SELF = SUCC;
			SUCC = SUCC->down[RIGHT];

			SELF->down[LEFT] = yeah->down[LEFT];
			SELF->down[RIGHT] = yeah->down[RIGHT];
		} else if (!yeah->down[LEFT] && !yeah->down[RIGHT])
			/* no children! donnnneeee. */
			SELF = NULL;
		else if (yeah->down[LEFT])
			/* just one. */
			SELF = yeah->down[LEFT];
		else
			/* likewise, */
			SELF = yeah->down[RIGHT];

		return yeah;
	}

	return __remove(&(SELF->down[dir]), key);
#undef SUCC
}

tree_t *tree_new()
{
	return calloc(1, sizeof(tree_t));
}

int tree_insert(tree_t *self, tree_node_t * leaf)
{
	assert(self);
	assert(leaf);

	if (!self->root) {
		self->root = leaf;
		return 0;
	}

	return __insert(&self->root, leaf);
}

tree_node_t *tree_remove(tree_t *self, const int key)
{
	assert(self);
	return __remove(&self->root, key);
}

tree_node_t *tree_search(tree_t *self, const int key)
{
	tree_node_t *node;
	tree_dir_t dir;

	assert(self);

	node = self->root;

	while (node) {
		if (key == node->key)
			return node;

		dir = key > node->key;
		node = node->down[dir];
	}

	return NULL;
}

void tree_walk_inorder(tree_t *self, tree_walk_callback_t cb)
{
	assert(self);
	assert(cb);

	__walk_inorder(self->root, cb);
}
