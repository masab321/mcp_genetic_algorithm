#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cstdlib>
#include <set>

using namespace std;

// knobs
int pop_size = 10;
int generations = 1000;
int elite_n = 6;
int tournament_size = 10;
int max_recursion = 100;

unsigned seed = 1500;
default_random_engine e(seed);

vector<vector<bool>> G;
string file_name;
int N;
vector<int> vertices;

vector<vector<int>> population;
vector<vector<int>> new_population;
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
vector<int> bron_kerbosch_with_pivot_driver(const vector<int>& chromosome);

void complete_search(vector<int> candidate_vertices, vector<int> clique, vector<int>& best_clique, int& recursion) {
    ++recursion;
    if (candidate_vertices.size() + clique.size() <= best_clique.size())
        return;
    
    if (candidate_vertices.empty() or recursion > max_recursion) {
        cout << recursion << ": ";  for (int i: clique) cout << i << ", "; cout << "=> " << clique.size() << endl;
        if (clique.size() > best_clique.size()) {
            best_clique = clique;
            
        }
        return;
    }
    for (int i = 0; i < candidate_vertices.size(); i++) {
        int u = candidate_vertices[i];
        clique.push_back(u);
        vector<int> new_candidate_vertices; new_candidate_vertices.reserve(N);
        for (int j = i + 1; j < candidate_vertices.size(); j++) if (G[u][candidate_vertices[j]]) {
            new_candidate_vertices.push_back(candidate_vertices[j]);
        } 
        complete_search(new_candidate_vertices, clique, best_clique, recursion);
        
        clique.pop_back();
        new_candidate_vertices.clear();
        for (int j = i + 1; j < candidate_vertices.size(); j++) 
            new_candidate_vertices.push_back(candidate_vertices[j]);
        complete_search(new_candidate_vertices, clique, best_clique, recursion);
    }
}
vector<int> complete_search_driver(const vector<int>& chromosome) {
    max_recursion = N * 5;
    int recursion = 0;
    vector<int> candidate_vertices = chromosome;
    vector<int> clique; clique.reserve(N);
    vector<int> best_clique; best_clique.reserve(N);
    complete_search(candidate_vertices, clique, best_clique, recursion);
    if (best_clique.size() > max_size) {
        max_clique = best_clique;
        max_size = best_clique.size();
    }
    return best_clique;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <DIMACS file>" << endl;
        return 1;
    }
    
    srand(seed);
    file_name = argv[1];
    read_dimacs_clique_file(file_name);
    print_global_variables();
    max_clique = complete_search_driver(vertices);

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
    cout << "Vertices: "; for (int i: vertices) cout << i << " "; cout << endl;
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