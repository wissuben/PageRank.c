# PageRank Implementation

This project implements the PageRank algorithm to calculate the importance of nodes in a directed graph. The graph is read from a file, and the PageRank vector is computed for various configurations.

## Files

- `main.c`: Main program file that includes the main function for running the PageRank algorithm with different configurations.
- `functions.c`: Implementation of various functions used in the PageRank algorithm.
- `page_rank.h`: Header file containing function declarations and structure definitions.
- `dataset_001.txt`: Sample dataset file representing a directed graph.
- `execution_times.dat`: File to store the execution times for different configurations.
- `plot_script.gp`: Gnuplot script for generating a graph from the execution times.

## How to Run

1. Compile the program using a C compiler (e.g., `gcc -o page_rank main.c functions.c -framework Accelerate`).
2. Run the compiled executable (`./page_rank`).

## Configuration Parameters

- **Damping Factor (`damping_factor`):** The damping factor used in the PageRank algorithm. The default value is 0.85.
- **Convergence Threshold (`convergence_threshold`):** The threshold for convergence in the PageRank algorithm. The default value is 1e-6.

## Dataset

The provided `dataset_001.txt` file represents a directed graph with 50 nodes and 100 edges. The file format is as follows:


## Results

The program will output execution times for different configurations to `execution_times.dat`. A graph of the results can be generated using the provided `plot_script.gp`.

## Dependencies

The program uses the Accelerate framework for certain mathematical operations. Ensure that the necessary dependencies are available on your system.

## Notes

- Set the `DEBUG` macro in `page_rank.h` to `true` for debugging output.
- Adjust the `NUM_RUNS` macro in `main.c` to control the number of runs for each configuration.

Feel free to customize this ReadMe to better fit your specific project structure and requirements.


## Results

The program will output execution times for different configurations to `execution_times.dat`. A graph of the results can be generated using the provided `plot_script.gp`.

## Dependencies

The program uses the Accelerate framework for certain mathematical operations. Ensure that the necessary dependencies are available on your system.

## CMake Configuration

The project includes a `CMakeLists.txt` file for building the project. The file specifies the project name, C standard version, source files, and links against the Accelerate Framework.
Don't forget to reload Cmake.
```cmake
cmake_minimum_required(VERSION 3.28)
project(pr_td C)

set(CMAKE_C_STANDARD 11)

add_executable(pr_td main.c
        functions.c
        page_rank.h)

# Link against the Accelerate Framework
target_link_libraries(pr_td PRIVATE "-framework Accelerate")


