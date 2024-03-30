#include "orderBookEntry.h"

orderBookEntry::orderBookEntry( double _price,
                                double _amount,
                                std::string _timestamp,
                                std::string _product,
                                orderBookType _orderType,
                                std::string _username)
  : price(_price),
    amount(_amount),
    timestamp(_timestamp),
    product(_product),
    orderType(_orderType),
    username(_username)
{


}

orderBookType orderBookEntry::stringToorderBookType(std::string s)
{
    if (s == "ask")
    {
        return orderBookType::ask;
    }
    if (s == "bid")
    {
        return orderBookType::bid;
    }
    return orderBookType::unknown;
}
