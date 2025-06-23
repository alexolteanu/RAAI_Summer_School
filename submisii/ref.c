#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define DIE(assertion, call_description)  \
    do                                    \
    {                                     \
        if (assertion)                    \
        {                                 \
            fprintf(stderr, "(%s, %d): ", \
                    __FILE__, __LINE__);  \
            perror(call_description);     \
            exit(errno);                  \
        }                                 \
    } while (0)

// Binary Tree Structures
typedef struct b_node_t b_node_t;
struct b_node_t {
    b_node_t *left;   // Left child
    b_node_t *right;  // Right child
    void *data;       // Vertex data (vertex number)
};

typedef struct b_tree_t b_tree_t;
struct b_tree_t {
    b_node_t *root;   // Root of the tree
    size_t data_size; // Size of data in nodes
};

// Helper function to create a node
static b_node_t *__b_node_create(void *data, size_t data_size)
{
    b_node_t *b_node = malloc(sizeof(*b_node));
    DIE(b_node == NULL, "b_node malloc");

    b_node->left = b_node->right = NULL;
    b_node->data = malloc(data_size);
    DIE(b_node->data == NULL, "b_node->data malloc");
    memcpy(b_node->data, data, data_size);

    return b_node;
}

b_tree_t *b_tree_create(size_t data_size)
{
    b_tree_t *b_tree = malloc(sizeof(*b_tree));
    DIE(b_tree == NULL, "b_tree malloc");

    b_tree->root = NULL;
    b_tree->data_size = data_size;
    return b_tree;
}

// Free the tree
static void __b_tree_free(b_node_t *b_node, void (*free_data)(void *))
{
    if (!b_node)
        return;
    __b_tree_free(b_node->left, free_data);
    __b_tree_free(b_node->right, free_data);
    free_data(b_node->data);
    free(b_node);
}

void b_tree_free(b_tree_t *b_tree, void (*free_data)(void *))
{
    __b_tree_free(b_tree->root, free_data);
    free(b_tree);
}



int solve(b_node_t *node, int streak, int m, int *cats) {

    if (node == NULL) {
        return 0;
    }

    if (cats[*(int *)node->data] == 1) {

        streak++;
    } else {
        streak = 0;
    }

    if (streak > m) {
        return 0;
    }

    if (node -> left == NULL && node -> right == NULL) {
        return 1; 
    }

    int left_count = solve(node -> left, streak, m, cats);
    int right_count = solve(node -> right, streak, m, cats);

    int sum = left_count + right_count;

    return sum;
}

int main(void) {
    int n, m;
    scanf("%d %d", &n, &m);

    int *cats = malloc(n * sizeof(int));
    DIE(cats == NULL, "malloc cats failed");
    for (int i = 0; i < n; i++) {
        scanf("%d", &cats[i]);
    }

    b_tree_t *tree = b_tree_create(sizeof(int));
    b_node_t **nodes = calloc(n, sizeof(b_node_t *));
    DIE(nodes == NULL, "calloc nodes failed");

    for (int i = 0; i < n; i++) {
        nodes[i] = __b_node_create(&i, sizeof(int));
        if (i == 0) {
            tree->root = nodes[i];
        }
    }

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        if (nodes[u]->left == NULL) {
            nodes[u]->left = nodes[v];
        } else {
            nodes[u]->right = nodes[v];
        }
    }

    int count = solve(tree->root, 0, m, cats);

    printf("%d\n", count);

    free(cats);
    free(nodes);
    b_tree_free(tree, free);

    return 0;
}