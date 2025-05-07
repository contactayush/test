#include <iostream>
#include <omp.h>
#include <vector>

using namespace std;

void seq_bs(vector<int> &arr){
    int n=arr.size();
    
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if(arr[j]>arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void para_bs(vector<int> &arr){
    int n=arr.size();

    #pragma omp parallel
    {
    for(int i=0;i<n-1;i++){
        #pragma omp for
        for(int j=0;j<n-i-1;j++){
            if(arr[j]>arr[j+1]){
                swap(arr[j],arr[j+1]);
            }
        }
    }
}
}

void merge(vector<int> &arr,int left,int mid,int right){
    int n1=mid-left+1;
    int n2=right-mid;

    vector<int> L(n1);
    vector<int> R(n2);

    for(int i=0;i<n1;i++){
        L[i]=arr[left+i];
    }

    for(int i=0;i<n2;i++){
        R[i]=arr[mid+1+i];
    }

    int i=0,j=0,k=left;

    while(i<n1 && j<n2){
        if(L[i]<=R[i]){
          arr[k]=L[i];
          i++;
        }
        else{
            arr[k]=R[j];
            j++;
        }
        k++;
    }

    while(i<n1){
        arr[k]=L[i];
        i++;
        k++;
    }

    while(j<n2){
        arr[k]=R[j];
        j++;
        k++;
    }
}

void seq_ms(vector<int>&arr,int left,int right){
    if(left<right){
        int mid=left+(right-left)/2;
        seq_ms(arr,left,mid);
        seq_ms(arr,mid+1,right);
        merge(arr,left,mid,right);
    }
}

void para_ms(vector<int>&arr,int left,int right){
    if(left<right){
        int mid=left+(right-left)/2;
        #pragma omp sections
        {

        #pragma omp section
        seq_ms(arr,left,mid);

        #pragma omp section
        seq_ms(arr,mid+1,right);

        #pragma omp section
        merge(arr,left,mid,right);
        }
    }
}





int main(){
    int n;
    cout<<"Enter the size of array:"<<endl;
    cin>>n;

    vector<int>arr(n);

    for(int i=0;i<arr.size();i++){
        arr[i]=rand()%1000;
    }

    std::vector<int> arr1 = arr;
    double start_time = omp_get_wtime();
    seq_bs(arr1);
    double end_time = omp_get_wtime();
    double sequential_time_bubble=(end_time - start_time);
    std::cout << "Sequential Bubble Sort Time: " <<sequential_time_bubble<< " seconds." << std::endl;

    
    std::vector<int> arr2 = arr;
    start_time = omp_get_wtime();
    para_bs(arr2);
    end_time = omp_get_wtime();
    double parallel_time_bubble=(end_time - start_time);
    std::cout << "Parallel Bubble Sort Time: " <<parallel_time_bubble<< " seconds." << std::endl;
    double speedupBubble = sequential_time_bubble/parallel_time_bubble;
    std::cout << "Bubble Sort Speedup: " << speedupBubble << std::endl;

  
    std::vector<int> arr3 = arr;
    start_time = omp_get_wtime();
    seq_ms(arr3, 0, arr3.size() - 1);
    end_time = omp_get_wtime();
    double sequential_time_merge=(end_time - start_time);
    std::cout << "Sequential Merge Sort Time: " <<sequential_time_merge<< " seconds." << std::endl;

    
    std::vector<int> arr4 = arr;
    start_time = omp_get_wtime();
    para_ms(arr4, 0, arr4.size() - 1);
    end_time = omp_get_wtime();
    double parallel_time_merge=(end_time - start_time);
    std::cout << "Parallel Merge Sort Time: " <<parallel_time_merge<< " seconds." << std::endl;
    double speedupMerge = sequential_time_merge/parallel_time_merge;
    std::cout << "Merge Sort Speedup: " << speedupMerge << std::endl;

    return 0;
}

