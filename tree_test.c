#include <assert.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "tree.h"

#define SET_LEN 42000

int current;

int here[SET_LEN];

int insert(tree_t *b, int data) {
	tree_node_t *leaf = calloc(1, sizeof(tree_node_t));
	leaf->key = data;

	return tree_insert(b, leaf);
}

void check_order(tree_node_t *n) {
	if( n->key < current ) {
		printf("FAIL.  data out of order!\n");
		exit(EXIT_FAILURE);
	}

	current = n->key;
}

void check_red_violations(tree_node_t *n) {
	if( IS_RED(n->RHT) && NOT_RED(n->LFT) ) {
		printf("RED VIOLATION (right is red) at %d\n", n->key);
		exit(EXIT_FAILURE);
	}

	if( IS_RED(n->LFT) && IS_RED(n->LFT->RHT) ) {
		printf("RED VIOLATION (left->right is red) at %d\n", n->key);
		exit(EXIT_FAILURE);
	}

	if( IS_RED(n->LFT) && IS_RED(n->LFT->LFT) ) {
		printf("RED VIOLATION (two left reds) at %d\n", n->key);
		exit(EXIT_FAILURE);
	}
}

int check_integrity(tree_t *tree, int *set, size_t len) {
	int i, okay, count;

	count = 0;

	printf("checking data integrity...\t");
	fflush(stdout);

	current = 0;
	tree_walk_inorder(tree, check_red_violations);
	tree_walk_inorder(tree, check_order);

	for( i = 0; i < len; i++ ) {
		okay = !!tree_search(tree, abs(set[i]));

		if( set[i] < 0 )
			okay = !okay;
		else
			count += okay;

		if( !okay ) {
			printf("FAIL at %d (%d).\n", i, set[i]);
			exit(EXIT_FAILURE);
		}
	}

	printf("pass (%d count)\n", count);
	return count;
}

void fail_if_called(tree_node_t *asfd) {
	printf("yeah, fuck.  :(\n\tFAIL.\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
	tree_node_t *sup;
	tree_t *yeah;
	int i, j, q, x, len;

	yeah = tree_new();
	len  = SET_LEN;

#if 0
	srandom(time(NULL));
#endif

	printf("populating tree with random data...\n");
	for( i = 0; i < SET_LEN; i++ ) {
		for( q = 0; !q; )
			q = (random() % (SET_LEN * 2)) + 1;

		here[i] = q;

		if( insert(yeah, q) )
			i--;
	}

	check_integrity(yeah, here, SET_LEN);

	while( len ) {
		x = ((random() % (SET_LEN / 2)) % len) + 1;
		x = 1;
		printf("\ndeleting %d random nod%s...\n", x, (x == 1) ? "e" : "es");

		for( j = 0, i = 0; j < x; j++ ) {
			q = random() % SET_LEN;
			if( here[q] < 0 ) {
				j--;
				continue;
			}

			sup = tree_remove(yeah, here[q]);

			if( sup && sup->key != here[q] ) {
				printf("FUCK.  %d %d\n", sup->key, here[q]);
			}

			here[q] *= -1;
			len--;
		}

		if( check_integrity(yeah, here, SET_LEN) != len ) {
			printf("\tFAIL. (expected %d\n)\n", len);
			exit(EXIT_FAILURE);
		}
	}

	printf("\nanything left?  ");
	tree_walk_inorder(yeah, fail_if_called);
	printf("nope!  we're good.\n\tpass!\n");

	return EXIT_SUCCESS;
}
