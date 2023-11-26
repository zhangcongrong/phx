//
// Created by matrix on 2023/11/25.
//

#ifndef ORDER_BOOK_ORDER_BOOK_H
#define ORDER_BOOK_ORDER_BOOK_H

#include <utility>

#include "./utils/typing.h"
#include "./objects/order_book_snapshot.h"
#include "./objects/order_book_update.h"

using namespace utils;
using namespace objects;


class OrderBook {
public:
    OrderBook(std::string exchange_, std::string symbol_,
              Books bids_,
              Books asks_)
            : exchange(std::move(exchange_)), symbol(std::move(symbol_)),
              bids(std::move(bids_)), asks(std::move(asks_)),
              exchange_ts(0), local_ts(0), initialized(false) {}

    explicit OrderBook(const OrderBookSnapshot &d);

    inline Ticker Key() const {
        return std::make_pair(exchange, symbol);
    }

    inline void SetTimestamp(Timestamp exchange_ts_, Timestamp local_ts_) {
        exchange_ts = exchange_ts_;
        local_ts = local_ts_;
    }

    inline void ApplySnapshot(const Books &bids_, const Books &asks_) {
        bids = bids_;
        asks = asks_;
        initialized = true;
    }

    inline bool IsValid() const {
        return initialized && !bids.empty() && !asks.empty();
    }

    inline float MidPrice() const {
        if (!bids.empty() && !asks.empty()) {
            return (bids.rbegin()->first + asks.begin()->first) / 2.0f;
        } else {
            return 0.0f;
        }
    }

    inline std::pair<float, float> TopOfBookPrice() const {
        auto bid = TopBidPrice();
        auto ask = TopAskPrice();
        return std::make_pair(bid, ask);
    }

    inline Book TopBid() const {
        if (!bids.empty()) {
            return *bids.rbegin();
        } else {
            return std::make_pair(0.0f, 0.0f);
        }
    }

    inline float TopBidPrice() const {
        return !bids.empty() ? bids.rbegin()->first : 0.0f;
    }

    inline Book TopAsk() const {
        if (!asks.empty()) {
            return *asks.begin();
        } else {
            return std::make_pair(0.0f, 0.0f);
        }
    }

    inline float TopAskPrice() const {
        return !asks.empty() ? bids.begin()->first : 0.0f;
    }

    void UpdateCumulativeLevels(size_t levels_ = 0) {
        std::tie(cum_bids, cum_asks) = CumulativeLevels(levels_);
    }

    std::pair<std::vector<float>, std::vector<float>> CumulativeLevels(size_t levels_ = 0) const;

    void ApplyUpdate(const OrderBookUpdate &updates);

    // Volume Weighted Average Price
    std::pair<float, float> VWAP(float target_quantity, bool is_bid);

private:

    std::pair<LevelBooks, LevelBooks> levels(size_t levels_ = 0) const;

    void update(float price, float amount, bool is_bid);


    std::string exchange;
    std::string symbol;
    Books bids;
    Books asks;
    std::vector<float> cum_bids;
    std::vector<float> cum_asks;
    Timestamp exchange_ts;
    Timestamp local_ts;
    bool initialized;


};

#endif  // ORDER_BOOK_ORDER_BOOK_H
