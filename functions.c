#include "page_rank.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Accelerate/Accelerate.h>
#include <math.h>
#include <time.h>

nodes_matrix get_node_list_from_file(char* filename) {
    char line[256];
    nodes_matrix matrix;
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }else {
        if(DEBUG) {
            printf("Opening file\n");
        }
    }

    // Read file while there is # at the beginning of the line
    while (fgets(line, sizeof(line), file) && line[0] == '#') {
        if(DEBUG) {
            printf("Ignoring line because of #\n");
        }
        continue;
    }

    sscanf(line, "%d", &matrix.node_count);
    if(DEBUG) {
        printf("file has  %d nodes in it\n", matrix.node_count);
    }

    matrix = initialize_matrix(matrix.node_count);

    // Read the nodes from the file
    int i;
    int j;

    // Read the nodes from the file
    while (fscanf(file, "%d\t%d", &i, &j) == 2) {
        matrix.nodes[i][j] = 1;
        if(DEBUG) {
            printf("Read node %d and adding link %d\n", i, j);
        }
    }

    // Check close file
    if (fclose(file) != 0) {
        perror("Erreur lors de la fermeture du fichier");
        exit(EXIT_FAILURE);
    }else {
        if(DEBUG) {
            printf("Closed file\n");
        }
    }

    return matrix;
}

nodes_vector get_sum_of_links_from_nodes_matrix(nodes_matrix matrix) {
    nodes_vector vector = initialize_vector(matrix.node_count);

    for (int i = 0; i < matrix.node_count; ++i) {
        for (int j = 0; j < matrix.node_count; ++j) {
            vector.nodes[i] += matrix.nodes[i][j];
        }
    }

    return vector;
}

