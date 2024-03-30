#pragma once

#include <string>

enum class orderBookType { bid, ask, unknown, asksale, bidsale };

class orderBookEntry
{
    public:


        orderBookEntry( double _price,
                        double _amount,
                        std::string _timestamp,
                        std::string _product,
                        orderBookType _orderType,
                        std::string username = "dataset");

        static orderBookType stringToorderBookType(std::string s);

        static bool compareByTimestamp(orderBookEntry& e1, orderBookEntry& e2)
        {
            return e1.timestamp < e2.timestamp;
        }
        static bool compareByPriceAsc(orderBookEntry& e1, orderBookEntry& e2)
        {
            return e1.price < e2.price;
        }
        static bool compareByPriceDesc(orderBookEntry& e1, orderBookEntry& e2)
        {
            return e1.price > e2.price;
        }

        double price;
        double amount;
        std::string timestamp;
        std::string product;
        orderBookType orderType;
        std::string username;
};


