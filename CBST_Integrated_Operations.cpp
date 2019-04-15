#include <string>
#include "CBST_Tree_Company.h"

int CBST::resetCBST(Info_collection *info)//reset the CBST
{//resetCBST

    int accounter = 0;
    int item = the_overallinfo[Min_Extremum]; if(item <= 0) return 0;
    while(item > 0){//loop
        info->Kind = item; accounter ++;
        if(!checkexitence(item)) return -1;

        int node = get_info(item, Cousin); info->Name = item;
        while(node > 0){//doing in CL
            info->Kind = node;
            if(!checkexitence(node)) return -2;

            int next = get_info(node, Cousin);
            resetAnItemInCL(node); updateItemInfo(node, Out, InOrOut); accounter ++;

            if(next <= 0) break; else node = next;
        }
        node = get_info(item, Successor); resetAUnit(item);//reset info of BST and ARA
        item = node;
    }//loop
    resetWholeInfo();
    return accounter ++;
}//resetCBST

bool CBST::InitializeAllItems()
{//resetAllItem
    if(Scalar <= 0) return false;
    for(int n = 1; n <= Scalar; n++) resetAUnit(n);
    resetWholeInfo();
    return true;
}//resetAllItem

int CBST::gain_Min_Extremum(Info_collection *info)
{//gain_Min_Extremum

    int extremum = get_MinEtremum(); if(extremum <= 0) return 0;//declear the tree is empty

    info -> Name = 0; info -> Parent_Port = 0; info -> Port = 0; info->Kind = 0;
    int type = deletion(extremum, info);
    if(type < 0){
        cout<<"\n deleting error in functor gain_Min_Extremum. "<<endl;
        return type;
    }
    return extremum;
}//gain_Min_Extremum

int CBST::gain_theMinCL(Info_collection *info)
{//gain_theMinCL

    /* method introduction
        1. get and check the extremum

        2. the likelihood we will have to settle:
          (1) extremum as a terminal, conversely (2) as a PK
          (3) as a root, on contrary (4) as an average item.

    ----------------------+-----------------------+------------------------------------+------------*/
    if(the_overallinfo[Root] <= 0) return 0;//declear the tree is empty

    int extremum = get_MinEtremum();

    //* 1st step: estimate that extremum.
    info -> Name = 0; info -> Parent_Port = 0; info -> Port = 0;
    int type = estimateItem(extremum, info);
    if(type < 0){
        cout<<"\n estimateItem, the position => "<<type<<"; "<<info->Kind<<endl;
        return type;
    }
    else if(type >= Complete_Knot){cout<<"\n error type => "<<type<<endl; return -1;}

    //* 2nd step: in ARA model
    int right_member = get_info(extremum, Successor);
    if(right_member <= 0){//(1) + (3)
        resetWholeInfo(); the_overallinfo[num_Agencies] --; updateItemInfo(extremum, Out, InOrOut);
        return extremum;// not need addressing in BST model
    }else{
        //(2) + ((3) || (4)) or (1) + (4)
        info->Kind = right_member; if(!judge_agency(right_member)) return -1;//check right_member

        updateItemInfo(extremum, 0, Successor); updateItemInfo(right_member, 0, Predecessor);//deleted off ARA
        the_overallinfo[Min_Extremum] = right_member;// update the overall information
        info->Length ++;
    }

    //* 3rd step: in BST model
    if(info -> Name > 0){//(4) + ((1) || (2))
        BST_delete_aBSTRelation(info -> Name, extremum, info->Parent_Port);//delete extremum off father if terminal then off BST
        info->Length ++;
    }

    if(type == Partial_Knot){//(2)
        int son = get_info(extremum, Right_child);
        if(info -> Name)//+ (4)
            BST_connection(info -> Name, son, Left_child);//father <->grandson
        else{//+ (3)
            the_overallinfo[Root] = son;
            updateItemInfo(son, 0, Parent); // child as the new root.
        }
        info -> Length ++;
    }
    the_overallinfo[num_Agencies] --; updateItemInfo(extremum, Out, InOrOut);

    return extremum;
}//gain_theMinCL

