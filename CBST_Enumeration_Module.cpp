#include "CBST_Tree_Company.h"

int CBST::Enumeration_MinList(int *emu_list, Info_collection *info)throw(Exception_about_memory)
{//Enumeration_MinList
    int node = 0; node = get_MinEtremum(); if(node <= 0) return 0; //means empty tree

    // wad emu_list, the best-item list
    if(!judge_agency(node)){Exception_about_memory e(1); string str = "\n node error in function Enumeration_MinList."; e.adding_info(str); throw e;}

    info->initialize(); emu_list[0] = 0; int counter = 0;
    while(node > 0){
        int next = get_info(node, Cousin);

        // 1.delete node off CBST
        try{
            int f = deletion(node, info);
            if(f <= 0) return f;
        }catch(Exception_about_memory &e){
            string str = "\n deletion error in function Enumeration_MinList.";
            e.adding_info(str); throw e;
        }
        // 2. push item into list
        if(!pushItemInto(emu_list, node, Scalar)){
            string str = "\n pushItemInto error in function Enumeration_MinList.";
            Exception_about_memory e(0); e.adding_info(str); throw e;
        }
        counter ++;
        if(next <= 0) break; else node = next;
    }
    info->Quantity = counter;
    return counter;
}//Enumeration_MinList

int CBST::popItem(int *item_list, int limit)
{//popItem
    if(item_list[0] <= 0 || item_list[0] > limit) return 0;

    int index = item_list[0]; index = item_list[index];
    item_list[0] --;

    if(judge_agency(index)) updateItemInfo(index, Out, InOrOut);

    return index;
}//popItem

int CBST::travelCL(int agency)
{//travelCL
    int new_agency = get_info(agency, Cousin);
    if(new_agency <= 0) return 0; else if(new_agency > Scalar) return -1;

    //cut agency <--> new_agency
    updateItemInfo(agency, 0, Cousin); if(get_info(agency, InOrOut) >= 0) updateItemInfo(agency, Out, InOrOut);//out CL
    updateItemInfo(new_agency, 0, Parent);

    return new_agency;
}//travelCL


