#include "signals.h"
#include "../analyzer/explorer_face.h"

template<typename T>
double
GetParameterValue(json& config, T& name) {
    double res;
    auto params = config["params"];
    for (auto& jparam : params) {
        if (jparam["name"] == name) {
            res = jparam["current"];
            return res;
        }
    }
    ERROR("a param with name: " + std::string(name) + " is not found in the config");
    return 0;
}

bool
over_percent(double small, double big, double percent) {
    double perc = ((big - small) / small) * 100.0;
    return perc > percent;
}

void
EMAPeriodCoeffCrossWithPrice(json& conf, std::vector<candle>& cls, std::vector<double>& signals) {
    assert(cls.size() != 0);

    double percent_up = GetParameterValue(conf, "percent_up");
    double percent_down = GetParameterValue(conf, "percent_down");
    double period = GetParameterValue(conf, "period");
    double coeff = 2.0 / (double(period) + 1.0);
//    double sub_alpha = 1.0 - alpha;

//    std::cout << "period: " << period << ", percent up: " << std::to_string(percent_up) << ", percent down: " << std::to_string(percent_down) <<
//              ", alpha: " << std::to_string(alpha) <<  std::endl;

    // create ema
    double last_ma = 0;
    double price = 0;


    for (int i = 0; i < cls.size(); ++i) {
        price = cls[i].LastPrice;

        last_ma = last_ma + (coeff * (price - last_ma));


        if (price > last_ma) {
            if (over_percent(last_ma, price, percent_up)) {
                signals[i] = 1;
            }
        } else {
            if (over_percent(price, last_ma, percent_down)) {
                signals[i] = -1;
            }
        }

    }


}

signal_block
EMA_Cross_Price_Strong(json& conf) {
    assert(cls.size() != 0);

    double percent_up = GetParameterValue(conf, "percent_up");
    double percent_down = GetParameterValue(conf, "percent_down");
    double period = GetParameterValue(conf, "period");
    double coeff = 2.0 / (double(period) + 1.0);

    // create ema
    double last_ma = 0;
    double price = 0;

    signal_block sb = GetTradeList()->GetEmptySignalBlock();

    for (int i = 0; i < sb.Len(); ++i) {
        price = (*sb.price_ls)[i];

        last_ma = last_ma + (coeff * (price - last_ma));


        if (price > last_ma) {
            if (over_percent(last_ma, price, percent_up)) {
                (*sb.signal_ls)[i] = 1;
            } else {
                (*sb.signal_ls)[i] = 0;
            }
        } else {
            if (over_percent(price, last_ma, percent_down)) {
                (*sb.signal_ls)[i] = -1;
            } else {
                (*sb.signal_ls)[i] = 0;
            }
        }

    }

    return sb;
}

void
EMAPeriodCoeffCrossWithoutExit(json& conf, std::vector<candle>& cls, std::vector<double>& signals) {
    assert(cls.size() != 0);

    double percent_up = GetParameterValue(conf, "percent_up");
    double period = GetParameterValue(conf, "period");
    double coeff = 2.0 / (double(period) + 1.0);
//    double sub_alpha = 1.0 - alpha;

//    std::cout << "period: " << period << ", percent up: " << std::to_string(percent_up) << ", percent down: " << std::to_string(percent_down) <<
//              ", alpha: " << std::to_string(alpha) <<  std::endl;

    // create ema
    double last_ma = 0;
    double price = 0;


    for (int i = 0; i < cls.size(); ++i) {
        price = cls[i].LastPrice;

        last_ma = last_ma + (coeff * (price - last_ma));


        if (price > last_ma) {
            if (over_percent(last_ma, price, percent_up)) {
                signals[i] = 1;
            }
        } else {
            if (over_percent(price, last_ma, percent_up)) {
                signals[i] = -1;
            }
        }

    }


}

void
EMAPeriodCoeffCrossHoldSignal(json& conf, std::vector<candle>& cls, std::vector<double>& signals) {
//    INFO("ema simple cross price constructor");
    assert(cls.size() != 0);

    double percent_up = GetParameterValue(conf, "percent_up");
    double percent_down = GetParameterValue(conf, "percent_down");
    double period = GetParameterValue(conf, "period");
    double coeff = 2.0 / (double(period) + 1.0);
//    double sub_alpha = 1.0 - alpha;

//    std::cout << "period: " << period << ", percent up: " << std::to_string(percent_up) << ", percent down: " << std::to_string(percent_down) <<
//              ", alpha: " << std::to_string(alpha) <<  std::endl;

    // create ema
    double last_ma = 0;
    double price = 0;
    double last_signal = -1;


    for (int i = 0; i < cls.size(); ++i) {
        price = cls[i].LastPrice;

        last_ma = last_ma + (coeff * (price - last_ma));


        if (price > last_ma) {
            if (over_percent(last_ma, price, percent_up)) {
                signals[i] = 1;
                last_signal = 1;
            } else {
                signals[i] = last_signal;
            }
        } else {
            if (over_percent(price, last_ma, percent_down)) {
                signals[i] = -1;
                last_signal = -1;
            } else {
                signals[i] = last_signal;
            }
        }

    }
//    INFO("ema simple cross price constructor end.");


}

