#include "Graph_Instance.h"

int graph_instance::get_LandLength(int index)
{//get_LandLength
    if(index <= 0 || index > Lane_Length_list[0]) return -1;
    return Lane_Length_list[index];
}//get_LandLength

int graph_instance::get_timeTableData(int index)
{//get_timeTableData
    if(index < 0 || index >= time_table_size) return -1;
    return Time_table[index];
}//get_timeTableData

int graph_instance::get_Velocity(int sampling_interval, int velocity_grade)
{//get_Velocity
    if(velocity_grade < 0 || velocity_grade > Velocity_grade) return -1;
    if(sampling_interval < 0 || sampling_interval > sampling_time_gap) return -2;
    return Velocity_table[sampling_interval][velocity_grade];
}//get_Velocity

int graph_instance::get_LanebySubgraph(int root, int pos)
{//get_LanebyNodes
    if(root <= 0 || root > nodes || pos <= 0 || pos > subgraph_interval) return -1;

    int w = get_arcWeight(root, pos); return get_LandLength(w);
}//get_LanebyNodes

int graph_instance::wad_LeafSetWithElement(int root, int element)
{//wad_LeafSetWithElement
    int offset = get_theSizeOfLeafSet(root);
    if(offset < 0) offset = 0; else if(offset >= subgraph_interval) return -1;
    offset ++; metropolis_inst[root][offset] = element; metropolis_inst[root][0] = offset;
    return offset;
}//wad_LeafSetWithElement

int graph_instance::push_ANodeIntoAList(int node, int *nodelist, int up_bound)
{//push_ANodeIntoAList
    int address = nodelist[0]; if(address < 0) address = 0;
    address ++; if(address > up_bound) return 0;
    nodelist[0] = address; nodelist[address] = node;
    return address;
}//push_ANodeIntoAList

int graph_instance::get_offsetInLeafSet(int root, int leaf)
{//get_offsetInLeafSet
    int limit = get_theSizeOfLeafSet(root); int pos = -1;
    for(int n = 1; n <= limit; n++){
        if(metropolis_inst[root][n] != leaf) continue;
        pos = n; break;
    }
    return pos;
}//get_offsetInLeafSet

bool graph_instance::wad_CostListWithWeight(int root, int pos, int weight)
{//wad_CostListWithWeight
    if(pos > subgraph_interval || pos <= 0) return false;
    weight_list[root][pos] = weight;
    return true;
}//wad_CostListWithWeight



