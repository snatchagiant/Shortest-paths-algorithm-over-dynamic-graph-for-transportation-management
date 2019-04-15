#ifndef CBST_TREE_COMPANY_H_INCLUDED
#define CBST_TREE_COMPANY_H_INCLUDED

#include "Auxiliary_Data_Structure.h"

class CBST
{
private:
    int **cbst_trees; // carry the all connected relationship in arc model.
    int *key_list; // each element is the total weight about that node.
    int Scalar;// the number of nodes

    int *the_overallinfo; // the pointer exclusively indicate the address of set recording the overall info. for CBST
    int the_num_of_items; // record the number of that items

    // --- + general operations + --- //
    bool check_memory(int item){if(item < 0 || item > Scalar) return false; return true;}// check the item if exceeding the extent of memory
    bool judge_inorout(int item){if(checkexitence(item)){if(cbst_trees[item][InOrOut] >= Juxta) return true;} return false;} // judge in or out CSBT
    bool judge_agency(int item){if(checkexitence(item)){if(cbst_trees[item][InOrOut] > Juxta) return true;} return false;}
    void updateItemInfo(int Item, int Value, int Port){cbst_trees[Item][Port] = Value;};

    //--- + CBST_Essential_Tools module + ---//
    int make_ABinaryNumeric(int numerice); // return a number 2^k - 1 = 1111...1 that > numerice/2 and < numerice.
    bool pushItemInto(int *mid_array, int item, int limit); // push an item into a heap the mid_arry

    void CL_Connection(int parent, int child);//connection in CL model pushItemInto
    void ARA_connection(int left_member, int right_member); //connection in ARA model
    bool BST_connection(int parent, int child, int port); // connection in BST model

    void BST_delete_aBSTRelation(int father, int child, int port); // cut out a connective relation in BST model

    int estimateItem(int item, Info_collection *info);
    //   The functor is important to those deleting functiors, they must consort together really able to achieve their own jobs, if not so,
    // there should be in error definitely.
    // A. Function: get the info about item's parent, including id and link that pointing to item; analoguously to item and its own child if they exist.
    // B. The outcome:
    //      Functor at first to estimate the type of item and return type, which including cousin? terminal? partial_knot? or complete_knot.
    //     If being negative, then there is an error been appeared, by info->Kind you can lock the position up.
     // C. Attached information:
     //      The info->Parent_Port carries link that pointing to item from parent.
     //      1. The info->Type backups the type of item.
     //      2. If item as being a Terminal or Complete: info->Port equal of 0; conversely for Partial Knot, it carries the link.
     //      3. If item be a cousin in CL, info->Port equal of Cousin
     //      4. all parent, child and next cousin(if item as a cousin) will be estimate for their eligibility.

    int seekatsomeone(int Weight, int start_item, Info_collection *info);
    // Function: Seek the appropriate position for fresh one by comparison its key the parameter Weight.
    //          The survey will begin at start_item that in BST
    // Outcome:
    //  1. Return the id of future father
    //  2. The info -> Name backups the father name, info -> Port carries the link
    //  3. The info->Length counts the numeric for complexity analyte.
    //  4. If node in CBST ineligible, then return -1.

    int chaseUpwards(int item, Info_collection *info);
    // Return father if existing, otherwise is 0; info -> Parent_Port records the link on fater pointing to item

    int chaseDownwards(int key, int item, Info_collection *info);
    // Return child if existing and apt. The search is in the way of comparing keys.

    void copyFlexion(int source, int target);
    bool Flexion_comparingTool(int symbol, int port); // Retrun true: match and able to computing flexion

    int Flexion_computingFlexion(int item, bool AddOrMinus);// AddOrMinus true, then flexion be caused to adding, inverse to minus on item
    int Flexion_selfComputing(int item, bool AddOrMinus); // do a flexion computing and update self'd data

    int Flexion_UpwardsAmend(int item, Info_collection *info);
    // The chasing and correcting felxion is from item along to root yeilding to the structure of BST.
    int Flexion_DownwardsAmend(int Weight, Info_collection *info);
    // the direction is just inverse to Flexion_UpwardsAmend: from root along to item

    /* --- + CBST_Sorting_Module + --- */

    int CL_mergingTwoCLs(int first_agency, int second_agency);//merge two CL curcuits
    int cbst_mergingTwoCLs(int Agency, int fresh); //merge two CL curcuit and change the InOrOut, exclusive for CBST model

