#include "CBST_Tree_Company.h"

// -------------------------------------+ generic gadgets  +--------------------------------*/
bool CBST::judge_deletion(int item, int cost, Info_collection *info)
{//judge_deletion
    // true: ineligible, damage to structure, then surely to be deleted off CBST;
    // false: there is nothing to do
    bool flag = false; int lateral_item = 0;// defualt ie keeping in CBST

    if(cost < get_TheKey(item)){
        if(get_info(item, InOrOut) == Juxta) return true;//as cousin
        lateral_item = get_info(item, Predecessor); info->Length ++;//as item in ARA
        if(judge_agency(lateral_item)){
            if(get_TheKey(lateral_item) >= cost) return true; else return false;
        }
        else
            return false;
    }

    if(cost > get_TheKey(item)){
        if(get_info(item, InOrOut) == Juxta) return true;//as cousin
        lateral_item = get_info(item, Successor); info->Length ++;  //get the right member
        if(judge_agency(lateral_item)){
            if(get_TheKey(lateral_item) <= cost) return true; else return false;
        }
        else
            return false;
    }

    return flag;
}//judge_deletion

int CBST::make_ABinaryNumeric(int numerice)
{//make_ABinaryNumeric
    if(numerice <= 0) return 0; else if(numerice == 1) return numerice;
    int result = 1;
    for(int n = 1; n <= numerice; n++){
        if((result & numerice) == numerice) break;
        result  = result << 1; result ++;
    }
    if(result != numerice) result = result >> 1;
    return result; // result = 2^k - 1 and, numerice/2 <= result  <= numerice;
}//make_ABinaryNumeric

bool CBST::pushItemInto(int *mid_array, int item, int limit)
{//pushItemInto
    int offset = mid_array[0];
    if(offset < 0) offset  = 0; offset ++; if(offset > limit) return false;

    mid_array[offset] = item; mid_array[0] = offset;
    return true;
}//pushItemInto

// -------------------------------------+ connection gadgets  +--------------------------------*/

void CBST::CL_Connection(int parent, int child)//cnnect in CL model and involving port
{//CL_Connection
    updateItemInfo(parent, child, Cousin); updateItemInfo(child, parent, Parent);
}//CL_Connection

void CBST::ARA_connection(int left_member, int right_member)//connect in ARA
{//ARA_insertion
    updateItemInfo(left_member, right_member, Successor); updateItemInfo(right_member, left_member, Predecessor);
}//ARA_insertion

bool CBST::BST_connection(int parent, int child, int port)//connect mutually at relative socket in BST
{//connecting
    if(port < Left_child || port > Cousin) return false;
    updateItemInfo(parent, child, port); updateItemInfo(child, parent, Parent);
    return true;
}//connecting

void CBST::BST_delete_aBSTRelation(int father, int child, int port)
{//BST_delete_aBSTRelation
    updateItemInfo(father, 0, port); updateItemInfo(child, 0, Parent);
}//BST_delete_aBSTRelation

// -------------------------------------+ gadget relating to deletion   +--------------------------------*/
int CBST::estimateItem(int item, Info_collection *info)
{//estimateItem

    //* ------------------------ + Method introduction + --------------------------------- *//
       // All transactions on CBST will depend on the relationships whose links residing in BST and CL, meanwhile these strucutures conversely support the BST and CT in use of as logical frame.
       // Because the key on item maybe has been update for less or greater than before, it will cause the estimation in error if the job relies on those comparisons among keys.

    //step:
       // 1. Check item.

       // 2. estimate relationship between father and item;

       // 3. Analoguously to item and its children.

       // likelihood: (1) PK with left child; (2) PK with right child; (3) terminal without child (4) CK with two children

    /*--------------------+-------------------+------------------------- */
//* 1st step.
    info->Kind = item;
    if(!judge_inorout(item)) return -1; //check up item

//* 2nd step.
    info->Name = get_info(item, Parent); //get the parent of item
    info->Port = 0;

    if(info->Name <= 0) info->Name = 0; // means item as the root to whole tree, otherwise it is an average
    else{
        // 2.1. check up item's parent
        info->Kind = info->Name;
        if(!judge_inorout(info->Name)){cout<<"error => "<<get_info(info->Name, InOrOut)<<"; node = "<<item<<endl; return -22;} //??????????????????????????????????????????????

        // 2.2. item as cousin in CL or not
        info->Parent_Port = 0; info->Kind = - 1;
        if(get_info(item, InOrOut) == Juxta){
            if(get_info(info->Name, Cousin) != item) return - 2;//check relationship

            info->Type = Juxta_Position;
            info->Parent_Port = Cousin;//defualt item as cousin whom pointed by its parent on Cousin link.

            int next_cousin = get_info(item, Cousin);
            if(next_cousin > 0){//
                info->Length ++; info->Kind = - 2;
                if(!judge_inorout(next_cousin)) return - 2;//check next_cousin
                else{
                    info->Port = Cousin; info->other = next_cousin;
                }
            }//
            return Juxta_Position;
        }//CL
        else{//not in CL
            info->Kind = - 3;
            if(get_info(info->Name, Cousin) == item) return - 2;//check relationship

            info->Length ++; info->Kind = - 4; info->Parent_Port = Left_child;
            if(get_info(info->Name, Right_child) == item) info->Parent_Port = Right_child;
            else if(get_info(info->Name, Left_child) != item) return - 2;
        }// father >--< item
    }//parent

//* 3rd step.
    int left_kid = get_info(item, Left_child); info->Kind = - 5;
    if(left_kid > 0){
        if(!judge_agency(left_kid)){cout<<"\n judge_agency = "<<item<<"; "<<left_kid<<endl; return - 3;}  else if(item != get_info(left_kid, Parent)) return - 3; //check left kid
    }

    int right_kid = get_info(item, Right_child); info->Kind = - 6;
    if(right_kid > 0){
        if(!judge_agency(right_kid))return - 3; else if(item != get_info(right_kid, Parent)) return - 3; //check right kid
    }

    info->Type  = Partial_Knot; info->Port = Left_child;// default is PK with left child
    if(left_kid <= 0){
        info->Port = Right_child; //perhaps PK with right child
        if(right_kid <= 0){info->Port = 0; info->Type = Terminal; return Terminal;}//(3)
    }
    else if(right_kid > 0){info->Port = 0; info->Type = Complete_Knot;}//(4)

    return info->Type;
}//estimateItem

