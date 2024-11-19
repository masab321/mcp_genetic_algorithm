# Parallel Genetic Algorithm for Solving the Maximum Clique Problem with Permutation Encoding

Implementation of a parallel genetic algorithm for the maximum clique problem in C++. A clique in a graph is a subset of vertices such that every pair of distinct vertices in the subset is connected by an edge. The maximum clique is the largest clique in the graph, i.e., the clique that contains the maximum number of vertices.

The problem is NP-hard, meaning that no polynomial-time algorithm is known to solve it for all graphs, and the computational complexity increases rapidly with the size of the graph.

The main idea of this algorithms is: If we can somehow find a perfect order of vertices than any greedy algorithm can solve the problem in linear time. So my genetic algorithm is implemented as follows:

1. Initialization (Population Generation): Population of randomly generated permutation of vertices
2. Selection: Tournament Selection
3. Crossover: Partially Mapped crossover
4. Mutation: Randomly selected two indexes and swap them
5. expand_clique:
   a. For each index of chromosome -> greedily search for a clique
   b. put the best clique found at the begining of the chromosome
6. Fitness: From begining of chromosome greedily search for a clique
7. If the max clique is repeated for a number of generation regenerate the population


#ToDo:

1. Refactor codes
2. Duplicates:
  a. Remove duplicates from the population.
  b. Fill rest of the population using randomly generated permutations
  c. Call expand_clique for newly added entries.
3. Optimize the expand_clique function
4. Experiment with heuristic approaches for the expand_clique function's clique search.
5. Improve the runtime and result for the dense and large graphs.


This table lists the best-known maximum clique sizes and the maximum clique sizes found by the Genetic Algorithm for DIMACS benchmark instances. For each instance, the maximum allotted run time is 1 hour.
| Filename               | Number of vertices | Number of edges | Best Known | Found           | Found at generation |
|------------------------|--------------------|-----------------|------------|-----------------|---------------------|
| brock200_1.clq         | 200                | 14834           | 21         | 21              |                     |
| brock200_2.clq         | 200                | 9876            | 12         | 12              |                     |
| brock200_3.clq         | 200                | 12048           | 15         | 15              |                     |
| brock200_4.clq         | 200                | 13089           | 17         | 17              |                     |
| brock400_1.clq         | 400                | 59723           | 27         | 27              |                     |
| brock400_2.clq         | 400                | 59786           | 29         | 29              |                     |
| brock400_3.clq         | 400                | 59681           | 31         | 31              |                     |
| brock400_4.clq         | 400                | 12500           | 33         | 33              |                     |
| brock800_1.clq         | 800                | 207505          | 23         | 20              |                     |
| brock800_2.clq         |                    |                 | 24         | 21              |                     |
| brock800_3.clq         |                    |                 | 25         | 25              |                     |
| brock800_4.clq         |                    |                 | 26         | 26              |                     |
| c-fat200-1.clq         | 200                | 1534            | 12         | 12              |                     |
| c-fat200-2.clq         | 200                | 3235            | 24         | 24              |                     |
| c-fat200-5.clq         | 200                | 8473            | 58         | 58              |                     |
| c-fat500-10.clq        | 500                | 46627           | 126        | 126             |                     |
| c-fat500-1.clq         | 500                | 4459            | 14         | 14              |                     |
| c-fat500-2.clq         | 500                | 9139            | 26         | 26              |                     |
| c-fat500-5.clq         | 500                | 23191           | 64         | 64              |                     |
| hamming10-2.clq        | 1024               | 518656          | 512        | 512             |                     |
| hamming10-4.clq        | 1024               | 434176          | 40         | 36              |                     |
| hamming6-2.clq         | 64                 | 1824            | 32         | 32              |                     |
| hamming6-4.clq         | 64                 | 704             | 4          | 4               |                     |
| hamming8-2.clq         | 256                | 31616           | 128        | 128             |                     |
| hamming8-4.clq         | 256                | 20864           | 16         | 16              |                     |
| johnson16-2-4.clq      |                    |                 | 8          | 8               |                     |
| johnson32-2-4.clq      | 496                | 107880          | 16         | 16              |                     |
| johnson8-2-4.clq       |                    |                 | 4          | 4               |                     |
| johnson8-4-4.clq       |                    |                 | 14         | 14              |                     |
| keller4.clq            | 171                | 9435            | 11         | 11              |                     |
| keller5.clq            | 776                | 225990          | 27         | 27              |                     |
| keller6.clq            |                    |                 | 59         | 55              |                     |
| MANN_a27.clq           | 378                | 70551           | 126        | 126             |                     |
| MANN_a45.clq           | 1035               | 533115          | 345        | 341             |                     |
| MANN_a81.clq           | 3321               | 5506380         | >=1100     | 1088            |                     |
| MANN_a9.clq            |                    |                 | 16         | 16              |                     |
| p_hat1000-1.clq        | 1000               | 122253          | >=10       | 10              |                     |
| p_hat1000-2.clq        | 1000               | 244799          | >=46       | 46              |                     |
| p_hat1000-3.clq        | 1000               | 371746          | >=68       | 68              |                     |
| p_hat1500-1.clq        | 1500               | 284923          | >=12       | 12              |                     |
| p_hat1500-2.clq        | 1500               | 568960          | >=65       | 65              |                     |
| p_hat1500-3.clq        | 1500               | 847244          | >=94       | 94              |                     |
| p_hat300-1.clq         | 300                | 10933           | 8          | 8               |                     |
| p_hat300-2.clq         | 300                | 21928           | 25         | 25              |                     |
| p_hat300-3.clq         | 300                | 33390           | 36         | 36              |                     |
| p_hat500-1.clq         | 500                | 31569           | 9          | 9               |                     |
| p_hat500-2.clq         | 500                | 62946           | 36         | 36              |                     |
| p_hat500-3.clq         | 500                | 93800           | >=50       | 50              |                     |
| p_hat700-1.clq         | 700                | 60999           | 11         | 11              |                     |
| p_hat700-2.clq         | 700                | 121728          | >=44       | 44              |                     |
| p_hat700-3.clq         | 700                | 183010          | >=62       | 62              |                     |
| san1000.clq            | 1000               | 250500          | 15         | 15              |                     |
| san200_0.7_1.clq       | 200                | 13930           | 30         | 30              |                     |
| san200_0.7_2.clq       | 200                | 13930           | 18         | 18              |                     |
| san200_0.9_1.clq       | 200                | 17910           | 70         | 70              |                     |
| san200_0.9_2.clq       | 200                | 17910           | 60         | 60              |                     |
| san200_0.9_3.clq       | 200                | 17910           | 44         | 44              |                     |
| san400_0.5_1.clq       | 400                | 39900           | 13         | 13              |                     |
| san400_0.7_1.clq       | 400                | 55860           | 40         | 40              |                     |
| san400_0.7_2.clq       | 400                | 55860           | 30         | 30              |                     |
| san400_0.7_3.clq       | 400                | 55860           | 22         | 22              |                     |
| san400_0.9_1.clq       | 400                | 71820           | 100        | 100             |                     |
| sanr200_0.7.clq        | 200                | 13868           | 18         | 18              |                     |
| sanr200_0.9.clq        | 200                | 17863           | 42         | 42              |                     |
| sanr400_0.5.clq        | 400                | 39984           | 13         | 13              |                     |
| sanr400_0.7.clq        | 400                | 55869           | 21         | 21              |                     |
