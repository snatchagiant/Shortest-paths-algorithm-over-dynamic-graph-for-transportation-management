#include <string>
#include "CBST_Tree_Company.h"

/*  -------------------- connection  -------------------- */
bool CBST::BST_insertion(int parent, int child, int port)//connect and modify the status identifying inside or outside; member of BST or cousin
{//BST_insertion
    if(!BST_connection(parent, child, port)) return false;
    int t = Juxta; if(port != Cousin) t = In;
    updateItemInfo(child, t, InOrOut);
    return true;
}//BST_insertion

int CBST::ARA_insertion(int the_origin_member, int the_fresh_member, int port)
{//ARA_insertion
    if(port < Left_child || port > Right_child) return 0;

    int counter = 0;
    int relating_member = 0; //default Left_child
    if(port == Left_child){
        relating_member = get_info(the_origin_member, Predecessor);
        ARA_connection(the_fresh_member, the_origin_member); counter ++;

        if(relating_member > 0){
            if(!checkexitence(relating_member)) return -1;
            counter ++; ARA_connection(relating_member, the_fresh_member);
        }
        else
            updateItemInfo(the_fresh_member, 0, Predecessor);
    }else{//Right_child
        relating_member = get_info(the_origin_member, Successor);
        ARA_connection(the_origin_member, the_fresh_member); counter ++;

        if(relating_member > 0){
            if(!checkexitence(relating_member)) return -2;
            counter ++; ARA_connection(the_fresh_member, relating_member);
        }
        else
            updateItemInfo(the_fresh_member, 0, Successor);
    }
    return counter;
}//ARA_insertion

int CBST::ARA_interpolation(int the_fixed, int the_fresh, int port)//insert member into queue of ARA
{//ARA_interpolation
    int f = ARA_insertion(the_fixed, the_fresh, port);
    if(f < 0){cout<<"\n error in ARA_insertion functor for the ineligible item, ARA_interpolation"<<endl; return f;}
    else if(!f){cout<<"\n error in ARA_insertion functor for the wrong port, ARA_interpolation"<<endl; return f;}

    // set the overall info for ARA
    if(get_info(the_fresh, Predecessor) <= 0) the_overallinfo[Min_Extremum] = the_fresh;
    if(get_info(the_fresh, Successor) <= 0) the_overallinfo[Max_Extremum] = the_fresh;

    return f;
}//ARA_interpolation

/*  -------------------- reset -------------------- */
void CBST::resetAnItemInCL(int item)
{//resetAnItemInCL
    updateItemInfo(item, 0, Parent); updateItemInfo(item, 0, Cousin);
}//resetAnItemInCL

void CBST::resetAMemberInARA(int item)//reset a member
{//resetAMemberInARA
    updateItemInfo(item, 0, Predecessor); updateItemInfo(item, 0, Successor);
}//resetAMemberInARA

void CBST::resetAnItemInBST(int item)//reset an item
{//resetAnItemInBST
    updateItemInfo(item, 0, Parent); updateItemInfo(item, 0, Left_child); updateItemInfo(item, 0, Right_child);
    updateItemInfo(item, Out, InOrOut); updateItemInfo(item, 0, Auxiliary);
}//resetAnItemInBST

void CBST::resetWholeInfo()//reset the overall info
{//resetWholeInfo
    the_overallinfo[Root] = 0; the_overallinfo[Min_Extremum] = 0; the_overallinfo[Max_Extremum] = 0;
    the_overallinfo[num_Items] = 0; the_overallinfo[num_Agencies] = 0;
}//resetWholeInfo

void CBST::resetParametersAboutFlexion(int item)
{//resetParametersAboutFlexion
    updateItemInfo(item, 0, Flexion); updateItemInfo(item, 0, Self_counter); updateItemInfo(item, 0, Auxiliary);
}//resetParametersAboutFlexion

int CBST::standard_seeking(int Weight, Info_collection *info)
{//standard_seeking
    if(the_overallinfo[Root] <= 0) return 0;//tree is empty

    return seekatsomeone(Weight, the_overallinfo[Root], info);//seeking position
}//standard_seeking

/*  -------------------- deletions area  -------------------- */

