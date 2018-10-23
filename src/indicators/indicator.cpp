#include "indicator.h"
#include "movings.h"


Indicator::Indicator(json& conf) {
    name = conf["type"];
}

IndiEMASimpleCrossWithPrice::IndiEMASimpleCrossWithPrice(json& conf, std::vector<candle>& cls) : Indicator(conf) {
}

Signal
IndiEMASimpleCrossWithPrice::GetSignal(int end) const {
    auto sig = Signal();
    sig.body = signals[end];
    return sig;
}

IndiSarParabolic::IndiSarParabolic(json& conf, std::vector<candle>& cls) : Indicator(conf) {

}

Signal
IndiSarParabolic::GetSignal(int end) const {
    auto sig = Signal();
    sig.body = signals[end];
    return sig;
}

