//
// Created by matrix on 2023/11/25.
//

#ifndef OBJECTS_ORDER_BOOK_SNAPSHOT_H
#define OBJECTS_ORDER_BOOK_SNAPSHOT_H

#include <utility>

#include "ticker_data.h"
#include "top_of_book.h"
#include "side.h"

namespace objects {

class OrderBookSnapshot : public TickerData {
public:
    Books bids;
    Books asks;

    OrderBookSnapshot(
            Ticker ticker_,
            Timestamp timestamp_,
            Timestamp local_timestamp_,
            Books bids_,
            Books asks_
    ) : TickerData(std::move(ticker_), timestamp_, local_timestamp_),
        bids(std::move(bids_)),
        asks(std::move(asks_)) {}

    OrderBookSnapshot(const OrderBookSnapshot &d) :
            TickerData{d.ticker, d.timestamp, d.local_timestamp},
            bids{d.bids},
            asks{d.asks} {}

    inline std::string ToString() const {
        // TODO(Leo): not fully formatted.
        std::stringstream stream;
        stream << "OrderBookSnapshot((" << ticker.first << ", " << ticker.second << ")"
               << "[" << timestamp << "|" << local_timestamp << "] : "
               << "(" << bids.rbegin()->first << ", " << bids.rbegin()->second << ") " << bids.size() << " | "
               << "(" << asks.begin()->first << ", " << asks.rbegin()->second << ") " << asks.size() << ")";
        return stream.str();
    }

    inline float MidPrice() const {
        return (bids.rbegin()->first + asks.begin()->first) / 2.0f;
    }

    inline Book TopBid() const {
        return *bids.rbegin();
    }

    inline float TopBidPrice() const {
        return bids.rbegin()->first;
    }

    inline Book TopAsk() const {
        return *asks.begin();
    }

    inline float TopAskPrice() const {
        return asks.begin()->first;
    }

    inline float Touch(Side side) const {
        return (side == Side::Sell) ? TopAskPrice() : TopBidPrice();
    }

    inline float FarTouch(Side side) const {
        return Touch((side == Side::Buy) ? Side::Sell : Side::Buy);
    }

    inline TopOfBook top_of_book() {
        auto bid = TopBid();
        auto ask = TopAsk();
        return {ticker, timestamp, local_timestamp, bid.first, bid.second, ask.first, ask.second};
    }
};

}  // namespace objects

#endif // OBJECTS_ORDER_BOOK_SNAPSHOT_H
