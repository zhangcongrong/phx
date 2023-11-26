#include <iostream>
#include <chrono>
//#include <map>

#include "order_book.h"

int main() {
    // TODO(Leo): should use gtest to test.

    std::cout << "-----------------test 1 start----------------------" << std::endl;

    std::string exchange = "binance";
    std::string symbol = "BTCUSDT";
    Books bids = {{37565.7, 171.834},
                  {37565,   2},
                  {37564.2, 69.709},
                  {37563,   2.498},
                  {37562.7, 89.915},
                  {37561.9, 0.01},
                  {37561.2, 39.588},
                  {37559.2, 0.003},
                  {37555.6, 26.596},
                  {37554.1, 119.74},
                  {37552.6, 42.209},
                  {37551.1, 7.091},
                  {37551,   0.01},
                  {37550.1, 0.806},
                  {37550,   13.915},
                  {37549.6, 5.947},
                  {37546.2, 0.398},
                  {37543.2, 0.133},
                  {37542.2, 1.331},
                  {37539,   1}};
    Books asks = {{37579.2, 4.943},
                  {37580.7, 6.097},
                  {37582.2, 43.882},
                  {37583.7, 8.571},
                  {37585.2, 5.696},
                  {37586.7, 16.168},
                  {37620,   44.53},
                  {37632.6, 0.005},
                  {37637.4, 0.918},
                  {37659.2, 0.003},
                  {37689.7, 0.01},
                  {37690,   500},
                  {37698.9, 187.833},
                  {37707.8, 0.005},
                  {37724.5, 0.1},
                  {37726.2, 0.1},
                  {37729,   187.683},
                  {37737.9, 0.1},
                  {37753.5, 1.871},
                  {37759.2, 187.533}};
    auto duration = std::chrono::system_clock::now().time_since_epoch();;
    Timestamp local_ts = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();;
    Timestamp exchange_ts = local_ts - 50;
    std::cout << "current timestamp: " << local_ts << std::endl;

    auto order_book = OrderBook(exchange, symbol, Books(), Books());
    std::cout << "order book status: " << order_book.IsValid() << std::endl;
    order_book.ApplySnapshot(bids, asks);
    std::cout << "order book status: " << order_book.IsValid() << std::endl;
    order_book.SetTimestamp(exchange_ts, local_ts);

    std::vector<float> cum_bids;
    std::vector<float> cum_asks;
    std::tie(cum_bids, cum_asks) = order_book.CumulativeLevels(10);
    std::cout << "cum_bids len: " << cum_bids.size() << "; last value: " << *cum_bids.rbegin() << std::endl;
    std::cout << "cum_asks len: " << cum_asks.size() << "; last value: " << *cum_asks.rbegin() << std::endl;

    std::cout << "-----------------test 1 end----------------------\n\n" << std::endl;

    std::cout << "-----------------test 2 start----------------------" << std::endl;
    Ticker ticker_ = {"binance", "BTCUSDT"};
    auto ticker = order_book.Key();
    Books new_bids = {{37565.5, 173.215},
                      {37565.1, 2.3},
                      {37564.2, 69.709},
                      {37563,   2.498},
                      {37562.7, 89.915},
                      {37561.9, 0.01},
                      {37561.2, 39.588},
                      {37559.2, 0.003},
                      {37555.6, 26.596},
                      {37554.1, 119.74},
                      {37552.6, 42.209},
                      {37551.1, 7.091},
                      {37550.1, 0.806},
                      {37550,   13.915},
                      {37549.6, 5.947},
                      {37549.5, 0.01},
                      {37546.2, 0.398},
                      {37543.2, 0.133},
                      {37542.3, 1.231},
                      {37541.1, 1.11}};
    Books new_asks = {{37582.2, 36.771},
                      {37583.7, 8.571},
                      {37585.2, 5.696},
                      {37586.7, 16.168},
                      {37620,   44.53},
                      {37632.6, 0.005},
                      {37637.4, 0.918},
                      {37659.2, 0.003},
                      {37689.7, 0.01},
                      {37690,   500},
                      {37698.9, 187.833},
                      {37707.8, 0.005},
                      {37724.5, 0.1},
                      {37726.2, 0.1},
                      {37729,   187.683},
                      {37737.9, 0.1},
                      {37753.5, 1.871},
                      {37759.2, 187.533},
                      {37759.9, 190.274},
                      {37760,   14.983}};
    auto order_book_snapshot = OrderBookSnapshot(ticker, exchange_ts, local_ts, bids, asks);
    auto new_order_book = OrderBook(order_book_snapshot);
    new_order_book.ApplySnapshot(new_bids, new_asks);

    std::tie(cum_bids, cum_asks) = new_order_book.CumulativeLevels(10);
    std::cout << "new cum_bids len: " << cum_bids.size() << "; last value: " << *cum_bids.rbegin() << std::endl;
    std::cout << "new cum_asks len: " << cum_asks.size() << "; last value: " << *cum_asks.rbegin() << std::endl;

    for (auto &item: new_bids) {
        std::cout << "first: " << item.first << " second: " << item.second << std::endl;
    }
    std::cout << "-----------------test 2 end----------------------\n\n" << std::endl;


    return 0;
}
