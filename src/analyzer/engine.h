#ifndef C_ALLA_EXPLORER_ENGINE_H
#define C_ALLA_EXPLORER_ENGINE_H

#include <string>
#include <vector>

class MyTrade {
public:
    MyTrade() {}

    MyTrade(double pr, int start, int stop) : profit(pr), start_idx(start), stop_idx(stop) {}

    double profit;
    int start_idx;
    int stop_idx;
};


struct Report {
    double profit;
    std::vector<MyTrade> my_trades;
    int trade_count;
    int trade_profit_count;
    int step;

    std::string String();
};


class Engine {
public:
    Engine() {}

    Engine(bool islong, double bls, int sloss, int step, int try_exit) : is_long(islong),
                                                                         base_balance(bls),
                                                                         stop_loss_percent(sloss),
                                                                         open(false),
                                                                         deposit(0),
                                                                         result_balance(0),
                                                                         open_price(0),
                                                                         stop_price(0),
                                                                         stop_amount(0),
                                                                         open_x(0),
                                                                         try_exit_count(try_exit),
                                                                         try_exit_amount(0) {
        // report
        report = Report{};
        report.trade_profit_count = 0;
        report.profit = 0;
        report.trade_count = 0;
        report.my_trades = std::vector<MyTrade>();
        report.step = step;
    }

    bool IsOpen() { return open; }

    void OpenPosition(double price, int t);

    void NextPrice(double price, int t);

    void ClosePosition(double price, int t);

    double GetBalance();

    Report GetReport() { return report; }

private:
    bool is_long;
    Report report;
    double base_balance;
    double stop_loss_percent;
    double result_balance;

    // position
    bool open;
    int open_x;
    double deposit;
    double open_price;
    double stop_price;
    double stop_amount;
    int try_exit_count;
    double try_exit_amount;

    // buy
    double buy(double price, double amount);

    // sell
    double sell(double price, double amount);

    // partition close
    void close(double price, double amount);

    // stop
    void set_stop(double price);

};


#endif //C_ALLA_EXPLORER_ENGINE_H
