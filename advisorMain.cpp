#include <iostream>
#include <vector>
#include <iomanip>
#include "advisorMain.h"
#include "orderBookEntry.h"
#include "dHandle.h"

advisorMain::advisorMain(){}

void advisorMain::init()
{
    std::string input;
    currentTime = orderBookCurr.getEarliestTime();

    walletCurr.insertCurrency("BTC", 10);

    printStart();

    while (true)
    {
        input = getUserInput();
        processUserInput(input);
    }
}
void advisorMain::printStart()
{
    std::cout << "     ------------------------------------------" << std::endl;
    std::cout << "     | Advisor bot v1.0 by Stevanus Timothyus |" << std::endl;
    std::cout << "     ------------------------------------------\n" << std::endl;
    std::cout << ("Advisorbot is a command line program that can carry out \n"
                 "various tasks to help a cryptocurrency investor like \n"
                 "yourself analyse the data available on an exchange\n") << std::endl;
    std::cout << "-----------------------------------------------------\n" << std::endl;
}

std::string advisorMain::getUserInput()
{
    std::string line;
    std::cout << "Please enter a command, or help for a list of commands" << std::endl;
    std::cout << "Enter your command: ";
    std::getline(std::cin, line);
    std::cout << "-----------------------------------------------------" << std::endl;
    return line;
}


//First command is checked against a string and for which if it matches then the correct function is called. 
void advisorMain::processUserInput(std::string userInputLine)
{
    std::vector<std::string> userOption = dHandle::tokenise(userInputLine, ' ');
    if (dHandle::cmdParse(userOption, cmdAcceptDef))
    {
        //if size is 1, it has to be help only
        if (userOption[0] == "help" && userOption.size() == 1)
        {
            printHelp();
        }
        //condition for size checking is required so there wont be 2 function called at the same time.
        if (userOption[0] == "help" && userOption.size() == 2)
        {
            printHelpCMD(userOption[1]);
        }
        if (userOption[0] == "prod")
        {
            prod();
        }
        if (userOption[0] == "min")
        {
            min(userOption);
        }
        if (userOption[0] == "max")
        {
            max(userOption);
        }
        if (userOption[0] == "avg")
        {
            avg(userOption);
        }
        if (userOption[0] == "predict")
        {
            predict(userOption);
        }
        if (userOption[0] == "time")
        {
            time();
        }
        if (userOption[0] == "step")
        {
            step();
        }
        if (userOption[0] == "resistance")
        {
            resistance(userOption);
        }
        if (userOption[0] == "debug")
        {
            debug();
        }
    }
    std::cout << "-----------------------------------------------------\n" << std::endl;
}

void advisorMain::printHelp()
{
    std::cout << "The available commands are :" << std::endl;
    std::cout << "help       - list all available commands" << std::endl;
    std::cout << "help <cmd> - output help for the specified command (does not apply for help commands)" << std::endl;
    for (std::vector<std::string> const& p : cmdList)
    {
        std::cout << p[1] << std::endl;
    }
}

void advisorMain::printHelpCMD(std::string option)
{
    for (std::vector<std::string> const& p : cmdList)
    {
        if (option == p[0])
        {
            std::cout << p[1] << std::endl;
            std::cout << "to use, enter command in the following order:" << std::endl;
            std::cout << p[2] << std::endl;
            std::cout << "\nexample of use : " << p[3] << std::endl;
            std::cout << "        result : " << p[4] << std::endl;
            break;
        }
    };

}

std::vector<std::string> advisorMain::prod()
{
    std::vector<std::string> result = orderBookCurr.getKnownProducts();
    std::cout << dHandle::detokenise(result,",") << std::endl;
    return result;
}

void advisorMain::min(std::vector<std::string> userOption)
{
    std::cout << std::setprecision(9);
    std::vector<orderBookEntry> entries = orderBookCurr.getOrders(
        orderBookEntry::stringToorderBookType(userOption[2]), userOption[1], currentTime);
    std::cout << "The min " << userOption[2] << " for " << userOption[1] << " is "
              << orderBook::getLowPrice(entries) << std::fixed  << std::endl;
}

void advisorMain::max(std::vector<std::string> userOption)
{
    std::cout << std::setprecision(9);
    std::vector<orderBookEntry> entries = orderBookCurr.getOrders(
        orderBookEntry::stringToorderBookType(userOption[2]), userOption[1], currentTime);
    std::cout << "The max " << userOption[2] << " for " << userOption[1] << " is "
              << orderBook::getHighPrice(entries) << std::fixed << std::endl;
}

