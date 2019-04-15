#include "CBST_Tree_Company.h"

/* -------------------------------+ building a proper tree in Flexion Model +----------------------------------------*/

int CBST::Flexion_pretreatmentForPM(int *Input_list, int total, Info_collection *info)
{//Flexion_pretreatmentForPM

    //  Method introduction
    //     1. Make a numeric Majority that equal of 2^k - 1 and n/2 <= Majority <= n. And the Rest = n - Majority;
    //     2. Scan the list from left to right:
    //        a. The node at that odd position will as child to event one; the event one would be re-written down Input_list.
    //          the connected relationship about them will be made in BST model.
    //        b. Check the number if up to Rest so as to end this operation that of odd item as event's child.

    if(total < 4) return 0;
    int Majority = make_ABinaryNumeric(total); int Rest = total - Majority;

    Input_list[0] = 0; int Index = 0; int Item = Input_list[1];
    while(Index < Majority){//loop
        if(!checkexitence(Item)){info->Kind = -1;  return -1;}
        //1. address those overplus elements to being cellars
        if(Rest){
            // first for left child:
            int Father = get_info(Item, Successor); if(!checkexitence(Father)){info->Kind = -2; return -1;}//check Father

            BST_connection(Father, Item, Left_child); updateItemInfo(Item, In, InOrOut); // Item <-- Father

            updateItemInfo(Item, -1, Self_counter); updateItemInfo(Item, 1, Auxiliary);//set pareamter in Flexion model for item

            updateItemInfo(Father, 2, Auxiliary);//set pareamter in Flexion model for item

            Rest --; info->Length ++;

            if(!pushItemInto(Input_list, Father, Scalar)){info->Kind = -3; return -1;}
            else
                Index ++;
            info->Length ++;// rewitten into array Input_list

            Item = get_info(Father, Successor); if(!Rest) continue;

            // turn to right child
            if(!checkexitence(Item)){info->Kind = -4; return -1;}
            BST_connection(Father, Item, Right_child); updateItemInfo(Item, In, InOrOut); // Father --> Item

            updateItemInfo(Item, 1, Self_counter); updateItemInfo(Item, 1, Auxiliary);//set pareamter in Flexion model for item

            updateItemInfo(Father, 3, Auxiliary);//set pareamter in Flexion model for item

            Rest --; info->Length ++;

            Item = get_info(Item, Successor); if(!checkexitence(Item)){info->Kind = -5; return -1;}
        }
        if(!pushItemInto(Input_list, Item, Scalar)){info->Kind = Index; return -1;}
        else{
            updateItemInfo(Item, 0, Flexion); updateItemInfo(Item, 0, Self_counter);
            updateItemInfo(Item, 1, Auxiliary);//set pareamter in Flexion model for item
            Index ++;
        }

        info->Length ++;
        Item = get_info(Item, Successor); // pull thos the bricks in list
    }//loop

    if(Index != Majority){info->Kind = Index; return -2;}

    Input_list[0] = Majority; info->other = Rest;// record the amount of bricks

    return Majority;
}//Flexion_pretreatmentForPM

