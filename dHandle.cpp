#include <iostream>
#include <fstream>
#include <iomanip>
#include "dHandle.h"
#include "advisorMain.h"
#include <cstdlib>

dHandle::dHandle()
{

}


/**read a csv file and extract it to a vector containing each data lines(orderBookEntry class) with its unique values*/
std::vector<orderBookEntry> dHandle::readCSV(std::string csvFilename)
{
    std::vector<orderBookEntry> entries;

    std::ifstream csvFile{ csvFilename };
    std::string line;
    if (csvFile.is_open())
    {
        while (std::getline(csvFile, line))
        {
            try {
                orderBookEntry obe = stringsToOBE(tokenise(line, ','));
                entries.push_back(obe);
            }
            catch (const std::exception& e)
            {
                std::cout << "dHandle::readCSV bad data" << std::endl;
            }
        }// end of while
    }

    std::cout << "dHandle::readCSV read " << entries.size() << " entries" << std::endl;
    return entries;
}

/**Takes a string and separates it by a separator*/
std::vector<std::string> dHandle::tokenise(std::string dLine, char separator)
{
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;
    start = dLine.find_first_not_of(separator, 0);
    do {
        end = dLine.find_first_of(separator, start);
        if (start == dLine.length() || start == end) break;
        if (end >= 0) token = dLine.substr(start, end - start);
        else token = dLine.substr(start, dLine.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end > 0);

    return tokens;
}


/**Takes in cmdTokenLine, the vector array of tokenised command line.
Takes in cmdAccept, a double nested array that is required for checking the commands and their respective arguments
returns true if the command is valid,else returns false*/
bool dHandle::cmdParse(std::vector<std::string> cmdTokenLine, std::vector<std::vector<std::vector<std::string>>> cmdAccept)
{
    //Checks for existence of command.
    if (cmdTokenLine.empty())
    {
        std::cout << "Please enter something." << std::endl;
        return false;
    }

    //Loop which checks for whether the “first” command argument is known, ie.help, min, predict are example of known commands.
    int c1 = -1;
    for (unsigned int i = 0; i < cmdAccept.size(); i++)
    {
        if (cmdTokenLine[0] == cmdAccept[i][0][0])
        {
            c1 = i;
            if (cmdTokenLine.size() == cmdAccept[i].size() && cmdTokenLine.size() == 1)
            {
                return true;
            }
        }
    }
    if (c1 == -1)
    {
        std::cout << "Command is not found. Please check help for all available commands." << std::endl;
        std::cout << "use 'help <cmd>' for more info on proper command usage" << std::endl;
        return false;
    }
    else 
    {
        //Checks if the first’s command required number of arguments is valid, ie min takes in 2 arguments
        if (cmdTokenLine.size() != cmdAccept[c1].size())
        {
            std::cout << "The command " << cmdAccept[c1][0][0] <<" takes in " << cmdAccept[c1].size()-1 << " arguments." << std::endl;
        }
        else
        {
            //Iteratively checks for each arguments if its valid with respect to the first function. Ie. min can only have products for argument no.1 
            int c2 = 0;
            for (unsigned int j = 1; j < cmdAccept[c1].size(); j++) 
            {
                int c3 = 0;
                for (const std::string& p : cmdAccept[c1][j])
                {
                    if (p == cmdTokenLine[j])
                    {
                        c2++;
                        c3++;
                        break;
                    }
                    //Cater for integer
                    if (p == "int") 
                    {
                        if (cmdTokenLine[j].find_first_not_of("0123456789") == std::string::npos)
                        {
                            c2++;
                            c3++;
                            break;
                        }
                        else
                        {
                            std::cout << "Argument no." << j << " isnt a valid integer." << std::endl;
                            return false;
                        }
                    }
                    //Cater for values with decimals
                    if (p == "double")
                    {
                        
                        if (cmdTokenLine[j].find_first_not_of("0123456789.") == std::string::npos && is_number(cmdTokenLine[j]))
                        {
                            c2++;
                            c3++;
                            break;
                        }
                        else
                        {
                            std::cout << "Argument no." << j << " isnt a valid price." << std::endl;
                            return false;
                        }
                    }
                }
                if (c3==0)
                {
                    std::cout << "Argument no." << j << " for the command " << cmdAccept[c1][0][0] << " takes in only: " << detokenise(cmdAccept[c1][j],",") << std::endl;
                    return false;
                }
                if (c2 == (cmdTokenLine.size() - 1))
                {
                    return true;
                }
            }
        }
    }
}

/**Reverses what tokenise function does for display purposes*/
std::string dHandle::detokenise(std::vector<std::string> dVector, std::string separator)
{
    std::string detoken;
    for (const std::string& p : dVector)
    {
        detoken.append(p);
        detoken.append(separator);
    }
    return detoken.substr(0, detoken.length() - 1);
}


orderBookEntry dHandle::stringsToOBE(std::vector<std::string> tokens)
{
    double price, amount;

    if (tokens.size() != 5) // bad
    {
        std::cout << "Bad line " << std::endl;
        throw std::exception{};
    }
    // we have 5 tokens
    try {
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    }
    catch (const std::exception& e) {
        std::cout << "dHandle::stringsToOBE Bad float! " << tokens[3] << std::endl;
        std::cout << "dHandle::stringsToOBE Bad float! " << tokens[4] << std::endl;
        throw;
    }

    orderBookEntry obe{ price,
                        amount,
                        tokens[0],
                        tokens[1],
                        orderBookEntry::stringToorderBookType(tokens[2]) };

    return obe;
}


orderBookEntry dHandle::stringsToOBE(std::string priceString,
    std::string amountString,
    std::string timestamp,
    std::string product,
    orderBookType orderType)
{
    double price, amount;
    try {
        price = std::stod(priceString);
        amount = std::stod(amountString);
    }
    catch (const std::exception& e) {
        std::cout << "dHandle::stringsToOBE Bad float! " << priceString << std::endl;
        std::cout << "dHandle::stringsToOBE Bad float! " << amountString << std::endl;
        throw;
    }
    orderBookEntry obe{ price,
                    amount,
                    timestamp,
                    product,
                    orderType };

    return obe;
}


bool dHandle::is_number(const std::string& s)
{
    char* end = nullptr;
    double val = strtod(s.c_str(), &end);
    return end != s.c_str() && *end == '\0' && val != HUGE_VAL;
}