#ifndef PAGE_RANK_H
#define PAGE_RANK_H
#define DEBUG false
#define MAX_ITERATIONS 1000
#define CONVERGENCE_THRESHOLD 1e-6
#define DAMPING_FACTOR 0.85

// Struct of a list of Nodes
typedef struct nodes_matrix {
    int node_count;
    double** nodes;
} nodes_matrix;

typedef struct nodes_vector {
    int node_count;
    double* nodes;
} nodes_vector;

nodes_matrix get_node_list_from_file(char*);

nodes_vector get_sum_of_links_from_nodes_matrix(nodes_matrix);

nodes_matrix initialize_matrix(int);

nodes_matrix initialize_identity_matrix(int);

nodes_vector initialize_vector(int);

void assign_identity(nodes_matrix);

void copy_matrix(nodes_matrix, nodes_matrix);

void copy_vector(nodes_vector, nodes_vector);

void normalize_matrix(nodes_matrix, nodes_matrix);

void transpose_matrix(nodes_matrix, nodes_matrix);

void matrix_multiply(nodes_matrix, nodes_matrix, nodes_matrix);

void matrix_vector_multiply(nodes_matrix, nodes_vector, nodes_vector);

void page_rank_vector(nodes_matrix, nodes_vector, double, double);

void matrix_power(nodes_matrix, int, nodes_matrix);

void print_node_list(nodes_matrix);

void print_node_vector(nodes_vector);

nodes_matrix define_matrix_A(nodes_matrix transition_matrix, double damping_factor);

void free_node_list(nodes_matrix);

void free_node_vector(nodes_vector);



#endif //PAGE_RANK_H
