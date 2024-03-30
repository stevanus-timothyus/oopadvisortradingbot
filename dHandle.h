#pragma once

#include <string>
#include <vector>
#include "orderBookEntry.h"

class dHandle
{
public:
    dHandle();
        static std::vector<orderBookEntry> readCSV(std::string csvFile);
        static std::vector<std::string> tokenise(std::string dLine, char separator);
        static std::string detokenise(std::vector<std::string> dVector, std::string separator);
        static bool cmdParse(std::vector<std::string> cmdTokenLine, std::vector<std::vector<std::vector<std::string>>> cmdAccept);


        static orderBookEntry stringsToOBE( std::string price,
                                            std::string amount,
                                            std::string timestamp,
                                            std::string product,
                                            orderBookType orderBookType);
        static bool is_number(const std::string& s);

    private:
        static orderBookEntry stringsToOBE(std::vector<std::string> strings);
};

