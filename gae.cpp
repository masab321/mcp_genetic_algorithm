#include <list>
#include <vector>
#include <random>
#include <thread>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <iostream>
#include <algorithm>

using namespace std;

// knobs
int pop_size = 100;
int generations = 400;
int tournament_size = 2;
int number_of_mutations = 1;
int regen_population_limit = 20;

unsigned seed = 12000;
default_random_engine e(seed);

int N;
string file_name;
vector<int> vertices;
vector<vector<uint8_t>> G;

int gen_max = 0;
int max_size = 0;
vector<int> max_clique;
vector<vector<int>> population;

int random_number(int min, int max) {
    return min + rand() % (max - min + 1);
}

vector<int> random_permutation(int n) {
    vector<int> permutation(n);
    iota(permutation.begin(), permutation.end(), 0);
    shuffle(permutation.begin(), permutation.end(), e);
    return permutation;
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

#pragma region Debugging and printing codes
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
    
    for (size_t i = 0; i < population.size(); ++i) {
        const vector<int>& chromosome = population[i];
        int fitness = assess_fitness(chromosome);

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


void print_current_state_of_population(const int& gen, const int& gen_max_repeated) {
    int total_fitness = 0;
    for (int pop = 0; pop < pop_size; pop++) total_fitness += assess_fitness(population[pop]);
    cout << "Gen: " << gen << ". Avg: " << total_fitness / pop_size << ", Gen Max: " << gen_max << ", Max: " << max_size << ", Gen. Max Repeated: "<< gen_max_repeated << endl;
}
#pragma endregion

int tournament_selection() {
    int best_index = random_number(0, pop_size - 1);
    int best_fitness = assess_fitness(population[best_index]);

    for (int i = 1; i < tournament_size; i++) {
        int next_index = random_number(0, pop_size - 1);
        int next_fitness = assess_fitness(population[next_index]);

        if (next_fitness > best_fitness) {
            best_index = next_index;
            best_fitness = next_fitness;
        }
    }
    return best_index;
}

void mutate(vector<int>& chromosome) {
    int f = N / 2;

    for (int i = 0; i < number_of_mutations; ++i) {
        int index_a = random_number(0, f);
        int index_b = random_number(f + 1, N - 1);
        swap(chromosome[index_a], chromosome[index_b]);
    }
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
        if (cur_fitness > gen_max) {
            gen_max = cur_fitness;
        }
    }
}

vector<int> find_circular_clique(int start_point, const vector<int>& chromosome, bool left) {
    vector<int> current_clique; current_clique.reserve(N);
    list<int> candidate_vertices;
    for (int i = 0; i < N; i++) {
        int index = -1;
        if (!left) {
            index = (start_point - i + N) % N;
        } else {
            index = (start_point + i) % N;
        }
        candidate_vertices.push_back(chromosome[index]);
    }

    while(candidate_vertices.size()) {
        auto v_it = candidate_vertices.begin();
        int v = *v_it;
        current_clique.push_back(v);
        candidate_vertices.erase(v_it);

        auto c_end = candidate_vertices.end();
        for (auto it = candidate_vertices.begin(); it != c_end; ) {
            if (!G[v][*it]) {
                it = candidate_vertices.erase(it);
            } else {
                ++it;
            }
        }

    }
    return current_clique;
}

vector<int> find_clique_for_startpoint(int start_point, const vector<int>& chromosome) {
    vector<int> left_best = find_circular_clique(start_point, chromosome, 1);
    vector<int> right_best = find_circular_clique(start_point, chromosome, 0);

    if (left_best.size() > right_best.size()) return left_best;
    return right_best;
}

vector<int> naive_clique(vector<int>& chromosome) {
    vector<int> best_clique;
    for (int pos = 0; pos < N; pos++) {
        vector<int> current_clique = find_clique_for_startpoint(pos, chromosome);
        if (current_clique.size() > best_clique.size()) {
            best_clique = current_clique;
        }
    }

    return best_clique;
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

void expand_clique_and_fitness_assessment() {
    vector<thread> threads;
    for (int i = 0; i < pop_size; i++) {
        threads.emplace_back(expand_clique, ref(population[i]));
    }
    for (auto &th: threads) {
        th.join();
    }
    update_max();
}

void initialize_population() {
    for (int i = 0; i < pop_size; i++) {
        population[i] = random_permutation(N);
    }
}

void check_and_reinitialize_population(int &prev_max, int &gen_max_repeated) {
    if (gen_max == prev_max) {
        gen_max_repeated++;
        if (gen_max_repeated == regen_population_limit + 1) {
            cout << "Regenerating population" << endl;
            initialize_population();
            gen_max_repeated = 1;
            gen_max = 0;
        }
    } else {
        gen_max_repeated = 1;
        prev_max = gen_max;
    }
}

pair<vector<int>, vector<int>> crossover() {
    auto& parent_a = population[tournament_selection()];
    auto& parent_b = population[tournament_selection()];
    int random_child_prob = random_number(1, 100);
    if (random_child_prob <= 4) {
        if (assess_fitness(parent_a) > assess_fitness(parent_b)) { // this is supposed to be the opposite. Fix it in the implementation of Adaptive variation.
            parent_a = parent_b;
        }
        parent_b = random_permutation(N);
    }
    auto children = pmx_crossover(parent_a, parent_b);
    return children;
}

void mutation(vector<int> &child_a, vector<int> &child_b) {
    mutate(child_a);
    mutate(child_b);
}

void run_genetic_algorithm() {
    population.resize(pop_size, vector<int>(N));
    vector<vector<int>> new_population; new_population.reserve(N);
    
    initialize_population();

    int gen_max_repeated = 0;
    int prev_max = gen_max;
    for (int gen = 1; gen <= generations; gen++) {
        expand_clique_and_fitness_assessment();
        print_current_state_of_population(gen, gen_max_repeated);

        check_and_reinitialize_population(prev_max, gen_max_repeated);

        new_population.clear();
        assert(pop_size % 2 == 0);
        for (int pop = 0; pop < population.size() / 2; pop++) {
            auto [child_a, child_b] = crossover();
            mutation(child_a, child_b);
            
            new_population.push_back(child_a);
            if (new_population.size() < pop_size) {
                new_population.push_back(child_b);
            }
        }
        population = new_population;
    }
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
            G.resize(N, vector<uint8_t>(N, 0));
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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <DIMACS file>" << endl;
        return 1;
    }
    
    srand(seed);
    file_name = argv[1];
    read_dimacs_clique_file(file_name);
    
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
