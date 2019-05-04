#include "Graph_Instance.h"

using namespace std;
//---------------- functions for compute through time -----------------------//

int graph_instance::T_PieceWiseCost(int velocity_grade, int velocity_interval, int Lane_length, Traffic_info *cord)
{//T_PieceWiseCost

    //1. gain the corresponding velocity
    cord->SubQueryTime = 0; cord->QueryTimes = 0;
    if(velocity_interval < sampling_time_gap) cord->SubQueryTime ++;

    int through_time = velocity_interval;
    int velocity = get_Velocity(velocity_interval, velocity_grade); // get the corresponding velocity
    if(velocity < 0) return -1;

    cord->Lane = Lane_length - velocity;
    //2. velocity does not cover the lane; default islane deyond velocity.
    if(cord->Lane < 0){
        velocity = get_Velocity(1, velocity_grade); cord->SubQueryTime ++; if(velocity < 0) return -2;
        through_time = Lane_length / velocity; int l = Lane_length % velocity; velocity = velocity >> 1;
        if(l > velocity) through_time ++;
        cord->Lane = 0;
    }
    return through_time;
}//T_PieceWiseCost

int graph_instance::T_costfunction(int departure_time, int Lane_length, Traffic_info *cord)
{//T_costfunction
    if(departure_time < 0) departure_time = 0; if(Lane_length <= 0) return -1;
    //1. gain the detial pos in time interval
    int time_stamp = departure_time % sampling_time_gap; time_stamp = sampling_time_gap - time_stamp;

    //2. transform the departure time to time table index;
    // renew to let query index fall in the range of time table if the value beyond the size of table
    int query_index = departure_time / sampling_time_gap;
    while(query_index >= time_table_size) query_index =  query_index - time_table_size;

    //3. calculate the through time
    int through_time = 0, Lane = Lane_length, shield_counter = Interruption + 1; Traffic_info info;
    cord->QueryTimes = 0; cord->SubQueryTime = 0;

    while(Lane > 0){
        //3.1. query time table for velocity
        int velocity_grade = get_timeTableData(query_index); cord->QueryTimes ++;

        //3.2. cost the through time about velocity and lane
        int t = T_PieceWiseCost(velocity_grade, time_stamp, Lane, &info); if(t < 0) return -2;

        //3.3. accumulate the total through time
        through_time = through_time + t; cord->SubQueryTime = cord->SubQueryTime + info.SubQueryTime;
        Lane = info.Lane;

        //3.4. if no travel through lane, then go on
        if(Lane > 0){
            time_stamp = sampling_time_gap;
            query_index ++; if(query_index >= time_table_size) query_index = 0;
        } else break;

        //3.5. shield function to prevent from no convergence for PAW when input a seiral zero velocity.
        shield_counter --; if(shield_counter <= 0) return 0;// 0 means interruption
        /* * * --+
        note that: shield_counter provents from the worst case in time table when introduce zero velocity into table
        for simulating traffic light or congestion.

        if beyond the limited, program view this links in interruption.
        +--* * */
    }

    return through_time;
}//T_costfunction

int graph_instance::calculateTravelALink(int departure_time, int Lane_length, Traffic_info *cord){
        int t = T_costfunction((departure_time + Lane_length), Lane_length, cord);
        if(t <= 0) return t;
        return departure_time + t;
}

//---------------- functions for statistics of transportation -----------------------//

INT64 graph_instance::T_computingPairs(int *status_list, INT64 *counter_list, int *node_list, int *relation_list)
{//T_computingPairs
    for(int n = 0; n <= nodes; n++){
        counter_list[n] = 0; status_list[n] = 0;
    }
    //1.enumerate the knots in search tree
    // scan parent list, record the appearing times in list by status_list
    for(int n = 1; n <= nodes; n++){
        int node = relation_list[n]; status_list[node] ++;
    }
    // 2. push the terminals in tree into array node_list,
    int offset = 0, length;
    for(int n = 1; n <= nodes; n++){
        if(status_list[n] <= 0){
            offset ++; node_list[offset] = n;
        }
    }// if its key is zero in array status_list, it is terminal

    if(offset <= 0) return 0;
    INT64 pairs = 0;
    for(int n = 1; n <= offset; n++)
    {//loop: a process from terminal to root, indeed that is of counting remote nodes for every level in search tree
        int child = node_list[n]; length = 0;
        while(length < nodes){//
            length ++;
            // a. enumerate father in tree from treminal
            int father = relation_list[child]; if(father <= 0) break;

            //b. array counter_list carries the number of descendents for every knot
            counter_list[father] = counter_list[father] + counter_list[child] + 1; status_list[father] --;
            if(status_list[father] >= 1) break;
            else{
                child = father; pairs = pairs + counter_list[child];
            }
        }//
    }//loop

    return pairs;
}//T_computingPairs

float graph_instance::T_calculateAverageLane()
{//T_calculateAverageLane
    int index = 0, sum = 0, average_lane = Lane_Length_list[0] >> 1;
    average_lane = Lane_Length_list[average_lane]; //get the mean member of length list

    if(average_lane <= 0) return -1;
// read through Choosing_Length_list for all lengthes
    for(int leaf = 1; leaf <= nodes; leaf ++){
        if(Choosing_Length_list[leaf]){
            sum = sum + Choosing_Length_list[leaf] - average_lane; index ++;
        }
    }
    if(!sum) return average_lane;
    return (float) sum /index + average_lane; // calculate the average value
}//T_calculateAverageLane

float graph_instance::T_calculateAverageTime(CBST *cbst)
{//T_calculateAverageTime
    INT64 sum = 0,
    gauge = Lane_Length_list[0] + 1; gauge = gauge >> 1; // get the mean of position
    gauge = Lane_Length_list[gauge]; if(gauge <= 0) return -1; // get the average length of link

// get the average velocity
    int index = Velocity_grade >> 1;
    index = get_Velocity(sampling_time_gap, index);
    if(index < 0){
        if(index < -1)
            cout<<"\n the setting of velocity_grade is error for function get_Velocity"<<endl;
        else
            cout<<"\n the setting of sampling_time_interval you slected is error for function get_Velocity"<<endl;
        return index;
    }
    gauge = gauge / index; index = 0;

// read through the arrival time table and parent list for the through time.
    for(int leaf = 1; leaf <= nodes; leaf ++){
        int root = parent_list[leaf];
        if(root > 0){
            index ++;
            sum = sum + (INT64)(cbst->get_TheKey(leaf) - cbst->get_TheKey(root)) - gauge;
        }
    }
    if(index <= 0) return 0;
    if(!sum) return (float)gauge;// calculate the average value.
    return ((float) sum / index) + gauge;
}//T_calculateAverageTime







