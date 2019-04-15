#include "Graph_Instance.h"

void graph_instance::printInstance()
{//printInstance
    cout<<endl;
    for(int n = 1; n <= nodes; n++){
        cout<<n<<". --> ";
        int limit = get_theSizeOfLeafSet(n);
        for(int m = 1; m <= limit; m++){
            cout<<get_leafFromSubgraph(n, m); if(m != subgraph_interval) cout<<"; "; else cout<<". ";
        }
        cout<<endl;
    }
}//printInstance

void graph_instance::printGraphPartition(int *region_set, int *status_set)
{//printGraphPartition
    int index = 0;
    if(region_set[0] > 0){
        for(int n = 1; n <= region_set[0]; n++)
        {
            int item = region_set[n];
            if(status_set[item] != index){
                cout<<"\n"<<item<<";"; index = status_set[item];
            }
            else
                cout<<item<<";";}
    }
}//printGraphPartition

void graph_instance::printWeightList()
{//printWeightList
    for(int n = 1; n <= nodes; n++){
        cout<<endl;
        for(int m = 1; m <= get_theSizeOfLeafSet(n); m ++){
            //int leaf = metropolis_inst[n][m];
            cout<<"Weight_list["<<n<<"]["<<m<<"] = "<<get_arcWeight(n, m)<<"; ";
        }
    }
}//printWeightList

void graph_instance::printTimeTable()
{//printTimeTable
    cout<<endl;
    for(int n = 0; n < time_table_size; n++){
        cout<<Time_table[n]<<"; ";
    }
    cout<<endl;
}//printTimeTable
