#include <vector>
#include "movings.h"
#include "../core/core.h"
#include <cassert>

double
create_ema(std::vector<double>& seq,
           double alfa) {
    auto sub_alfa = 1 - alfa;

    // initialize main body seq_ls
    double last_ma = 0;
    last_ma = seq[0];
    for (int i = 1; i < seq.size(); ++i) {
        last_ma = alfa * seq[i] + sub_alfa * last_ma;
    }

    return last_ma;
}

SarResult
create_sar_parabolic(const std::vector<double>& seq_high,
                     const std::vector<double>& seq_low,
                     double acc_start,
                     double acc_step,
                     double acc_max) {
    SarResult res;
    bool IS_BUY = false;
    double SAR = seq_high[0];
    double ALPHA = acc_start;

    double max_high = seq_high[0];
    double max_low = seq_low[0];
    double curr_high = 0; //
    double curr_low = 0;

    for (int i = 1; i < seq_high.size(); i++) {
        curr_high = seq_high[i];
        curr_low = seq_low[i];

        if (IS_BUY) {
            if (SAR < curr_low) {
                if (curr_high > max_high) {
                    max_high = curr_high;
                    ALPHA += acc_step;
                    if (ALPHA > acc_max) ALPHA = acc_max;
                }
                SAR += ALPHA * (max_high - SAR);
            } else {
                IS_BUY = false;
                SAR = max_high;
                ALPHA = acc_start;
                max_low = curr_low;
            }
        } else {
            if (SAR > curr_high) {   //
                if (curr_low < max_low) {
                    max_low = curr_low;
                    ALPHA += acc_step;
                    if (ALPHA > acc_max) ALPHA = acc_max;
                }
                SAR += ALPHA * (max_low - SAR);
            } else {
                IS_BUY = true;
                SAR = max_low;
                ALPHA = acc_start;
                max_high = curr_high;
            }
        }
    }

    return res;
}