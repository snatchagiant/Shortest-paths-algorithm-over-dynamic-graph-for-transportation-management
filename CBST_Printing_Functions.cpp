#include "CBST_Tree_Company.h"

//print functions
bool CBST::print_CL(int head, Info_collection *info)
{//print_CL
    if(!checkexitence(head)){info->Kind = -1; return false;}

    int key = get_TheKey(head); info->Kind = 0;
    int item = get_info(head, Cousin);
    if(item  > 0){
        cout<<"(";
        while(item > 0){//print loop
            if(get_TheKey(item) != key){info->Kind = item; return false;}
            cout<<item; item = get_info(item, Cousin);if(item > 0) cout<<"; "; else break;
        }//loop
        cout<<")";
    }
    return true;
}//print_CL

bool CBST::print_ARA(int head, Info_collection *info)
{//print_ARA
    bool flag = true;
    if(!checkexitence(head)){info->Kind = -1; return false;}

    int Item = head;
    while(Item > 0){
        int next = get_info(Item, Successor);
        if(next > 0 ){
            if(!checkexitence(head)){info->Kind = -2; return false;}
            else if(get_TheKey(Item) >= get_TheKey(next)){info->Kind = -3; return false;}
        }
        cout<<Item;
        if(get_info(Item, Cousin)){//print CL
            flag = print_CL(Item, info);
            if(!flag){cout<<"\n print CL, position =>"<<info->Kind; return false;}
        }
        cout<<"; "; Item = next;
    }
    return flag;
}//print_ARA

void CBST::printOverallInfo()
{
    cout<<endl;
    cout<<"1. min  = "<<the_overallinfo[Min_Extremum]<<endl;
    cout<<"2. max = "<<the_overallinfo[Max_Extremum]<<endl;
    cout<<"3. root = "<<the_overallinfo[Root]<<endl;
    cout<<"4. num_agencies = "<<the_overallinfo[num_Agencies]<<endl;
    cout<<"5. num_items = "<<the_overallinfo[num_Items]<<endl;
}

bool CBST::printTree(int *aid_1, int *aid_2, Info_collection *info)
{
    printOverallInfo(); cout<<endl;
    if(the_overallinfo[Root] > 0)
    {
        if(!checkexitence(the_overallinfo[Root])){info->Kind = -1; return false;}//check root

        int index = 1; aid_1[0] = index; aid_1[index] = the_overallinfo[Root];
        while(index){//loop
            index = 0;
            for(int n = 1; n <= aid_1[0]; n++){
                int father = aid_1[n]; if(!checkexitence(father)){info->Kind = father; return false;}

                int left = get_info(father, Left_child); int right = get_info(father, Right_child);
                if(left > 0){
                    index ++; aid_2[index] = left; aid_2[0] = index; cout<<left<<"<-- ";
                }
                cout<<father;
                /**/
                if(get_info(father, Cousin) > 0){
                    if(!print_CL(father, info)) {info->Kind = father; return false;}
                }

                if(right > 0){
                    index ++; aid_2[index] = right; aid_2[0] = index; cout<<" -->"<<right;
                }
                cout<<"; ";
            }cout<<endl;
        int *p = aid_1; aid_1 = aid_2; aid_2 = p; aid_2[0] = 0;
        }//loop
    }
    else{
        cout<<"\n empty tree"<<endl;
    }
    return true;
}

bool CBST::PrintItem(int item)
{
    if(!checkexitence(item)) return false;
    cout<<endl;
    cout<<"1. Parent = "<<get_info(item, Parent)<<endl;
    cout<<"2. Left_Child = "<<get_info(item, Left_child)<<endl;
    cout<<"3. Right_Child = "<<get_info(item, Right_child)<<endl;
    cout<<"4. Couisin = "<<get_info(item, Cousin)<<endl;
    cout<<"5. InOrOut = "<<get_info(item, InOrOut)<<endl;
    cout<<"6. Successor = "<<get_info(item, Successor)<<endl;
    cout<<"7. Predecessor = "<<get_info(item, Predecessor)<<endl;

    return true;
}

//flexion model

bool CBST::Flexion_printTree(int *aid_1, int *aid_2, Info_collection *info)
{//Flexion_printTree
    printOverallInfo(); cout<<endl;
    if(the_overallinfo[Root] > 0)
    {
        if(!checkexitence(the_overallinfo[Root])){info->Kind = -1; return false;}//check root

        int index = 1; aid_1[0] = index; aid_1[index] = the_overallinfo[Root];
        while(index){//loop
            index = 0;
            for(int n = 1; n <= aid_1[0]; n++){
                int father = aid_1[n]; if(!checkexitence(father)){info->Kind = father; return false;}

                int left = get_info(father, Left_child); int right = get_info(father, Right_child);
                if(left > 0){
                    index ++; aid_2[index] = left; aid_2[0] = index; cout<<left<<"<-- ";
                }
                cout<<father<<"("<<get_info(father, Flexion)<<", "<<get_info(father, Self_counter)<<")";

                if(right > 0){
                    index ++; aid_2[index] = right; aid_2[0] = index; cout<<" -->"<<right;
                }
                cout<<"; ";
            }cout<<endl;
        int *p = aid_1; aid_1 = aid_2; aid_2 = p; aid_2[0] = 0;
        }//loop
    }
    else{
        cout<<"\n empty tree"<<endl;
    }
    return true;
}//Flexion_printTree

bool CBST::Flexion_print_ARA(int head, Info_collection *info)
{//print_ARA
    bool flag = true;
    if(!checkexitence(head)){info->Kind = -1; return false;}

    int Item = head;
    while(Item > 0){
        int next = get_info(Item, Successor);
        if(next > 0 ){
            if(!checkexitence(Item)){info->Kind = -2; return false;}
            else if(get_TheKey(Item) >= get_TheKey(next)){info->Kind = -3; return false;}
        }
        cout<<Item<<"("<<get_info(Item, Flexion)<<", "<<get_info(Item, Self_counter)<<"); ";
        Item = next;
    }
    return flag;
}//print_ARA






