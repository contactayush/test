#include <iostream>
#include <chrono>
#include <vector>
#include <omp.h>
#include <stack>
#include <queue>
#include <random>

using namespace std;

class Graph{
    public:
    int vertices;
    vector<vector<int>> graph;
    vector<bool> visited;

    Graph(int v,int e):vertices(v){
        graph.resize(vertices);
        generate_random_graph(e);
    }

    void generate_random_graph(int edges){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(0,vertices-1);

        for(int i=0;i<edges;i++){
            int u=dist(gen);
            int v=dist(gen);

            if(u!=v){
                graph[u].push_back(v);
                graph[v].push_back(u);
            }
        }
    }

    void initialize_visited(){
        visited.assign(vertices,false);
    }

    void parallel_dfs(int start){
         visited[start]=true;
#pragma omp parallel
{
            stack<int> local_stack;
            local_stack.push(start);

            while(!local_stack.empty()){
                int current;
#pragma omp critical
{
                    if(!local_stack.empty()){
                        current=local_stack.top();
                        local_stack.pop();
                    }
                    else{
                        current=-1;
                    }
}
                if(current==-1) continue;

                for(int neighbour:graph[current]){
                    if(!visited[neighbour]){
                        visited[neighbour]=true;

#pragma omp critical
{
                            local_stack.push(neighbour);
}
                    }
                }
            }
         }
    }

    void parallel_bfs(int start){
        visited[start]=true;
        queue<int> q;
        q.push(start);

#pragma omp parallel
{
            queue<int> local_queue;
            while(!q.empty()){
                int current;
#pragma omp critical
{
                    if(!q.empty()){
                        current=q.front();
                        q.pop();
                    }
                    else{
                        current=-1;
                    }
                }
                if(current==-1) continue;

                for(int neighbour:graph[current]){
                    if(!visited[neighbour]){
                        visited[neighbour]=true;

#pragma omp critical
{
                            local_queue.push(neighbour);
}
                    }
                }

#pragma omp critical
{
                    if(!local_queue.empty()){
                        q.push(local_queue.front());
                        local_queue.pop();
                    }
}
            }
        }
    }

    void dfs(int start){
        visited[start]=true;
        stack<int> st;
        st.push(start);
        while(!st.empty()){
            int current=st.top();
            st.pop();
            for(int neighbour:graph[current]){
                if(!visited[neighbour]){
                    st.push(neighbour);
                    visited[neighbour]=true;
                }
            }
        }
    }

    void bfs(int start){
        visited[start]=true;
        queue<int> q;
        q.push(start);
        while(!q.empty()){
            int current=q.front();
            q.pop();
            for(int neighbour:graph[current]){
                if(!visited[neighbour]){
                    q.push(neighbour);
                    visited[neighbour]=true;
                }
            }
        }
    }

    
};



int main(){

    omp_set_num_threads(omp_get_max_threads());

    int V=1000000;
    int E=10000000;

    // cout<<"Enter the number of vertices:"<<endl;
    // cin>>V;

    // cout<<"Enter the number of edges:"<<endl;
    // cin>>E;

    Graph g(V,E);

    double seq_time_dfs,para_time_dfs,seq_time_bfs,para_time_bfs;

    g.initialize_visited();
    auto start=chrono::high_resolution_clock::now();
    g.bfs(0);
    auto end=chrono::high_resolution_clock::now();
    seq_time_bfs=chrono::duration<double>(end-start).count();
    cout<<"Sequential Time(BFS):"<<seq_time_bfs<<endl;

    g.initialize_visited();
    start=chrono::high_resolution_clock::now();
    g.parallel_bfs(0);
    end=chrono::high_resolution_clock::now();
    para_time_bfs=chrono::duration<double>(end-start).count();
    cout<<"Parallel Time(BFS):"<<para_time_bfs<<endl;

    g.initialize_visited();
    start=chrono::high_resolution_clock::now();
    g.dfs(0);
    end=chrono::high_resolution_clock::now();
    seq_time_dfs=chrono::duration<double>(end-start).count();
    cout<<"Sequential Time(DFS):"<<seq_time_dfs<<endl;

    g.initialize_visited();
    start=chrono::high_resolution_clock::now();
    g.parallel_dfs(0);
    end=chrono::high_resolution_clock::now();
    para_time_dfs=chrono::duration<double>(end-start).count();
    cout<<"Parallel Time(DFS):"<<para_time_dfs<<endl;

    cout<<"BFS Speedup:"<<(para_time_bfs> 0?seq_time_bfs/para_time_bfs:0)<<endl;
    cout<<"DFS Speedup:"<<(para_time_dfs> 0?seq_time_dfs/para_time_dfs:0)<<endl;

    return 0;

}
