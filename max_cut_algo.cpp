#include <iostream>
#include <vector>
#include <list>
#include <cstdlib> 
using namespace std;

int randomized(const vector< vector<int> >& adj_list) {
  int num_vertices = adj_list.size();
  vector<bool> decision(num_vertices);
  for (int i = 0; i < num_vertices; i++) {
    if (rand()%2 == 0) {
      decision[i] = true;
    }
    else {
      decision[i] = false;
    }
  }
  int cut_size = 0;
  for (int i = 0; i < num_vertices; i++) {
    for (int j = 0; j < adj_list[i].size(); j++) {
      if (decision[i] != decision[adj_list[i][j]]) {
        cut_size++;
      }
    }
  }
  return cut_size/2;
}


typedef struct VertexDegree {
  int index;
  int degree;
} VertexDegree;

bool compare(VertexDegree v1, VertexDegree v2) {
  return v1.degree > v2.degree;
}



int deterministic(const vector< vector<int> >& adj_list, bool determined) {
  int num_vertices = adj_list.size();
  vector<int> vertex_order;
  int decision[num_vertices];
  for (int i = 0; i < num_vertices; i++) {
      vertex_order.push_back(i);
      decision[i] = -1;
  }
  if (!determined) {
    random_shuffle(vertex_order.begin(), vertex_order.end());
  }
  else {
    VertexDegree degree[num_vertices];
    for (int i = 0; i < num_vertices; i++) {
      degree[i].index = i;
      degree[i].degree = adj_list[i].size();
    }  
    sort(degree, degree + num_vertices, compare);
    for (int i = 0; i < num_vertices; i++) {
      vertex_order[i] = degree[i].index;
    }
  }

  for (int i = 0; i < num_vertices; i++) {
    int index = vertex_order[i];
    const vector<int>& neighbors = adj_list[index];
    int positive = 0;
    int negative = 0;
    for (int j = 0; j < neighbors.size(); j++) {
      if (decision[neighbors[j]] == 1) 
        positive++;
      else if (decision[neighbors[j]] == 0) 
        negative++; 
    }
    if (positive >= negative) 
      decision[index] = 0;
    else 
      decision[index] = 1;
  }

  int cut_size = 0;
  for (int i = 0; i < num_vertices; i++) {
    for (int j = 0; j < adj_list[i].size(); j++) {
      if (decision[i] != decision[adj_list[i][j]]) {
        cut_size++;
      }
    }
  }

  return cut_size/2;
}



int main() {
  srand (time(NULL));
  int num_vertices = 1000;
  cout << "Total number of vertices " << num_vertices << endl;
  int edges_per_vertex = 20;
  cout << "Total number of edges per vertex " << edges_per_vertex << endl;

  vector< vector<int> > adj_list(num_vertices);

  int cnt = 0;
  for (int i = 0; i < num_vertices; i++) {
    for (int j = i + 1; j < num_vertices; j++) {
        int temp = rand() % (num_vertices/(edges_per_vertex));
        if(temp == 0) {
          adj_list[i].push_back(j);  
          adj_list[j].push_back(i);
          cnt++;
        }
    }
  }
  cout << "Total number of edges = " << cnt << endl;  

  int temp1 = deterministic(adj_list, true);
  cout << "Deterministic decreasing order of degree = " << temp1 << endl;
  int temp2 = randomized(adj_list);

  int max_rand = 0;
  int min_rand = INT_MAX;

  int max_det = 0;
  int min_det = INT_MAX;

  long avg_rand = 0;
  long avg_det = 0;
  
  int totalIterations = num_vertices;
  cout << "Total number of iterations " << totalIterations << endl;

  for (int i = 0; i < totalIterations; i++) {
    int rand_max_cut = randomized(adj_list);
    max_rand = max(max_rand, rand_max_cut);
    min_rand = min(min_rand, rand_max_cut);
    avg_rand += rand_max_cut;    

    int det_max_cut = deterministic(adj_list, false);
    max_det = max(max_det, det_max_cut);
    min_det = min(min_det, det_max_cut);
    avg_det += det_max_cut;
  }

  cout << "Max from deterministic = " << max_det << endl;
  cout << "Min from deterministic = " << min_det << endl;
  cout << "Avg from deterministic = " << avg_det/totalIterations << endl;

  cout << "Max from randomized = " << max_rand << endl;
  cout << "Min from randomized = " << min_rand << endl;
  cout << "Avg from randomized = " << avg_rand/totalIterations << endl;
  return 0;
}



