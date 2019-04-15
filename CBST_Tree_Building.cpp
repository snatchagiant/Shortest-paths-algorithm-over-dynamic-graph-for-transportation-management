#include "CBST_Tree_Company.h"

//Pyramid Method
int CBST::pretreatmentForPM(int *Input_list, int total, Info_collection *info)
{//pretreatmentForPM

    /*  Method introduction

        1. Make a numeric Majority that equal of 2^k - 1 and n/2 <= Majority <= n. And the Rest = n - Majority;

        2. Scan the list from left to right:
            a. The node at that odd position will as child to event one; the event one overwritten down Input_list.
            the onnected relationship about them will be made in BST model.

            b. Check the number if up to Rest so as to end this search.

    ------------------------+-------------------+------------------- */
    if(total < 4) return 0;
    int Majority = make_ABinaryNumeric(total); int Rest = total - Majority;
    //* * * Majority-> 2^k - 1; and, total/2 =< Majority =< total

    Input_list[0] = 0; int Index = 0; int Item = Input_list[1]; info->Kind = 0;
    while(Index < Majority){//loop
        if(!checkexitence(Item)){info->Kind = -1; cout<<"\n error"<<endl; return -1;}
        //1. address those overplus elements to being cellars
        if(Rest){
            // first for left child:
            int Father = get_info(Item, Successor); if(!checkexitence(Father)){info->Kind = -2; return -1;}//check Father

            BST_connection(Father, Item, Left_child); updateItemInfo(Item, In, InOrOut); // Item <-- Father
            Rest --; info->Length ++;

            if(!pushItemInto(Input_list, Father, Scalar)){info->Kind = -3; return -1;}
            else
                Index ++;
            info->Length ++;// rewitten into array Input_list

            Item = get_info(Father, Successor); if(!Rest) continue;

            // turn to right child
            if(!checkexitence(Item)){info->Kind = -4; return -1;}
            BST_connection(Father, Item, Right_child); updateItemInfo(Item, In, InOrOut); // Father --> Item
            Rest --; info->Length ++;

            Item = get_info(Item, Successor); if(!checkexitence(Item)){info->Kind = -5; return -1;}
        }
        if(!pushItemInto(Input_list, Item, Scalar)){info->Kind = Index; return -1;}
        else
            Index ++;
        info->Length ++;
        Item = get_info(Item, Successor); // pull thos the bricks in list
    }//loop

    if(Index != Majority){info->Kind = Index; return -2;}
    else{
        Input_list[0] = Majority; info->other = Rest;// record the amount of bricks
    }
    return Majority;
}//pretreatmentForPM

int CBST::Pyramid_Method(int *Input_list, int majority, Info_collection *info)
{//Pyramid_Method
    int Father = 0; int Child = 0;
    int Cursor = 2; int Backup = Cursor;// the mobiled Cursor to get the parent

    int Width = Cursor >> 1; // the length counting from parent for pick up the children
    int step = Cursor << 1; // step for Cursor jumping rightwards in ARA

    while(step < majority || Cursor < majority){//loop
        Father = Input_list[Cursor]; if(!checkexitence(Father)){info->Kind = 0; return -1;}//check father
        updateItemInfo(Father, In, InOrOut);

        Child = Input_list[Cursor - Width]; BST_insertion(Father, Child, Left_child); info -> Length ++;
        Child = Input_list[Cursor + Width]; BST_insertion(Father, Child, Right_child); info -> Length ++;

        Cursor = Cursor + step;
        if(Cursor > majority){
            Cursor = Backup << 1; Backup = Cursor;
            Width = Cursor >> 1; step = Cursor << 1;
        }
    }//loop
    updateItemInfo(Father, 0, Parent);
    return Father;
}//Pyramid_Method

int CBST::cbst_BuildingCBST(int *Input_list, Info_collection *info_cost)throw(Exception_about_memory)
{//cbst_BuildingCBST
    info_cost->initialize(); int amount = 0;

    //* 1st step.
    amount = Sorting(Input_list, info_cost, true); // amount is the number of members in ARA
    if(Input_list[0] <= 0 || Input_list[0] > 1 || amount <= 0){
        cout<<"\n position => "<<info_cost->Kind<<"; return => "<<amount<<endl;
        string str = "\n Sorting module in cbst_BuildingCBST; ";
        Exception_about_memory e(0); e.adding_info(str); throw e;
    }

    //* 2nd step.
    int Item = Input_list[1];

    //* 2.1. set the overall information
    the_overallinfo[num_Agencies] = amount;
    the_overallinfo[num_Items] = info_cost->Quantity; the_overallinfo[Min_Extremum] = Item;
    updateItemInfo(Item, In, InOrOut);

    //* 2.2 construct small tree
    if(amount == 1){
        the_overallinfo[Root] = Item; the_overallinfo[Max_Extremum] = Item;
        updateItemInfo(Item, 0, Parent);
        Input_list[0] = 0; info_cost->Length ++;
        return Item;
    }
    int child = Item;
    Item = cbst_trees[Item][Successor]; if(!checkexitence(Item)){info_cost->Kind = Item; return -1;}
    if(amount <= 3)
    {
        // default two members in ARA
        the_overallinfo[Root] = Item; updateItemInfo(Item, In, InOrOut);  updateItemInfo(Item, 0, Parent);// set the root for BST
        BST_connection(Item, child , Left_child); info_cost->Length ++;// add left child

        the_overallinfo[Max_Extremum] = Item;
        child = cbst_trees[Item][Successor];

        if(amount == 3){
            BST_connection(Item, child, Right_child); info_cost->Length ++;// add right child
            the_overallinfo[Max_Extremum] = child;
        }
        Input_list[0] = 0; return Item;
    }

    //* 3rd step.
    amount = pretreatmentForPM(Input_list, amount, info_cost); //return the majority
    if(Input_list[0] <= 0 || Input_list[0] > Scalar || amount <= 0){
        cout<<"\n position => "<<info_cost->Kind<<"; return => "<<amount<<endl;
        string str = "\n pretreatmentForPM module in cbst_BuildingCBST;\n";
        Exception_about_memory e(0); e.adding_info(str); throw e;
    }

    Input_list[0] = amount; the_overallinfo[Max_Extremum] = Input_list[amount];

    //* 4th step.
    the_overallinfo[Root] = Pyramid_Method(Input_list, amount, info_cost);
    if(the_overallinfo[Root] <= 0){
       the_overallinfo[Root] = 0; cout<<"\n position => "<<info_cost->Kind<<endl;
       string str = "\n PM, function Pyramid_Method module in cbst_BuildingCBST; ";
       Exception_about_memory e(0); e.adding_info(str); throw e;
    }

    return the_overallinfo[Root];
}//cbst_BuildingCBST