int CBST::Flexion_Pyramid_Method(int *Input_list, int majority, Info_collection *info)
{//Flexion_Pyramid_Method
    int Father = 0; int Child = 0;
    int Cursor = 2; int Backup = Cursor;// the mobiled Cursor to get the parent

    int Width = Cursor >> 1; // the length counting from parent for pick up the children
    int step = Cursor << 1; // step for Cursor jumping rightwards in ARA

    while(step < majority || Cursor < majority){//loop
        Father = Input_list[Cursor]; if(!checkexitence(Father)){info->Kind = 0; return -1;}//check father
        updateItemInfo(Father, In, InOrOut);

        //left child
        Child = Input_list[Cursor - Width]; BST_insertion(Father, Child, Left_child); info -> Length ++;

        //set parameter for Flexion model
        updateItemInfo(Child, -1, Self_counter); int kid = get_info(Child, Right_child);
        if(kid > 0){
            int m = get_info(kid, Auxiliary); updateItemInfo(Child, (m *(-1)), Flexion);
            updateItemInfo(kid, 0, Auxiliary); if(get_info(Child, Left_child) > 0) updateItemInfo(get_info(Child, Left_child), 0, Auxiliary);
        }
        int numeric = get_info(Child, Auxiliary);

        //right child
        Child = Input_list[Cursor + Width]; BST_insertion(Father, Child, Right_child); info -> Length ++;

        //set parameter for Flexion model
        updateItemInfo(Child, 1, Self_counter);
        kid = get_info(Child, Left_child);
        if(kid > 0){
            int m = get_info(kid, Auxiliary); updateItemInfo(Child, m, Flexion);
            updateItemInfo(kid, 0, Auxiliary); if(get_info(Child, Right_child) > 0) updateItemInfo(get_info(Child, Right_child), 0, Auxiliary);
        }

        numeric = numeric + get_info(Child, Auxiliary) + 1;
        updateItemInfo(Father, numeric, Auxiliary);//set father's parameter for flexion model

        Cursor = Cursor + step;
        if(Cursor > majority){
            Cursor = Backup << 1; Backup = Cursor; Width = Cursor >> 1; step = Cursor << 1;
        }
    }//loop
    // We suppose there is virtual supper item as root's parent and with left link to point root,
    // so root have the positive fexion.
    updateItemInfo(Father, 1, Self_counter); updateItemInfo(Father, 0, Parent);

    Cursor = get_info(Father, Left_child); updateItemInfo(Father, get_info(Cursor, Auxiliary), Flexion);

    updateItemInfo(Cursor, 0, Auxiliary); updateItemInfo(get_info(Father, Right_child), 0, Auxiliary);
    updateItemInfo(Father, 0, Auxiliary);

    return Father;
}//Flexion_Pyramid_Method

int CBST::cbst_FlexionBuildingCBST(int *Input_list, Info_collection *info_cost)throw(Exception_about_memory)
{//cbst_FlexionBuildingCBST
    info_cost->initialize(); int amount = 0;

    //* 1st step.
    amount = Sorting(Input_list, info_cost, true); // amount is the number of members in ARA
    if(Input_list[0] <= 0 || Input_list[0] > 1 || amount <= 0){
        cout<<"\n position => "<<info_cost->Kind<<"; return => "<<amount<<endl;
        string str = "\n Sorting module in cbst_FlexionBuildingCBST; ";
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
        updateItemInfo(Item, 0, Parent); Input_list[0] = 0;
        updateItemInfo(Item, 0, Flexion); updateItemInfo(Item, 1, Self_counter);
        info_cost->Length ++;
        return Item;
    }
    int child = Item;
    Item = cbst_trees[Item][Successor]; updateItemInfo(Item, 1, Self_counter);//set Flexion
    if(!checkexitence(Item)){info_cost->Kind = Item; return -1;}
    if(amount <= 3)
    {
        // default two members in ARA
        the_overallinfo[Root] = Item; updateItemInfo(Item, In, InOrOut);  updateItemInfo(Item, 0, Parent);// set the root for BST
        BST_connection(Item, child, Left_child); info_cost->Length ++;// add left child

        updateItemInfo(child, 0, Flexion); updateItemInfo(child, -1, Self_counter);
        updateItemInfo(Item, 1, Flexion); //set Flexion

        the_overallinfo[Max_Extremum] = Item; child = get_info(Item, Successor);
        if(amount == 3){
            BST_connection(Item, child, Right_child); info_cost->Length ++;// add right child

            updateItemInfo(child, 0, Flexion); updateItemInfo(child, 1, Self_counter);
            updateItemInfo(Item, 2, Flexion); //set Flexion

            the_overallinfo[Max_Extremum] = child;
        }
        Input_list[0] = 0; return Item;
    }

    //* 3rd step.
    amount = Flexion_pretreatmentForPM(Input_list, amount, info_cost); //return the majority
    if(Input_list[0] <= 0 || Input_list[0] > Scalar || amount <= 0){
        cout<<"\n position => "<<info_cost->Kind<<"; return => "<<amount<<endl;
        string str = "\n Flexion_pretreatmentForPM module in cbst_FlexionBuildingCBST;\n";
        Exception_about_memory e(0); e.adding_info(str); throw e;
    }

    Input_list[0] = amount; the_overallinfo[Max_Extremum] = Input_list[amount];

    //* 4th step.
    the_overallinfo[Root] = Flexion_Pyramid_Method(Input_list, amount, info_cost);
    if(the_overallinfo[Root] <= 0){
       the_overallinfo[Root] = 0; cout<<"\n position => "<<info_cost->Kind<<endl;
       string str = "\n PM, Flexion_Pyramid_Method module in cbst_FlexionBuildingCBST; ";
       Exception_about_memory e(0); e.adding_info(str); throw e;
    }

    return the_overallinfo[Root];
}//cbst_FlexionBuildingCBST

