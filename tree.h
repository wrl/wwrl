#define IS_RED(n)  (n && n->c == RED)
#define NOT_RED(n) (n && n->c != RED)

#define LFT down[LEFT]
#define RHT down[RIGHT]

typedef struct tree tree_t;
typedef struct tree_node tree_node_t;

typedef enum {
	LEFT  = 0,
	RIGHT = 1
} tree_dir_t;

typedef enum {
	RED = 0,
	BLACK = 1
} tree_node_color_t;

typedef void (*tree_walk_callback_t)(tree_node_t *);

struct tree {
	tree_node_t *root;
};

struct tree_node {
	int key;
	tree_node_color_t c;

	tree_node_t *down[2];
};

tree_t *tree_new();

int tree_insert(tree_t *self, tree_node_t *leaf);
tree_node_t *tree_remove(tree_t *tree, const int key);

tree_node_t *tree_search(tree_t *tree, const int key);
void tree_walk_inorder(tree_t *tree, tree_walk_callback_t);
