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
#include <deque>
#include <list>

using namespace std;
mutex mtx; 

// knobs
int pop_size = 100;
int regen_population_limit = 10;
int generations = 200;
int naive_restart_number = 399;
int number_of_mutations = 1;
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

void print_locals(vector<int> &chromosome, vector<int>& best, deque<int>& clique, vector<int>& result_chromosome, list<pair<int, set<int>>>& dis_candidate, list<pair<int, set<int>>>& dis_extra) {
    cout << "-------------------" << endl;
    cout << "chromosome: "; for (int i: chromosome) cout << i + 1 << " "; cout << endl;
    cout << "best: "; for (int i: best) cout << i + 1 << " "; cout << endl;
    cout << "clique: "; for (int i: clique) cout << i + 1 << " "; cout << endl;
    cout << "result_chromosome: "; for (int i: result_chromosome) cout << i + 1 << " "; cout << endl;
    cout << "dis_candidate: " << endl;
    for (auto it = dis_candidate.begin(); it != dis_candidate.end(); it++) {
        cout << "\t" << it->first + 1 << ": "; for (auto v: it->second) cout << v + 1<< " "; cout << endl;
    }
    cout << "dis_extra: " << endl;
    for (auto it = dis_extra.begin(); it != dis_extra.end(); it++) {
        cout << "\t" << it->first + 1 << ": "; for (auto v: it->second) cout << v + 1<< " "; cout << endl;
    }
    char c; cin >> c;
    cout << "___________________\n" << endl;
}

void quadratic_greedy_clique(vector<int> &chromosome) {
    vector<int> best;
    deque<int> clique;
    vector<int> result_chromosome;
    list<pair<int, set<int>>> dis_candidate;
    list<pair<int, set<int>>> dis_extra;

    for (int i: chromosome) dis_candidate.push_back({i, set<int>()});

    for (int i = 0; i < N; i++) {
        bool all_connected = 1;
        for (int j: clique) {
            if (G[chromosome[i]][j] == false) {
                all_connected = false;
                break;
            }
        }

        if (all_connected) {
            clique.push_back(chromosome[i]);
            best.push_back(chromosome[i]);
            result_chromosome.push_back(chromosome[i]);

            for (auto it = dis_candidate.begin(); it != dis_candidate.end(); ) {
                if (it->first == chromosome[i]) {
                    it = dis_candidate.erase(it);
                }
                else {
                    if (G[chromosome[i]][it->first] == false) {
                        it->second.insert(chromosome[i]);
                    }
                    it++;
                }
            }
        }
    }

    while(dis_candidate.size()) {
        vector<int> can_be_added;
        while (can_be_added.empty()) {
            int vertex = clique.front();
            clique.pop_front();
            for (auto it = dis_candidate.begin(); it != dis_candidate.end(); it++) {
                it->second.erase(vertex);
                if (it->second.empty()) {
                    can_be_added.push_back(it->first);
                }
            }

            for (auto it = dis_extra.begin(); it != dis_extra.end(); it++) {
                it->second.erase(vertex); 
            }
            dis_extra.push_back({vertex, set<int>()});
        }

        for (int c: can_be_added) {
            bool is_empty = 0;
            for (auto it = dis_candidate.begin(); it != dis_candidate.end(); it++) {
                if (it->first == c and it->second.size() > 0) {
                    is_empty = 1;
                    break;
                }
            }

            if (is_empty) continue;
            clique.push_back(c);
            result_chromosome.push_back(c);
            for (auto it = dis_candidate.begin(); it != dis_candidate.end(); it++) {
                if (G[it->first][c] == false and it->first != c) {
                    it->second.insert(c);
                }
            }
            for (auto it = dis_extra.begin(); it != dis_extra.end(); it++) {
                if (G[it->first][c] == false) {
                    it->second.insert(c);
                }
            }
        }

        for (auto it = dis_candidate.begin(); it != dis_candidate.end(); ) {
            if (it->second.empty()) {
                it = dis_candidate.erase(it);
            }
            else {
                it++;
            }
        }

        for (auto it = dis_extra.begin(); it != dis_extra.end();) {
            if (it->second.empty()) {
                clique.push_back(it->first);
                for (auto dc = dis_candidate.begin(); dc != dis_candidate.end(); dc++) {
                    if (G[dc->first][it->first] == false){
                        dc->second.insert(it->first);
                    }
                }

                for (auto de = dis_extra.begin(); de != dis_extra.end(); de++) {
                    if (G[de->first][it->first] == false and it->first != de->first){
                        de->second.insert(it->first);
                    }
                }

                it = dis_extra.erase(it);
            }
            else {
                it++;
            }
        }

        if (clique.size() > best.size()) {
            best.clear();
            for (int i: clique) {
                best.push_back(i);
            }
        }
    }
    chromosome = result_chromosome;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <DIMACS file>" << endl;
        return 1;
    }
    
    srand(seed);
    file_name = argv[1];
    read_dimacs_clique_file(file_name);

    vector<int> chromosome = {7, 3, 1, 9, 4, 6, 8, 10, 5, 2};
    for (int &i: chromosome) i -= 1;

    quadratic_greedy_clique(chromosome);
    for (int i: chromosome) cout << i + 1 << " "; cout << endl;
    return 0;
}