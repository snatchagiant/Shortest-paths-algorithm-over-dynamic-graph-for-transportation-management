int graph_instance::T_DoHybirdDijkstraMethod(CBST *cbst, int *origins_list, int *regions, int *state_list, int source, Info_collection *info, Traffic_info *cord)
{//T_DoHybirdDijkstraMethod

    if(!get_initialize_status()) {
        cout<<"\n the initializing work has not been achieved "<<endl;
        return -1;
    }

    int index = 1, startup = 1; INT64 counter_1 = 0, counter_2 = 0; info->Accounter = 0;
    if(source <= 0 || source > nodes) return 0;

    //initialize graph partition
    Traffic_info T_info; T_info.initialize();
    regions[index] = source; regions[0] = index;
    state_list[source] = 1;
    for(int n = 0; n <= nodes; n++){//big loop
        int amount = 0; int offset = regions[0]; int address = 0;
        for(int m = startup; m <= offset; m++){//scan each region
            int root = regions[m]; if(root <= 0 || root > nodes) return -2;//get the node in subregion as root of subgraph
            int gap = get_theSizeOfLeafSet(root); if(gap > subgraph_interval) return -3;
            bool signal = true;
            for(int k = 1; k <= gap; k++){//relax leaf set
                int leaf = get_leafFromSubgraph(root, k);

                if(leaf <= 0 || leaf > nodes) return -4; // there is error in data structure of leaf set

                info->Accounter ++; counter_1 = T_info.QueryTimes + counter_1; counter_2 = T_info.SubQueryTime + counter_2;

                int flag = -1;
                if(state_list[leaf] <= 0){//A. partition nodes into next region, label it no labeled
                    state_list[leaf] = index + 1;
                    amount ++; address = offset + amount; regions[address] = leaf; //push the node into region.
                    flag = 1; //enable to label
                }//A
                else{//B. address the case if node been partitined and in next, previous or native region

                    if(state_list[leaf] > state_list[root]) flag = 0;
                    else if(signal){ //in previous or native region
                        if(cbst->get_TheKey(leaf) > cbst->get_TheKey(root)){
                            // the behind less than previous, possible as origin
                            if(push_ANodeIntoAList(root, origins_list, nodes) <= 0) return -6;
                            signal = false; // label root has been recorded
                        }
                    }
                }
                 int oracle_time = 0; int Link_length = 0;
                if(flag >= 0){//labeling or correcting
                    //get the oracle
                    oracle_time = cbst->get_TheKey(root); Link_length = get_LanebySubgraph(root, k); if(Link_length < 0) return -2;// get the parameter for orcale
                    oracle_time = calculateTravelALink(oracle_time, Link_length, &T_info);

                    if(oracle_time < 0) return -5;// calculation error
                    else if(!oracle_time) continue; // this link has been interrupted in reality

                    if(!flag && cbst->get_TheKey(leaf) <= oracle_time) continue; // no necessary to correct

                    cbst->set_TheKey(leaf, oracle_time); parent_list[leaf] = root; // label or correct target.
                    Choosing_Length_list[leaf] = Link_length;
                }
            }//leaf set
        }//region
        if(amount <= 0)break;
        else{
            startup = offset + 1; regions[0] = address; index ++;
        }
    }//big
    cord->QueryTimes = counter_1; cord->SubQueryTime = counter_2;
    return index;
}//T_DoHybirdDijkstraMethod
