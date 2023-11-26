//
// Created by matrix on 2023/11/25.
//

#ifndef OBJECTS_ORDER_BOOK_UPDATE_H
#define OBJECTS_ORDER_BOOK_UPDATE_H

#include <utility>

#include "ticker_data.h"
#include "top_of_book.h"

namespace objects {

class OrderBookUpdate : public TickerData {
public:
    BookUpdates bids;
    BookUpdates asks;

    OrderBookUpdate(
            Ticker ticker_,
            Timestamp timestamp_,
            Timestamp local_timestamp_,
            BookUpdates bids_,
            BookUpdates asks_
    ) : TickerData(std::move(ticker_), timestamp_, local_timestamp_),
        bids(std::move(bids_)),
        asks(std::move(asks_)) {}

    OrderBookUpdate(const OrderBookUpdate &d) :
            TickerData{d.ticker, d.timestamp, d.local_timestamp},
            bids{d.bids},
            asks{d.asks} {}

    inline std::string ToString() const {
        // TODO(Leo): not fully formatted.
        std::stringstream stream;
        stream << "OrderBookUpdate((" << ticker.first << ", " << ticker.second << ")"
               << "[" << timestamp << "|" << local_timestamp << "] : "
               << "( bids len:" << bids.size() << ")  | "
               << "( asks len:" << asks.size() << ")";
        return stream.str();
    }
};

}  // namespace objects

#endif // OBJECTS_ORDER_BOOK_UPDATE_H
