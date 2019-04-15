# Introduce

This source code is encoded by C++ and, implemented in console platform. It is for the experimental simulation of fastest path problem on dynamic graph.  

# Arguments Setting

Firstly, in main function, we use the grid instance for our experiments. Note that our laptop only runs within 4G memory that  would lead to the application to OS for memory perhaps to limited below 1220,0000 (3500 x 3500)  nodes. If you want to use another type of instance, please define that by yourself without hesitance, but to comply the data structure used for define the connecting relationship among nodes in graph: for 2D array, the horizon index is root pointing to the second dimension array that is leaf set. In this set, the head of list[0] is the number to commend the leaf amount in native set, of which into general terminology is outgoing degree . The graph construct is the 2D pointer Instance, the describing parameter is gap the upper bound of the leave set. 

The Weight_list companied with Instance with identical construct for both. The function for this data structure is representing the length of lane, which connects two nodes as same as terminological edge. Over there, the list just carries the indices which used in array Length_list. We use this list to define the length having 46 grades for this scalar. In class Graph_Instance, that lane would be assigned randomly and the value is yet out from list Length_list. In this code, we define the two inverse links that contained by lane, for example (x, y) and (y, x), we let their length are same. In fact, you can define inequality, because that inequality feather would not affect the algorithm yielding correct results. 

Then the two constructs, Instance and Weight_list commonly comprises the fixed relationship as static domain through the whole computation over the dynamic graph. Via the same construct, a link in Instance is easy to connect with the length that in Weight_list by the same position information in leaf set.

The second, we introduce the dynamic domain, in general, people call time dependent table, whose function is to carry a table to record the traffic state change over time. In our computing model, we use the time continuous model to define the cost function, of which, it almost always called piecewise function. The calculation is through the segments piecing together to solve for the through time over certain link. These functions have been defined in class as member function (in file Graph_Arithmetic_Travels.cpp):    T_PieceWiseCost();T_costfunction() and calculateTravelALink(). Note that in the third function, we let the value for computing index for table to be of the sum of departure time and length. This wise we set, is for adding a variable factor in querying table, otherwise we have no other aim.

The time table is defined by 23 grades, from 10 kph to 120 kph; we call velocity, denoted by capital V, likewise which is a 2D list. Note that at the row, we define the time interval in table by 10 sec. also it is the sampling interval for gather the traffic data, in that 2D list V, there are 9 grades to represent 9 second to 1 second, as it was, that minimum accuracy is 1 second. 

In our computing model, the time table might be infinite, but you can see the description parameter time_table_size but is 100000 (indeed you can define the number liberally) on time table T_time_depentent_table. The important step is defined the manner of visiting time table V: we let the quotient and remainder of time interval into departure that has been reformed. The quotient at the end becomes the index for time table for retrieve velocity for piecing operation. The remainder is just for the subtle accuracy. The key step is on quotient, if it beyond the size of table where we have referred to 100000, then we set a function to drag it into the scope of table. 

Because our interesting is only just put on studying computation, therefore, we let all links share with the same time table other than literatures do in the way of one link one time table.

# The Computing Process

The first phrase is HDM, as similar as Dijkstra’s method or A star method. The source and initial departure time is stochastic. The condition of program halting is all targets obtain optimality that from the previous region.

The second phrase is CA, like Bellmans. The LE is still as being the priority heap like in static version (source code: https://github.com/snatchagiant/with-CBST-solve-shortest-paths-problem). In source code, the instance is embodied by the class CBST.  

The finally part is printing various data for describing the algorithm and overhead.

# The license

The Intelligent Property about this entire solution including CBST and on shortest paths problem is deserved to author in nature. It is only just for the academic application or research works, so far not in any commerce. If you intend to apply those algorithms in duplication, distribution, publishing, curriculum or teaching works; or on the other hand as a module in your software or project; please annotate clearly the reference or citation. If necessary, notice author for grant please.

Copyright (C) Yong Tan; if contact with us by email: yongtan_navigation@outlook.com

