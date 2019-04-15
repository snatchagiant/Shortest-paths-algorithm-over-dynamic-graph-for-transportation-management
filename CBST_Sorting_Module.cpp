#include "CBST_Tree_Company.h"

//data structure introduction
    /* -----------------------

        1.ARA (Adjoint Reference Axis): essential data structure is Doubly Linked List, whereas not a circuit queue.
    those items with their own keys would be sorting by their keys in ascent.
    Here, we make each key in that list not equal of, if some equal of, they will be organized
    in CL.

        2. CL (Cousin List): meanwhile it is a Doubly Linked List as well, but only one memeber in list can be permited
    to appear and take part into constrcuting the ARA we call it agency to the whole CL; wherefore, you can view it as
    the attached list to ARA.

        3. In this module, each CL will be a weak circuit -- the angency's Parent link points the distal of CL, but not inverse link
    from distal pointing to agency.

    -----------------------+----------------------------+------------------------------------ */

// merge two CLs which are weak circuit.
int CBST::CL_mergingTwoCLs(int first_agency, int second_agency)
{//CL_mergingTwoCLs
    int F_distal = get_info(first_agency, Parent);
    if(F_distal <= 0) F_distal = first_agency; else if(!checkexitence(F_distal)) return -1;// agency and distal is the same one.

    int S_distal = get_info(second_agency, Parent);
    if(S_distal <= 0) S_distal = second_agency; else if(!checkexitence(S_distal)) return -2;

    //* * * make weak circuit
    CL_Connection(F_distal, second_agency);// second agency connect to first's tail
    updateItemInfo(first_agency, S_distal, Parent);// first agency record the second's distal

    //* * * dismiss the second's relationship whose' in ARA
    updateItemInfo(second_agency, 0, Predecessor); updateItemInfo(second_agency, 0, Successor);
    return 1;
}//CL_mergingTwoCLs

int CBST::cbst_mergingTwoCLs(int first_agency, int second_agency)
{//cbst_mergingTwoCLs
    int result = CL_mergingTwoCLs(first_agency, second_agency);
    if(result < 0){
        cout<<"\n CL_mergingTwoCLs module in cbst_mergingTwoCLs; error! position =>"<<result<<endl;
        return result;
    }
    updateItemInfo(second_agency, Juxta, InOrOut);
    return result;
}//cbst_mergingTwoCLs

