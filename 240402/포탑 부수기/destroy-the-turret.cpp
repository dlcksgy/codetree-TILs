#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
using namespace std;

#define INF 100000

vector<pair<int,int>> attackers;

pair<int,int> find_min(const vector<vector<int>> &m){
    int min = INF;
    int x,y;
    for(int i = 0; i < m.size(); i++){
        for(int j = 0; j < m[0].size(); j++){
            if(m[i][j] == 0) continue;  //부서진 포탑 제외
            if(m[i][j] < min){
                x = j;
                y = i;
                min = m[i][j];
            }else if(m[i][j] == min){
                pair<int,int> o(x,y);
                pair<int,int> n(j,i);
                auto oi = find_end(attackers.begin(),attackers.end(),&o,&o+1);
                auto ni = find_end(attackers.begin(),attackers.end(),&n,&n+1);
                int oi_int = distance(attackers.begin(),oi);
                int ni_int = distance(attackers.begin(),ni);
                if(ni_int > oi_int){
                    x = j;
                    y = i;
                    min = m[i][j];
                }else if(ni_int == oi_int){
                    int o_xy = x+y;
                    int n_xy = j+i;
                    if(n_xy < o_xy){
                        x = j;
                        y = i;
                        min = m[i][j];
                    }else if(n_xy == o_xy){
                        if(j < x){
                            x = j;
                            y = i;
                            min = m[i][j];
                        }
                    }
                }
            }
        }
    }
    return pair<int,int>(x,y);
}


pair<int,int> find_max(const vector<vector<int>> &m){
    int max = 0;
    int x,y;
    for(int i = 0; i < m.size(); i++){
        for(int j = 0; j < m[0].size(); j++){
            if(m[i][j] == 0) continue;  //부서진 포탑 제외
            if(m[i][j] > max){
                x = j;
                y = i;
                max = m[i][j];
            }else if(m[i][j] == max){
                pair<int,int> o(x,y);
                pair<int,int> n(j,i);
                auto oi = find_end(attackers.begin(),attackers.end(),&o,&o+1);
                auto ni = find_end(attackers.begin(),attackers.end(),&n,&n+1);
                int oi_int = distance(attackers.begin(),oi);
                int ni_int = distance(attackers.begin(),ni);
                if(ni_int < oi_int){
                    x = j;
                    y = i;
                    max = m[i][j];
                }else if(ni_int == oi_int){
                    int o_xy = x+y;
                    int n_xy = j+i;
                    if(n_xy > o_xy){
                        x = j;
                        y = i;
                        max = m[i][j];
                    }else if(n_xy == o_xy){
                        if(j > x){
                            x = j;
                            y = i;
                            max = m[i][j];
                        }
                    }
                }
            }
        }
    }
    return pair<int,int>(x,y);
}
//N,y : 행
//M,x : 열
//레이저가 가능하면 방향 벡터를 반환. 가능하지 않다면 빈 벡터를 반환
vector<int> find_root(const vector<vector<int>>& matrix, pair<int,int> start, pair<int,int> target){
    const int dx[] = {1,0,-1,0};    //우 하 좌 상
    const int dy[] = {0,1,0,-1};
    const int N = matrix.size();
    const int M = matrix[0].size();
    deque<pair<pair<int,int>,vector<int>>> Q;  //큐에 담기는 정보: 시작 좌표, 지금까지 경로
    vector<vector<int>> dist(N,vector<int>(M,INF));
    Q.emplace_back(start,vector<int>({}));
    dist[start.second][start.first] = 0;
    while(!Q.empty()){
        pair<pair<int,int>,vector<int>> temp = Q.front();
        Q.pop_front();
        int x = temp.first.first;
        int y = temp.first.second;
        //cout << x << "/" << y << endl;
        for(int i = 0; i < 4; i++){
            int nx = (x + dx[i]) % M;
            nx = nx < 0 ? nx+M : nx;
            int ny = (y + dy[i]) % N;
            ny = ny < 0 ? ny+N : ny;
            if(matrix[ny][nx] != 0 && dist[ny][nx] == INF){
                pair<int,int> n(nx,ny);
                vector<int> root = temp.second;
                root.push_back(i);
                if(n == target){;
                    return root;
                }
                dist[ny][nx] = root.size();
                Q.emplace_back(n,root);
            }
        }
    }
    return vector<int>({});
}