/* ----------------------------------+ inquary by ordinal in ARA +-------------------------------------- */
int CBST::Sql_inquiryWithOrdinal(int ordinal, Info_collection *info)
{//Sql_inquiryWithOrdinal
    if(the_overallinfo[Root] <= 0) return 0;//empty tree

    int Item = the_overallinfo[Root]; int match_number = 0;
    while(Item){//loop
        if(!judge_agency(Item)){info->Kind = Item; return - 1;}//check item

        match_number = match_number + Flexion_computingFlexion(Item, true);

        if(match_number == ordinal) break;
        else if(match_number < ordinal) Item = get_info(Item, Right_child);
        else
            Item = get_info(Item, Left_child);
        info->Length ++;
        if(Item <= 0){info->Kind = Item; return 0;} //failure to search
    }//loop

    return Item;
}//Sql_inquiryWithOrdinal

/* ----------------------------------+ deletion in flexion model+-------------------------------------- */
int CBST::Flexion_deletion(int item, Info_collection *info) throw(Exception_about_memory)
{//Flexion_deletion
    if(the_overallinfo[Root] <= 0 || !judge_inorout(item)) return 0;//empty tree

    int type = estimateItem(item, info);
    if(type < 0){cout<<"\n estimateItem error in Flexion_deletion, position =>"<<info->Kind<<endl; return type;}

    int value = 0;
     switch(type){
        case Terminal: value = Flexion_deleting_terminal(item, info); break;
        case Partial_Knot: value = Flexion_deleting_PK(item, info); break;
        case Complete_Knot: value = Flexion_deleting_CK(item, info); break;
        case Juxta_Position: value = deleting_cousin(item, info); break;
        default: value = -1;
    }
    if(value < 0){
        switch(type){
        case Terminal: cout<<"\n Flexion_deleting_terminal, position => "<<info->Kind<<endl; break;
        case Partial_Knot: cout<<"\n Flexion_deleting_PK, position => "<<info->Kind<<endl; break;
        case Complete_Knot: cout<<"\n Flexion_deleting_CK, position => "<<info->Kind<<endl; break;
        case Juxta_Position: cout<<"\n deleting_cousin, position => "<<info->Kind<<endl; break;
        default: value = -1;
        }
        Exception_about_memory e(0);
        string str ="\n deletion, function Flexion_deletion\n"; e.adding_info(str); throw e;
    }
    return type;
}//Flexion_deletion

int CBST::Flexion_deleting_terminal(int item, Info_collection *info)
{//Flexion_deleting_terminal

    int alternate = deleting_memberinARA(item, info);
    if(alternate < 0){
        cout<<"\n deleting_memberinARA error in Flexion_deleting_terminal, position => "<<info->Kind<<endl; return alternate;
    }

    if(alternate > 0){//(2)
        if(info->Name <= 0){//(1)
            the_overallinfo[Root] = alternate; updateItemInfo(alternate, 0, Parent);// cut the <--> between item and cousin
        }
        else//(3)
            BST_insertion(info->Name, alternate, info->Parent_Port); // parent <--> alternate

        copyFlexion(item, alternate);// inhiret item's flexion
        resetAnItemInCL(item); updateItemInfo(alternate, In, InOrOut); info->Length ++;// as agency in CBST
    }
    else if(info->Name > 0){//(3)
        //address the flextion issue first
        Info_collection info_; info_.initialize(); int f = Flexion_UpwardsAmend(item, &info_);
        if(f < 0){
            cout<<"\n Flexion_UpwardsAmend error in Flexion_deleting_terminal, position => "<<info->Kind<<endl; return f;
        }
        info->Length = info->Length + info_.Length;

        BST_delete_aBSTRelation(info->Name, item, info->Parent_Port); // cut the <--> between item and parent
        the_overallinfo[num_Agencies] --; info->Length ++;
    }
    else//(1)
        resetWholeInfo();//empty the tree

    //reset item
    resetAnItemInBST(item); resetParametersAboutFlexion(item);
    info->Parent_Port = 0; info->Port = 0; info->Name = -1;
    the_overallinfo[num_Items] --;

    return alternate;// return the alternate if existing, otherwiae 0
}//Flexion_deleting_terminal