//void advisorMain::avg(std::vector<std::string> userOption)
//{
//    std::cout << std::setprecision(9);
//    std::vector<orderBookEntry> entries = orderBookCurr.getOrders(
//        orderBookEntry::stringToorderBookType(userOption[2]), userOption[1], currentTime);
//
//    std::string iteratedTime = currentTime;
//    int steps = std::stoi(userOption[3]);
//    int stepsc = 1;
//    for (unsigned int i = 2; i <= steps; i++)
//    {
//        iteratedTime = orderBookCurr.getPrevTime(iteratedTime);
//        if (iteratedTime == "nil")
//        {
//            stepsc = i-1;
//            break;
//        }
//        stepsc = i;
//        std::vector<orderBookEntry> entries2 = orderBookCurr.getOrders(
//            orderBookEntry::stringToorderBookType(userOption[2]), userOption[1], iteratedTime);
//        entries.insert(entries.end(), entries2.begin(), entries2.end());
//    }
//    if (stepsc != steps) 
//    {
//        std::cout << "There wasn't enough data to show " << steps << " timesteps. However..." << std::endl;
//    }
//    std::cout << "The average " << userOption[1] << " " << userOption[2] << " price over the last "
//                << stepsc << " timesteps was " << orderBook::getAvgPrice(entries) << std::endl;
//}

void advisorMain::avg(std::vector<std::string> userOption)
{
    std::cout << std::setprecision(9);
    std::vector<orderBookEntry> entries = orderBookCurr.getTotalOrders(orderBookEntry::stringToorderBookType(userOption[2]),
        userOption[1],
        currentTime,
        userOption[3]);
    std::cout << "the average " << userOption[1] << " " << userOption[2] << " price is "
              << orderBook::getAvgPrice(entries) << std::endl;
}

void advisorMain::predict(std::vector<std::string> userOption)
{
    std::cout << std::setprecision(9);
    std::vector<std::vector<orderBookEntry>> entries = {orderBookCurr.getOrders(
        orderBookEntry::stringToorderBookType(userOption[3]), userOption[2], currentTime) };

    std::string iteratedTime = currentTime;
    int steps = 10;
    for (unsigned int i = 1; i < 10; i++)
    {
        iteratedTime = orderBookCurr.getPrevTime(iteratedTime);
        if (iteratedTime == "nil")
        {
            steps = i;
            break;
        }
        entries.push_back(orderBookCurr.getOrders(orderBookEntry::stringToorderBookType(userOption[3]), userOption[2], iteratedTime));
    }
    double result = 0;
    if (steps != 10)
    {
        std::cout << "There wasn't enough data to give a decent prediction. However..." << std::endl;
    }
    if (userOption[1] == "min")
    {
        for (unsigned int i = 0; i < entries.size(); i++)
        {
            result += orderBook::getLowPrice(entries[i]);
        }
        std::cout << "Using the last " << steps << " timesteps, the predicted " << userOption[1] << " " << userOption[2] << " " << userOption[3] << " price will be " << result / entries.size() << std::endl;
    }
    if (userOption[1] == "max")
    {
        for (unsigned int i = 0; i < entries.size(); i++)
        {
            result += orderBook::getHighPrice(entries[i]);
        }
        std::cout << "Using the last " << steps << " timesteps, the predicted " << userOption[1] << " " << userOption[2] << " " << userOption[3] << " price will be " << result / entries.size() << std::endl;
    }
}


void advisorMain::time()
{
    prodCurr = orderBookCurr.getKnownProducts();
    std::cout << currentTime.substr(0,19) << std::endl;
}

void advisorMain::step()
{
    //From template
    //for (std::string p : orderBookCurr.getKnownProducts())
    //{
    //    std::cout << "matching " << p << std::endl;
    //    std::vector<orderBookEntry> sales = orderBookCurr.matchAsksToBids(p, currentTime);
    //    std::cout << "Sales: " << sales.size() << std::endl;
    //    for (orderBookEntry& sale : sales)
    //    {
    //        std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
    //        if (sale.username == "simuser")
    //        {
    //            // update the wallet
    //            walletCurr.processSale(sale);
    //        }
    //    }
    //}

    currentTime = orderBookCurr.getNextTime(currentTime);
    std::cout << "now at ";
    time();
}



