# Parallel Genetic Algorithm for Solving the Maximum Clique Problem with Permutation Encoding

Implementation of a parallel genetic algorithm for the maximum clique problem in C++. A clique in a graph is a subset of vertices in which each pair of vertices is connected by an edge. The maximum clique is the clique that contains the maximum number of vertices.The problem is NP-hard, and the computational complexity increases rapidly with the size of the graph.

<b>The key idea:</b> <pre>_"With an optimal order of vertices, a greedy algorithm can solve the MCP in linear time"_</pre> The genetic algorithm is implemented as follows:
<pre>
1. Initialization: Population of randomly generated permutation of vertices
2. Selection: Tournament Selection
3. Crossover: Two Point Crossover (Partially Mapped Crossover)
4. Mutation: Randomly select two indices and swap them
5. expand_clique:
   - For each index of individual (parallelly calculated) greedily search for a clique
   - Put the best clique found at the begining of individual
   - Update the global maximum clique if current best > global maximum.
6. Fitness: From the begining of an individual greedily search for a clique
7. If the max clique is repeated for a certain number of generations, then regenerate the population (step 1).
</pre><br>
<b>Usage:</b>
<pre>
- Compile the code using g++
- e.g. <b>g++ gae.cpp</b>
- Run <b>./a.out filename generations</b>
- e.g. <b>./a.out brock200_1.clq</b>
- e.g. (Build and Run) <b>g++ gae.cpp -o gae && ./gae brock400_1.clq</b>
</pre><br>