void
EMAPeriodCoeffCrossWithOutExitNoEmpty(json& conf, std::vector<candle>& cls, std::vector<double>& signals) {
    assert(cls.size() != 0);

    double percent_up = GetParameterValue(conf, "percent_up");
    double period = GetParameterValue(conf, "period");
    double coeff = 2.0 / (double(period) + 1.0);
//    double sub_alpha = 1.0 - alpha;

//    std::cout << "period: " << period << ", percent up: " << std::to_string(percent_up) << ", percent down: " << std::to_string(percent_down) <<
//              ", alpha: " << std::to_string(alpha) <<  std::endl;

    // create ema
    double last_ma = 0;
    double price = 0;
    double last_price = 0;


    for (int i = 0; i < cls.size(); ++i) {
        price = cls[i].LastPrice;

        if (price != last_price) {
            last_ma = last_ma + (coeff * (price - last_ma));
            if (price > last_ma) {
                if (over_percent(last_ma, price, percent_up)) {
                    signals[i] = 1;
                }
            } else {
                if (over_percent(price, last_ma, percent_up)) {
                    signals[i] = -1;
                }
            }
            last_price = price;
        } else {
            signals[i] = 0;
        }

    }


}


double
get_sma(std::vector<double>& data, size_t period, size_t end) {
    size_t i = 0;
    if (end < period) {
        i = 0;
        period = end + 1;
    } else {
        i = end - period + 1;
    }

    double sum = 0;

    for (; i <= end; ++i) {
        sum += data[i];
    }

    return sum / period;
}

double
get_simple_ema(std::vector<candle>& cls, size_t period, size_t end, double alpha, double sub_alpha) {
    double last_ma = 0;

    size_t i = 0;
    if (end < period) {
        i = 0;
        period = end + 1;
    } else {
        i = end - period + 1;
    }

    double sum = 0;

    for (; i <= end; ++i) {
        last_ma = alpha * cls[i].LastPrice + sub_alpha * last_ma;
    }

    return last_ma;
}

void
MACD_Cross_SignalLine(json& conf, std::vector<candle>& cls, std::vector<double>& signals) {
    assert(cls.size() != 0);


    // MACD
    double period_a = GetParameterValue(conf, "period_a");
    double coeff_a = 2.0 / (double(period_a) + 1.0);

    double period_b = GetParameterValue(conf, "period_b");
    double coeff_b = 2.0 / (double(period_b) + 1.0);

    size_t signal_period = GetParameterValue(conf, "signal_period");
    std::vector<double> macd_ls = std::vector<double>(cls.size());



//    double sub_alpha = 1.0 - alpha;

//    std::cout << "period: " << period << ", percent up: " << std::to_string(percent_up) << ", percent down: " << std::to_string(percent_down) <<
//              ", alpha: " << std::to_string(alpha) <<  std::endl;

    // create ema
    double ema_a = 0;
    double ema_b = 0;
    double price = 0;
//    double macd_zero = 0;
    double macd = 0;
    double signal_line = 0;
    double histogram = 0;

    for (int i = 0; i < cls.size(); ++i) {
        price = cls[i].LastPrice;

        ema_a = ema_a + (coeff_a * (price - ema_a));
        ema_b = ema_b + (coeff_b * (price - ema_b));

//        macd_zero = ema_a;
        // macd
        macd = ema_a - ema_b;
        macd_ls[i] = macd;

        // signal line
        signal_line = get_sma(macd_ls, signal_period, i);


        if (signal_line > 0) {
            signals[i] = 1;
        } else {
            signals[i] = -1;
        }



    }
//    INFO("ema simple cross price constructor end.");


}

