#include "engine.h"
#include "../core/core.h"
#include "../core/core_str.h"
#include <cassert>

const std::string obj = "Engine";

double
calc_percent(double big, double small) {
    // ((V2-V1)/V1) × 100. Где V1 – старое значение, а V2 – новое
    return ((big - small) / small) * 100.0;
}

double
from_percent(double prm, double percent) {
    return (prm / 100.0) * percent;
}

void
Engine::OpenPosition(double price, int x) {
    deposit = 0;
    result_balance = 0;
    open_price = price;
    if (stop_loss_percent > 0) {
        stop_amount = from_percent(price, stop_loss_percent);
        set_stop(price);
    }

    // new trade
    if (is_long) {
        // buy
        deposit = buy(price, base_balance);
    } else {
        // sell
        deposit = sell(price, base_balance);
    }
//    LOG(obj, "open position, deposit: " + std::to_string(deposit));

    // trying
    try_exit_amount = deposit/try_exit_count;

    // open
    open = true;
    open_x = x;
}

void
Engine::NextPrice(double price, int x) {
    if (is_long) {
        // long
        if (price <= stop_price) {
            ClosePosition(price, x);
        }
        if (price > (stop_price + stop_amount)) {
            set_stop(price);
        }
    } else {
        // short
        if (price >= stop_price) {
            ClosePosition(price, x);
        }
        if (price < (stop_price - stop_amount)) {
            set_stop(price);
        }
    }
//    if (is_long) {
//        // long
//        if (price > open_price) {
//            if (stop_price != 0) {
//                if (price <= stop_price) {
//                    close(price, amount);
//                    stop_price = 0;
//                } else {
//                    auto sub_percent_price_stop = calc_percent(price, stop_price);
//                    if (sub_percent_price_stop > stop_loss_percent) {
//                        stop_price = price - ((price / 100.0) * stop_loss_percent);
//                    }
//                }
//
//            } else {
//                auto sub_percent_price_stop = calc_percent(price, open_price);
//                if (sub_percent_price_stop > stop_loss_percent) {
//                    stop_price = price - ((price / 100.0) * stop_loss_percent);
//                }
//
//            }
//
//        } else {
//
//        }
//
//    } else {
//        // short
//        if (price < open_price) {
//            if (stop_price != 0) {
//                if (price >= stop_price) {
//                    double amount = deposit / 2.0;
//                    close(price, amount);
//                    stop_price = 0;
//                } else {
//                    auto sub_percent_price_stop = calc_percent(stop_price, price);
//                    if (sub_percent_price_stop > stop_loss_percent) {
//                        stop_price = price + ((price / 100.0) * stop_loss_percent);
//                    }
//                }
//
//            } else {
//                auto sub_percent_price_stop = calc_percent(open_price, price);
//                if (sub_percent_price_stop > stop_loss_percent) {
//                    stop_price = price + ((price / 100.0) * stop_loss_percent);
//                }
//
//            }
//
//        } else {
//
//        }
//
//    }

}

void
Engine::ClosePosition(double price, int x) {
    if (try_exit_count > 1) {
        // close
        close(price, try_exit_amount);
        // sub try
        try_exit_count--;
        // move stop
        set_stop(price);
    } else {
        // close
        close(price, deposit);

        // profit
        auto prof = result_balance-base_balance;
//                LOG("analyzer", "calc profit => balance: " + std::to_string(pos.GetBalance()) + ",  trader balance: " + std::to_string(trader_balance));
        report.profit += prof;
        // count
        if (prof > 0) {
            report.trade_profit_count++;
            report.my_trades.emplace_back(prof, open_x, x);
        }
        report.trade_count++;

        open = false;
        open_price = 0;
    }
}

double
Engine::GetBalance() {
    return result_balance;
}

double
Engine::buy(double price, double amount) {
    auto res = (amount / price) * 0.998;
//    LOG(obj, "buy:: price: " + std::to_string(price) + ", amount: " + std::to_string(amount) + ", result: " + std::to_string(res));
    return res;
}

double
Engine::sell(double price, double amount) {
    auto res = (amount * price) * 0.998;
    return res;
}

void
Engine::close(double price, double amount) {
    assert(amount <= deposit);

    deposit -= amount;

    if (is_long) {
        // sell
        result_balance += sell(price, amount);
    } else {
        // buy
        result_balance += buy(price, amount);
    }


}

void Engine::set_stop(double price) {
    if (is_long) {
        stop_price = price - stop_amount;
    } else {
        stop_price = price + stop_amount;
    }
}

std::string Report::String() {
    auto trades = std::vector<std::string>();
    for (auto& trade : my_trades) {
        float duration = ((trade.stop_idx - trade.start_idx)*step) / 86400.0f;
        trades.push_back("{\"profit\":" + std::to_string(trade.profit) +
                         ", \"start_idx\":" + std::to_string(trade.start_idx) +
                         ", \"stop_idx\":" + std::to_string(trade.stop_idx) +
                         ", \"days_duration\":" + std::to_string(duration) + "}");
    }
    auto my_trades_str = STR_JOIN(trades, ",");

    auto report = "{\"profit\":" + std::to_string(profit) +
                  ", \"count\":" + std::to_string(trade_count) +
                  ", \"step\":" + std::to_string(step) +
                  ", \"my_trades\":[" + my_trades_str + "]" +
                  ", \"profit_count\":" + std::to_string(trade_profit_count) + "}";
//    INFO(report);
    return report;
}
