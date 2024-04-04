#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <deque>

using namespace std;

int main() {
    // 여기에 코드를 작성해주세요.
    int N;
    cin >> N;
    unordered_map<int,unordered_set<int>> tree_graph;
    vector<int> parent(N+1,0);
    for(int i = 0; i < N-1; i++){
        int t1,t2;
        cin >> t1 >> t2;
        if(tree_graph.find(t1) == tree_graph.end()){
            tree_graph.emplace(t1,unordered_set<int>({t2}));
        }else{
            tree_graph[t1].insert(t2);
        }
        if(tree_graph.find(t2) == tree_graph.end()){
            tree_graph.emplace(t2,unordered_set<int>({t1}));
        }else{
            tree_graph[t2].insert(t1);
        }
    }
    unordered_set<int> visited;
    deque<int> Q;
    Q.push_back(1);
    visited.insert(1);
    while(!Q.empty()){
        int p_node = Q.front();
        Q.pop_front();
        for(int i = 0; i < N+1; i++){
            if(tree_graph[p_node].find(i) != tree_graph[p_node].end() && visited.find(i) == visited.end()){
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