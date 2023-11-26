//
// Created by matrix on 2023/11/26.
//

#include "order_book.h"

OrderBook::OrderBook(const OrderBookSnapshot &d)
        : exchange(d.GetExchange()), symbol(d.GetSymbol()),
          bids{d.bids}, asks{d.asks},
          cum_bids(0), cum_asks(0),
          exchange_ts(d.GetExchangeTs()), local_ts(d.GetLocalTs()), initialized(false) {}

void OrderBook::ApplyUpdate(const OrderBookUpdate &updates) {
    auto apply = [&](const BookUpdates &levels, bool is_bid) {
        for (const auto &level: levels) {
            auto price_it = level.find("price");
            auto amount_it = level.find("amount");
            if (price_it != level.end() && amount_it != level.end()) {
                update(price_it->second, amount_it->second, is_bid);
            }
        }
    };

    if (!initialized) {
        return;
    }

    apply(updates.bids, true);
    apply(updates.asks, false);
    SetTimestamp(updates.GetExchangeTs(), updates.GetLocalTs());
}

// Volume Weighted Average Price
std::pair<float, float> OrderBook::VWAP(float target_quantity, bool is_bid) {
    float total_filled = 0;
    float weighted_sum = 0;

    if (is_bid) {
        for (auto it = bids.rbegin(); it != bids.rend(); ++it) { // reverse read
            auto price = it->first;
            auto quantity = it->second;
            auto remaining = std::max(target_quantity - total_filled, 0.0f);
            if (remaining <= 0) {
                break;
            } else {
                auto filled = std::min(quantity, remaining);
                total_filled += filled;
                weighted_sum += filled * price;
            }
        }
    } else {
        for (auto &ask: asks) {
            auto price = ask.first;
            auto quantity = ask.second;
            auto remaining = std::max(target_quantity - total_filled, 0.0f);
            if (remaining <= 0) {
                break;
            } else {
                auto filled = std::min(quantity, remaining);
                total_filled += filled;
                weighted_sum += filled * price;
            }
        }
    }
    return total_filled > 0 ? std::make_pair(weighted_sum / total_filled, total_filled) : std::make_pair(0.0f,
                                                                                                         0.0f);
}

std::pair<LevelBooks, LevelBooks> OrderBook::levels(size_t levels_) const {
    LevelBooks bid_levels;
    LevelBooks ask_levels;

    for (auto it = bids.rbegin(); it != bids.rend() && (levels_ == 0 || bid_levels.size() < levels_); ++it) {
        bid_levels.emplace_back(*it);
    }

    for (auto it = asks.begin(); it != asks.end() && (levels_ == 0 || ask_levels.size() < levels_); ++it) {
        ask_levels.emplace_back(*it);
    }

    return std::make_pair(bid_levels, ask_levels);
}

std::pair<float, float> OrderBook::CumulativeLevels(size_t levels_) const {
    auto [bids_, asks_] = levels(levels_);
    float sum_b = 0;
    float sum_a = 0;

    for (auto &i: bids_) {
        sum_b += i.second;
    }

    for (auto &i: asks_) {
        sum_a += i.second;
    }

    return std::make_pair(sum_a, sum_b);
}

void OrderBook::update(float price, float amount, bool is_bid) {
    if (is_bid) {
        if (amount == 0) {
            bids.erase(price);
        } else {
            bids[price] = amount;
        }
    } else {
        if (amount == 0) {
            asks.erase(price);
        } else {
            asks[price] = amount;
        }
    }
}