void
EMA_SimpleCross_WithPrice(json& conf, std::vector<candle>& cls, std::vector<double>& signals) {
//    INFO("ema simple cross price constructor");
    assert(cls.size() != 0);

    double percent_up = GetParameterValue(conf, "percent_up");
    double percent_down = GetParameterValue(conf, "percent_down");
    double alpha = GetParameterValue(conf, "alpha");
    alpha = 2 / (1 + alpha);
    double sub_alpha = 1.0 - alpha;
    int period = GetParameterValue(conf, "period");


//    std::cout << ", period: " << period << ", percent up: " << std::to_string(percent_up) << ", percent down: " << std::to_string(percent_down) <<
//              ", alpha: " << std::to_string(alpha) <<  std::endl;

    // create ema
    double last_ma = 0;
    int idx = 0;
    double price = 0;

    for (int i = period; i < cls.size(); ++i) {
        idx = i - period;
        last_ma = cls[idx].LastPrice;
        assert(last_ma != 0);

        for (int j = idx + 1; j < i; ++j) {
            last_ma = alpha * cls[idx].LastPrice + sub_alpha * last_ma;
//            last_ma = 0 + 1 * last_ma;
        }

        price = cls[i].LastPrice;

        if (price > last_ma) {
            if (over_percent(last_ma, price, percent_up)) {
                signals[i] = 1;
            }
        } else {
            if (over_percent(price, last_ma, percent_down)) {
                signals[i] = -1;
            }
        }

    }
//    INFO("ema simple cross price constructor end.");


}


void
MACD_Simple_Cross_SignalLine(json& conf, std::vector<candle>& cls, std::vector<double>& signals) {
//    INFO("ema simple cross price constructor");
    assert(cls.size() != 0);


    // MACD
    size_t period_a = GetParameterValue(conf, "period_a");
    double alpha_a = GetParameterValue(conf, "alpha_a");
    alpha_a = 2 / (1 + alpha_a);
    double sub_alpha_a = 1.0 - alpha_a;

    size_t period_b = GetParameterValue(conf, "period_b");
    double alpha_b = GetParameterValue(conf, "alpha_b");
    alpha_b = 2 / (1 + alpha_b);
    double sub_alpha_b = 1.0 - alpha_b;

    size_t signal_period = GetParameterValue(conf, "signal_period");
    std::vector<double> macd_ls = std::vector<double>(cls.size());



//    double sub_alpha = 1.0 - alpha;

//    std::cout << "period: " << period << ", percent up: " << std::to_string(percent_up) << ", percent down: " << std::to_string(percent_down) <<
//              ", alpha: " << std::to_string(alpha) <<  std::endl;

    // create ema
    double ema_a = 0;
    double ema_b = 0;
//    double macd_zero = 0;
    double macd = 0;
    double signal_line = 0;

    for (int i = 0; i < cls.size(); ++i) {
//        price = cls[i].LastPrice;

        ema_a = get_simple_ema(cls, period_a, i, alpha_a, sub_alpha_a);
        ema_b = get_simple_ema(cls, period_b, i, alpha_b, sub_alpha_b);

//        macd_zero = ema_a;
        // macd
        macd = ema_a - ema_b;
        macd_ls[i] = macd;

        // signal line
        signal_line = get_sma(macd_ls, signal_period, i);


        if (signal_line > 0) {
            signals[i] = 1;
        } else {
            signals[i] = -1;
        }


    }
}

void
CalcSignals(json& conf, std::vector<candle>& cls, std::vector<double>& signals) {
    std::string type = conf["type"];

    if (type == "ema_simple_cross_price") {
        EMA_SimpleCross_WithPrice(conf, cls, signals);
    } else if (type == "sar_parabolic") {
        IndiSarParabolic(conf, cls, signals);
    } else if (type == "ema_coeff_cross_price") {
        EMAPeriodCoeffCrossWithPrice(conf, cls, signals);
    } else if (type == "ema_coeff_cross_price_hold_signal") {
        EMAPeriodCoeffCrossHoldSignal(conf, cls, signals);
    } else if (type == "ema_coeff_cross_price_without_exit") {
        EMAPeriodCoeffCrossWithoutExit(conf, cls, signals);
    } else if (type == "ema_coeff_cross_price_without_exit_no_empty") {
        EMAPeriodCoeffCrossWithOutExitNoEmpty(conf, cls, signals);
    } else if (type == "macd_cross_signal_line") {
        MACD_Cross_SignalLine(conf, cls, signals);
    } else if (type == "macd_simple_cross_signal_line") {
        MACD_Simple_Cross_SignalLine(conf, cls, signals);
    }


}

signal_block
CalcSignalsStrong(json& conf) {
//    LOG("signals", "in method");
    std::string type = conf["type"];

    if (type == "ema_coeff_cross_price") {
        return EMA_Cross_Price_Strong(conf);
    }
}


