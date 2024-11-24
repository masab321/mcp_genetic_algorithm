#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cstdlib>
#include <set>
#include <unordered_set>
#include <chrono>
#include <future>
#include <mutex>
#include <thread>

using namespace std;
mutex mtx; 

// knobs
int pop_size = 76;
int regen_population_limit = 20;
int generations = 200;
int naive_restart_number = 399;
int number_of_mutations = 3;
int elite_n = 1;
int tournament_size = 2;
int max_recursion = 100;

unsigned seed = 1500;
default_random_engine e(seed);

vector<vector<bool>> G;
string file_name;
int N;
vector<int> vertices;

vector<vector<int>> population;
vector<int> max_clique;
int max_size = 0;
default_random_engine generator(random_device{}());

void read_dimacs_clique_file(const string& filename);
void print_global_variables();
vector<int> random_permutation(int n);
bool fitness_comparator(const vector<int>& a, const vector<int>& b);
int assess_fitness(const vector<int>& chromosome);
void expand_clique(vector<int>& vertices);
int select_with_replacement();
pair<vector<int>, vector<int>> pmx_crossover(const vector<int>& parent_a, const vector<int>& parent_b);
void mutate(vector<int>& chromosome);
void run_genetic_algorithm();
int random_number(int min, int max);
bool has_duplicate(const vector<int>& chromosome);
vector<int> naive_clique(vector<int>& chromosome);

vector<int> complete_search_driver(const vector<int>& chromosome);
void complete_search(vector<int> candidate_vertices, vector<int> clique, vector<int>& best_clique, int& recursion);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <DIMACS file>" << endl;
        return 1;
    }
    
    srand(seed);
    file_name = argv[1];
    read_dimacs_clique_file(file_name);
    // print_global_variables();
    run_genetic_algorithm();

    cout << "Clique result for file: " << file_name << endl;
    cout << "Max clique size: " << max_size << endl;
    cout << "Vertices in max clique: ";
    for (int v : max_clique) {
        cout << v << " ";
    }
    cout << endl;
    return 0;
}

void read_dimacs_clique_file(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    string line, tmp;
    while (getline(file, line)) {
        if (line[0] == 'c') {
            continue;
        } else if (line[0] == 'p') {
            stringstream ss(line);
            string tmp;
            ss >> tmp >> tmp >> N;
            G.resize(N, vector<bool>(N, false));
            vertices.resize(N);
            iota(vertices.begin(), vertices.end(), 0);
        } else if (line[0] == 'e') {
            stringstream ss(line);
            int u, v;
            ss >> tmp >> u >> v;
            u--; v--;
            G[u][v] = G[v][u] = true;
        }
    }

    file.close();
}

void print_global_variables() {
    cout << "File name: " << file_name << endl;
    cout << "Number of vertices: " << N << endl;
    cout << "Graph adjacency matrix:" << endl;
    cout << setw(3) << " ";
    for (int i = 0; i < N; i++) {
        cout << setw(2) << i << " ";
    }
    cout << endl;

    for (int i = 0; i < N; i++) {
        cout << setw(2) << i << " ";
        for (int j = 0; j < N; j++) {
            cout << setw(2) << G[i][j] << " ";
        }
        cout << endl;
    }
}

void print_population_with_fitness(const vector<vector<int>>& population) {
    cout << "Chromosome\tFitness\n";
    cout << "------------------------\n";
    
    // Iterate over each chromosome in the population
    for (size_t i = 0; i < population.size(); ++i) {
        const vector<int>& chromosome = population[i];
        int fitness = assess_fitness(chromosome);
        
        // Print the chromosome and its fitness value
        cout << "[";
        for (size_t j = 0; j < chromosome.size(); ++j) {
            cout << chromosome[j];
            if (j < chromosome.size() - 1) {
                cout << ", ";
            }
        }
        cout << "]\t" << fitness << "\n";
    }
    char c; cin >> c;
}

void update_max() {
    for (int i = 0; i < pop_size; i++) {
        int cur_fitness = assess_fitness(population[i]);
        if (cur_fitness > max_size) {
            max_size = cur_fitness;
            max_clique.clear();
            for (int j = 0; j < max_size; j++) {
                max_clique.push_back(population[i][j]);
            }
        }
    }
}

void run_genetic_algorithm() {
    population.resize(pop_size, vector<int>(N));
    vector<vector<int>> new_population; new_population.reserve(N);
    for (int i = 0; i < pop_size; i++) {
        population[i] = random_permutation(N);
        expand_clique(population[i]);
    }
    int max_clique_repeated = 0;
    int prev_max = max_size;

    for (int gen = 0; gen < generations; gen++) {
        if (max_size == prev_max) {
            max_clique_repeated++;
            if (max_clique_repeated == regen_population_limit) {
                cout << "Regenerating population" << endl;
                for (int i = 0; i < pop_size; i++) {
                    population[i] = random_permutation(N);
                }
                max_clique_repeated = 0;
            }
        } else {
            max_clique_repeated = 0;
            prev_max = max_size;
        }

        // Fitness Assessment
        for (int i = 0; i < pop_size; i++) {
            expand_clique(population[i]);
        }
        update_max();

        int total_fitness = 0;
        for (int pop = 0; pop < pop_size; pop++) total_fitness += assess_fitness(population[pop]);
        cout << "Generation: " << gen << ". Average fitness: " << total_fitness / pop_size << " Max size: " << max_size << ", Max repeated: "<< max_clique_repeated << endl;
        
        new_population.clear();
        for (int pop = 0; pop < population.size() / 2; pop++) {
            auto& parent_a = population[select_with_replacement()];
            auto& parent_b = population[select_with_replacement()];
            auto [child_a, child_b] = pmx_crossover(parent_a, parent_b);
            mutate(child_a);
            mutate(child_b);
            new_population.push_back(child_a);
            if (new_population.size() < pop_size) {
                new_population.push_back(child_b);
            }
        }
        population = new_population;
    }
}

