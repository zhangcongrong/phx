import time
from pandas import Timestamp
from sortedcontainers import SortedDict

from objects import OrderBookSnapshot
from order_book import OrderBook
from utils.typing import Ticker

print("-----------------test 1 start----------------------")
exchange = "binance"
symbol = "BTCUSDT"
bids = SortedDict({37565.7: 171.834,
                   37565: 2,
                   37564.2: 69.709,
                   37563: 2.498,
                   37562.7: 89.915,
                   37561.9: 0.01,
                   37561.2: 39.588,
                   37559.2: 0.003,
                   37555.6: 26.596,
                   37554.1: 119.74,
                   37552.6: 42.209,
                   37551.1: 7.091,
                   37551: 0.01,
                   37550.1: 0.806,
                   37550: 13.915,
                   37549.6: 5.947,
                   37546.2: 0.398,
                   37543.2: 0.133,
                   37542.2: 1.331,
                   37539: 1})

asks = SortedDict({37579.2: 4.943,
                   37580.7: 6.097,
                   37582.2: 43.882,
                   37583.7: 8.571,
                   37585.2: 5.696,
                   37586.7: 16.168,
                   37620: 44.53,
                   37632.6: 0.005,
                   37637.4: 0.918,
                   37659.2: 0.003,
                   37689.7: 0.01,
                   37690: 500,
                   37698.9: 187.833,
                   37707.8: 0.005,
                   37724.5: 0.1,
                   37726.2: 0.1,
                   37729: 187.683,
                   37737.9: 0.1,
                   37753.5: 1.871,
                   37759.2: 187.533})
now = time.time()
local_ts = Timestamp.utcfromtimestamp(now)
exchange_ts = Timestamp.utcfromtimestamp(now - 0.05)
print("current timestamp: ", local_ts)

order_book = OrderBook(exchange, symbol, None, None)
print("order book status: ", not order_book.is_invalid)
order_book.apply_snapshot(bids, asks)
# is_valid = order_book.is_valid  # the result is not a bool value?
print("order book status: ", not order_book.is_invalid)
order_book.set_timestamp(exchange_ts, local_ts)

cum_bids, cum_asks = order_book.cumulative_levels(10)
print("cum_bids len:", len(cum_bids), "cum_bids:", cum_bids)
print("cum_asks len:", len(cum_asks), "cum_asks:", cum_asks)
print("-----------------test 1 end----------------------\n\n")

print("-----------------test 2 start----------------------")
# ticker: Ticker = Ticker(exchange, symbol)
ticker = order_book.key()
new_bids = SortedDict({37565.5: 173.215,
                       37565.1: 2.3,
                       37564.2: 69.709,
                       37563: 2.498,
                       37562.7: 89.915,
                       37561.9: 0.01,
                       37561.2: 39.588,
                       37559.2: 0.003,
                       37555.6: 26.596,
                       37554.1: 119.74,
                       37552.6: 42.209,
                       37551.1: 7.091,
                       37550.1: 0.806,
                       37550: 13.915,
                       37549.6: 5.947,
                       37549.5: 0.01,
                       37546.2: 0.398,
                       37543.2: 0.133,
                       37542.3: 1.231,
                       37541.1: 1.11})

new_asks = SortedDict({37582.2: 36.771,
                       37583.7: 8.571,
                       37585.2: 5.696,
                       37586.7: 16.168,
                       37620: 44.53,
                       37632.6: 0.005,
                       37637.4: 0.918,
                       37659.2: 0.003,
                       37689.7: 0.01,
                       37690: 500,
                       37698.9: 187.833,
                       37707.8: 0.005,
                       37724.5: 0.1,
                       37726.2: 0.1,
                       37729: 187.683,
                       37737.9: 0.1,
                       37753.5: 1.871,
                       37759.2: 187.533,
                       37759.9: 190.274,
                       37760: 14.983})

order_book_snapshot = OrderBookSnapshot(ticker, exchange_ts, local_ts, bids, asks)
new_order_book = order_book.from_snapshot(order_book_snapshot);
new_order_book.apply_snapshot(new_bids, new_asks);

new_cum_bids, new_cum_asks = new_order_book.cumulative_levels(10)
print("new_cum_bids len:", len(new_cum_bids), "new_cum_bids:", new_cum_bids)
print("new_cum_asks len:", len(new_cum_asks), "new_cum_asks:", new_cum_asks)

for price in new_bids.keys():
    print("first:", price, "second:", new_bids[price])

print("-----------------test 2 end----------------------\n\n")

