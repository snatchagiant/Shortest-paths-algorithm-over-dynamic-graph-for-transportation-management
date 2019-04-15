#include <iostream>
#include "Exceptions_About_Memory.h"

using namespace std;

void Exception_about_memory::adding_info(string s)
{
    str.append(" And,"); str.append(s);
}

void Exception_about_memory::what(){cout<<str<<endl;}
