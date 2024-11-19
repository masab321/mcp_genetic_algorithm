# Parallel Genetic Algorithm for Solving the Maximum Clique Problem with Permutation Encoding

Implementation of a parallel genetic algorithm for the maximum clique problem in C++. A clique in a graph is a subset of vertices such that every pair of distinct vertices in the subset is connected by an edge. The maximum clique is the largest clique in the graph, i.e., the clique that contains the maximum number of vertices.

The problem is NP-hard, meaning that no polynomial-time algorithm is known to solve it for all graphs, and the computational complexity increases rapidly with the size of the graph.

ToDo:
1. Duplicates:
  a. Remove duplicates from the population.
  b. Fill rest of the population using randomly generated permutations
  c. Call expand_clique for newly added entries.
2. Optimize the expand_clique function
3. Experiment with heuristic approaches for the expand_clique function's clique search.
4. Improve the runtime and result for the dense and large graphs.


This table lists the best-known maximum clique sizes and Max clique found by the Genetic Algorithm for DIMACS benchmark instances.
| Filename               | Number of vertices | Number of edges | Best Known | Found           | Found at generation |
|------------------------|--------------------|-----------------|------------|-----------------|---------------------|
| brock200_1.clq         | 200                |                 | 21         | 21              |                     |
| brock200_2.clq         | 200                | 9876            | 12         | 12              |                     |
| brock200_3.clq         | 21                 | 21              | 15         | 15              |                     |
| brock200_4.clq         | 21                 | 21              | 17         | 17              |                     |
| brock400_1.clq         | 21                 | 21              | 27         | 27              |                     |
| brock400_2.clq         | 21                 | 21              | 29         | 29              |                     |
| brock400_3.clq         | 21                 | 21              | 31         | 31              |                     |
| brock400_4.clq         | 21                 | 21              | 33         | 33              |                     |
| brock800_1.clq         | 21                 | 21              | 23         | 20              |                     |
| brock800_2.clq         | 21                 | 21              | 24         | 21              |                     |
| brock800_3.clq         | 21                 | 21              | 25         | 25              |                     |
| brock800_4.clq         | 21                 | 21              | 26         | 26              |                     |
| c-fat200-1.clq         | 21                 | 21              | 12         | 12              |                     |
| c-fat200-2.clq         | 21                 | 21              | 24         | 24              |                     |
| c-fat200-5.clq         | 21                 | 21              | 58         | 58              |                     |
| c-fat500-10.clq        | 21                 | 21              | 126        | 126             |                     |
| c-fat500-1.clq         | 21                 | 21              | 14         | 14              |                     |
| c-fat500-2.clq         | 21                 | 21              | 26         | 26              |                     |
| c-fat500-5.clq         | 21                 | 21              | 64         | 64              |                     |
| hamming10-2.clq        | 21                 | 21              | 512        | 512             |                     |
| hamming10-4.clq        | 21                 | 21              | 40         | 36              |                     |
| hamming6-2.clq         | 21                 | 21              | 32         | 32              |                     |
| hamming6-4.clq         | 21                 | 21              | 4          | 4               |                     |
| hamming8-2.clq         | 21                 | 21              | 128        | 128             |                     |
| hamming8-4.clq         | 21                 | 21              | 16         | 16              |                     |
| johnson16-2-4.clq      | 21                 | 21              | 8          | 8               |                     |
| johnson32-2-4.clq      | 21                 | 21              | 16         | 16              |                     |
| johnson8-2-4.clq       | 21                 | 21              | 4          | 4               |                     |
| johnson8-4-4.clq       | 21                 | 21              | 14         | 14              |                     |
| keller4.clq            | 21                 | 21              | 11         | 11              |                     |
| keller5.clq            | 21                 | 21              | 27         | 27              |                     |
| keller6.clq            | 21                 | 21              | 59         | 55              |                     |
| MANN_a27.clq           | 21                 | 21              | 126        | 126             |                     |
| MANN_a45.clq           | 21                 | 21              | 345        | 341             |                     |
| MANN_a81.clq           | 21                 | 21              | >=1100     | 1088            |                     |
| MANN_a9.clq            | 21                 | 21              | 16         | 16              |                     |
| p_hat1000-1.clq        | 21                 | 21              | >=10       | 10              |                     |
| p_hat1000-2.clq        | 21                 | 21              | >=46       | 46              |                     |
| p_hat1000-3.clq        | 21                 | 21              | >=68       | 68              |                     |
| p_hat1500-1.clq        | 21                 | 21              | >=12       | 12              |                     |
| p_hat1500-2.clq        | 21                 | 21              | >=65       | 65              |                     |
| p_hat1500-3.clq        | 21                 | 21              | >=94       | 94              |                     |
| p_hat300-1.clq         | 21                 | 21              | 8          | 8               |                     |
| p_hat300-2.clq         | 21                 | 21              | 25         | 25              |                     |
| p_hat300-3.clq         | 21                 | 21              | 36         | 36              |                     |
| p_hat500-1.clq         | 21                 | 21              | 9          | 9               |                     |
| p_hat500-2.clq         | 21                 | 21              | 36         | 36              |                     |
| p_hat500-3.clq         | 21                 | 21              | >=50       | 50              |                     |
| p_hat700-1.clq         | 21                 | 21              | 11         | 11              |                     |
| p_hat700-2.clq         | 21                 | 21              | >=44       | 44              |                     |
| p_hat700-3.clq         | 21                 | 21              | >=62       | 62              |                     |
| san1000.clq            | 21                 | 21              | 15         | 15              |                     |
| san200_0.7_1.clq       | 21                 | 21              | 30         | 30              |                     |
| san200_0.7_2.clq       | 21                 | 21              | 18         | 18              |                     |
| san200_0.9_1.clq       | 21                 | 21              | 70         | 70              |                     |
| san200_0.9_2.clq       | 21                 | 21              | 60         | 60              |                     |
| san200_0.9_3.clq       | 21                 | 21              | 44         | 44              |                     |
| san400_0.5_1.clq       | 21                 | 21              | 13         | 13              |                     |
| san400_0.7_1.clq       | 21                 | 21              | 40         | 40              |                     |
| san400_0.7_2.clq       | 21                 | 21              | 30         | 30              |                     |
| san400_0.7_3.clq       | 21                 | 21              | 22         | 22              |                     |
| san400_0.9_1.clq       | 21                 | 21              | 100        | 100             |                     |
| sanr200_0.7.clq        | 21                 | 21              | 18         | 18              |                     |
| sanr200_0.9.clq        | 21                 | 21              | 42         | 42              |                     |
| sanr400_0.5.clq        | 21                 | 21              | 13         | 13              |                     |
| sanr400_0.7.clq        | 21                 | 21              | 21         | 21              |                     |
