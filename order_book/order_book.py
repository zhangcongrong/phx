from typing import Optional, Tuple, List, Dict

import numpy as np
import numpy.typing as npt
from sortedcontainers import SortedDict

from objects import OrderBookSnapshot, OrderBookUpdate
from utils.typing import Ticker

class OrderBook:

    @classmethod
    def from_snapshot(cls, snapshot: OrderBookSnapshot):
        book = OrderBook(
            snapshot.exchange, snapshot.symbol, snapshot.bids, snapshot.asks
        )
        book.set_timestamp(snapshot.timestamp, snapshot.local_timestamp)
        return book

    def __init__(
            self,
            exchange,
            symbol,
            bids: Optional[SortedDict] = None,
            asks: Optional[SortedDict] = None,
    ):
        self.exchange = exchange
        self.symbol = symbol
        self.bids = bids
        self.asks = asks
        self.cum_bids = None
        self.cum_asks = None
        self.exchange_ts = None
        self.local_ts = None
        self.initialized = False

    def key(self) -> Ticker:
        return self.exchange, self.symbol

    def set_timestamp(self, exchange_ts, local_ts):
        self.exchange_ts = exchange_ts
        self.local_ts = local_ts

    def apply_snapshot(self, bids, asks):
        if isinstance(bids, SortedDict) and isinstance(asks, SortedDict):
            self.bids = bids
            self.asks = asks
        else:
            self.bids = SortedDict(bids)
            self.asks = SortedDict(asks)
        self.initialized = True

    def apply_update(self, updates: OrderBookUpdate):
        def apply(levels: List[Dict[str, float]], is_bid):
            for level in levels:
                price = level.get("price", None)
                amount = level.get("amount", None)
                if price is not None and amount is not None:
                    self.update(price, amount, is_bid)

        if not self.initialized:
            return
        apply(updates.bids, True)
        apply(updates.asks, False)
        self.set_timestamp(updates.timestamp, updates.local_timestamp)

    def update(self, price, amount, is_bid):
        if is_bid:
            if amount == 0:
                self.bids.pop(price, None)
            else:
                self.bids[price] = amount
        else:
            if amount == 0:
                self.asks.pop(price, None)
            else:
                self.asks[price] = amount

    @property
    def is_valid(self) -> bool:
        return self.initialized and self.bids and self.asks

    @property
    def is_invalid(self) -> bool:
        return not self.is_valid

    @property
    def mid_price(self) -> Optional[float]:
        if self.bids and self.asks:
            return (self.bids.peekitem(-1)[0] + self.asks.peekitem(0)[0]) / 2.0
        else:
            return None

    @property
    def top_of_book_price(self) -> Optional[Tuple[float, float]]:
        bid = self.top_bid_price
        ask = self.top_ask_price
        return (bid, ask) if (bid is not None and ask is not None) else None

    @property
    def top_bid(self) -> Optional[Tuple[float, float]]:
        if self.bids:
            return self.bids.peekitem(-1)
        else:
            return None

    @property
    def top_bid_price(self) -> Optional[float]:
        if self.bids:
            return self.bids.peekitem(-1)[0]
        else:
            return None

    @property
    def top_ask(self) -> Optional[Tuple[float, float]]:
        if self.asks:
            return self.asks.peekitem(0)
        else:
            return None

    @property
    def top_ask_price(self) -> Optional[float]:
        if self.asks:
            return self.asks.peekitem(0)[0]
        else:
            return None

    def levels(self, levels=None) -> Tuple[npt.NDArray, npt.NDArray]:
        """
        Get the levels as np arrays. May be costly as it
        returns full book.
        """
        bid_levels = np.flip(np.asarray(self.bids.items()), 0)
        ask_levels = np.asarray(self.asks.items())
        if levels is not None:
            bid_levels = bid_levels[0:levels, :]
            ask_levels = ask_levels[0:levels, :]
        return bid_levels, ask_levels

    def cumulative_levels(self, levels=None):
        b, a = self.levels(levels)
        cum_b = np.cumsum(b[:, 1])
        cum_a = np.cumsum(a[:, 1])
        return cum_b, cum_a

    def update_cumulative_levels(self, levels=None):
        self.cum_bids, self.cum_asks = self.cumulative_levels(levels)

    def vwap(self, target_quantity, is_bid):
        total_filled = 0
        weighted_sum = 0
        if is_bid:
            for price in reversed(self.bids):
                quantity = self.bids[price]
                remaining = max(target_quantity - total_filled, 0)
                if remaining <= 0:
                    break
                else:
                    filled = min(quantity, remaining)
                    total_filled += filled
                    weighted_sum += filled * price
        else:
            for price in self.asks.keys():
                quantity = self.asks[price]
                remaining = max(target_quantity - total_filled, 0)
                if remaining <= 0:
                    break
                else:
                    filled = min(quantity, remaining)
                    total_filled += filled
                    weighted_sum += filled * price
        return weighted_sum / total_filled, total_filled