int CBST::deleting_memberinARA(int member, Info_collection *info)
{//deleting_memberinARA
    /* method introduction:
        it is supposed that the member was estimated by function estimateItem so that the input info carries some information.

        1. get the left member, right member. address the case that member is only one in ARA.

        2. check left member and right member if they are exsiting.

        3. if cousin existing, let it supersede the member's position and inherit its whole relationship

        4. otherwise, left and right connect mutually and address the case that member is an extremum in ARA.

    ********************************************************** */
    //* 1st setp
    int left_member = get_info(member, Predecessor); int right_member = get_info(member, Successor);

    int cousin = 0; cousin = get_info(member, Cousin); if(cousin < 0) cousin = 0;
    info->Kind = - 1;
    if(cousin > 0){
        info->Length ++;
        if(!judge_inorout(cousin) || get_info(cousin, InOrOut) != Juxta) return -1;
    }//check cousin

    if(left_member <= 0 && right_member <= 0){//just one member in ARA
        the_overallinfo[Min_Extremum] = 0; the_overallinfo[Max_Extremum] = 0;
        if(cousin > 0){
            the_overallinfo[Min_Extremum] = cousin; the_overallinfo[Max_Extremum] = cousin;
        }
        return cousin;
    }
    else if(left_member > 0){//* 2nd step,
        info->Kind = - 2; info->Length ++;
        if(!judge_agency(left_member)) return -1;//check left_member
    }
    else if(right_member > 0){
        info->Kind = - 3; info->Length ++;
        if(!judge_agency(right_member)) return -1;//check right_member
    }

    //* 3rd step
    if(cousin > 0){//cousin as alternate to supersede member
        if(left_member){ARA_connection(left_member, cousin); info->Length ++;} // connect with left one
        else
            the_overallinfo[Min_Extremum] = cousin;// extremum
        if(right_member){ARA_connection(cousin, right_member); info->Length ++;} // connect with right one
        else
            the_overallinfo[Max_Extremum] = cousin;// extremum
    }//cousin
    else{//no cousin
        if(!right_member){
            the_overallinfo[Max_Extremum] = left_member; updateItemInfo(left_member, 0, Successor);
        }
        else if(!left_member){
            the_overallinfo[Min_Extremum] = right_member; updateItemInfo(right_member, 0, Predecessor);
        }
        else{// neither of them is empty
            ARA_connection(left_member, right_member); info->Length ++;
        }
    }//no
    resetAMemberInARA(member);//delete member off ARA entirely

    return cousin;//if  cousin in existence then return it; otherwise return 0.
}//deleting_memberinARA

int CBST::deleting_cousin(int cousin, Info_collection *info)
{//deleting_cousin
    info->Kind = cousin;
    if(the_overallinfo[num_Agencies] <= 0) return -1;
    /* method introduction:
        1. cut the father's link that points to input cousin
        2. get next cousin, if existing, connect father and next cousin
        3. reset input cousin, return the next cousin.
    ********************************************************** */
    //* 1st step.
    BST_delete_aBSTRelation(info->Name, cousin, Cousin); info->Length ++;//info->Name, the parent of cousin

    //* 2nd step.
    int next_cousin = 0; next_cousin = get_info(cousin, Cousin);
    if(next_cousin > 0){
        BST_connection(info->Name, next_cousin, Cousin); info->Length ++; // connection in BST model
    }//next cousin existing

    //* 3rd step.
    info->Name = -1; info->Port = 0; info->Parent_Port = 0;
    resetAnItemInCL(cousin); updateItemInfo(cousin, Out, InOrOut); // delete item entirely
    the_overallinfo[num_Items] --;//update the overall info

    return next_cousin;
}//deleting_cousin

