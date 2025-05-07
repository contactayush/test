#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void sequential_min(vector<int> &arr,int &min_val, double &seq_time){
    double start=omp_get_wtime();
    min_val=arr[0];
    for(int i=1;i<arr.size();i++){
        if(arr[i]<min_val){
            min_val=arr[i];
        }
    }
    double end=omp_get_wtime();
    seq_time=end-start;
    cout<<"MIN:"<<min_val<<endl;
    cout<<"Sequential Time:"<<seq_time<<endl;
}

void parallel_min(vector<int> &arr,double &seq_time){
    double start=omp_get_wtime();
    int min_val=arr[0];
    #pragma omp parallel for reduction(min:min_val)
    for(int i=1;i<arr.size();i++){
        if(arr[i]<min_val){
            min_val=arr[i];
        }
    }
    double end=omp_get_wtime();
    double para_time=end-start;
    cout<<"MIN:"<<min_val<<endl;
    cout<<"Parallel Time:"<<para_time<<endl;
    cout<<"Speedup:"<<seq_time/para_time<<endl;
}

void sequential_max(vector<int> &arr,int &max_val, double &seq_time){
    double start=omp_get_wtime();
    max_val=arr[0];
    for(int i=1;i<arr.size();i++){
        if(arr[i]>max_val){
            max_val=arr[i];
        }
    }
    double end=omp_get_wtime();
    seq_time=end-start;
    cout<<"MAX:"<<max_val<<endl;
    cout<<"Sequential Time:"<<seq_time<<endl;
}

void parallel_max(vector<int> &arr,double &seq_time){
    double start=omp_get_wtime();
    int max_val=arr[0];
    #pragma omp parallel for reduction(max:max_val)
    for(int i=1;i<arr.size();i++){
        if(arr[i]>max_val){
            max_val=arr[i];
        }
    }
    double end=omp_get_wtime();
    double para_time=end-start;
    cout<<"MAX:"<<max_val<<endl;
    cout<<"Parallel Time:"<<para_time<<endl;
    cout<<"Speedup:"<<seq_time/para_time<<endl;
}

void sequential_sum(vector<int> &arr,int &sum_val,double &seq_time){
    double start=omp_get_wtime();
    sum_val=0;
    for(int i=0;i<arr.size();i++){
        sum_val+=arr[i];
    }
    double end=omp_get_wtime();
    seq_time=end-start;
    cout<<"SUM:"<<sum_val<<endl;
    cout<<"Sequential Time:"<<seq_time<<endl;
}

void parallel_sum(vector<int> &arr,double &seq_time){
    double start=omp_get_wtime();
    int sum_val=0;
    #pragma omp parallel for reduction(+:sum_val)
    for(int i=0;i<arr.size();i++){
        sum_val+=arr[i];
    }
    double end=omp_get_wtime();
    double para_time=end-start;
    cout<<"SUM:"<<sum_val<<endl;
    cout<<"Parallel Time:"<<para_time<<endl;
    cout<<"Speedup:"<<seq_time/para_time<<endl;
}

void sequential_avg(vector<int> &arr,double &avg_val,double &seq_time){
    double start=omp_get_wtime();
    int sum_val=0;
    for(int i=0;i<arr.size();i++){
        sum_val+=arr[i];
    }
    avg_val=sum_val/arr.size();
    double end=omp_get_wtime();
    seq_time=end-start;
    cout<<"AVG:"<<avg_val<<endl;
    cout<<"Sequential Time:"<<seq_time<<endl;
}

void parallel_avg(vector<int> &arr,double &seq_time){
    double start=omp_get_wtime();
    int sum_val=0;
    #pragma omp parallel for reduction(+:sum_val)
    for(int i=0;i<arr.size();i++){
        sum_val+=arr[i];
    }
    double avg_val=sum_val/arr.size();
    double end=omp_get_wtime();
    double para_time=end-start;
    cout<<"AVG:"<<avg_val<<endl;
    cout<<"Parallel Time:"<<para_time<<endl;
    cout<<"Speedup:"<<seq_time/para_time<<endl;
}

int main(){
    int n;
    cout<<"Enter the value of n:"<<endl;
    cin>>n;

    vector<int> arr(n);
    for(int i=0;i<arr.size();i++){
        arr[i]=rand()%10000;
    }

    int max_val,min_val,sum_val;
    double seq_time;
    double avg_val;

    cout<<"======================================="<<endl;
    cout<<"MIN"<<endl;
    sequential_min(arr,min_val,seq_time);
    cout<<endl;
    parallel_min(arr,seq_time);
    cout<<"========================================"<<endl;

    cout<<"======================================="<<endl;
    cout<<"MAX"<<endl;
    sequential_max(arr,max_val,seq_time);
    cout<<endl;
    parallel_max(arr,seq_time);
    cout<<"========================================"<<endl;

    cout<<"======================================="<<endl;
    cout<<"SUM"<<endl;
    sequential_sum(arr,sum_val,seq_time);
    cout<<endl;
    parallel_sum(arr,seq_time);
    cout<<"========================================"<<endl;

    cout<<"======================================="<<endl;
    cout<<"AVG"<<endl;
    sequential_avg(arr,avg_val,seq_time);
    cout<<endl;
    parallel_avg(arr,seq_time);
    cout<<"========================================"<<endl;


    return 0;
}








