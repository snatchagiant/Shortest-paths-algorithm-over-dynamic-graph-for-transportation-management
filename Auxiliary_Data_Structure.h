#ifndef AUXILIARY_DATA_STRUCTURE_H_INCLUDED
#define AUXILIARY_DATA_STRUCTURE_H_INCLUDED


#include "Exceptions_About_Memory.h"

enum InstType{Grid = 1, Random, Complete};

enum OverAllInfo{Root = 0, Min_Extremum, Max_Extremum, num_Items, num_Agencies};

enum Type_item{Terminal = 1, Partial_Knot, Complete_Knot, Juxta_Position};

enum Type_Position{Out = -1, Juxta = 0, In = 1};

enum Functional_cellular{Parent = 0, Left_child, Right_child, Cousin, Predecessor, Successor, Flexion, Self_counter, InOrOut, Auxiliary};

struct Cost_Range{
    int Low_bound; int Up_bound;
    void Initialize(){Low_bound = -1; Up_bound = -1;}
};

struct Info_collection{
    INT64 Accounter;
    int Flex_type;
    int Key;
    int Kind;
    INT64 Length;
    int Name;
    int other;
    int Quantity;
    int Parent_Port;
    int Port;
    int Type;
    void initialize(){
        Accounter = 0; Kind = 0;//Flex_type = 0; Key = 0;
        Length = 0; Name = 0; other = 0; Quantity = 0; Parent_Port = -1; Port = 0; Type = 0;
    }
};

struct Traffic_info{
    int ArrivalTime;
    int CostTime;
    int Lane;
    INT64 QueryTimes;
    INT64 SubQueryTime;
    void initialize(){ArrivalTime = 0; CostTime = 0; Lane = 0; QueryTimes = 0; SubQueryTime = 0;}
};
#endif // AUXILIARY_DATA_STRUCTURE_H_INCLUDED