void repair(vector<vector<int>>& matrix, const vector<pair<int,int>> no_repair){
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j < matrix[0].size(); j++){
            if(find(no_repair.begin(),no_repair.end(),make_pair(j,i)) == no_repair.end() && matrix[i][j] != 0){
                matrix[i][j]++;
            }
        }
    }
}
void bomb_attack_and_repair(vector<vector<int>>& matrix, const pair<int,int>& attacker, const pair<int,int>& target, const int& damage){
    const int dx[] = {1,1,0,-1,-1,-1,0,1};
    const int dy[] = {0,1,1,1,0,-1,-1,-1};
    const int N = matrix.size();
    const int M = matrix[0].size();
    vector<pair<int,int>> no_repair;
    no_repair.push_back(attacker);
    no_repair.push_back(target);
    int x = target.first;
    int y = target.second;
    matrix[y][x] -= damage;
    if(matrix[y][x] < 0) matrix[y][x] = 0;
    for(int i = 0; i < 8; i++){
        int nx = (x + dx[i]) % M;
        nx = nx < 0 ? nx+M : nx;
        int ny = (y + dy[i]) % N;
        ny = ny < 0 ? ny+N : ny;
        matrix[ny][nx] -= damage/2;
        if(matrix[y][x] < 0) matrix[y][x] = 0;
        no_repair.emplace_back(nx,ny);
    }
    repair(matrix,no_repair);
}
void laser_attack_and_repair(vector<vector<int>>& matrix, const pair<int,int>& attacker, const vector<int>& laser_root,const int& attack_damage){
    const int dx[] = {1,0,-1,0};
    const int dy[] = {0,1,0,-1};
    int x = attacker.first;
    int y = attacker.second;
    vector<pair<int,int>> no_repair;
    no_repair.emplace_back(x,y);
    for(int i = 0; i < laser_root.size(); i++){
        x += dx[laser_root[i]];
        y += dy[laser_root[i]];
        //cout << x << "/" << y << endl;
        if(i == laser_root.size()-1){
            matrix[y][x] -= attack_damage;
        }else{
            matrix[y][x] -= attack_damage/2;
        }
        if(matrix[y][x] < 0) matrix[y][x] = 0;
        no_repair.emplace_back(x,y);
    }
    repair(matrix,no_repair);
}
int find_remains(const vector<vector<int>>& m){
    int ret = 0;
    for(int i = 0; i < m.size(); i++){
        for(int j = 0; j < m[0].size(); j++){
            if(m[i][j] != 0){
                ret++;
            }else if(m[i][j] < 0) return -1;
        }
    }
    return ret;
}

int main() {
    // 여기에 코드를 작성해주세요.
    int N, M;
    int K;
    cin >> N >> M >> K;
    
    vector<vector<int>> TURRETS(N,vector<int>(M));
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            cin >> TURRETS[i][j];
        }
    }
    while(K != 0){
        pair<int,int> attacker = find_min(TURRETS);

        int attack_damage = TURRETS[attacker.second][attacker.first] + N+M; //공격자 공격력 업
        TURRETS[attacker.second][attacker.first] = attack_damage;
        pair<int,int> target = find_max(TURRETS);
        //cout << "target" << target.first << "/" << target.second << '\n';
        attackers.push_back(attacker);
        vector<int> laser_root = find_root(TURRETS,attacker,target);

        // for(int i = 0; i < laser_root.size(); i++){
        //     cout << laser_root[i] << " ";
        // }
        // cout << endl;

        if(laser_root.size() == 0){
            bomb_attack_and_repair(TURRETS,attacker,target,attack_damage);
        }else{
            laser_attack_and_repair(TURRETS,attacker,laser_root,attack_damage);
        }
        int remain = find_remains(TURRETS);
        if(remain == 1) break;
        if(remain == -1) cout << "error code 0" << "\n";
        K--;
    }
    
    // for(int i = 0; i < N; i++){
    //     for(int j = 0; j < M; j++){
    //         cout << TURRETS[i][j] << "/";
    //     }
    //     cout << endl;
    // }
    pair<int,int> answer = find_max(TURRETS);
    cout << TURRETS[2][3] << "\n";
    return 0;
}