    int PretreatSeq(int *Input_list, Info_collection *info, bool if_cbst = false);
    /*
        The pretreatment is the first step for sorting work:
        according to the original order of permutation of members in list, function makes the input list come to
        a set of component the ARA whereat each contains a subqueue in ascent whose sorting yielding to items' key. The input
        list regains those members but their identities are the head of subqueue, others depends on the relation of Doubly
        Linked List to connect together.

        if_cbst is a switch, if true, this function can set those elements' InOrOut port beng 0 when they were pulled into CL.

        Output:
        info -> Accounter: the number of CLs' members
        info -> Qunatity: the number of input items that in Input_list.
        info -> other: the number of members in ARA

        Return a numeric that is the number of heads in Input_list.
    */
    // the engine as the centrol unit component to merge that separate subsets by one with one.
    int SortTwoQueues(int X_head, int Y_head, Info_collection *info, bool if_cbst);//genric tool
    /*
        The core engine for merging and sorting two subqueues. If two heads of CLs, then merge them. The if_cbst switch, its
        function same as in function PretreatSeq.

        Output:
          Reutrn the head of new queue that in ascent of members' key.
          info -> Accounter: the number of CLs' members
          info -> Qunatity: the number of input items that in Input_list.
          info -> other: the number of members in ARA

    */

    /* --- + CBST_Tree_Basic_Operations + --- */
    bool BST_insertion(int parent, int child, int port);
    //connect a parent and child at the correspondent port, if return false, then the port is wrong.

    int ARA_insertion(int the_origin_member, int the_fresh_member, int port);
    // the fresh one will be interpolated preceding or following the origining one.
    // return the number of connections, if negative, error is over there.

    int ARA_interpolation(int the_fixed, int the_fresh, int port);
    // interpolate item into ARA, meanwhile update overall information of ARA.

    void resetAnItemInCL(int item);
    void resetAnItemInBST(int item);
    void resetAMemberInARA(int item);
    void resetParametersAboutFlexion(int item);
    void resetWholeInfo();

    int standard_seeking(int Weight, Info_collection *info);
    // Look up an appropriate position for a fresh one in BST, but search starts up from root of tree.

    int deleting_memberinARA(int member, Info_collection *info);
    // Deleting an item off ARA.
    // Return the alternate if item with a nonempty CL, otherwise return 0.
    // Return negative numeric, error happened, info->Kind tags the positin.

    int deleting_cousin(int cousin, Info_collection *info);
    // Remove a cousin item off BST and return cousin's next brother if existing.

    int deleting_terminal(int item, Info_collection *v);
    // Will  return the alternate who replace the item's position.
    // If return 0, then there wasn't a CL follow item.

    int deleting_PK(int item, Info_collection *v);
    // Return the alternate, either the cousin or the child; if negative numeric, then erroe

    int deleting_CK(int item, Info_collection *v);
    // Return alternate; if negative numeric, then erroe

    /* --- + CBST_Tree_Building + --- */
    // pretreat the sorting sequence for PM
    int pretreatmentForPM(int *Input_list, int total, Info_collection *info);
    //   The input list Input_list is containing a doubly linked list ARA that in ascending sorting
    // Amid those elememts, someone can with a CL, oneself as being an agency.
    //   The output is a primary tree with majority items that of 2^k - 1, as the basical ingradients will used to build the BST.
    // The rest members in ARA has been settle undert those bricks as their children.
    //  Output: the numeric of majority. The info->other is the difference of n - majority

    //build a proper tree.
    int Pyramid_Method(int *Input_list, int majority, Info_collection *info);
    //  Function is building a proper tree that job starts up from an ARA bottom; of course, the final shape likes a pryramid.
    // The parameter Input_list must be treated by functor pretreatmentForPM.
    //  The result being returned is the root of BST.

    /* --- + CBST_Flexion_Module + --- */
    int Flexion_pretreatmentForPM(int *Input_list, int total, Info_collection *info);// pretread sorting sequence for PM in flexion model
    int Flexion_Pyramid_Method(int *Input_list, int majority, Info_collection *info);

    // Make a proper tree underlying flexion model.
    int Flexion_deleting_terminal(int item, Info_collection *info);
    int Flexion_deleting_PK(int item, Info_collection *info);
    int Flexion_deleting_CK(int item, Info_collection *info);

public:
    CBST(int **c, // the CBST frame in memory
         int *k, // the nth linear list, as the form: node's id -> total weight
         int s //the number of nodes
    ):cbst_trees(c), key_list(k), Scalar(s){
        the_overallinfo = cbst_trees[0];
        if(s <= 0){
            cout<<"\n The scalar of instance is empty"; exit(1);
        }
        InitializeAllItems();
    }

    /*--- + CBST_Enumeration_Module + --- */
    int Enumeration_MinList(int *emu_list, Info_collection *info)throw(Exception_about_memory);
    // Functor is to collect all minmum items in CBST one-by-one and bag them with emu_list.
    // 1. Return 0: the empty tree
    // 2. other errors would be threw out by Exception class.

    int popItem(int *item_list, int limit);
    // 1. Pop the last item from a heap the parameter item_list.
    // 2. If return 0; then means empty;
    // 3. If item's InOrOut signal indicated in CBST, it will be corrected to Out.
    // 4. If item is other type, the (3) operation will be ingored.