int CBST::Flexion_deleting_PK(int item, Info_collection *info)
{//Flexion_deleting_PK

    //* 1st step
    int alternate = deleting_memberinARA(item, info);// and settle the (4)

    //* 2nd step
    if(alternate <= 0){//(4)
        alternate = get_info(item, info->Port); // child as alternate

        //in flexion model, (1) upwards amend those flexions on nodes whose progress begins at item and along to root
        Info_collection info_; info_.initialize();
        if(Flexion_UpwardsAmend(item, &info_) < 0){
            cout<<"\n Flexion_UpwardsAmend error in Flexion_deleting_PK, position => "<<info_.Kind<<endl; return -1;
        }
        info->Length = info->Length + info_.Length;

        //(2) computing the new flexion will on alternate.
        if(Flexion_comparingTool(get_info(item, Self_counter), info->Port)){
            int m = Flexion_computingFlexion(item, false) + get_info(alternate, Flexion);//computing flexions with item and its alternate

            updateItemInfo(alternate, m, Flexion); updateItemInfo(alternate, get_info(item, Self_counter), Self_counter);//alternate inhiret item's self counter
            info->Length ++;
        }
        the_overallinfo[num_Agencies] --;
    }else{//(3)
        BST_connection(alternate, get_info(item, info->Port), info->Port); // alternate <--> child
        copyFlexion(item, alternate); updateItemInfo(alternate, In, InOrOut);
    }

    //* 3rd step
    if(info->Name){//(2)
        BST_connection(info->Name, alternate, info->Parent_Port); // father <--> alternate
        info->Length ++;// alternate <--> parent
    }
    else{//(1)
        updateItemInfo(alternate, 0, Parent); the_overallinfo[Root] = alternate;//alternate as an egilible root
    }
    resetAnItemInCL(item); resetAnItemInBST(item); resetParametersAboutFlexion(item);

    info->Name = -1; info->Port = 0; info->Parent_Port = 0;
    the_overallinfo[num_Items] --;

    return alternate; //return alternate, the cousin or child
}//Flexion_deleting_PK