nodes_matrix initialize_matrix(int nodes_count) {
    nodes_matrix matrix;
    matrix.node_count = nodes_count;

    // Allocate memory for the nodes (2D array)
    matrix.nodes = malloc(sizeof(double*) * matrix.node_count);
    if (matrix.nodes == NULL) {
        perror("Erreur lors de l'allocation de mémoire i powered_matrix\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for each row
    for (int i = 0; i < matrix.node_count; ++i) {
        matrix.nodes[i] = malloc(sizeof(double) * matrix.node_count);
        if (matrix.nodes[i] == NULL) {
            perror("Erreur lors de l'allocation de mémoire pour powered_matrix.nodes[i]\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < matrix.node_count; ++i) {
        for (int j = 0; j < matrix.node_count; ++j) {
            matrix.nodes[i][j] = 0.0;
        }
    }

    return matrix;
}

nodes_matrix initialize_identity_matrix(int nodes_count) {
    nodes_matrix matrix = initialize_matrix(nodes_count);

    for (int i = 0; i < matrix.node_count; ++i) {
        matrix.nodes[i][i] = 1.0;
    }
    return matrix;
}

nodes_vector initialize_vector(int nodes_count) {
    nodes_vector vector;
    vector.node_count = nodes_count;

    // Allocate memory for the nodes (array)
    vector.nodes = malloc(sizeof(double) * vector.node_count);
    if (vector.nodes == NULL) {
        perror("Erreur lors de l'allocation de mémoire pour vector.nodes\n");
        exit(EXIT_FAILURE);
    }

    srand((unsigned int)time(NULL));
    int randomIndex = rand() % vector.node_count;

    for (int i = 0; i < vector.node_count; ++i) {
        vector.nodes[i] = 1/vector.node_count;
    }

    return vector;
}

void assign_identity(nodes_matrix matrix) {
    for (int i = 0; i < matrix.node_count; ++i) {
        matrix.nodes[i][i] = 1.0;
    }
}

void copy_matrix(nodes_matrix source, nodes_matrix destination) {
    for (int i = 0; i < source.node_count; i++) {
        for (int j = 0; j < source.node_count; j++) {
            destination.nodes[i][j] = source.nodes[i][j];
        }
    }
}

void copy_vector(nodes_vector source, nodes_vector destination) {
    for (int i = 0; i < source.node_count; i++) {
        destination.nodes[i] = source.nodes[i];
    }
}

void normalize_matrix(nodes_matrix source, nodes_matrix destination) {
    for (int i = 0; i < destination.node_count; i++) {
        int out_degree = 0;
        for (int j = 0; j < source.node_count; j++) {
            out_degree += source.nodes[i][j];
        }
        if (out_degree == 0) {
            for (int j = 0; j < destination.node_count; j++) {
                destination.nodes[i][j] = 1;
            }
        } else {
            for (int j = 0; j < destination.node_count; j++) {
                destination.nodes[i][j] = source.nodes[i][j] / out_degree;
            }
        }
    }
}

void transpose_matrix(nodes_matrix source, nodes_matrix destination) {
    // Transpose the matrix
    for (int i = 0; i < source.node_count; i++) {
        for (int j = 0; j < source.node_count; j++) {
            destination.nodes[j][i] = source.nodes[i][j];
        }
    }
}

void matrix_multiply(nodes_matrix matrix1, nodes_matrix matrix2, nodes_matrix result) {
    // Perform matrix multiplication
    for (int i = 0; i < matrix1.node_count; ++i) {
        for (int j = 0; j < matrix2.node_count; ++j) {
            for (int k = 0; k < matrix1.node_count; ++k) {
                result.nodes[i][j] += matrix1.nodes[i][k] * matrix2.nodes[k][j];
            }
        }
    }
}

void matrix_vector_multiply(nodes_matrix matrix, nodes_vector vector, nodes_vector result) {
    for (int i = 0; i < result.node_count; ++i) {
        result.nodes[i] = 0.0;
    }

    // Perform matrix-vector multiplication
    for (int i = 0; i < matrix.node_count; ++i) {
        for (int j = 0; j < matrix.node_count; ++j) {
            result.nodes[i] += matrix.nodes[i][j] * vector.nodes[j];
        }
    }
}

void page_rank_vector(nodes_matrix matrix, nodes_vector result, double damping_factor, double tolerance) {
    nodes_vector prev_result = initialize_vector(matrix.node_count);
    double convergence;

    for(int i = 1; i <= MAX_ITERATIONS; i++)
    {
        for (int i = 0; i < matrix.node_count; i++)
        {
            result.nodes[i] = 0.0;
        }

        matrix_vector_multiply(matrix, prev_result, result);

        if(damping_factor >= 0)
        {
            for(int i=0; i<matrix.node_count; i++)
            {
                result.nodes[i] = damping_factor * result.nodes[i] + (1 - damping_factor) / matrix.node_count;
            }
        }

        convergence = 0;
        for(int i = 0; i < matrix.node_count; i++)
        {
            convergence += fabs(result.nodes[i] - prev_result.nodes[i]);
        }

        if (convergence < tolerance)
        {
            printf("Damping factor: %f\n", damping_factor);
            printf("Convergenve threshold: %f\n", tolerance);
            printf("Convergence reached at iteration %d\n", i);
            break;
        }
        copy_vector(result, prev_result);
    }
    free_node_vector(prev_result);
}

void matrix_power(nodes_matrix matrix, int power, nodes_matrix result) {
    if (power == 0) {
        // Return the identity matrix for power = 0
        assign_identity(result);
    }else {
        copy_matrix(matrix, result);  // Initialize result to the input matrix

        // Perform matrix exponentiation 'power' times
        for (int i = 1; i < power; ++i) {
            matrix_multiply(matrix, result, result);
        }
    }
}

void print_node_list(nodes_matrix matrix) {
    printf("-----------------\n");
    for (int i = 0; i < matrix.node_count; ++i) {
        for (int j = 0; j < matrix.node_count; ++j) {
            printf("%f  ", matrix.nodes[i][j]);
        }
        printf("\n");
    }
    printf("-----------------\n");
}

void print_node_vector(nodes_vector vector) {
    printf("-----------------\n");
    for (int i = 0; i < vector.node_count; ++i) {
        printf("%f\n", vector.nodes[i]);
    }
    printf("-----------------\n");
}

void free_node_list(nodes_matrix matrix) {
    for (int i = 0; i < matrix.node_count; i++) {
        free(matrix.nodes[i]);
        if(DEBUG) {
            printf("Freed memory for node %d\n", i);
        }
    }
    free(matrix.nodes);
    if(DEBUG) {
        printf("Freed memory for nodes\n");
    }
}

void free_node_vector(nodes_vector vector) {
    free(vector.nodes);
    if(DEBUG) {
        printf("Freed memory for nodes\n");
    }
}

nodes_matrix define_matrix_A(nodes_matrix transition_matrix, double damping_factor) {
    // Number of nodes in the matrix
    int num_nodes = transition_matrix.node_count;

    // Initialize the G matrix (matrix of all elements equal to 1/N)
    nodes_matrix matrix_gout = initialize_matrix(num_nodes);
    for (int i = 0; i < num_nodes; ++i) {
        for (int j = 0; j < num_nodes; ++j) {
            matrix_gout.nodes[i][j] = 1.0 / num_nodes;
        }
    }

    // Calculate A = damping_factor * P + (1 - damping_factor) * G
    nodes_matrix matrix_A = initialize_matrix(num_nodes);
    for (int i = 0; i < num_nodes; ++i) {
        for (int j = 0; j < num_nodes; ++j) {
            matrix_A.nodes[i][j] = damping_factor * transition_matrix.nodes[i][j] + (1 - damping_factor) * matrix_gout.nodes[i][j];
        }
    }

    // Free memory allocated for matrix_gout
    free_node_list(matrix_gout);

    return matrix_A;


}


