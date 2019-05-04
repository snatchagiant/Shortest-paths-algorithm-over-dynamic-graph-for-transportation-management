#include "Graph_Instance.h"

int graph_instance::T_OptimizingAnEndpoint(int *Origins_list, int Endpoint, CBST *cbst, Traffic_info *cord)throw(Exception_about_memory)
{//T_OptimizingAnEndpoint

    int counter = 0; Traffic_info T_info; cord->QueryTimes = 0; cord->SubQueryTime = 0;
    int offset = get_theSizeOfLeafSet(Endpoint);
    for(int pos = 1; pos <= offset; pos ++){//release leaf set
        int leaf = get_leafFromSubgraph(Endpoint, pos);
        if(!cbst->checkexitence(leaf)){
            cout<<"\n leaf error in function T_OptimizingAnEndpoint";
            return -1;
        }
        else if(cbst->get_TheKey(leaf) <= cbst->get_TheKey(Endpoint)) continue;

        int oracle_time = cbst->get_TheKey(Endpoint);
        int lane = get_LanebySubgraph(Endpoint, pos); if(lane < 0) return -2;// get the parameter for orcale

        oracle_time = calculateTravelALink(oracle_time, lane, &T_info);

        if(oracle_time < 0) return -3;// computing error
        else if(!oracle_time) continue; //the link has been interrupted

        cord->QueryTimes = T_info.QueryTimes + cord->QueryTimes; cord->SubQueryTime = T_info.SubQueryTime + cord->SubQueryTime;

        if(cbst->get_TheKey(leaf) <= oracle_time) continue;

         //2.1 if this arc is sa, delete leaf off CBST if resides in CBST
        Info_collection info; string str = "\n deletion, function ContestMethod";
        try{
            info.initialize();
            if(cbst->deletion(leaf, &info) < 0){
                Exception_about_memory e(0); e.adding_info(str); throw e;
            }
        }catch(Exception_about_memory &e){e.what(); throw e;}

        // 2.2 update the total weight and parent-child relation
        cbst->set_TheKey(leaf, oracle_time); parent_list[leaf] = Endpoint; Choosing_Length_list[leaf] = lane;

        // 2.3 backup the new origins
        if(cbst->get_info(leaf, Auxiliary) < 1){//backup, if leaf being clean
            if(push_ANodeIntoAList(leaf, Origins_list, nodes) <= 0 || !cbst->set_AuxiliaryOnItem(leaf, 1)){
                str.erase(); str = "\n push_ANodeIntoAList or set_AuxiliaryOnItem, function T_OptimizingAnEndpoint";
                Exception_about_memory e(0); e.adding_info(str); throw e;
            }
            counter ++;
        }//bcakup
    }//leaf
    return counter;
}//T_OptimizingAnEndpoint

int graph_instance::T_ContestMethodCL(int *Origins_list, int Agency_of_CL, CBST *cbst, Traffic_info *cord)throw(Exception_about_memory)
{//T_ContestMethodCL

    if(Agency_of_CL <= 0) return 0; cord->QueryTimes = 0; cord->SubQueryTime = 0;

    int root = Agency_of_CL; int num_cousin = 0; Traffic_info T_info;
    string str ="\n root, function T_ContestMethodCL";
    while(root){//search loop

        //1. enumerate item
        if(!cbst->checkexitence(root)){
            Exception_about_memory e(0); e.adding_info(str); throw e;
        }
        //test

        //2. search
        try{
            int num_origins = T_OptimizingAnEndpoint(Origins_list, root, cbst, &T_info);
            cord->QueryTimes = cord->QueryTimes + T_info.QueryTimes;
            cord->SubQueryTime = cord->SubQueryTime + T_info.SubQueryTime;
            if(num_origins < 0){
                cout<<"\n leaf in error, function T_ContestMethodCL, error = "<<num_origins<<endl; return num_origins;
            }
        }catch(Exception_about_memory &e){
            str.erase();
            str = "\n T_OptimizingAnEndpoint error, function T_ContestMethodCL";
            Exception_about_memory e(0); e.adding_info(str); throw e;
        }

        root = cbst->travelCL(root);
        if(root <= 0) break; else{ num_cousin ++; cbst->itemdecrement();}
    }//loop

    return num_cousin;
}//T_ContestMethodCL