int CBST::Flexion_deleting_CK(int item, Info_collection *info)
{//Flexion_deleting_CK

    //*1st step
    int left_member = get_info(item, Predecessor); info->Kind = - 1;
    if(get_info(item, Left_child) != left_member){
        if(!judge_agency(left_member)) return - 1;
    }//check left_member

    int right_member = get_info(item, Successor); info->Kind = - 2;
    if(get_info(item, Right_child) != right_member){
       if(!judge_agency(right_member)) return - 1;
    }//check right_member

    int alternate = 0; alternate = get_info(item, Cousin);

    //* 2nd step
    if(alternate > 0){//(1)
        info->Kind = - 3;
        if(!judge_inorout(alternate)) return - 2;//check alternate
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

            copyFlexion(item, alternate);//copy flextion
            updateItemInfo(alternate, In, InOrOut);//alternate as an eligible item in CBST
            resetAnItemInCL(item);//reset item's relationship in CL
        }
    }
    else{//(2) + ((4) || (3))//*3rd step
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

        //in Flexion model signal = Partial_Knot;
        //(1) address the flexion on alternate if it as a PK
        info->Kind = - 4;
        if(son_of_alternate > 0){
            if(!judge_agency(son_of_alternate)) return - 2;//check son of alternate

            if(Flexion_comparingTool(get_info(alternate, Self_counter), main_port)){
                int m = Flexion_computingFlexion(alternate, false) + get_info(son_of_alternate, Flexion);
                updateItemInfo(son_of_alternate, m, Flexion);
                updateItemInfo(son_of_alternate, get_info(alternate, Self_counter), Self_counter);
            }// son of alternate inhiret alternate
        }

        // (2) upwards amend those flextions
        Info_collection info_; info_.initialize(); info->Kind = - 5;
        if(Flexion_UpwardsAmend(alternate, &info_) < 0){
            cout<<"\n Flexion_UpwardsAmend error in Flexion_deleting_CK, position => "<<info_.Kind<<endl; return - 2;
        }
        else
            copyFlexion(item, alternate);//alternate inhiret item's flexion information

        info->Length = info->Length + info_.Length;

        //connect alternatec and the vice_port child
        int node = get_info(item, vice_port);
        BST_connection(alternate, node, vice_port); info->Length ++; // item's child at vice_port link <--> alternate

        //b.3 address (5)
        if(info->Name > 0){//(4)
            BST_connection(info->Name, alternate, info->Parent_Port); info->Length ++; // item's parent <--> alternate
        }else{//(3)
            updateItemInfo(alternate, 0, Parent); the_overallinfo[Root] = alternate;//as new root of BST
        }

        //b.4 address (6)
        info->Kind = - 6;
        if(get_info(item, main_port) != alternate){//(6)
            node = get_info(item, main_port); BST_connection(alternate, node, main_port); // item's child at main_port link <--> alternate
            info->Length ++;

            if(!judge_agency(father_of_alternate)) return - 2;
            else
                updateItemInfo(father_of_alternate, 0, vice_port);//default is (8)

            if(signal == Partial_Knot){//(7)
                BST_connection(father_of_alternate, son_of_alternate, vice_port);
                info->Length ++;
            }
        }//(6)
        the_overallinfo[num_Agencies] --;
    }//(2)
    info->Name = - 1; info->Parent_Port = 0;
    resetAnItemInBST(item); resetAMemberInARA(item); resetParametersAboutFlexion(item);//reset item in BST, ARA AND Flexion model
    the_overallinfo[num_Items] --;

    return alternate;
}//Flexion_deleting_CK

bool CBST::Flexion_insertion(int item, Info_collection *info)
{//Flexion_insertion
    info->Kind = item; if(judge_inorout(item)) return false; //check item

    updateItemInfo(item, 0, Flexion);
    if(the_overallinfo[Root] <= 0){//as the root in an empty tree
        updateItemInfo(item, In, InOrOut);//for itself
        updateItemInfo(item, 1, Self_counter); // in flextion model

        the_overallinfo[Root] = item; // for BST
        the_overallinfo[Max_Extremum] = item; the_overallinfo[Min_Extremum] = item; //for ARA

        the_overallinfo[num_Agencies] ++; //for overall info
        info->Length ++; //for statistices
    }//empty
    else{
        int father = Flexion_DownwardsAmend(get_TheKey(item), info); // find the position
        if(father <= 0){
            cout<<"\n Flexion_DownwardsAmend error in Flexion_insertion; position => "<<info->Kind<<"; return => "<<father<<endl; return false;
        }
        //1. if in CL
        if(info->Port == Cousin){
            int cousin = get_info(father, Cousin); info->Kind = cousin;
            if(cousin > 0){//father with another cousin
                if(!judge_inorout(cousin)) return false;
                else{
                    BST_insertion(item, cousin, info->Port); info->Length ++;
                }
            }//another
        }//CL
        else{// as a child
            //2.1. in ARA model
            info->Kind = - 2;
            if(!ARA_interpolation(father, item, info->Port)){cout<<"\n ARA_interpolation error in function insertion\n"; return false;}

            //2.2. in flexion model
            updateItemInfo(item, 1, Self_counter);
            if(info->Port == Left_child) updateItemInfo(item, -1, Self_counter);
            if(Flexion_comparingTool(get_info(father, Self_counter), info->Port)) Flexion_selfComputing(father, true);

            //2.3. in overall information
            info->Length ++; the_overallinfo[num_Agencies] ++;
        }//child

        //2.4. connet father and item in BST model, finally.
        //Remark: at this step, the father's information should be updated when in CL, wherefore this step at last.
        BST_insertion(father, item, info->Port); info->Length ++;// father <--> item.
    }

    info->Port = 0; info->Name = -1; the_overallinfo[num_Items] ++;
    return true;
}//Flexion_insertion