void advisorMain::resistance(std::vector<std::string> userOption)
{
    std::cout << std::setprecision(5) << std::fixed;
    std::vector<orderBookEntry> entries = orderBookCurr.getTotalOrders(orderBookEntry::stringToorderBookType(userOption[2]),
        userOption[1],
        currentTime,
        userOption[3]);
    std::cout << std::setprecision(2);
    double result = orderBook::getClosePrice(entries, std::stod(userOption[4]), std::stod(userOption[5]));
    std::cout << "the resistance of " << userOption[1] << " at " << userOption[2] << " price of " << userOption[4] << std::endl;
    std::cout << "with " << userOption[5] << " price fluctation range is " << result*100 << "%"  << std::endl;
}

void advisorMain::debug()
{
    std::vector<std::string> cmdAllParse = { "","hi","hi 2","hi hi 3", "hi hi hi 4", "hi hi hi hi 5", "1", "?", "Help", "HELP",
        "help","help me","help prod","help min","help max","help avg","help predict","help time","help step","help resistance", "help me 3",
        "prod","prod 2",
        "min ETH/BTC ask","min ETH/BTC bid","min DOGE/BTC ask", "min ETH/BDC ask", "min EtH/BTC bid", "min EtH/BTC ask e", "min EtH/BTC bid 1",
        "max ETH/BTC ask","max ETH/BTC bid","max DOGE/BTC ask", "max ETH/BDC ask", "max EtH/BTC bid", "max EtH/BTC ask e", "max EtH/BTC bid 1",
        "avg ETH/BTC ask 1", "avg ETH/BTC ask 2", "avg ETH/BTC ask 10", "avg ETH/BTC bid 1", "avg ETH/BTC bid 2", "avg ETH/BTC bid 10",
        "predict max ETH/BTC bid", "predict min ETH/BTC bid", "predict max ETH/BTC ask", "predict min ETH/BTC ask",
        "resistance ETH/BTC ask 1 0.02495 0.00001", "resistance ETH/BTC ask 2 0.02495 0.00001", "resistance ETH/BTC ask j 0..2 3dd"
        "time", "time 1", "time 1 2"
        "step", "step 1", "step 1 2", "step",
        "avg ETH/BTC ask 1", "avg ETH/BTC ask 2", "avg ETH/BTC ask 10", "avg ETH/BTC bid 1", "avg ETH/BTC bid 2", "avg ETH/BTC bid 10",
        "resistance ETH/BTC ask 2 0.02495 0.00001","resistance ETH/BTC ask 3 0.02495 0.00001",
        "step", "step", "step", "step", "step", "step", "step",
        "predict max ETH/BTC bid","step",
        "predict max ETH/BTC bid", "predict min ETH/BTC bid", "predict max ETH/BTC ask", "predict min ETH/BTC ask",
        "resistance ETH/BTC ask 10 0.02495 0.00001","resistance ETH/BTC ask 2 0.02495 0.00001","resistance ETH/BTC ask 1 0.02495 0.00001"
    };
    for (std::string const& p : cmdAllParse)
    {
        std::cout << "Command Checked: " << p << std::endl;
        processUserInput(p);
    }

    //std::vector<std::string> cmdAllValues = { "" };
}

//From template
void advisorMain::printMarketStats()
{
    for (std::string const& p : orderBookCurr.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<orderBookEntry> entries = orderBookCurr.getOrders(orderBookType::ask,
            p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << orderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << orderBook::getLowPrice(entries) << std::endl;



    }
    // std::cout << "orderBook contains :  " << orders.size() << " entries" << std::endl;
    // unsigned int bids = 0;
    // unsigned int asks = 0;
    // for (orderBookEntry& e : orders)
    // {
    //     if (e.orderType == orderBookType::ask)
    //     {
    //         asks ++;
    //     }
    //     if (e.orderType == orderBookType::bid)
    //     {
    //         bids ++;
    //     }  
    // }    
    // std::cout << "orderBook asks:  " << asks << " bids:" << bids << std::endl;

}

void advisorMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = dHandle::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "advisorMain::enterAsk Bad input! " << input << std::endl;
    }
    else {
        try {
            orderBookEntry obe = dHandle::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                orderBookType::ask
            );
            obe.username = "simuser";
            if (walletCurr.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBookCurr.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << " advisorMain::enterAsk Bad input " << std::endl;
        }
    }
}

void advisorMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = dHandle::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "advisorMain::enterBid Bad input! " << input << std::endl;
    }
    else {
        try {
            orderBookEntry obe = dHandle::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                orderBookType::bid
            );
            obe.username = "simuser";

            if (walletCurr.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBookCurr.insertOrder(obe);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << " advisorMain::enterBid Bad input " << std::endl;
        }
    }
}

void advisorMain::printWallet()
{
    std::cout << walletCurr.toString() << std::endl;
}