// -------------------------------------+ gadgets relating to insertion item into a BST  +--------------------------------*/
int CBST::seekatsomeone(int Weight, int start_item, Info_collection *info)
{//seekatsomeone
    int Item = start_item;
    //* start_item is the beginning, yielding to the relationship of parent towards child, the mobile cursor moves down till
    // finding the appropriate position.

    while(Item > 0){//search loop
        if(!judge_agency(Item)){info->Kind = Item; return - 1;}//check relationship
        info -> Name = Item; Item = chaseDownwards(Weight, Item, info);
        if(Item <= 0 || info -> Port == Cousin) break;
    }//loop

    // info -> Name records the id of future parent
    // info -> Port records the link

    return info -> Name;
}//seekatsomeone

// -------------------------------------+ gadget for Flextion model +--------------------------------*/
int CBST::chaseUpwards(int item, Info_collection *info)
{//chaseUpwards
    info->Kind = - 1;
    int father = get_info(item, Parent); if(!judge_agency(father)) return - 1;//check eligibility

    info->Parent_Port = Left_child; info->Kind = - 2;
    if(get_info(father, Right_child) == item) info->Parent_Port = Right_child;

    if(get_info(father, info->Parent_Port) != item){info->Parent_Port = 0; return - 1;}
    //check frame
    info->Length ++;
    return father;
}//chaseUpwards

int CBST::chaseDownwards(int key, int item, Info_collection *info)
{//chaseDownwards
    info->Port = Left_child; int child = 0;
    if(get_TheKey(item) < key) info->Port = Right_child;
    else if(get_TheKey(item) == key) info->Port = Cousin;

    child = get_info(item, info->Port); if(child < 0) child = 0;
    info->Length ++;
    return child;
}//chaseDownwards

void CBST::copyFlexion(int source, int target)
{//copyFlexion
    updateItemInfo(target, get_info(source, Flexion), Flexion); updateItemInfo(target, get_info(source, Self_counter), Self_counter);
}//copyFlexion

bool CBST::Flexion_comparingTool(int symbol, int port)
{//Flexion_comparingTool
    if((symbol > 0 && port == Left_child) ||(symbol < 0 && port == Right_child)) return true;
    return false;
}//Flexion_comparingTool

int CBST::Flexion_computingFlexion(int item, bool AddOrMinus)
{//Flexion_computingFlexion
    if(AddOrMinus) return get_info(item, Flexion) + get_info(item, Self_counter);
    return get_info(item, Flexion) - get_info(item, Self_counter);
}//Flexion_computingFlexion

int CBST::Flexion_selfComputing(int item, bool AddOrMinus)
{//Flexion_selfComputing
    int f = Flexion_computingFlexion(item, AddOrMinus); updateItemInfo(item, f, Flexion);
    return f;
}//Flexion_selfComputing

int CBST::Flexion_UpwardsAmend(int item, Info_collection *info)
{//Flexion_UpwardsAmend
    if(the_overallinfo[Root] <= 0) return 0;//empty tree

    int Item = item;
    while(Item != the_overallinfo[Root]){//loop
        if(!judge_agency(Item)){info->Kind = -1; return -1;}

        int parent = chaseUpwards(Item, info);
        if(parent <= 0){cout<<"\n Flexion_chaseUpwards error in Flexion_UpwardsAmend, position =>"<<info->Kind<<endl; return parent;}
        else{//
            int symbol = get_info(parent, Self_counter);
            if(Flexion_comparingTool(symbol, info->Parent_Port)) Flexion_selfComputing(parent, false);
        }//
        info->Length ++;
        Item = parent;
    }//loop
    return 1;
}//Flexion_UpwardsAmend

int CBST::Flexion_DownwardsAmend(int Key, Info_collection *info)
{//Flexion_DownwardsAmend
    if(the_overallinfo[Root] <= 0) return 0;//empty tree
    int Item = the_overallinfo[Root]; if(!judge_agency(Item)){info->Kind = Item; return -1;}//check item

    while(Item > 0){//loop
        int child = chaseDownwards(Key, Item, info);

        //defaut is cousin
        info -> Name = Item; if(info->Port < Left_child || info->Port > Cousin){info->Kind = -1; return -1;}//check port
        if(info->Port == Cousin) break;
        if(child <= 0) break;
        else{
            if(!judge_agency(child)){info->Kind = child; return -2;}//check child
            if(Flexion_comparingTool(get_info(Item, Self_counter), info->Port)) Flexion_selfComputing(Item, true); // update the flexion on item
            Item = child;//go on
        }//child exist
        info->Length ++;
    }//loop

    return Item;
}//Flexion_DownwardsAmend




