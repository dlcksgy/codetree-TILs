#include <iostream>
#include <vector>
#include <unordered_set>
#include <deque>

using namespace std;

int main() {
    // 여기에 코드를 작성해주세요.
    int N;
    cin >> N;
    vector<vector<int>> tree_graph(N+1,vector<int>(N+1,0));
    vector<int> parent(N+1,0);
    for(int i = 0; i < N-1; i++){
        int t1,t2;
        cin >> t1 >> t2;
        tree_graph[t1][t2] = 1;
        tree_graph[t2][t1] = 1;
    }
    unordered_set<int> visited;
    deque<int> Q;
    Q.push_back(1);
    visited.insert(1);
    while(!Q.empty()){
        int p_node = Q.front();
        Q.pop_front();
        for(int i = 0; i < N+1; i++){
            if(tree_graph[p_node][i] == 1 && visited.find(i) == visited.end()){
                Q.push_back(i);
                visited.insert(i);
                parent[i] = p_node;
            }
        }
    }
    for(int i = 2; i < N+1; i++){
        cout << parent[i] << '\n';
    }
    return 0;
}