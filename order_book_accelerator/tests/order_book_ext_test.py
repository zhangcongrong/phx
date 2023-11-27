import time
from pandas import Timestamp
from sortedcontainers import SortedDict

from build import order_book_ext



print(order_book_ext.add(1, 2))

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

order_book = order_book_ext.OrderBook(exchange, symbol, bids, asks)
print(order_book.key())
print(order_book.is_valid())
order_book.apply_snapshot(bids, asks)
print(order_book.is_valid())