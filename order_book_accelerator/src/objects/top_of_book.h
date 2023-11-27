//
// Created by matrix on 2023/11/25.
//

#ifndef OBJECTS_TOP_OF_BOOK_H
#define OBJECTS_TOP_OF_BOOK_H

#include <sstream>

#include "ticker_data.h"

namespace objects {

class TopOfBook : public TickerData {

public:
    TopOfBook(Ticker ticker_, Timestamp timestamp_, Timestamp local_timestamp_, float bid_, float bid_size_, float ask_, float ask_size_)
            : TickerData(std::move(ticker_), timestamp_, local_timestamp_), bid(bid_), bid_size(bid_size_), ask(ask_), ask_size(ask_size_) {}

    inline std::string ToString() const{
        std::stringstream stream;
        stream << "TopOfBook((" << ticker.first << ", " << ticker.second << ")"
           << "[" << timestamp << "|" << local_timestamp << "] : "
           << bid << ":" << bid_size << " | "
           << ask << ":" << ask_size << ")";
        return stream.str();
    }

    inline float GetMidPrice() const{
        return (bid + ask) / 2.0f;
    }

private:
    float bid;
    float bid_size;
    float ask;
    float ask_size;

};

}  // namespace objects

#endif  // OBJECTS_TOP_OF_BOOK_H
