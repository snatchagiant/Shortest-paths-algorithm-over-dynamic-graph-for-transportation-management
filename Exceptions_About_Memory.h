#ifndef EXCEPTIONS_ABOUT_MEMORY_H_INCLUDED
#define EXCEPTIONS_ABOUT_MEMORY_H_INCLUDED

#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <string>
#include <time.h>

using namespace std;

class Exception_about_memory
{
private:
    int str_number;
    string str;

public:
    Exception_about_memory(int signal):str_number(signal)
    {
        switch(signal){
            case 1: str.append("1. the number beyond the range of memory! \n"); break;

            case 2: str.append("2. The item outside the tree! \n"); break;
            case 3: str.append("3. The item inside the tree! \n"); break;
            case 4: str.append("4. The socket error! \n"); break;

            case 5: str.append("5. This item is not a terminal! \n"); break;
            case 6: str.append("6. This item is not a complete knot! \n"); break;
            case 7: str.append("7. This item is not a partial knot! \n"); break;

            case 8: str.append("8. This instance type is error, not a grid instance! \n"); break;
            case 9: str.append("9. This instance type is error, not a random instance! \n"); break;
            case 10: str.append("10. This instance type is error, not a complete instance! \n"); break;
            case 11: str.append("11. This data format weight error! \n"); break;
            case 12: str.append("12. This class has been initialized \n"); break;

            case 13: str.append("13. Failure to make a weight-grid-instance \n"); break;
            case 14: str.append("14. Failure to initialize an instance \n"); break;

            default: str = "Unknown error happen! \n";
        }
    }
    void adding_info(string s);
    void what();
};


#endif // EXCEPTIONS_ABOUT_MEMORY_H_INCLUDED
