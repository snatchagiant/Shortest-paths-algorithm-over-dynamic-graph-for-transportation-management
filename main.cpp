#include "Graph_Instance.h"

using namespace std;

void print_time(double time_data);
int main()
{
    INT64 start_time = 0, end_time = 0, diff = 0, HDM_time = 0;

    //pre-definitions for traffic system

    //1. define time table
    const int time_table_size = 100000;
    int *T_time_depentent_table = new int[time_table_size];// time table, it will be shared by all links on instance

    //2. the velocity table 10* 23 matrix 10 grades for time interval; 23 grades for meters
    const int sampling_scope = 10;
    const int velocity_grades = 23;
    int V[10 + 1][velocity_grades + 1]= {// 0th row just is comment for smapling, using kph as being physical unit
        {(velocity_grades + 1), 167, 250, 333, 417, 500, 583, 667, 750, 833, 917, 1000, 1083, 1167, 1250, 1333, 1417, 1500, 1583, 1667, 1750, 1833, 1917, 2000},
        {1, 3, 4, 6, 7, 8, 10, 11, 13, 14, 15, 17, 18, 19, 21, 22, 24, 25, 26, 28, 29, 31, 32, 33},
        {2, 6, 8, 11, 14, 17, 19, 22, 25, 28, 31, 33, 36, 39, 42, 44, 47, 50, 53, 56, 58, 61, 64, 67},
        {3, 8, 13, 17, 21, 25, 29, 33, 38, 42, 46, 50, 54, 58, 63, 67, 71, 75, 79, 83, 88, 92, 96, 100},
        {4, 11, 17, 22, 28, 33, 39, 44, 50, 56, 61, 67, 72, 78, 83, 89, 94, 100, 106, 111, 117, 122, 128, 133},
        {5, 14, 21, 28, 35, 42, 49, 56, 63, 69, 76, 83, 90, 97, 104, 111, 118, 125, 132, 139, 146, 153, 160, 167},
        {6, 17, 25, 33, 42, 50, 58, 67, 75, 83, 92, 100, 108, 117, 125, 133, 142, 150, 158, 167, 175, 183, 192, 200},
        {7, 19, 29, 39, 49, 58, 68, 78, 88, 97, 107, 117, 126, 136, 146, 156, 165, 175, 185, 194, 204, 214, 224, 233},
        {8, 22, 33, 44, 56, 67, 78, 89, 100, 111, 122, 133, 144, 156, 167, 178, 189, 200, 211, 222, 233, 244, 256, 267},
        {9, 25, 38, 50, 63, 75, 88, 100, 113, 125, 138, 150, 163, 175, 188, 200, 213, 225, 238, 250, 263, 275, 288, 300},
        {10, 28, 42, 56, 69, 83, 97, 111, 125, 139, 153, 167, 181, 194, 208, 222, 236, 250, 264, 278, 292, 306, 319, 333}
    };
    //physical unit on row is meter, the first element on every row is time number (second).
    int **Velocity_list = new int*[sampling_scope + 1];
    for(int n = 0; n <= sampling_scope; n++) Velocity_list[n] = V[n];

    //3. define the length for every lane
    const int Lane_grades = 46;
    //physical unit is meter
    int Length_list[Lane_grades + 1] = {Lane_grades, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950, 1000, 1050, 1100, 1150, 1200, 1250, 1300, 1350, 1400, 1450, 1500, 1550, 1600, 1650, 1700, 1750, 1800, 1850, 1900, 1950, 2000, 2050, 2100, 2150, 2200, 2250, 2300, 2350, 2400, 2450, 2500};

    //* ------------------------ input essencial parameter for experiment ------------------------ *//
    int x = 0, y = 0, type= 0, sampling_time_interval = sampling_scope; //the maximum interval is equivalnet to 10 seconds initially

    //A. define the grid instance scale
    string str = "\n Firstly type two natural numbers to assign two scalars for grid instance on the row and column!";
    cout<<str<<endl;
    cin>>x>>y;
    if(x < 2 || y < 2){
        str.erase(); str = "\n the number for nodes is more less, program will halt.";
        cout<<str<<endl; return -100;
    }

    int Nodes = x * y; // the nodes amount

    //B. set the time interval you want
    str.erase(); str = "\n second input the sampling interval (note that default is 10 you can view as 10 seconds the up bound, the low bound is only 1); don't tape a number more than the max value";
    cout<<str<<endl;
    cin>>sampling_time_interval;

    if(sampling_time_interval < 1 || sampling_time_interval > 10){
        str.erase(); str = "\n the number for sampling interval is error, program will halt.";
        cout<<str<<endl; return -200;
    }

    //C. define the manner of assingment to time table.
    str.erase(); str = "\n third input the type of assignment manner on time-dpendent table: 0 is wave; 1 is random";
    cout<<str<<endl;
    cin>>type;

    //D. set the upper bound for piecing lane, if beyond this limit, then the link viewed as interruption.
    int source = 0, index = V[1][1];
    if(index <= 0){
        str.erase(); str = "\n error in setting velocity grades!\n";
        cout<<str<<endl; return -300;
    }
    int the_through_limit = Length_list[Lane_grades];
    if(the_through_limit <= 0){
        str.erase(); str = "\n error in setting lane grades!\n";
        cout<<str<<endl; return -3;
    }

    source = the_through_limit % index; the_through_limit = the_through_limit / index; index = index >> 1;
    if(source > index) the_through_limit ++;

    //E. define various lists for investigating shortest paths
    /* ------------------------ define a group of data structures in a computing system ------------------------ */
start_time = GetTickCount();

    /* * * * -- define labeling lists for investigating shortest paths -- * * * */
    const int labeling_size = 3; int **Labeling_array = new int*[labeling_size];
    for(int n = 0; n < labeling_size; n++){
        Labeling_array[n] = new int[Nodes + 1];
        for(int m = 0; m <= Nodes; m++) Labeling_array[n][m] = 0;
    }
    index = labeling_size - 1;
    int *Parent_list; if(index >= 0) Parent_list = Labeling_array[index]; index --;
    int *Reach_Time_list; if(index >= 0) Reach_Time_list = Labeling_array[index]; index --;
    int *Candidates; if(index >= 0) Candidates = Labeling_array[index]; index --;

    INT64 *Pairs_statistics = new INT64[Nodes + 1];// for statistics about s-t-pairs amount in search tree

    /* * * * -- define a LE for managing search -- * * * */
    int **CBST_Tree = new int*[Nodes + 1];
    for(int n = 0; n <= Nodes; n ++){
        CBST_Tree[n] = new int[Auxiliary + 1];
        Pairs_statistics[n] = 0;
    }
    CBST cbst(CBST_Tree, Reach_Time_list, Nodes);//initialize a LE

    /* * * * -- define a group of auxiliary lists -- * * * */
    const int size_ = 3; int **Auxiliary_array = new int*[size_];
    for(int n = 0; n < size_; n++){
        Auxiliary_array[n] = new int[Nodes + 1];
        for(int m = 0; m <= Nodes; m++) Auxiliary_array[n][m] = 0;
    }
    index = size_ - 1;
    int *Lane_list; if(index >= 0) Lane_list = Auxiliary_array[index]; index --;
    int *Region_list; if(index >= 0) Region_list = Auxiliary_array[index]; index --;
    int *Status_list; if(index >= 0) Status_list = Auxiliary_array[index]; index --;

    /* * * * -- define a objective zone which is a grid instance-- * * * */
    const int gap = 4;
    int **Instance = new int*[Nodes + 1]; int **Weight_list = new int*[Nodes + 1];
    for(int n = 0; n <= Nodes; n++){
        Instance[n] = new int[gap + 1]; Weight_list[n] = new int[gap + 1];
        Instance[n][0] = 0; Weight_list[n][0] = 0;
    }

    //initialize an instance, including fiexd domain and dynamic domain
    graph_instance state_inst(Instance, // 2D array structure that loading an instance with connected relationships
                              Weight_list, // carry index for each link as weight to constract a fixed layout of metroplies
                              Velocity_list, // 2D velocity table to carry various grades of velocity
                              Parent_list, // will carry search: child -> parent
                              Length_list, // a list assisted to computing average length of result.
                              T_time_depentent_table, // time table to carry those velocities over times.
                              Lane_list, // carry each number of length for each lane
                              the_through_limit // the scalar restricts the piecing operation, if beyond, then means this link has been interrupted in reality.
                              );
    //initialize various parameters for control.
    state_inst.initialize_anNetWork(x, // number of nodes on row
                                    y,  // number of nodes on column
                                    time_table_size, // the size of time table
                                    velocity_grades, // the number of velocity grades
                                    gap,             // the max size of leaf set
                                    sampling_scope, // the highest sampling time interval, 0th row is loading definition by physical unit kph
                                    sampling_time_interval, // we select sampling time interval
                                    type);

    cout<<"\n the scale, arcs quantity is "<<state_inst.get_Arcs()<<endl;

end_time = GetTickCount(); diff = end_time - start_time;
    // print the runtime for initializing phrase
    cout<<endl;
    cout<<"constract redical data structure took "; print_time(diff);
    cout<<endl;

    // F. investigating shortest phrase
/* ----------------- do investigation --------------------------- */
    //F.0. initialize source and departure time
    index = rand() % sampling_time_interval; index ++;
    source = rand() % Nodes; source ++;
    cbst.set_TheKey(source, index);

    cout<<"\n initializing weight for source is: "<<index<<"; source is "<<source<<endl;

    // auxiliary parameters
    Info_collection info; Traffic_info cord;
    INT64 Query_time_table = 0, Query_velocity_table = 0, s_t_pairs = 0; // statistics variants

start_time = GetTickCount();

    //F.1. do HDM
    index = state_inst.T_DoHybirdDijkstraMethod(&cbst, // the CBST instance for the task: record total weigth and serve for querying weights
                                                Candidates, // record those origins for next stage
                                                Region_list, // record graph partition results
                                                Status_list, // record region id for each node
                                                source,      // source the departure point
                                                &info, //  statistics structure
                                                &cord);
    if(index <= 0){
        cout<<"\n failed on HDM, the error id is: "<<index<<endl; return -400;
    }

end_time = GetTickCount(); HDM_time = end_time - start_time;

    //F.2. do a statistices about querying time table or velocity table.
    Query_time_table = cord.QueryTimes; Query_velocity_table = cord.SubQueryTime;

    if(!Candidates[0]){
        s_t_pairs = state_inst.T_computingPairs(Status_list, Pairs_statistics, Region_list, Parent_list);
        cout<<endl;
        cout<<"NOTE. the pair amount is: "<<s_t_pairs<<endl;
        cout<<"\n no candidate for next stage!"<<endl;
        return -500;
    }

    int *Backup_array = Status_list;

    //F.3. matirialize a CBST with Candidates
    try{
        info.initialize();
        index = cbst.cbst_BuildingCBST(Candidates, &info); // building a CBST by Pyramid Method
        if(index <= 0){cout<<"\n error => "<<index<<endl; return - 600;}
    }catch(Exception_about_memory &e){e.what(); return - 601;}

    //F.4. do CA
    index = state_inst.get_Arcs();
    for(int N = 0; N <= index; N ++)
    {//big loop
        Candidates[0] = 0; int Agency = 0;

        //F.4.1. Collect a LE wherein all members with mininal key.
        Agency = cbst.gain_theMinCL(&info);
        if(Agency < 0) return - 700; else if(!Agency) break;

        //F.4.2. Improve those labels
        try{
            Backup_array[0] = 0;
            if(state_inst.T_ContestMethodCL(Backup_array, Agency, &cbst, &cord) < 0) return -800; //
        }catch(Exception_about_memory &e){e.what(); cout<<"\n N = "<<N<<endl; return -801;}

        Query_time_table = Query_time_table + cord.QueryTimes;
        Query_velocity_table = Query_velocity_table + cord.SubQueryTime;

        if(Backup_array[0] <= 0){
            if(cbst.get_MinEtremum() <= 0) break; // CBST is empty
            else
                continue;
        }

        //F.4.3. Pushing those new origins into CBST.
        try{
            info.initialize();
            if(Backup_array[0] == 1){
                if(cbst.insertion(Backup_array[1], &info) < 0) return - 900;
                cbst.set_AuxiliaryOnItem(Backup_array[1], 0);
            }
            else if(cbst.insertionAList(Backup_array, &info) < 0) return - 901;
        }catch(Exception_about_memory &e){e.what(); return - 902;}
    }//big loop

end_time = GetTickCount(); diff = end_time - start_time;

    //F.5. print results.
    cout<<endl;
    cout<<"1. the amount of querying time table is: "<<Query_time_table<<endl;
    cout<<"2. the amount of querying velocity table is: "<<Query_velocity_table<<endl;

    float average_L = state_inst.T_calculateAverageLane();
    if(average_L > 0){
        cout<<"3. the average length is: "<<average_L<<endl;
    }
    else{
        cout<<"3. the average length is error: "<<average_L<<endl; return -1000;
    }

    float average_T = state_inst.T_calculateAverageTime(&cbst);
    if(average_T > 0){
        cout<<"4. the average time is: "<<average_T<<endl;
        average_T = average_L/average_T; average_L = average_T * 3.6;
        cout<<"5. the average velocity is: "<<average_T<<"; eqaul of "<<average_L<<" kph."<<endl;
    }else{
        cout<<"4. the average time is error: "<<average_T<<endl; return -1001;
    }

    s_t_pairs = state_inst.T_computingPairs(Status_list, Pairs_statistics, Region_list, Parent_list);
    cout<<endl; cout<<"6. those s-t-pairs in search tree, their quantity is: "<<s_t_pairs<<endl;

    cout<<endl; cout<<" do heuristic search, which took ";
    print_time(diff);
    cout<<" therein the HDM takes";
    print_time(HDM_time);
    cout<<endl;

    //G. release memory space
/* ----------------- delete memory --------------------------- */


    for(int n = 0; n <= Nodes; n++) delete CBST_Tree[n]; delete CBST_Tree;
    for(int n = 0; n <= Nodes; n++){
        delete Instance[n]; delete Weight_list[n];
    }
    delete Instance; delete Weight_list;

    delete Pairs_statistics;

    for(int n = 0; n <= size_; n++) delete Auxiliary_array[n]; delete Auxiliary_array;
    for(int n = 0; n < labeling_size; n++) delete Labeling_array[n]; delete Labeling_array;

    return 0;
}

void print_time(double time_data)
{//print_time
    cout<<"  "<<time_data<<" ms; ";
    INT64 count_1 = (INT64) time_data / 1000; INT64 millisecond = (INT64)time_data % 1000;

    INT64 hour = 3600; INT64 minute = 60; cout<<"  H:M:S:ms --> ";

    if(count_1 >= hour){
        cout<<(count_1/hour)<<" : "; count_1 = count_1 % hour;
    }else cout<<"0 : ";

    if(count_1 >= minute){
        cout<<(count_1/minute)<<" : "; count_1 = count_1 % minute;
    }else cout<<"0 : ";

    if(count_1 >= 1) cout<<count_1<<" : ";
    else cout<<"0 : ";

    cout<<millisecond<<";";
}//print_time




