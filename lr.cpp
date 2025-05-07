#include <iostream>
#include <omp.h>
#include <vector>

using namespace std;


void sequential_lr(vector<double> &a,vector<double> &b,double &seq_time,double &beta0_seq,double &beta1_seq){

    double start=omp_get_wtime();

    int n=a.size();
    double sum_x=0.0,sum_y=0.0,sum_xy=0.0,sum_x2=0.0;

    for(int i=0;i<n;i++){
        sum_x+=a[i];
        sum_y+=b[i];
        sum_xy+=a[i]*b[i];
        sum_x2+=a[i]*a[i];
    }
 

    beta1_seq=(n*sum_xy-sum_x*sum_y)/(n*sum_x2-sum_x*sum_x);
    beta0_seq=(sum_y-beta1_seq*sum_x)/n;

    double end=omp_get_wtime();
    seq_time=end-start;
    
    cout<<"beta0:"<<beta0_seq<<",beta1:"<<beta1_seq<<endl;
    cout<<"Linear Equation is:"<<"y="<<beta1_seq<<"x +"<<beta0_seq<<endl;

}


void parallel_lr(vector<double> &a,vector<double> &b,double &para_time,double &beta0_para,double &beta1_para){

    double start=omp_get_wtime();
    int n=a.size();

    double sum_x=0.0,sum_y=0.0,sum_xy=0.0,sum_x2=0.0;

    #pragma omp parallel for reduction(+:sum_x,sum_y,sum_xy,sum_x2)
    for(int i=0;i<n;i++){
        sum_x+=a[i];
        sum_y+=b[i];
        sum_xy+=a[i]*b[i];
        sum_x2+=a[i]*a[i];
    }


    beta1_para=(n*sum_xy-sum_x*sum_y)/(n*sum_x2-sum_x*sum_x);
    beta0_para=(sum_y-beta1_para*sum_x)/n;

    double end=omp_get_wtime();
    para_time=end-start;

    cout<<"beta0:"<<beta0_para<<",beta1:"<<beta1_para<<endl;
    cout<<"Linear Equation is:"<<"y="<<beta1_para<<"x +"<<beta0_para<<endl;
}

int main(){
    int n;
    cout<<"Enter the value of n:"<<endl;
    cin>>n;

    vector<double>a(n),b(n);

    for(int i=0;i<n;i++){
        a[i]=i;
        b[i]=6.0*i+3.0;
    }

    double beta0_seq,beta1_seq,beta0_para,beta1_para,seq_time,para_time;

    cout<<"Serial Execution:"<<endl;
    sequential_lr(a,b,seq_time,beta0_seq,beta1_seq);
    cout<<"Serial Time"<<seq_time<<endl;
     
    cout<<"==========================================="<<endl;

    cout<<"Parallel Execution:"<<endl;
    parallel_lr(a,b,para_time,beta0_para,beta1_para);
    cout<<"Parallel Time:"<<para_time<<endl;
    cout<<"Speedup:"<<seq_time/para_time<<endl;
    return 0;
}