int CBST::deleting_terminal(int item, Info_collection *info)
{//deleting_terminal
    info->Kind = item;
    if(the_overallinfo[num_Agencies] <= 0) return -1;
    /* method induction
        1. delete item off ARA in which, check the cousin if in existence.
           if return 0; then no cousin as alternate to replace item's position.

        2. function need to address four sole cases: (1) item as root of BST concurrently. (2) a CL hanging on item
        (3) an average.

           the likelihood can be composed of:
             (1); (1) + (2); (3); (3) + (2);
    ------------------+-----------------------+----------------------+---------------- */

    int alternate = deleting_memberinARA(item, info);
    if(alternate < 0){cout<<"\n deleting_memberinARA error in deleting_terminal, position => "<<info->Kind<<endl; return alternate;}

    if(alternate > 0){//(2)
        if(info->Name <= 0){//(1)
            the_overallinfo[Root] = alternate; updateItemInfo(alternate, 0, Parent); // as root
        }
        else//(3)
            BST_insertion(info->Name, alternate, info->Parent_Port);//parent <--> alternate
        resetAnItemInCL(item);
        updateItemInfo(alternate, In, InOrOut);// as a formal agency in CBST
        info->Length ++;
    }
    else if(info->Name > 0){//(3)
        BST_delete_aBSTRelation(info->Name, item, info->Parent_Port); // cut the <--> between item and parent
        the_overallinfo[num_Agencies] --; info->Length ++;
    }
    else//(1)
        resetWholeInfo();//empty the tree

    //reset item
    resetAnItemInBST(item); the_overallinfo[num_Items] --;

    return alternate;// return the alternate if existing, otherwiae 0
}//deleting_terminal

int CBST::deleting_PK(int item, Info_collection *info)
{//deleting_PK
    info->Kind = item;
    if(the_overallinfo[num_Agencies] <= 0) return -1;
    /* method introduction

        00. Need to address those case:
              (1) as a root (2) as an anverage PK (3) with a CL (4) no CL hanging on

           the combinatorial likelihood:
             (1) + (4); (1) + (3) + (4); (2); (2) + (4); (2) + (3) + (4)

        01. Delete member off ARA where achieve two jobs:
            (1) seek out cousin if existing;
            (2) address the item as an extremum in ARA.

        02. Confirm the alternate, cousin or child. If being cousin, then connect item's child and its cousin.

        03. Address the father and alternate: connenct wherefore father still living.

    *********************************************************** */
    //* 1st step
    int alternate = deleting_memberinARA(item, info);// and settle the (4)

    //* 2nd step
    if(alternate <= 0){//(4)
        alternate = get_info(item, info->Port); // child as alternate
        the_overallinfo[num_Agencies] --;
    }else{//(3)
        updateItemInfo(alternate, In, InOrOut);
        BST_connection(alternate, get_info(item, info->Port), info->Port); // alternate <--> child
    }

    //* 3rd step
    if(info->Name){//(2)
        BST_connection(info->Name, alternate, info->Parent_Port); // father <--> alternate
        info->Length ++;// alternate <--> parent
    }
    else{//(1)
        updateItemInfo(alternate, 0, Parent); the_overallinfo[Root] = alternate;//alternate as a formal root
    }
    resetAnItemInCL(item); resetAnItemInBST(item);

    info->Name = -1; info->Port = 0; info->Parent_Port = 0;
    the_overallinfo[num_Items] --;

    return alternate; //return alternate, the cousin or child
}//deleting_PK