    int travelCL(int agency);
    // Given a CL with a CL, this functor will elect a new agency for CL and cut the presentive agency off CL.
    // Return the id of new agency

    /*--- + CBST_Essential_Tools + --- */
    bool judge_deletion(int item, int cost, Info_collection *info);
    // Determine if the item's continual existence damages the logical frame of CBST while its key changes.
    // Return true: need to be deleted off, inversely for false.

     /* --- + CBST_Flexion_Module + --- */
    int Sql_inquiryWithOrdinal(int ordinal, Info_collection *info);

    int Flexion_deletion(int item, Info_collection *info)throw(Exception_about_memory);
    bool Flexion_insertion(int item, Info_collection *info);

    int cbst_FlexionBuildingCBST(int *Input_list, Info_collection *info_cost)throw(Exception_about_memory);

    /* --- + CBST_Integrated_Operations + --- */
    int resetCBST(Info_collection *info);// reset all items in CBST
    // Reset all items in CBST and, return the amount of dones. If return negative numeric, then error had happened

    bool InitializeAllItems();// initialize all nodes.

    int gain_Min_Extremum(Info_collection *info);
    // delete an extremum element off CSBT and return it; if negative, error.

    int gain_theMinCL(Info_collection *info);
    // delete an extremum element off CSBT and return it; maybe it is an agency with a CL

    bool insertion(int item, Info_collection *info)throw(Exception_about_memory);//insertion an item into the CBST
    // insert an item into CBST. If error happens, the info->King will carries the info of error's position

    int insertionAList(int *item_list, Info_collection *info)throw(Exception_about_memory);
    // Batch inserting a group of items into CBST and return the number of items whom have pushed into CBST
    // info->other carries the ineligible item, if return negative numeric.

    int deletion(int item, Info_collection *info)throw(Exception_about_memory);//insertion an item off the CBST
    // If return 0 or negative numeric, then error.

    bool set_AuxiliaryOnItem(int item, int value); // In an item, the unit of Auxiliary is auxiliary for computing

    /* --- + CBST_Sorting_Module  + --- */
    int Sorting(int *Input_list, Info_collection *info, bool if_cbst = true);//genric tool
    //  The task is sorting a stochastics sequeue into a doubly linked list and in ascent by elements' keys.
    // The result of the number of available members in ARA will be returned
    // Those atteched information:
    //  1. info->Quantity: the total number of items in ARA including those in CL and agencies.
    //  2. info->other: the total agencies of items in ARA.
    //  3. info->Accounter: total number of cousins in CL exluding each CL's agency that residing in ARA


    /* --- + CBST_Tree_Building + --- */
    int cbst_BuildingCBST(int *Input_list, Info_collection *info)throw(Exception_about_memory);
    // The work is buidling a proper tree. Input_list is a sequence with sochastic permutation; return root.

    /* --- + general operations + --- */
    int get_MinEtremum(){return the_overallinfo[Min_Extremum];}
    // return the extremum's id in CSBT which with minimum key if it existing

    int get_MaxEtremum(){return the_overallinfo[Max_Extremum];}
    // return the extremum's id in CSBT which with max key if it existing

    void set_ItemOut(int item){updateItemInfo(item, Out, InOrOut);};
    // set item's label means outside the CBST

    // obtain various info.
    int get_info(int node, int port){if(node < 0 || node > Scalar) return -1; return cbst_trees[node][port];}
    int get_TheKey(int item){return key_list[item];}


    int get_RootInCBST(){return the_overallinfo[Root];};
    int get_NumberOfAgencies(){return the_overallinfo[num_Agencies];}
    int get_NumberOfItems(){return the_overallinfo[num_Items];}
    int get_MemoryScale(){return Scalar;};

    // operations
    void itemdecrement(){the_overallinfo[num_Items] --;} // self-subtract the number of items in CBST
    void set_TheKey(int item, int key){key_list[item] = key;} // change the key
    bool checkexitence(int item){if(item > 0 && item <= Scalar) return true; return false;} // check item's eligibility

    void resetAUnit(int item){
        resetAMemberInARA(item); resetAnItemInBST(item); resetAnItemInCL(item); resetParametersAboutFlexion(item);
    };//reset an item in CBST


    /* ------------------ Printing functors ------------------*/

    bool print_CL(int head, Info_collection *info);
    bool print_ARA(int head, Info_collection *info);
    void printOverallInfo();
    bool printTree(int *aid_1, int *aid_2, Info_collection *info);
    bool PrintItem(int item);

    bool Flexion_printTree(int *aid_1, int *aid_2, Info_collection *info);
    bool Flexion_print_ARA(int head, Info_collection *info);

};

#endif // CBST_TREE_COMPANY_H_INCLUDED
