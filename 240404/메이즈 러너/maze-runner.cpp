#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#define INF 1000000;

bool comp_player(const pair<int,int> &a, const pair<int,int> &b){
    if(a.first == b.first) return a.second < b.second;
    return a.first < b.first;
}

bool is_road(const vector<vector<int>>& MAZE, const int& y, const int& x){
    if(MAZE.size() <= y) return false;
    else if(MAZE[0].size() <= x) return false;
    else if(y < 0) return false;
    else if(x < 0) return false;
    else if(MAZE[y][x] != 0) return false;
    else return true;
}

int player_move(const vector<vector<int>>& MAZE, pair<int,int>& player, pair<int,int> out){
    if(out.first < player.first && is_road(MAZE,player.first-1,player.second)){
        player.first--;
    }else if(out.first > player.first && is_road(MAZE,player.first+1,player.second)){
        player.first++;
    }else if(out.second < player.second && is_road(MAZE,player.first,player.second-1)){
        player.second--;
    }else if(out.second > player.second && is_road(MAZE,player.first,player.second+1)){
        player.second++;
    }else{
        return 0;
    }
    return 1;
}

//out 과 player의 x,y좌표 차이의 최댓값이 최소인 player를 골라 정사각형을 만듬.
vector<int> search_rectangle(const vector<vector<int>>& MAZE, const pair<int,int> out, vector<pair<int,int>> players){
    const int N = MAZE.size();
    const int ry = out.first;
    const int rx = out.second;
    int rectangle_min = INF;
    int px_tg,py_tg;
    vector<int> small;
    for(int i = 0; i < players.size(); i++){
        int dif_y = ry - players[i].first;
        dif_y = dif_y < 0 ? dif_y * -1 : dif_y;
        int dif_x = rx - players[i].second;
        dif_x = dif_x < 0 ? dif_x * -1 : dif_x;
        int lg = dif_x > dif_y ? dif_x : dif_y;
        if(rectangle_min > lg){
            rectangle_min = lg;
            small = vector<int>({i});
        }else if(rectangle_min == lg){
            small.push_back(i);
        }
    }
    int sy, sx;
    int ly = INF;
    int lx = INF;
    for(int i = 0; i < small.size();i++){
        int ly_t,lx_t;
        py_tg = players[small[i]].first;
        px_tg = players[small[i]].second;
        ly_t = ry > py_tg ? ry : py_tg;
        ly_t = rectangle_min > ly_t ? rectangle_min : ly_t;
        lx_t = rx > px_tg ? rx : px_tg;
        lx_t = rectangle_min > lx_t ? rectangle_min : lx_t;
        if(ly_t < ly){
            ly = ly_t;
            lx = lx_t;
        }else if(ly_t == ly && lx_t < lx){
            ly = ly_t;
            lx = lx_t;
        }
    }
    sy = ly - rectangle_min;
    sx = lx - rectangle_min;

    return vector<int>({sy,sx,ly,lx});
}
void maze_spin(vector<vector<int>>& MAZE, pair<int,int> &out, vector<pair<int,int>> &players,
                            const int &sy, const int &sx, const int &ly, const int &lx){
    int N = lx - sx + 1;
    vector<vector<int>> temp(N,vector<int>(N));
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j++){
            temp[i][j] = MAZE[sy+i][sx+j];
            if(temp[i][j] > 0) temp[i][j]--;
        }
    }
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j++){
            MAZE[sy+j][lx-i] = temp[i][j];
        }
    }
    int of = out.first;
    int os = out.second;
    out.first = sy + (os - sx);
    out.second = lx - (of - sy);
    for(int i = 0; i < players.size(); i++){
        if(sy <= players[i].first && players[i].first  <= ly && sx <= players[i].second && players[i].second <= lx ){
            int pf = players[i].first;
            int ps = players[i].second;
            players[i].first = sy + (ps - sx);
            players[i].second = lx - (pf - sy);
        }
    }
}

int escape_check(const pair<int,int> &out, vector<pair<int,int>> &players){
    int ret = 0;
    for(vector<pair<int,int>>::iterator i = players.begin(); i != players.end();){
        if(*i == out){
            i = players.erase(i);
            ret++;
        }else{
            i++;
        }
    }
    return ret;
}



int main() {
    int N, M, K;
    cin >> N >> M >> K;
    int distance_sum = 0;
    vector<vector<int>> MAZE(N,vector<int>(N,0));
    vector<pair<int,int>> players;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            cin >> MAZE[i][j];
        }
    }
    for(int i = 0; i < M; i++){
        int player_y, player_x;
        cin >> player_y >> player_x;
        players.emplace_back(player_y-1,player_x-1);
    }
    pair<int,int> out;
    cin >> out.first >> out.second;
    out.first--;
    out.second--;
    // K = 2;
    while(K != 0){
        for(int i = 0; i < players.size(); i++){
            distance_sum += player_move(MAZE,players[i],out);
        }
        escape_check(out,players);
        if(players.size() == 0) break;
        vector<int> spin_area = search_rectangle(MAZE,out,players);
        maze_spin(MAZE,out,players,spin_area[0],spin_area[1],spin_area[2],spin_area[3]);
        
        K--;
    }
    cout << distance_sum <<'\n';
    cout << out.first+1 << " " << out.second+1 << '\n';



    return 0;
}