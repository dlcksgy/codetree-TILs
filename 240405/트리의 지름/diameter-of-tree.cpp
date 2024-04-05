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
    //[노드][노드]=가중치
    unordered_map<int,unordered_map<int,int>> w_tree;
    deque<pair<int,int>> Q;
    unordered_set<int> visited;
    for(int i = 1; i < N; i++){
        int n1,n2,w;
        cin >> n1 >> n2 >> w;
        if(w_tree.find(n1) == w_tree.end()){
            w_tree.emplace(n1,unordered_map<int,int>());
            w_tree[n1].emplace(n2,w);
        }else{
            w_tree[n1].emplace(n2,w);
        }
        if(w_tree.find(n2) == w_tree.end()){
            w_tree.emplace(n2,unordered_map<int,int>());
            w_tree[n2].emplace(n1,w);
        }else{
            w_tree[n2].emplace(n1,w);
        }
    }
    Q.emplace_back(1,0);
    visited.insert(1);
    int far1 = 0;
    int max_dist = 0;
    while(!Q.empty()){
        int node = Q.front().first;
        int dist_sum = Q.front().second;
        Q.pop_front();
        for(auto i = w_tree[node].begin(); i != w_tree[node].end(); i++){
            int n_node =  i->first;
            int dist = i->second;
            if(visited.find(n_node) == visited.end()){
                Q.emplace_back(n_node,dist_sum + dist);
                visited.insert(n_node);
                if(dist_sum + dist > max_dist){
                    max_dist = dist_sum + dist;
                    far1 = n_node;
                }
            }
        }
    }
    
    Q.emplace_back(far1,0);
    visited = unordered_set<int>();
    visited.insert(far1);
    int far2;
    max_dist = 0;
    while(!Q.empty()){
        int node = Q.front().first;
        int dist_sum = Q.front().second;
        Q.pop_front();
        for(auto i = w_tree[node].begin(); i != w_tree[node].end(); i++){
            int n_node =  i->first;
            int dist = i->second;
            if(visited.find(n_node) == visited.end()){
                Q.emplace_back(n_node,dist_sum + dist);
                visited.insert(n_node);
                if(dist_sum + dist > max_dist){
                    max_dist = dist_sum + dist;
                    far2 = n_node;
                }
            }
        }
    }
    cout << max_dist << '\n';

    return 0;
}