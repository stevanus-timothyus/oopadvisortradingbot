#include <iostream>
#include "advisorMain.h"

int main()
{
    //std::cout << "Hello World!\n";
    advisorMain app{};
    app.init();
}
/*TASKS
---Classes
advisorbot - main() handles running with init() ????
advisorMain - displaying menu and calling out commands
dHandle - parsing of command & csv tokenise & exception handling
orderBook 
orderBookEntry
wallet



---Tasks   (COMMENT PROPERLY AT ALL TIMES)
plan what to do with what functions are required for best efficiency - when parsing data from csv, min,max,avg of ask/bid is directly calculated per time stamp.  
    classification of data [timeframe,[pair,[transactType,[transact],min,max]]]
think of personal function - resistance level?? ie. resist pair price currency -> resist BTC/USDT 0.00000001 BTC
find good prediction techniques online (RMA shits like that)
exception errors handlers
unit testing for commands as well as csv reading
write reports
take video
*/

/*
 points available if you are able to figure out how to make the data parsing run faster for the exchange data
*/

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
