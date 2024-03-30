#pragma once

#include <vector>
#include "orderBookEntry.h"
#include "orderBook.h"
#include "Wallet.h"

class advisorMain
{
	public:
		advisorMain();
		/**Call this to start the advisor bot*/
		void init();
	private:
		void printStart();
        std::string getUserInput();
        void processUserInput(std::string userInputLine);
        void printHelp();
        void printHelpCMD(std::string option);
        std::vector<std::string> prod();
        void min(std::vector<std::string> userOption);
        void max(std::vector<std::string> userOption);
        void avg(std::vector<std::string> userOption);
        void predict(std::vector<std::string> userOption);
        void time();
        void step();
        void resistance(std::vector<std::string> userOption);
        void debug(); //hidden from help command

        

        //From template
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();

        std::string currentTime;

        //orderBook orderBookCurr{"20200317.csv"};
        //orderBook orderBookCurr{"20200601.csv"};
   
        //Modified 20200601.csv contains till 2020/06/01 12:00:00
        orderBook orderBookCurr{"20200602.csv"}; 

        //Custom csv
        //orderBook orderBookCurr{ "20200603.csv" };

        wallet walletCurr;
        std::vector<std::string> prodCurr = orderBookCurr.getKnownProducts();
        std::vector<std::vector<std::vector<std::string>>> cmdAcceptDef =
        {
            {{"help"}},
            {{"help"},{"prod","min","max","avg","predict","time","step","resistance"}},
            {{"prod"}},
            {{"min"},prodCurr,{"bid","ask"}},
            {{"max"},prodCurr,{"bid","ask"}},
            {{"avg"},prodCurr,{"bid","ask"},{"int"}},
            {{"predict"},{"max","min"},prodCurr,{"bid","ask"}},
            {{"time"}},
            {{"step"}},
            {{"resistance"},prodCurr,{"bid","ask"},{"int"},{"double"},{"double"}},
            {{"debug"}}
        };

        //vector data structure to store known commands, can be easily manipulated in the future when added more functionality.
        //{"cmd","cmd info", "cmd use", "cmd eg use", "eg output"}
        std::vector<std::vector<std::string>> cmdList =
        {
            {"prod","prod       - list available products","prod","prod","ETH/BTC,DOGE/BTC","1"},
            {"min","min        - find minimum bid or ask for product in current time step"
                  ,"min product bid/ask","min ETH/BTC ask","The min ask for ETH/BTC is 1.0","3"},
            {"max","max        - find maximum bid or ask for product in current time step"
                  ,"max product bid/ask","max ETH/BTC ask","The max ask for ETH/BTC is 1.0","3"},
            {"avg","avg        - compute average ask or bid for the sent product over the sent number of time steps"
                  ,"avg product ask/bid timesteps","avg ETH/BTC ask 10"
                  ,"The average ETH/BTC ask price over the last 10 timesteps was 1.0","4"},
            {"predict","predict    - predict max or min ask or bid for the sent product for the next time step"
                      ,"predict max/min product ask/bid","predict max ETH/BTC bid"
                      ,"The average ETH/BTC ask price over the last 10 timesteps was 1.0","4"},
            {"time","time       - state current time in dataset, i.e. which timeframe are we looking at"
                   ,"time","time","2020/03/17 17:01:24","1"},
            {"step","step       - move to next time step"
                   ,"step","step","now at 2020/03/17 17:01:30","1"},
            {"resistance","resistance - resistance to change at price, the higher it is the less likely it will change"
                         ,"resistance product bid/ask timesteps price price_diff"
                         ,"resistance ETH/BTC ask 1 0.02495 0.00001"
                         ,"In the past 1 timesteps, the resistance of ETH/BTC at ask price of 0.02495 with 0.00001 price fluctation range is 6.00%"}
        };
};

