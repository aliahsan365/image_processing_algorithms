#include "utils.hpp"

namespace Utils
{

int maxim_map(map<int,int>& mymap) {
  map<int,int>::iterator it;
  it = mymap.begin();
  int maxim = it->second;
  ++it;
  while (it!=mymap.end()) {
    if (it->second > maxim) { 
      maxim = it->second;
    }
    ++it;
  }
  return maxim;
}


void map_percentage(map<int,int>& mymap, int maxim) {
  map<int,int>::iterator it;
  for(it = mymap.begin(); it!=mymap.end(); ++it) { 
    it->second = it->second * 100/ maxim;
  }
}

void print_map(map<int,int>& mymap) {
  map<int,int>::iterator it;
  for(it = mymap.begin(); it!=mymap.end(); ++it) { 
    cout << "#regions " << it->first << " => " << " area " << it->second << endl;
  }
}

void print_matrix(vector<vector<int>> &m) {
  for (int i = 0; i < int(m.size()); ++i) {
    for (int j = 0; j < int (m[i].size()); ++j) {
      cout << m[i][j] << " ";
    }
    cout << endl;    
  }
  cout << endl;
} 

}