**Table:** Results of some DIMACS instances solved by the genetic algorithm. The maximum allotted runtime for each instance is 1 hour.
| Instance           | Vertices           | Edges           | Best Known | Found           | Found at<br> Generation|
|--------------------|--------------------|-----------------|------------|-----------------|---------------------|
| brock200_1         | 200                | 14834           | 21         | 21              | 4                   |
| brock200_2         | 200                | 9876            | 12         | 12              | 1                   |
| brock200_3         | 200                | 12048           | 15         | 15              | 1                   |
| brock200_4         | 200                | 13089           | 17         | 17              | 3                   |
| brock400_1         | 400                | 59723           | 27         | 27              | 70                  |
| brock400_2         | 400                | 59786           | 29         | 29              | 103                 |
| brock400_3         | 400                | 59681           | 31         | 31              | 43                  |
| brock400_4         | 400                | 12500           | 33         | 33              | 13                  |
| brock800_1         | 800                | 207505          | 23         | 20     -        | 3                   |
| brock800_2         | 800                | 208166          | 24         | 21     -        | 99                  |
| brock800_3         | 800                | 207333          | 25         | 25              | 46                  |
| brock800_4         | 800                | 207643          | 26         | 26              | 150                 |
| c-fat200-1         | 200                | 1534            | 12         | 12              | 1                   |
| c-fat200-2         | 200                | 3235            | 24         | 24              | 1                   |
| c-fat200-5         | 200                | 8473            | 58         | 58              | 1                   |
| c-fat500-10        | 500                | 46627           | 126        | 126             | 1                   |
| c-fat500-1         | 500                | 4459            | 14         | 14              | 1                   |
| c-fat500-2         | 500                | 9139            | 26         | 26              | 1                   |
| c-fat500-5         | 500                | 23191           | 64         | 64              | 1                   |
| hamming10-2        | 1024               | 518656          | 512        | 512             | 3                   |
| hamming10-4        | 1024               | 434176          | 40         | 36     -        | 9                   |
| hamming6-2         | 64                 | 1824            | 32         | 32              | 1                   |
| hamming6-4         | 64                 | 704             | 4          | 4               | 1                   |
| hamming8-2         | 256                | 31616           | 128        | 128             | 2                   |
| hamming8-4         | 256                | 20864           | 16         | 16              | 1                   |
| johnson16-2-4      | 120                | 5460            | 8          | 8               | 1                   |
| johnson32-2-4      | 496                | 107880          | 16         | 16              | 1                   |
| johnson8-2-4       | 28                 | 210             | 4          | 4               | 1                   |
| johnson8-4-4       | 70                 | 1855            | 14         | 14              | 1                   |
| keller4            | 171                | 9435            | 11         | 11              | 0                   |
| keller5            | 776                | 225990          | 27         | 27              | 6                   |
| keller6            | 3361               | 4619898         | 59         | 55     -        | 34                  |
| MANN_a27           | 378                | 70551           | 126        | 126             | 62                  |
| MANN_a45           | 1035               | 533115          | 345        | 341    -        | 5                   |
| MANN_a81           | 3321               | 5506380         | >=1100     | 1088   -        | 1                   |
| MANN_a9            | 45                 | 918             | 16         | 16              | 1                   |
| p_hat1000-1        | 1000               | 122253          | >=10       | 10              | 1                   |
| p_hat1000-2        | 1000               | 244799          | >=46       | 46              | 8                   |
| p_hat1000-3        | 1000               | 371746          | >=68       | 68              | 20                  |
| p_hat1500-1        | 1500               | 284923          | >=12       | 12              | 68                  |
| p_hat1500-2        | 1500               | 568960          | >=65       | 65              | 12                  |
| p_hat1500-3        | 1500               | 847244          | >=94       | 94              | 16                  |
| p_hat300-1         | 300                | 10933           | 8          | 8               | 1                   |
| p_hat300-2         | 300                | 21928           | 25         | 25              | 2                   |
| p_hat300-3         | 300                | 33390           | 36         | 36              | 8                   |
| p_hat500-1         | 500                | 31569           | 9          | 9               | 1                   |
| p_hat500-2         | 500                | 62946           | 36         | 36              | 3                   |
| p_hat500-3         | 500                | 93800           | >=50       | 50              | 19                  |
| p_hat700-1         | 700                | 60999           | 11         | 11              | 2                   |
| p_hat700-2         | 700                | 121728          | >=44       | 44              | 6                   |
| p_hat700-3         | 700                | 183010          | >=62       | 62              | 10                  |
| san1000            | 1000               | 250500          | 15         | 15              | 6                   |
| san200_0.7_1       | 200                | 13930           | 30         | 30              | 1                   |
| san200_0.7_2       | 200                | 13930           | 18         | 18              | 1                   |
| san200_0.9_1       | 200                | 17910           | 70         | 70              | 2                   |
| san200_0.9_2       | 200                | 17910           | 60         | 60              | 1                   |
| san200_0.9_3       | 200                | 17910           | 44         | 44              | 7                   |
| san400_0.5_1       | 400                | 39900           | 13         | 13              | 1                   |
| san400_0.7_1       | 400                | 55860           | 40         | 40              | 1                   |
| san400_0.7_2       | 400                | 55860           | 30         | 30              | 3                   |
| san400_0.7_3       | 400                | 55860           | 22         | 22              | 96                  |
| san400_0.9_1       | 400                | 71820           | 100        | 100             | 2                   |
| sanr200_0.7        | 200                | 13868           | 18         | 18              | 5                   |
| sanr200_0.9        | 200                | 17863           | 42         | 42              | 31                  |
| sanr400_0.5        | 400                | 39984           | 13         | 13              | 18                  |
| sanr400_0.7        | 400                | 55869           | 21         | 21              | 8                   |

- The implementation could not find the best known result for these instances:
     1. brock800_1
     2. brock800_2
     3. hamming10-4
     4. keller6
     5. MANN_a45
     6. MANN_a81

<b>TODO:</b>
<pre>
1. Refactor codes
2. Duplicates:
   1. Remove duplicates from the population.
   2. Fill rest of the population using randomly generated permutations
   3. Call expand_clique for newly added entries.
3. Optimize the expand_clique function
4. Experiment with heuristic approaches for the expand_clique function's clique search.
5. Improve the runtime and result for the dense and large graphs.
</pre>