//pretreat the raw sequence
int CBST::PretreatSeq(int *Input_list, Info_collection *info, bool if_cbst)
{//PretreatSeq

    /* -----------------------

        The task for this function: make the Input_list wherein with stochastic permutation become a component set,

        as a pretreatment for future sorting work.

        Where our operation will be yeilding to the initial order of Input_list from left to right

        to deal with each member. In outcome, each component will be in ascent and the head of each will be pushed into array Input_list again.

        Therefore, each new subsequence will be carried by Double Linked List what we call ARA

        That is a key part for CBST ogical frame.

        Here, it is none the necessary to emphysize each size of component are same as another.

    ------------------------------------------------------------------------------------------- */
    //remark: the number of members in Input_list had been set in Input_list[0] before input.

    info->Quantity = Input_list[0]; info->other = Input_list[0]; info->Accounter = 0;

    info->Kind = -1; if(info->Quantity <= 0)return info->Quantity;

    //get the first item
    int fixed_node = Input_list[1]; info->Kind = -2;
    if(!checkexitence(fixed_node)) return -1; //check first node
    else if(info->Quantity == 1){
       info->other = 1; return 1;
    }

    //* * * input scale larger than 2
    int index = 0;
    bool search_begining = false; // false is startup; true, has been in course.
    int relation_of_seq = 0; // 0: equal, in CL; 1: in ascent. -1: descent
    int head = 0; //the head of sub-sequence would be recorded in Input_list

    //data structure introduction
    /* * *
        1. each item in future will be supported by Double Linked List so that for anyone it can eaily
           to look up it former or later neighbor in a sorting queue.

        2. Cousins will be gathered in a list called CL (Cousin List) the weak circuit.

    ------------------------------------------------------------------------------------ */

    /* *******************************
        Method introduction:

        1. fixed_node is a relative static cursor in this travels in list;
        2. mobile_node is a mobile cursor sliding from left side to right side, it is surely that this cursor will visit the list through

        3. So we can by the means of comparing fixed_node's and mobile_node's key deal with that organizing job.

        4. If present comparing result incident to the initial one in a subsequence, such as one of ascent, descent or equal, the search goes on.

        5. Otherwise, the search will be checked. There is two cases for jerk the search: (1) ascent meeting descent, or inverse (Local Constrain
        or LC). (2) another is the coerced interruption (Supreme Constrain or SC)-- mobile cursor at the end of list that the whole search has to being checked.


        6. Alternatively, a special case for a search in CL, the seach is a half legal search because CL merely supplis an agency in ARA. So, function
        only record the first element wherein CL the program firstly meets. Wherefore, that agency in an ascent subsequence, the initial comparing relation
        will keep the fix cursor's key less than mobile's, and vice versa.

        7. On a certain sense, function only record the head whereto an asecent subsequence, therefore, when a descent meets the ascent, the first kind of
        interruption, the mobile cursor will be recorded as head. That is easy able to realize.

    ******************************* */
    Input_list[0] = 0;
    //scan the list for tidy them
    for(int n = 2; n <= info->Quantity; n++)
    {//loop
        int mobile_node = Input_list[n]; // get every elements

        if(!checkexitence(mobile_node)){info->Kind = Input_list[n]; return -2;}//check item

        // default: fixed_node.key < mobile_node.key
        int present_relation = 1;
        bool moving = true; //* idefault fixed_node takes its own ergodic moving.
        bool connection = true; //* default connect fixed_node with mobile_node

    //a. compare their keys and set correspondent status
        if(get_TheKey(fixed_node) == get_TheKey(mobile_node)){
            present_relation = 0; moving = false;
        }
        else if (get_TheKey(fixed_node) > get_TheKey(mobile_node)) present_relation = -1;
        info->Length ++;

    //b. if at startup of component search
        if(!search_begining){
            if(!present_relation){// in CL, half legal search
                if(n == info->Quantity) //SC interruption
                    head = fixed_node;//agency as head
            }// CL
            else{
                if(present_relation > 0) // ascent
                    head = fixed_node; // record the head;
                else if(n == info->Quantity) // SC + descent
                    head = mobile_node; // head at right side

                search_begining = true;
                relation_of_seq = present_relation;//confirm the tune of queue
            }// ascent or descent
        }//startup
        else{//in course
            //*1. LC
            if(!(relation_of_seq + present_relation)){
                if(relation_of_seq < 0) head = fixed_node;//descent + LC
                if(n == info->Quantity){//LC + SC
                    index ++;
                    if(!pushItemInto(Input_list, mobile_node, Scalar)){
                        cout<<"\n pushItemInto error; position => aaaaa."<<endl;
                        return -1;
                    } //the last item will be in a singular set
                }
                connection = false; relation_of_seq = 0;
                search_begining = false;  //restart a new search
            }// LC
            //* 2 SC
            else if(n == info->Quantity){
                if(relation_of_seq < 0){// descent + SC
                    head = mobile_node;
                    if(!present_relation) head = fixed_node; //descent + CL + SC
                }
            }//SC
        }//course

    //c. address those heads and update cursors and other parameters

        if(head){
            if(!pushItemInto(Input_list, head, Scalar)){
                cout<<"\n pushItemInto error; position => bbbbb."<<endl;
                return -3;
            }
            index ++; head = 0;
        }// record head

        //connecting in two models the CL or ARA:
        if(connection){
            if(!present_relation){
                int f = CL_mergingTwoCLs(fixed_node, mobile_node);
                if(f < 0){cout<<"\n CL_mergingTwoCLs in PretreatSeq, error! position => "<<f<<endl; return f;}  // in CL model;
                info->Accounter ++; info->other --;// count the number of cousins  and agencies
                if(if_cbst) updateItemInfo(mobile_node, Juxta, InOrOut);//that port InOrOut will be applied in CBST
            }//CL
            else{
                if(present_relation > 0) ARA_connection(fixed_node, mobile_node);// in ARA model for fixed_node < mobile_node
                else
                    ARA_connection(mobile_node, fixed_node);// in ARA model; mobile_node > fixed_node
            }//ARA
        }//connecting

        //pull fixed_node rightwards
        if(moving) fixed_node = mobile_node;
    }//loop
    if(Input_list[0] != index){info->Kind = -3; return -4;}

    //info->Accounter: the quantity of those elements vanished in ARA but pushed into CL.
    //info->other; the number of present agenies in ARA.
    //v->Length; record the quantity of rounds.

    if(info->other + info->Accounter != info->Quantity){info->Kind = -4; return -4;}
    return index; // return quantity of heads
}//PretreatSeq

