#include "second.h"

void second::Add(double price, double amount) {
    LastPrice = price;
    // amount
    if (amount < 0) {
        amount *= -1;
    }
    Amount += amount;
    //count
    Count++;
    // max price
    if (MaxPrice < price) {
        MaxPrice = price;
    }
    // min price
    if (MinPrice == 0) {
        MinPrice = price;
    } else {
        if (MinPrice > price) {
            MinPrice = price;
        }
    }
    // volume
    Volume += amount * price;
}

void second::InsertDummy(double price) {
    if (IsEmpty()) {
        LastPrice = price;
        MaxPrice = price;
        MinPrice = price;
        Amount = 0.0;
        Volume = 0.0;
        Count = 0.0;
    } else {
        ERROR("this second is not empty");
    }
}
