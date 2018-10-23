#ifndef C_ALLA_EXPLORER_MOVINGS_H
#define C_ALLA_EXPLORER_MOVINGS_H

#include <vector>

double
create_ema(std::vector<double>& seq, double alfa);

struct SarResult {
    double body;
    bool buy;
};

SarResult
create_sar_parabolic(const std::vector<double>& seq_high, const std::vector<double>& seq_low, double acc_start,
                     double acc_step,
                     double acc_max);


#endif //C_ALLA_EXPLORER_MOVINGS_H