//the core engine that merga two ARAs X,Y
int CBST::SortTwoQueues(int X_head, int Y_head, Info_collection *info, bool if_cbst)
{//SortTwoQueues
    /* method introduction
        1 y.key < x.key: insert y preceding x in X queue, then after y goes on in Y queue.

        2. y.key == x.key: merge these two CLs then both go in if they have succeessors.
            if at the X queue's end, stop wander in X.

        3. y.key > x.key: if at the X queue's end, join y to x whereat to complete an whole new queue.
            if there is rest in X queue, then x goes on.

    ----------------+---------------+--------------+---------------------+-----------------------+-----------------*/
    int head = X_head;

    if(!checkexitence(X_head) || !checkexitence(Y_head)){info->Kind = -1; return -1;}
    else if(get_TheKey(X_head) > get_TheKey(Y_head)){head = Y_head; info->Length ++;} // confirm the head of new sequence

    int X_later = 0; int Y_later = 0; int x_cursor = X_head; int y_cursor = Y_head;//set two pointers

    while(y_cursor > 0){//loop
        X_later = get_info(x_cursor, Successor); Y_later = get_info(y_cursor, Successor);
        info->Length ++;
        //* 1st setp.
        if(get_TheKey(y_cursor) < get_TheKey(x_cursor)){//y.key < x.key
            //back up the next element in Y queue
            if(!ARA_insertion(x_cursor, y_cursor, Left_child)){info->Kind = -1; return -2;} //interpolation
            y_cursor = 0;
            if(Y_later > 0){
                if(!checkexitence(Y_later)){info->Kind = -2; return -2;}
                else
                    y_cursor = Y_later; // y goes on in Y ARA
            }
        }//<
        //* 2nd step.
        else if(get_TheKey(y_cursor) == get_TheKey(x_cursor)){// y.key = x.key
            int f = 0;
            if(if_cbst){
                f = cbst_mergingTwoCLs(x_cursor, y_cursor); if(f < 0){cout<<"\n cbst_mergingTwoCLs error"<<endl; return f;}
            }
            else{
                f = CL_mergingTwoCLs(x_cursor, y_cursor); if(f < 0){cout<<"\n CL_mergingTwoCLs error"<<endl; return f;}
            }

            info->Accounter ++; info->other --;// count the number of cousins and agencies

            if(X_later > 0){
                if(!checkexitence(X_later)){info->Kind = -3; return -2;}
                else
                    x_cursor = X_later;
            }
            y_cursor = 0;
            if(Y_later > 0){
                if(!checkexitence(Y_later)){info->Kind = -4; return -2;}
                else
                    y_cursor = Y_later;// both go on
            }
        }// =
        //* 3rd step.
        else{// y.key > x.key
            if(X_later > 0){
                if(!checkexitence(X_later)){info->Kind = -5; return -2;}
                else
                    x_cursor = X_later;
            }
            else{
                ARA_connection(x_cursor, y_cursor); break;// y_cursor joints x_cursor at the right end
           }
        }// >
    }//loop
    //info->Accounter: the quantity of those elements vanished in ARA but pushed into CL.
    //info->other --; the number of present agenies in ARA.
    //v->Length; record the quantity of rounds.
    return head;
}//SortTwoQueues

int CBST::Sorting(int *Input_list, Info_collection *info, bool if_cbst)
{//Sorting
    int Qty_of_head = 0;

    //*1. pretreatment
    Qty_of_head = PretreatSeq(Input_list, info, if_cbst);
    if(Qty_of_head <= 0){cout<<"\n PretreatSeq error; position => "<<info->Kind<<endl; return Qty_of_head;} //empty? the fate error appeared in function PretreatSeq

    int index = 1;  int counter = 0; Input_list[0] = 0;
    //*2. make the final queue by two contiguous components merging.
    while(Qty_of_head > 1){//loop
        int X_head =  Input_list[index]; int NewHead  = X_head; // default new head is X_head .
        int Y_head = 0; if(index < Qty_of_head) Y_head = Input_list[index + 1];

        if(Y_head){
            NewHead = SortTwoQueues(X_head, Y_head, info, true);//merge two components by the way to sort those items
            if(NewHead <= 0){cout<<"\n SortTwoQueues error, position => "<<info->Kind<<endl; return NewHead;}
        }// merging
        if(!pushItemInto(Input_list, NewHead, Scalar)){cout<<"\n pushItemInto in Sorting error!"<<endl; info->Kind = NewHead; return -1;} //backup the new head
        counter ++;
        index = index + 2;

        if(index > Qty_of_head && Qty_of_head > 1){//update those parameter for next round
            Qty_of_head = counter; index = 1; Input_list[0] = 0; counter = 0;
        }
    }//loop
    if(Qty_of_head == 1) Input_list[0] = Qty_of_head; else {info->Kind = info->Quantity; return -1;}

    counter = info->Quantity - info->Accounter; if(info->other != counter){info->Kind = counter; return -1;}

    //info->Quantity: the total number of items in ARA including those in CL and agencies.
    //info->other: the total agencies of items in ARA.
    //info->Accounter: total number of cousins in CL exluding each CL's agency that residing in ARA

    return counter;// return the number of available members in ARA
}//Sorting
