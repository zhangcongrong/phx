//
// Created by matrix on 2023/11/25.
//

#ifndef OBJECTS_TICKER_DATA_H
#define OBJECTS_TICKER_DATA_H

#include <string>
#include <utility>

#include "../utils/typing.h"

using namespace utils;

namespace objects {

class TickerData {
public:
    TickerData(Ticker ticker_, Timestamp timestamp_, Timestamp local_timestamp_)
            : ticker(std::move(ticker_)), timestamp(timestamp_), local_timestamp(local_timestamp_) {}

    inline std::string GetSymbol() const {
        return ticker.second;
    }

    inline std::string GetExchange() const {
        return ticker.first;
    }

    inline Timestamp GetExchangeTs() const {
        return timestamp;
    }

    inline Timestamp GetLocalTs() const {
        return local_timestamp;
    }

protected:
    Ticker ticker;
    Timestamp timestamp;
    Timestamp local_timestamp;
};

}  // namespace objects


#endif //OBJECTS_TICKER_DATA_H
