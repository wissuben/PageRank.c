#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "page_rank.h"

#define NUM_RUNS 3

int main() {
    double damping_factors[NUM_RUNS] = {0.85, 0.3, 0.0}; // Changez les valeurs selon vos besoins

    FILE *output_file = fopen("execution_times.dat", "w"); // Fichier pour enregistrer les temps d'exécution
    if (output_file == NULL) {
        perror("Error opening output file");
        return 1;
    }

    nodes_matrix matrix;
    nodes_matrix normalized_matrix;
    nodes_matrix transposed_normalized_matrix;
    nodes_vector sum_of_links_of_matrix;
    nodes_vector pr_vector;

    printf("Hello, World!\n");
    printf("-----------------\n");

    for (int i = 0; i < NUM_RUNS; i++) {
        printf("Run %d with DAMPING_FACTOR = %f\n", i + 1, damping_factors[i]);

        clock_t start = clock(); // Enregistre le temps de début

        matrix = get_node_list_from_file("/Users/wissu/Downloads/pr_td-6-2/graph_wissu_addinglinks.txt");

        normalized_matrix = initialize_matrix(matrix.node_count);
        normalize_matrix(matrix, normalized_matrix);

        transposed_normalized_matrix = initialize_matrix(matrix.node_count);
        transpose_matrix(normalized_matrix, transposed_normalized_matrix);

        printf("Sum of outgoing links of each nodes\n");
        sum_of_links_of_matrix = get_sum_of_links_from_nodes_matrix(matrix);
        print_node_vector(sum_of_links_of_matrix);

        pr_vector = initialize_vector(matrix.node_count);
        page_rank_vector(transposed_normalized_matrix, pr_vector, damping_factors[i], CONVERGENCE_THRESHOLD);
        print_node_vector(pr_vector);

        clock_t end = clock(); // Enregistre le temps de fin
        double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; // Calcule la différence

        printf("Time taken to execute: %f seconds\n", cpu_time_used);

        fprintf(output_file, "%f %f\n", damping_factors[i], cpu_time_used); // Enregistre le damping_factor et le temps d'exécution dans le fichier

        free_node_list(matrix);
        free_node_list(normalized_matrix);
        free_node_list(transposed_normalized_matrix);
        free_node_vector(sum_of_links_of_matrix);
        free_node_vector(pr_vector);
    }

    fclose(output_file); // Ferme le fichier

    // Gnuplot script pour afficher le graphique
    FILE *gnuplot_script = fopen("plot_script.gp", "w");
    if (gnuplot_script == NULL) {
        perror("Error opening gnuplot script file");
        return 1;
    }

    fprintf(gnuplot_script, "set xlabel 'Damping Factor'\n");
    fprintf(gnuplot_script, "set ylabel 'Execution Time (seconds)'\n");
    fprintf(gnuplot_script, "plot 'execution_times.dat' with linespoints title 'Execution Times'\n");
    fclose(gnuplot_script);

    // Appel système pour exécuter gnuplot avec le script
    system("gnuplot -persist plot_script.gp");

    return 0;
}