bool CBST::insertion(int item, Info_collection *info)throw(Exception_about_memory)
{//insertion
    /*
        1. if CBST is empty, then appoint item as being a root
        2.1 nonempty, seek appropriate position
        2.2 insertion in BST model
        2.3 in ARA model if will as an average terminal in BST.

    ----------------------+--------------------+---------------------------------*/
    info->Kind = item; int effort = 0;
    if(judge_inorout(item)) return false; //check item

    if(the_overallinfo[Root] <= 0){//empty tree
        updateItemInfo(item, In, InOrOut);//for itself

        the_overallinfo[Root] = item; // for BST
        the_overallinfo[Max_Extremum] = item; the_overallinfo[Min_Extremum] = item; //for ARA
        the_overallinfo[num_Agencies] ++; //for overall info
        effort ++; //for statistices
    }
    else{
        int father = standard_seeking(get_TheKey(item), info); // find the position
        if(father <= 0){
            cout<<"\n standard_seeking in insertion; position => "<<info->Kind<<"; return => "<<father<<endl; return false;
        }
        //1. if in CL
        if(info->Port == Cousin){
            int cousin = get_info(father, Cousin); info->Kind = cousin;
            if(cousin > 0){//with another cousin
                if(!judge_inorout(cousin)) return false;
                else{
                    BST_insertion(item, get_info(father, Cousin), info->Port); //will be insert into the between of agency and agency's cousin
                    effort ++;
                }
            }//another
        }//CL
        else{// as a child
            int c = ARA_interpolation(father, item, info->Port);//in ARA model
            if(c <= 0){Exception_about_memory e(0); string str = "ARA, function insertion\n"; e.adding_info(str); throw e;}

            effort =  effort + c; the_overallinfo[num_Agencies] ++;
        }//child

        //3. connet father and item in BST model, finally.
        //Remark: at this step, the father's information should be updated when in CL, wherefore this step at last.
        BST_insertion(father, item, info->Port); effort ++;// father <--> item.
    }

    info->Port = 0; info->Name = - 1; info->Length = effort;
    the_overallinfo[num_Items] ++;
    return true;
}//insertion

int CBST::insertionAList(int *item_list, Info_collection *info)throw(Exception_about_memory)
{//insertionAList
    if(item_list[0] <= 0) return 0;

    info -> initialize(); int counter = 0;
    if(item_list[0] == 1)
        try{
            info->other = item_list[1]; if(insertion(item_list[1], info) < 0) return - 2;
            updateItemInfo(item_list[1], 0, Auxiliary); counter ++;
        }catch(Exception_about_memory &e){string str = "\n single insertion, function insertionAList"; e.adding_info(str); throw e;}
    else{
        while(item_list[0] > 0){//loop
            int i = item_list[0]; item_list[0] --;
            try{
                info->other = item_list[i]; if(insertion(item_list[i], info) < 0) return - 2;
                updateItemInfo(item_list[i], 0, Auxiliary); counter ++;
            }catch(Exception_about_memory &e){string str = "\n a list insertion, function insertionAList"; e.adding_info(str); throw e;}
        }//loop
    }
    item_list[0] = 0;
    return counter ++;
}//insertionAList

int CBST::deletion(int item, Info_collection *info)throw(Exception_about_memory)
{//deletion

    info->Kind = -1;
    if(!judge_inorout(item) || the_overallinfo[Root] <= 0) return 0; //check relation

    info -> Name = -1; info -> Port = 0; info -> Parent_Port = 0;
    int type = estimateItem(item, info);
    if(type < 0){
        cout<<"\n estimateItem position => "<<info->Kind<<"; type = "<<type<<endl;
        return -1;
    }
    int value = 0;
    switch(type){
        case Terminal: value = deleting_terminal(item, info); break;
        case Partial_Knot: value = deleting_PK(item, info); break;
        case Complete_Knot: value = deleting_CK(item, info); break;
        case Juxta_Position: value = deleting_cousin(item, info); break;
        default: value = -1;
    }
    if(value < 0){
        switch(type){
        case Terminal: cout<<"\n deleting_terminal, position => "<<info->Kind<<endl; break;
        case Partial_Knot: cout<<"\n deleting_PK, position => "<<info->Kind<<endl; break;
        case Complete_Knot: cout<<"\n deleting_CK, position => "<<info->Kind<<endl; break;
        case Juxta_Position: cout<<"\n deleting_cousin, position => "<<info->Kind<<endl; break;
        default: value = -1;
        }
        Exception_about_memory e(0);
        string str ="deletion, function deletion\n"; e.adding_info(str); throw e;
    }
    return type;
}//deletion

bool CBST::set_AuxiliaryOnItem(int item, int value)
{//set_AuxiliaryOnItem
    if(!checkexitence(item)) return false; updateItemInfo(item, value, Auxiliary);
    return true;
}//set_AuxiliaryOnItem


