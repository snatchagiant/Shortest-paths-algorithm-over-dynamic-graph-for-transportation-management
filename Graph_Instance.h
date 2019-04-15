#ifndef GRAPH_INSTANCE_H_INCLUDED
#define GRAPH_INSTANCE_H_INCLUDED

#include "CBST_Tree_Company.h"

class graph_instance
{
private:

    int **metropolis_inst; // fixed layout on graph with unit subgraph structure: root -> leaf set
    int **weight_list; // structure as well as graph
    int **Velocity_table;// 2D array for calculating velocity, the form is a Cartesian product by velocities times time intervals

    int *parent_list, // maintaining the search tree;
    *Lane_Length_list, // maintaining a lane length grades
    *Time_table, // a large linear list to carry the stats whose varying over time.
    *Choosing_Length_list;// carry the lengths for lane

    int Interruption, // define the upper bound for piecing length, if beyond, viewed as road block.
    num_row, num_column, subgraph_interval, nodes, Arcs, // network arguments: the number of nodes on row, column; and the arcs.
    time_table_size, // time table size
    sampling_scope, // the max sampling interval in table Velocity_table
    sampling_time_gap, // the sampling interval we want in project
    Velocity_grade; // the number of velocity grade

    bool successful; // monitor the initializing class instance in phase of loading various pointers
    bool initializing_instaces; // monitor initialzing various parameters.

    CBST *cbst; // the auxiliary CBST.

    // ----- + essential kits + ----- //
    int get_theSizeOfLeafSet(int root){if(root > 0 && root <= nodes) return metropolis_inst[root][0]; return -1;}
    int get_leafFromSubgraph(int root, int address){if(root > 0 && root <= nodes) return metropolis_inst[root][address]; return -1;}
    void updateInstanceRelation(int root, int offset, int item){metropolis_inst[root][offset] = item;}

    void assign_arcWeight(int index, int offset, int value){weight_list[index][offset] = value;}
    int get_arcWeight(int root, int address){return weight_list[root][address];}

    // ----- + Graph Basic Operations + ----- //
    int get_timeTableData(int index); // return the velocity grade from time table the querying with index available as offset in list
    int get_LandLength(int index); // return the length of link, index is offset in list
    int get_Velocity(int sampling_interval, int velocity_grade); // access a 2D array, the first parameter is row index, second is the column counterpart

    int get_LanebySubgraph(int root, int pos); // get the length of link via subgraph structure
    int wad_LeafSetWithElement(int root, int element); // squeeze an element into leaf set

    int push_ANodeIntoAList(int node, int *nodelist, int up_bound); // the scalar of nodelist is nodes
    int get_offsetInLeafSet(int root, int leaf);// return the offset in leaf set occupied by leaf with that root and leaf

    bool wad_CostListWithWeight(int root, int pos, int weight);// assign a number to weight list

    // ---- + Graph_Instance + -----//
    // make instance
    int T_MaketheMetropliesInst(); // make a fixed structure of graph
    bool T_MakeTimeTableIntoWave(); // make a time table where those numbers aligned likes multimodal
    bool T_MakeTimeTableIntoRandom(); // randomly assign those data into time table

    // ---- + Graph_Arithmetic_Travels + ---- //
    //about computing through time
    //a. only calculate a velocity and length in a time interval
    int T_PieceWiseCost(int velocity_grade, int velocity_interval, int Lane_length, Traffic_info *cord);
    // first parameter indicates the grade of velocity, the index on horizon in 2d velocity table
    // second is the index on vertical, which is the grade of time interval
    // third is the length needing to piecing
    // fourth is assisting structure
    // return the number of counting segments in piecing process

    //b. computing a piecewise function with initial departure time and length of lane
    int T_costfunction(int departure_time, int Lane_length, Traffic_info *cord); // calculate a through time

    //c. the shell of function T_costfunction which calculate a through time on a lane with those data of velocity in time table.
    int calculateTravelALink(int departure_time, int Lane_length, Traffic_info *cord);
    // the aim for design this function is purely to add some variable factor for querying time table.


    // ---- + Graph_ContestMethod_Module + ---- //
    int T_OptimizingAnEndpoint(int *Origins_list, int Endpoint, CBST *cbst, Traffic_info *cord)throw(Exception_about_memory);
    // This function is employed to correct or lable nodes.
    // the first argument is a list to store candidates for next search;
    // the third is online data cbst to support investigation
    // functor would return the number of new origins, that in list Origins_list.

public:

    graph_instance(int **graph, // a pointer to an instance with nth subgraph, 2-dimension array. the fixed topology relationship
                   int **weights, // a weighted list whose structure as same as instance, where each member is the index which pointer that member in Lane_Length_list, also is private member
                   int **V, // a 2d list to carry a computing table the Cartesian Product for time intervals times velocity.
                   int *R, // a linear list where index is parent list used to maintain a search tree.
                   int *L, // a linear list for record the length is the picked lane's
                   int *T, // a linear table of time-dependent table
                   int *Len, // a linear list carrying the grades of length for each lane.
                   int Through_Limit //the minimum velocity, which for precise truncated for casting through time.
                   ): metropolis_inst(graph), weight_list(weights), Velocity_table(V), parent_list(R), Lane_Length_list(L), Time_table(T), Choosing_Length_list(Len), Interruption(Through_Limit){
        successful = false;
        if(Lane_Length_list[0] <= 0)
            cout<<"\n failure in initializing data phase for array Lane_Length_list[0] less than and equal of 0"<<endl;
        else
        {
            if(Interruption <= 0) cout<<"\n failure in initializing data phase for argument Interruption less than and equal of 0"<<endl;
            else
                successful = true;
        }
        initializing_instaces = false;
    }

    bool initialize_anNetWork(int x, // the nodes number on row
                              int y, // the nodes number on column
                              int tts, // the size of time table
                              int Vg, // the number of velocity grades
                              int gap, // the wideth of leaf set of subgraph, the max outgoing degree
                              int sc, // the max velocity grade we want
                              int sf, // the time interval we actually select
                              int type);// the type of assignment in time table: wave (0) or random (1)

    bool get_initialize_status(){return initializing_instaces;}

    // ---- + Graph_HDM_Module + ---- //
    int T_DoHybirdDijkstraMethod(CBST *cbst, int *origins_list, int *regions, int *state_list, int source, Info_collection *info, Traffic_info *cord);
    // Execute initial process, people call labeling process.
    // Additionally, yielding a candiate list.

    // ---- + Graph_ContestMethod_Module + ---- //
    int T_ContestMethodCL(int *Origins_list, int Agency_of_CL, CBST *cbst, Traffic_info *cord)throw(Exception_about_memory);
    // This functor is used to handle a list of cousins which are the best ones for search

    // ---- + Graph_Arithmetic_Travels + ---- //
    //1. counting the number of s-t-pairs that in resulting search tree.
    INT64 T_computingPairs(int *status_list, INT64 *counter_list, int *node_list, int *relation_list);

    //2. the ratio of totally length over n-1 the average of length on each
    float T_calculateAverageLane();

    //3. the ratio of average through time over each link
    float T_calculateAverageTime(CBST *cbst);

    // --- + general operations + ---- //
    int get_Nodes(){return nodes;}
    int get_Arcs(){return Arcs;}

    // -------------------------+ Graph_Printing_Module +------------------------------- //
    void printGraphPartition(int *region_set, int *status_set);
    void printInstance();
    void printWeightList();
    void printTimeTable();
};

#endif // GRAPH_INSTANCE_H_INCLUDED
