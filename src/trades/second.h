#pragma once


#include <vector>
#include "../../src/core/core.h"


class second {
public:
    second() : LastPrice(0.0), MaxPrice(0.0), MinPrice(0.0), Amount(0.0), Volume(0.0), Count(0.0) {}

    bool IsEmpty() { return Count == 0; }

    bool IsDummy() { return Amount == 0 && LastPrice != 0; }

    void InsertDummy(double p);

    void Add(double price, double amount);

    // fields
    double LastPrice;
    double MaxPrice;
    double MinPrice;
    double Amount;
    double Volume;
    int Count;

};

