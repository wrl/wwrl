#include <assert.h>
#include <stdlib.h>

#include <stdio.h>

#include "tree.h"

/* makes double-pointers look a little less nasty */
#define SELF (*self)

static void __rotate_left(tree_node_t **self) {
	tree_node_t *rght = SELF->RHT;

	SELF->RHT = rght->LFT;
	rght->LFT = SELF;

	rght->c = SELF->c;
	SELF->c = RED;

	SELF = rght;
}

static void __rotate_right(tree_node_t **self) {
	tree_node_t *left = SELF->LFT;

	SELF->LFT  = left->RHT;
	left->RHT = SELF;

	left->c = SELF->c;
	SELF->c = RED;

	SELF = left;
}

static void __colorflip(tree_node_t *self) {
	self->c = !self->c;

	if( self->LFT )
		self->LFT->c  = !self->LFT->c;

	if(	self->RHT )
		self->RHT->c = !self->RHT->c;
}

#define word(sup) \
		printf(sup " %d(%d) %d(%d) [%d(%d) %d(%d) %d(%d)]\n", \
			   SELF->LFT->key, !SELF->LFT->c,\
			   SELF->key, !SELF->c,\
			   SELF->RHT->LFT->key, !SELF->RHT->LFT->c,\
			   SELF->RHT->key, !SELF->RHT->c,\
			   SELF->RHT->RHT->key, !SELF->RHT->RHT->c\
			   )

static void __fixup(tree_node_t **self) {
	if( IS_RED(SELF->RHT) )
		__rotate_left(self);

	if( IS_RED(SELF->LFT) && IS_RED(SELF->LFT->LFT) )
		__rotate_right(self);

	if( IS_RED(SELF->LFT) && IS_RED(SELF->RHT) )
		__colorflip(SELF);
}

static void __move_red_right(tree_node_t **self) {
	__colorflip(SELF);

	if( SELF->LFT && IS_RED(SELF->LFT->LFT) ) {
		__rotate_right(self);
		__colorflip(SELF);
	}
}

static void __move_red_left(tree_node_t **self) {
	__colorflip(SELF);

	if( SELF->RHT && IS_RED(SELF->RHT->LFT) ) {
		__rotate_right(&(SELF->RHT));
		__rotate_left(self);
		__colorflip(SELF);
	}
}

static tree_node_t *__remove_min(tree_node_t **self) {
	tree_node_t *ret;

	if( !SELF->LFT ) {
		ret = SELF;
		SELF = ret->RHT;
		return ret;
	}

	if( NOT_RED(SELF->LFT) && NOT_RED(SELF->LFT->LFT) )
		__move_red_left(self);

	printf("%d\n", SELF->key);

	ret = __remove_min(&(SELF->LFT));
	__fixup(self);

	return ret;
}

static tree_node_t *__remove(tree_node_t **self, const int key) {
	tree_node_t *ret;

	if( !SELF )
		return NULL;

	if( key < SELF->key ) {
		if( NOT_RED(SELF->LFT) && NOT_RED(SELF->LFT->LFT) )
			__move_red_left(self);

		ret = __remove(&(SELF->LFT), key);
	} else {
		if( IS_RED(SELF->LFT) )
			__rotate_right(self);

		if( SELF->key == key && !SELF->RHT ) {
			ret = SELF;
			SELF = ret->LFT;
			return ret;
		}

		if( SELF->LFT && NOT_RED(SELF->RHT) && NOT_RED(SELF->RHT->LFT) )
			__move_red_right(self);

		if( SELF->key == key ) {
			ret = SELF;
			SELF = __remove_min(&(SELF->RHT));

			SELF->LFT = ret->LFT;
			SELF->RHT = ret->RHT;
		} else
			ret = __remove(&SELF->RHT, key);
	}

	__fixup(self);

	return ret;
}

static void __walk_inorder(tree_node_t *self, tree_walk_callback_t cb) {
	if( !self )
		return;

	__walk_inorder(self->LFT, cb);
	cb(self);
	__walk_inorder(self->RHT, cb);
}

static int __insert(tree_node_t **self, tree_node_t *leaf) {
	tree_dir_t dir;
	int status;

	if( !SELF ) {
		SELF = leaf;
		return 0;
	}

	if( IS_RED(SELF->LFT) && IS_RED(SELF->RHT) )
		__colorflip(*self);

	if( leaf->key != SELF->key ) {
		dir = leaf->key > SELF->key;
		status =  __insert(&(SELF->down[dir]), leaf);
	} else
		status = 1;

	if( IS_RED(SELF->RHT) )
		__rotate_left(self);

	if( IS_RED(SELF->LFT) && IS_RED(SELF->LFT->LFT) )
		__rotate_right(self);

	return status;
}

tree_t *tree_new() {
	return calloc(1, sizeof(tree_t));
}

int tree_insert(tree_t *self, tree_node_t *leaf) {
	int ret;

	assert(self);
	assert(leaf);

	if( !self->root ) {
		self->root = leaf;
		return 0;
	}

	ret = __insert(&self->root, leaf);
	self->root->c = BLACK;

	return ret;
}

tree_node_t *tree_remove(tree_t *self, const int key) {
	tree_node_t *ret;

	assert(self);

	ret = __remove(&self->root, key);

	if( self->root )
		self->root->c = BLACK;

	return ret;
}

tree_node_t *tree_search(tree_t *self, const int key) {
	tree_node_t *node;
	tree_dir_t dir;

	assert(self);

	node = self->root;

	while( node ) {
		if( key == node->key )
			return node;

		dir  = key > node->key;
		node = node->down[dir];
	}

	return NULL;
}

void tree_walk_inorder(tree_t *self, tree_walk_callback_t cb) {
	assert(self);
	assert(cb);

	__walk_inorder(self->root, cb);
}
