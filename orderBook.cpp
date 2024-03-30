#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include "orderBook.h"
#include "dHandle.h"


/** construct, reading a csv data file */
orderBook::orderBook(std::string filename)
{
    orders = dHandle::readCSV(filename);
}

/** return vector of all know products in the dataset*/
std::vector<std::string> orderBook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string, bool> prodMap;

    for (orderBookEntry& e : orders)
    {
        prodMap[e.product] = true;
    }

    // now flatten the map to a vector of strings
    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}

/** return vector of Orders according to the sent filters*/
std::vector<orderBookEntry> orderBook::getOrders(orderBookType type,
    std::string product,
    std::string timestamp)
{
    std::vector<orderBookEntry> orders_sub;
    for (orderBookEntry& e : orders)
    {
        if (e.orderType == type &&
            e.product == product &&
            e.timestamp == timestamp)
        {
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}

std::vector<orderBookEntry> orderBook::getTotalOrders(orderBookType type,
    std::string product,
    std::string timestamp,
    std::string timesteps)
{
    std::vector<orderBookEntry> orders_sub;
    std::string iteratedTime = timestamp;
    int steps = std::stoi(timesteps);
    int stepsc = 1;
    for (unsigned int i = 1; i <= steps; i++)
    {
        for (orderBookEntry& e : orders)
        {
            if (e.orderType == type &&
                e.product == product &&
                e.timestamp == iteratedTime)
            {
                orders_sub.push_back(e);
            }
        }
        iteratedTime = getPrevTime(iteratedTime);
        if (iteratedTime == "nil")
        {
            stepsc = i;
            break;
        }
        stepsc = i;
    }
    if (steps == stepsc)
    {
        std::cout << "In the past " << steps << " timesteps, ";
    }
    else
    {
        std::cout << "There wasn't enough data to show " << steps << " timesteps. However... " << std::endl;
        std::cout << "With the past " << stepsc << " timesteps, ";
    }
    
    return orders_sub;
}


double orderBook::getHighPrice(std::vector<orderBookEntry>& orders)
{
    double max = orders[0].price;
    for (orderBookEntry& e : orders)
    {
        if (e.price > max)max = e.price;
    }
    return max;
}


double orderBook::getLowPrice(std::vector<orderBookEntry>& orders)
{
    double min = orders[0].price;
    for (orderBookEntry& e : orders)
    {
        if (e.price < min)min = e.price;
    }
    return min;
}

double orderBook::getAvgPrice(std::vector<orderBookEntry>& orders)
{
    double total = 0;
    for (orderBookEntry& e : orders)
    {
       total += e.price;
    }
    double avg = total / orders.size();
    //std::cout << "total: " << total << std::endl;
    //std::cout << "size: " << orders.size() << std::endl;
    return avg;
}
double orderBook::getClosePrice(std::vector<orderBookEntry>& orders,double pricePoint, double priceRange)
{
    double total = 0;
    for (orderBookEntry& e : orders)
    {
        if ((pricePoint + priceRange) > e.price && e.price > (pricePoint - priceRange))
            total++;
    }
    //std::cout << "total: " << total << std::endl;
    //std::cout << "ordersize: " << orders.size() << std::endl;
    return (total / orders.size());
}


std::string orderBook::getEarliestTime()
{
    return orders[0].timestamp;
}

std::string orderBook::getPrevTime(std::string timestamp)
{
    std::string prev_timestamp = "";
    for (orderBookEntry& e : orders)
    {
        if (e.timestamp < timestamp)
        {
            prev_timestamp = e.timestamp;
        }
        if (e.timestamp == timestamp)
        {
            break;
        }
    }
    if (timestamp == orders[0].timestamp)
    {
        return "nil";
    }
    return prev_timestamp;
}

std::string orderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (orderBookEntry& e : orders)
    {
        if (e.timestamp > timestamp)
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}

void orderBook::insertOrder(orderBookEntry& order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), orderBookEntry::compareByTimestamp);
}

std::vector<orderBookEntry> orderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    // asks = orderbook.asks
    std::vector<orderBookEntry> asks = getOrders(orderBookType::ask,
        product,
        timestamp);
    // bids = orderbook.bids
    std::vector<orderBookEntry> bids = getOrders(orderBookType::bid,
        product,
        timestamp);

    // sales = []
    std::vector<orderBookEntry> sales;

    // I put in a little check to ensure we have bids and asks
    // to process.
    if (asks.size() == 0 || bids.size() == 0)
    {
        std::cout << " orderBook::matchAsksToBids no bids or asks" << std::endl;
        return sales;
    }

    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), orderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), orderBookEntry::compareByPriceDesc);
    // for ask in asks:
    std::cout << "max ask " << asks[asks.size() - 1].price << std::endl;
    std::cout << "min ask " << asks[0].price << std::endl;
    std::cout << "max bid " << bids[0].price << std::endl;
    std::cout << "min bid " << bids[bids.size() - 1].price << std::endl;

    for (orderBookEntry& ask : asks)
    {
        //     for bid in bids:
        for (orderBookEntry& bid : bids)
        {
            //         if bid.price >= ask.price # we have a match
            if (bid.price >= ask.price)
            {
                //             sale = new order()
                //             sale.price = ask.price
                orderBookEntry sale{ ask.price, 0, timestamp,
                    product,
                    orderBookType::asksale };

                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = orderBookType::bidsale;
                }
                if (ask.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = orderBookType::asksale;
                }

                //             # now work out how much was sold and 
                //             # create new bids and asks covering 
                //             # anything that was not sold
                //             if bid.amount == ask.amount: # bid completely clears ask
                if (bid.amount == ask.amount)
                {
                    //                 sale.amount = ask.amount
                    sale.amount = ask.amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
                    //                 # can do no more with this ask
                    //                 # go onto the next ask
                    //                 break
                    break;
                }
                //           if bid.amount > ask.amount:  # ask is completely gone slice the bid
                if (bid.amount > ask.amount)
                {
                    //                 sale.amount = ask.amount
                    sale.amount = ask.amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 # we adjust the bid in place
                    //                 # so it can be used to process the next ask
                    //                 bid.amount = bid.amount - ask.amount
                    bid.amount = bid.amount - ask.amount;
                    //                 # ask is completely gone, so go to next ask                
                    //                 break
                    break;
                }


                //             if bid.amount < ask.amount # bid is completely gone, slice the ask
                if (bid.amount < ask.amount &&
                    bid.amount > 0)
                {
                    //                 sale.amount = bid.amount
                    sale.amount = bid.amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 # update the ask
                    //                 # and allow further bids to process the remaining amount
                    //                 ask.amount = ask.amount - bid.amount
                    ask.amount = ask.amount - bid.amount;
                    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
                    //                 # some ask remains so go to the next bid
                    //                 continue
                    continue;
                }
            }
        }
    }
    return sales;
}