vector<int> random_permutation(int n) {
    vector<int> permutation(n);
    iota(permutation.begin(), permutation.end(), 0);
    shuffle(permutation.begin(), permutation.end(), e);
    return permutation;
}

int select_with_replacement() {
    int best_index = random_number(0, pop_size - 1);
    int best_fitness = assess_fitness(population[best_index]);

    for (int i = 1; i < tournament_size; ++i) {
        int next_index = random_number(0, pop_size - 1);
        int next_fitness = assess_fitness(population[next_index]);

        if (next_fitness > best_fitness) {
            best_index = next_index;
            best_fitness = next_fitness;
        }
    }
    return best_index;
}

bool fitness_comparator(const vector<int>& a, const vector<int>& b) {
    return assess_fitness(a) > assess_fitness(b);
}

int assess_fitness(const vector<int>& chromosome) {
    int clq_size = 0;
    for (int i = 1; i < N; i++) {
        bool all_connected = true;
        for (int j = 0; j < i; j++) {
            if (!G[chromosome[i]][chromosome[j]]) {
                all_connected = false;
                break;
            }
        }
        if (all_connected) {
            clq_size = i;
        } else {
            break;
        }
    }
    return clq_size + 1;
}

void expand_clique(vector<int>& chromosome) {
    vector<int> best_clique = naive_clique(chromosome);
    vector<int> new_order; new_order.reserve(N);
    vector<bool> in_clique(N, false);
    for (int v : best_clique) {
        new_order.push_back(v);
        in_clique[v] = true;
    }
    for (int v : chromosome) {
        if (!in_clique[v]) {
            new_order.push_back(v);
        }
    }

    chromosome = new_order;
}

pair<vector<int>, vector<int>> pmx_crossover(const vector<int>& parent_a, const vector<int>& parent_b) {
    int crossover_start_point = random_number(0, N - 1);
    int crossover_length = random_number(N/5, N / 2);

    vector<int> child_a(N, -1), child_b(N, -1);
    vector<int> mapping_a(N, -1), mapping_b(N, -1);

    for (int i = 0; i < crossover_length; ++i) {
        int index = (crossover_start_point + i) % N;
        child_a[index] = parent_b[index];
        child_b[index] = parent_a[index];
        mapping_a[parent_b[index]] = parent_a[index];
        mapping_b[parent_a[index]] = parent_b[index];
    }

    for (int i = 0; i < N; ++i) {
        if (child_a[i] == -1) {
            child_a[i] = parent_a[i];
            while (mapping_a[child_a[i]] != -1) {
                child_a[i] = mapping_a[child_a[i]];
            }
        }
        if (child_b[i] == -1) {
            child_b[i] = parent_b[i];
            while (mapping_b[child_b[i]] != -1) {
                child_b[i] = mapping_b[child_b[i]];
            }
        }
    }
    return {child_a, child_b};
}

void mutate(vector<int>& chromosome) {
    int f = assess_fitness(chromosome);

    for (int i = 0; i < number_of_mutations; ++i) {
        int index_a = random_number(0, f);
        int index_b = random_number(f + 1, N - 1);
        // reverse(chromosome.begin() + index_a, chromosome.begin() + index_b);
        swap(chromosome[index_a], chromosome[index_b]);
    }
}

int random_number(int min, int max) {
    return min + rand() % (max - min + 1);
}

bool has_duplicate(const vector<int>& chromosome) {
    vector<bool> freq(N, 0);
    for (int i: chromosome) {
        if (freq[i] == 1) return true;
        freq[i] = true;
    }
    return false;
}

vector<int> find_clique_for_startpoint(int start_point, const vector<int>& chromosome) {
    vector<int> current_clique;
    unordered_set<int> candidate_vertices(chromosome.begin(), chromosome.end());

    current_clique.push_back(chromosome[start_point]);
    candidate_vertices.erase(chromosome[start_point]);

    for (auto it = candidate_vertices.begin(); it != candidate_vertices.end(); ) {
        int v = *it;
        if (!G[chromosome[start_point]][v]) {
            it = candidate_vertices.erase(it);
        } else {
            ++it;
        }
    }
    
    for (int i = start_point + 1; i < chromosome.size() && !candidate_vertices.empty(); i++) {
        if (candidate_vertices.find(chromosome[i]) != candidate_vertices.end()) {
            current_clique.push_back(chromosome[i]);
            candidate_vertices.erase(chromosome[i]);

            for (auto it = candidate_vertices.begin(); it != candidate_vertices.end(); ) {
                int v = *it;
                if (!G[chromosome[i]][v]) {
                    it = candidate_vertices.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    for (int i = 0; i < start_point && !candidate_vertices.empty(); i++) {
        if (candidate_vertices.find(chromosome[i]) != candidate_vertices.end()) {
            current_clique.push_back(chromosome[i]);
            candidate_vertices.erase(chromosome[i]);

            for (auto it = candidate_vertices.begin(); it != candidate_vertices.end(); ) {
                int v = *it;
                if (!G[chromosome[i]][v]) {
                    it = candidate_vertices.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    return current_clique;
}

vector<int> naive_clique(vector<int>& chromosome) {
    int restart_number = N - 1;
    vector<int> best_clique;
    best_clique.reserve(N);
    vector<future<vector<int>>> futures;

    for (int pos = 0; pos < N; pos++) {
        vector<int> current_clique = find_clique_for_startpoint(pos, chromosome);
        if (current_clique.size() > best_clique.size()) {
            best_clique = current_clique;
        }
    }

    return best_clique;
}

