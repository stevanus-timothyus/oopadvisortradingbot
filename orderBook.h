#pragma once
#include "orderBookEntry.h"
#include "dHandle.h"
#include <string>
#include <vector>

class orderBook
{
public:
    /** construct, reading a csv data file */
    orderBook(std::string filename);
    /** return vector of all know products in the dataset*/
    std::vector<std::string> getKnownProducts();
    /** return vector of Orders according to the sent filters*/
    std::vector<orderBookEntry> getOrders(orderBookType type,
        std::string product,
        std::string timestamp);
    /** return vector of Orders according to the sent filters over a timesteps period*/
    std::vector<orderBookEntry> getTotalOrders(orderBookType type,
        std::string product,std::string timestamp, std::string timesteps);

    /** returns the earliest time in the orderbook*/
    std::string getEarliestTime();
    /** returns the previous time before the
     * sent time in the orderbook
     * If there is no previous timestamp, returns "nil"
     * */
    std::string getPrevTime(std::string timestamp);
    /** returns the next time after the
     * sent time in the orderbook
     * If there is no next timestamp, wraps around to the start
     * */
    std::string getNextTime(std::string timestamp);

    void insertOrder(orderBookEntry& order);

    std::vector<orderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

    static double getHighPrice(std::vector<orderBookEntry>& orders);
    static double getLowPrice(std::vector<orderBookEntry>& orders);
    static double getAvgPrice(std::vector<orderBookEntry>& orders);

    static double getClosePrice(std::vector<orderBookEntry>& orders, 
           double pricePoint, double priceRange);

private:
    std::vector<orderBookEntry> orders;


};
