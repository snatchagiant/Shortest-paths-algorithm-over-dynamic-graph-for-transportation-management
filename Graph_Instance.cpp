#include "Graph_Instance.h"

using namespace std;

int graph_instance::T_MaketheMetropliesInst()
{//T_MaketheMetropliesInst
    srand((unsigned int)(time(NULL)));//deposit a seed for random assignment
    int cursor = 1, RightMost_Post = num_row;

    for(int n = 1; n <= num_column; n++)
    {//on row
        while(cursor <= RightMost_Post)
        {// on column

            //1. on horizon towards to right
            int neighbor = cursor + 1; int cost = (rand() % Lane_Length_list[0] + 1); int pos = 0;

            if(cursor < RightMost_Post){//stretch to right side
                pos = wad_LeafSetWithElement(cursor, neighbor); if(pos <= 0) return -1;
                if(!wad_CostListWithWeight(cursor, pos, cost)){ return -2;}

                pos = wad_LeafSetWithElement(neighbor, cursor); if(pos < 0) return -3;
                if(!wad_CostListWithWeight(neighbor, pos, cost)) return -4;
            }

            //2. on vertical towards to ground
            neighbor = cursor + num_row; cost = (rand() % Lane_Length_list[0] + 1);

            if(neighbor <= nodes){
                pos = wad_LeafSetWithElement(cursor, neighbor); if(pos < 0) return -5;
                if(!wad_CostListWithWeight(cursor, pos, cost)) return -6;

                pos = wad_LeafSetWithElement(neighbor, cursor); if(pos < 0) return -7;
                if(!wad_CostListWithWeight(neighbor, pos, cost)) return -8;
            }
            //3. set the rest to zero
            cost = get_theSizeOfLeafSet(cursor); cost ++;
            if(cost <= subgraph_interval){
                for(int n = cost; n <= subgraph_interval; n++){
                    updateInstanceRelation(cursor, n, 0);
                    assign_arcWeight(cursor, n, 0);
                }
            }
            cursor ++;
        }// on column
        RightMost_Post = RightMost_Post + num_row;
    }//on row

    return cursor;
}//T_MaketheMetropliesInst

bool graph_instance::T_MakeTimeTableIntoWave()
{//T_MakeTimeTable
    srand((unsigned int)(time(NULL)));//deposit a seed for random assignment

    int control = rand() % sampling_scope; control ++;
    int index = 1, velocity = 1, limit = Velocity_grade;
    bool flag = true;
    for(int n = 0; n < time_table_size; n++){
        if(control > 0){
            Time_table[n] = velocity; control --;
        }
        else{
            control = rand() % sampling_scope; control ++;

            if(index >= limit) flag = false; else if(index <= 1) flag = true;

            if(flag) index ++; else index --; velocity = index;
            Time_table[n] = velocity;
        }
    }
    return true;
}//T_MakeTimeTable

bool graph_instance::T_MakeTimeTableIntoRandom()
{//T_MakeTimeTableIntoRandom
    srand((unsigned int)(time(NULL)));//deposit a seed for random assignment
    for(int n = 0; n < time_table_size; n++){
        int v = rand() % Velocity_grade; v++;
        Time_table[n] = v;
    }
    return true;
}//T_MakeTimeTableIntoRandom

bool graph_instance::initialize_anNetWork(int x, int y, int tts, int Vg, int gap, int sc, int sf, int type)
{//initialize_anNetWork
    if(!successful){
            cout<<"\n the initialzing work on various list has not achieved"<<endl;
            return false;
        }
        successful = true;

        num_row = x; num_column = y; time_table_size = tts, Velocity_grade = Vg;
        subgraph_interval = gap;
        sampling_scope = sc; sampling_time_gap = sf;

        if(num_row <= 1 || num_column <= 1 || time_table_size <= 0 || Velocity_grade <= 0 || subgraph_interval <= 0)
        {
            cout<<"\n some parameter is error!"<<endl;
            return false;
        }
        nodes = num_row * num_column; int i = nodes << 2;
        Arcs = num_row + num_column; Arcs = Arcs << 1;
        Arcs = i - Arcs;

        // make an instance
        if(T_MaketheMetropliesInst() <= 0){
            cout<<"\n failed at the initial stage for making an instance "<<endl;
            return false;
        }

        // make time table
        if(type <= 0){
            if(!T_MakeTimeTableIntoWave()){
                cout<<"\n failed at the initial stage for making an time table in using wave manner "<<endl;
                return false;
            }
        }
        else if(type == 1){
            if(!T_MakeTimeTableIntoRandom()){
                cout<<"\n failed at the initial stage for making an time table in using random manner "<<endl;
                return false;
            }
        }
        else{
            cout<<"\n failed at the initial stage for making an time table with an error id to label manner "<<endl;
            return false;
        }
        initializing_instaces = true;

        return initializing_instaces;
}//initialize_anNetWork