int CBST::deleting_CK(int item, Info_collection *info)
{//deleting_CK
    info->Kind = item;
    if(the_overallinfo[num_Agencies] <= 0) return -1;
    //* method introduction
        //1. Get two neighbors in ARA that at left and right side and check them up if exist.
        // We need to encounter these likelihood as follow:
            //(1) cousin as alternate, inversely, (2) descenent as alternate

            //(3) item as root, inversely, (4) item being an average item

            //(5) alternate is directive child, inversely, (6) far away item (the path among them at least longer than 2)

            //(7) alternate is a PK, on contrary, (8) may be a terminal

            //The combination forms as follows:
                //(1) + (3); (1) + (4);

                //(2) + (3); (3) + (5); (3) + (5) + (8); (3) + (5) + (7);
                           //(3) + (6); (3) + (6) + (8); (3) + (6) + (7);

                //(2) + (4); (4) + (5); (4) + (5) + (8); (4) + (5) + (7);
                           //(4) + (6); (4) + (6) + (8); (4) + (6) + (7);

        //2. Get the cousin of item if exist. (1) versus (2)
            //(1) address the case of (3) versus (4)

        //3. (2) the descendent as alternate,
            //a. first connect left neighbor and right neighbor in ARA model.
            //b. settle those things in BST model.
                // b.1. confirm the alternate if in left subtree or right subtree. Here, we comply such rule:
               // left one first if they are with same identity in BST; and PK first when they with distinct identity.

                // b.2. back up alternate's info.

                // b.3. address alternative with item's parent and child at that vice-port for (5) and (6), these operations are
                // the intersection of theirs would do; what is more, wherein it includes (4) and (3) cases.

                // b.4. is of treating (6), including the (7) and (8).
        //The whole process is yielding to a strategy: from near to far; terminal is first than PK.

   /* -------------------------------------+-------------------------+------------------------------------------- */

    //*1st step
    int left_member = get_info(item, Predecessor); info->Kind = - 1;
    if(get_info(item, Left_child) != left_member){
            if(!judge_agency(left_member)) return -1;}//check left_member

    int right_member = get_info(item, Successor); info->Kind = - 2;
    if(get_info(item, Right_child) != right_member){
       if(!judge_agency(right_member)) return -1;} //check right_member

    int alternate = 0; alternate = get_info(item, Cousin);

    //* 2nd step
    if(alternate){//(1)
        info->Kind = - 3;
        if(!judge_inorout(alternate)) return -1;//check alternate
        else{//(1)
            //a. in ARA model
            ARA_connection(left_member, alternate); info->Length ++; // alternate <--> item's predecessor
            ARA_connection(alternate, right_member); info->Length ++; // alternate <--> item's successor

            //b. in BST model
            BST_connection(alternate, get_info(item, Left_child), Left_child); info->Length ++; // alternate <--> item's Left child
            BST_connection(alternate, get_info(item, Right_child), Right_child); info->Length ++; // alternate <--> item's Right child

            updateItemInfo(alternate, 0, Parent); //default is root
            if(info->Name){//(4)
                BST_connection(info->Name, alternate, info->Parent_Port); info->Length ++;
            }
            else//(3)
                the_overallinfo[Root] = alternate;// registered as a root to replace item

            updateItemInfo(alternate, In, InOrOut);//alternate as an eligible item in CBST
            resetAnItemInCL(item);//reset item's relationship in CL
        }
    }
    else{//(2) or (3) //*3rd step
        //a. in ARA model
        ARA_connection(left_member, right_member); info->Length ++;

        //b. in BST model
        //b.1 confirm the alternate
        alternate = left_member; int signal = Partial_Knot;
        int main_port = Left_child; int vice_port = Right_child; //default is left member and it is a PK

        //unless left_member is terminal but right_member is PK
        if(!get_info(left_member, main_port)){//as a Terminal
            if(get_info(right_member, vice_port)){//right_member as a PK
               alternate = right_member;
               main_port = vice_port; vice_port = Left_child;
            }
            else
                signal = Terminal;//both are terminal
            info->Length ++;
        }

        //b.2 backup some info
        int father_of_alternate = get_info(alternate, Parent); int son_of_alternate = get_info(alternate, main_port);

        // b.3 connect alternatec and the vice_port child (5)
        int node = get_info(item, vice_port);
        BST_connection(alternate, node, vice_port); info->Length ++; // item's child at vice_port link <--> alternate

        if(info->Name > 0){//(4)
            BST_connection(info->Name, alternate, info->Parent_Port); info->Length ++; // item's parent <--> alternate
        }else{//(3)
            updateItemInfo(alternate, 0, Parent); the_overallinfo[Root] = alternate;//as new root of BST
        }

        //b.4 address (6)
        if(get_info(item, main_port)!= alternate){//(6)
            node = get_info(item, main_port);
            BST_connection(alternate, node, main_port); info->Length ++; // item's child at main_port link <--> alternate

            info->Kind = -4;
            if(!judge_agency(father_of_alternate)) return -1;
            else
                updateItemInfo(father_of_alternate, 0, vice_port);//default is (8)

            info->Kind = -5;
            if(signal == Partial_Knot){//(7)
                if(!judge_agency(son_of_alternate)) return -1;
                else
                    BST_connection(father_of_alternate, son_of_alternate, vice_port);
                info->Length ++;
            }
        }//(6)
        the_overallinfo[num_Agencies] --;
    }//(2)
    info->Name = -1; info->Parent_Port = 0;
    resetAnItemInBST(item); resetAMemberInARA(item);//reset item in BST and ARA model
    the_overallinfo[num_Items] --;

    return alternate;
}//deleting_CK
