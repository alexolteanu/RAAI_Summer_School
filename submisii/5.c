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
#define NMAX 200
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

int nr_leafes_helper(b_node_t *root)
{
    if (!root)
    {
        return 0;
    }

    if (!root->left && !root->right) {
        return 1;
    }
    
    return nr_leafes_helper(root->left) + nr_leafes_helper(root->right);
}

int nr_leafes(b_tree_t *tree)
{
    return nr_leafes_helper(tree->root);
}

void leafes_helper(b_node_t *root, int *v, int *size)
{
    if (!root) {
        return;
    }
    
    if (!root->left && !root->right) {
        v[(*size)++] = *(int *)root->data;
        return;
    }

    leafes_helper(root->left, v, size);
    leafes_helper(root->right, v, size);
}


int has_node(b_node_t *root, int value)
{
    if (!root) {
        return 0;
    }
    if (*(int *)(root->data) == value) {
        return 1;
    }
    return has_node(root->left, value) || has_node(root->right, value);
}

void search(b_node_t *root, int value, int *m, int *cats, int real)
{
   if (!root) {
    return;
   }
   // printf("%d ", *(int *)root->data);
   int curr_value = *(int *)root->data;
   if (cats[curr_value] == 1) {
        //printf("A scazut la %d \n", curr_value);
       (*m)--;
   } else {
    (*m) = real;
   }

   if ((*m)==-1) {
    return;
   }
   
   

    if (has_node(root->left, value) == 1) {
        search(root->left, value, m, cats,real);
    } else {
        search(root->right, value, m, cats,real);
    }


    

}

int leafes(b_node_t *root, int m, int *cats)
{
    int *v = malloc(sizeof(int)*NMAX);
    int size = 0;
    int cnt = 0;
    leafes_helper(root, v, &size);
    for (int i = 0; i < size; i++) {
        int a = m;
       // printf("INceput\n");
        search(root, v[i], &a, cats, m);
         
        //  printf("DA:%d\n", a);
        //  printf("Sfarsit\n");
        if (a >= 0) {
            cnt++;
        }
        // printf("\n");
    }

    return cnt;

}

int solve(b_node_t *node, int streak, int m, int *cats)
{
    //TODO
    return leafes(node, m, cats);
    

}

int main(void)
{
    int n, m;
    scanf("%d %d", &n, &m);

    int *cats = malloc(n * sizeof(int));
    DIE(cats == NULL, "malloc cats failed");
    for (int i = 0; i < n; i++) {
        scanf("%d", &cats[i]);
    }

    b_tree_t *tree = b_tree_create(sizeof(int));
    b_node_t **nodes = calloc(n + 1, sizeof(b_node_t *));
    DIE(nodes == NULL, "calloc nodes failed");

    for (int i = 0; i < n; i++) {
        nodes[i] = __b_node_create(&i, sizeof(int));
        if (i == 0) {
            tree->root = nodes[i];
        }
    }

    for (int i = 1 ; i < n ; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        if (nodes[u]->left == NULL) {
            nodes[u]->left = nodes[v];
        } else if (nodes[u]->right == NULL) {
            nodes[u]->right = nodes[v];
        } else {
            if (nodes[v]->left == NULL) {
                nodes[v]->left = nodes[u];
            } else if (nodes[v]->right == NULL) {
                nodes[v]->right = nodes[u];
            }
        }
    }

    int count = solve(tree->root, 0, m, cats);

    printf("%d\n", count);

    free(cats);
    free(nodes);
    b_tree_free(tree, free);

    return 0;
}