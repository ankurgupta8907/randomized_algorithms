#include <iostream>
#include <set>
#include <vector>
#include <math.h>
using namespace std;

int randomized(const set< set<int> > &S, int numberOfElements) {
  vector<int> decision;
  for (int i = 0; i < numberOfElements; i++) {
    decision.push_back(rand()%2);
  }
  int splitCnt = 0;
  for (auto it1 = S.begin(); it1 != S.end(); it1++) {
    set<int> SET = *it1;
    int cnt[2];
    cnt[0] = 0; cnt[1] = 0;
    for (auto it2 = SET.begin() ; it2 != SET.end(); it2++) {
      cnt[decision[*it2]]++;
      if (cnt[0] > 0 && cnt[1] > 0) {
        splitCnt++;
        break;
      }
    }
  }
  return splitCnt;
}


int johnson(const set< set<int> > &S, int numberOfElements) {
  vector< set<int> > sets;
  vector< vector<int> > reverseIndex(numberOfElements);
  vector< int > initial(S.size(), -1);
  vector< double > risk(S.size(), 0);

  vector<int> vertex_order;
  for (int i = 0; i < numberOfElements; i++) {
    vertex_order.push_back(i);
  }
  random_shuffle(vertex_order.begin(), vertex_order.end());  

  int cnt = 0;
  for (auto it1 = S.begin(); it1 != S.end(); it1++) {
    set<int> SET = *it1;
    sets.push_back(SET);
    for (auto it2 = SET.begin(); it2 != SET.end(); it2++) {
      int temp = *it2;
      reverseIndex[temp].push_back(cnt);
    }
    if (SET.empty())
      risk[cnt] = 1.0;
    else
      risk[cnt] = 1.0 / pow(2.0, SET.size() - 1);
    cnt++;
  }

  for (int i = 0; i < numberOfElements; i++) {
    int index = vertex_order[i];
    vector<int> containingSets = reverseIndex[index];
    double positive = 0;
    double negative = 0;
    for (int j = 0; j < containingSets.size(); j++) {
      if (risk[containingSets[j]] <= 0 || risk[containingSets[j]] >= 1.0) {
        continue;
      }
      if (initial[containingSets[j]] == 1) {
        positive += risk[containingSets[j]];  
      }
      else if(initial[containingSets[j]] == 0) {
        negative += risk[containingSets[j]];
      }
    }
    int decision = (positive >= negative) ? 0 : 1;
    
    for (int j = 0; j < containingSets.size(); j++) {
      if (risk[containingSets[j]] <= 0 || risk[containingSets[j]] >= 1.0) {
        continue;
      }
      if (initial[containingSets[j]] == 1) {
        risk[containingSets[j]] = decision == 1 ? 2*risk[containingSets[j]] : 0;
      }
      else if(initial[containingSets[j]] == 0) {
        risk[containingSets[j]] = decision == 0 ? 2*risk[containingSets[j]] : 0;
      }
      else {
        initial[containingSets[j]] = decision;
      }
    }  
  }

  int setsSplit = 0;
  for (int i = 0; i < sets.size(); i++) {
    if (risk[i] == 0) {
      setsSplit++;
    }
  }
  return setsSplit;
}




int main() {
  srand (time(NULL));
  int numberOfElements = 14;
  cout << "Total number of elements = " << numberOfElements << endl;
  int numberOfSets = 20480;
  cout << "Total number of sets = " << numberOfSets << endl;
  int probabilityOfInclusion = 2;
  cout << "Probability of inclusion = " << probabilityOfInclusion << endl;

  set< set<int> > S;
  int totalCnt = 0; 
  for (int i = 0; i < numberOfSets; i++) {
    set<int> temp;
    for (int j = 0; j < numberOfElements; j++) {
      if (rand()%probabilityOfInclusion == 0) {
        temp.insert(j);
        totalCnt++;
      }
    }
    S.insert(temp);
  }
  cout << "Total elements in all the sets = " << totalCnt << endl;
  cout << "Total actual sets = " << S.size() << endl;

  int totalIterations = 1000;
  cout << "Total iterations " << totalIterations << endl;
  
  long randomizeAvg = 0;
  int maxSplit = 0;
  int minSplit = INT_MAX;
  for (int i = 0; i < totalIterations; i++) {
    int splitSets = randomized(S, numberOfElements);
    randomizeAvg += splitSets;
    maxSplit = max(maxSplit, splitSets);
    minSplit = min(minSplit, splitSets);
  }
  cout << "Randomized:Avg set split = " << randomizeAvg/totalIterations << endl;
  cout << "Randomized:Max set split = " << maxSplit << endl;
  cout << "Randomized:Min set split = " << minSplit << endl;

  long johnsonAvg = 0;
  maxSplit = 0;
  minSplit = INT_MAX;
  for (int i = 0; i < totalIterations; i++) {
    int splitSets = johnson(S, numberOfElements);
    johnsonAvg += splitSets;
    maxSplit = max(maxSplit, splitSets);
    minSplit = min(minSplit, splitSets);
  }
  cout << "Johnson:Avg set split = " << johnsonAvg/totalIterations << endl;
  cout << "Johnson:Max set split = " << maxSplit << endl;
  cout << "Johnson:Min set split = " << minSplit << endl;
  
  return 0;
}
