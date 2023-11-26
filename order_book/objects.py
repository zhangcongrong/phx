from enum import IntEnum
from typing import List, Dict

import pandas as pd
from pandas import Timestamp
from sortedcontainers import SortedDict
from utils.typing import Ticker

class Side(IntEnum):
    Sell = -1
    Buy = 1

    @property
    def opposite(self):
        return Side.Sell if self.value == Side.Buy else Side.Buy

    def __str__(self):
        return "buy" if self.value == Side.Buy else "sell"

    @classmethod
    def parse(cls, s: str):
        if s.lower() == "buy":
            return Side.Buy
        if s.lower() == "sell":
            return Side.Sell
        raise ValueError(f"cannot parse Side from {s}")


class TickerData(object):
    __slots__ = "ticker", "timestamp", "local_timestamp"

    def __init__(
            self, ticker: Ticker, timestamp: Timestamp, local_timestamp: Timestamp
    ):
        self.ticker = ticker
        self.timestamp = timestamp
        self.local_timestamp = local_timestamp

    @property
    def symbol(self):
        return self.ticker[1]

    @property
    def exchange(self):
        return self.ticker[0]


class Trade(TickerData):
    __slots__ = "trade_id", "price", "side", "amount"

    def __init__(
            self,
            ticker: Ticker,
            timestamp: Timestamp,
            local_timestamp: Timestamp,
            trade_id,
            price,
            side: Side,
            amount,
    ):
        TickerData.__init__(self, ticker, timestamp, local_timestamp)
        self.price = price
        self.trade_id = trade_id
        self.side = side
        self.amount = amount

    def __str__(self):
        return f"Trade({self.ticker}[{self.timestamp}|{self.local_timestamp}] : {self.side} {self.price} {self.amount})"

    @classmethod
    def from_dict(cls, d: dict):
        return Trade(
            tuple(d["ticker"]),
            pd.Timestamp(d["timestamp"]),
            pd.Timestamp(d["local_timestamp"]),
            d["trade_id"],
            float(d["price"]),
            Side.parse(d["side"]),
            float(d["amount"]),
        )


class TradeBar(TickerData):
    __slots__ = (
        "name",
        "interval",
        "kind",
        "open",
        "high",
        "low",
        "close",
        "volume",
        "buy_volume",
        "sell_volume",
        "trades",
        "vwap",
        "open_timestamp",
        "close_timestamp",
        "timestamp",
    )

    def __init__(
            self,
            ticker: Ticker,
            name,
            interval: int,
            kind: str,
            open,
            high,
            low,
            close,
            volume,
            buy_volume,
            sell_volume,
            trades,
            vwap,
            open_timestamp: Timestamp,
            close_timestamp: Timestamp,
            timestamp,
    ):
        TickerData.__init__(self, ticker, timestamp, timestamp)
        self.name = name
        self.interval = interval
        self.kind = kind
        self.open = open
        self.close = close
        self.high = high
        self.low = low
        self.volume = volume
        self.buy_volume = buy_volume
        self.sell_volume = sell_volume
        self.trades = trades
        self.vwap = vwap
        self.open_timestamp = open_timestamp
        self.close_timestamp = close_timestamp
        self.timestamp = timestamp

    @classmethod
    def from_dict(cls, d: dict):
        return TradeBar(
            tuple(d["ticker"]),
            d["name"],
            int(d["interval"]),
            d["kind"],
            float(d["open"]),
            float(d["high"]),
            float(d["low"]),
            float(d["close"]),
            float(d["volume"]),
            float(d["buy_volume"]),
            float(d["sell_volume"]),
            int(d["trades"]),
            float(d["vwap"]),
            pd.Timestamp(d["open_timestamp"]),
            pd.Timestamp(d["close_timestamp"]),
            pd.Timestamp(d["timestamp"]),
        )


class TopOfBook(TickerData):
    __slots__ = "bid", "bid_size", "ask", "ask_size"

    def __init__(
            self,
            ticker: Ticker,
            timestamp: Timestamp,
            local_timestamp: Timestamp,
            bid,
            bid_size,
            ask,
            ask_size,
    ):
        TickerData.__init__(self, ticker, timestamp, local_timestamp)
        self.bid = bid
        self.bid_size = bid_size
        self.ask = ask
        self.ask_size = ask_size

    def __str__(self):
        return (
            f"TopOfBook({self.ticker}"
            f"[{self.timestamp}|{self.local_timestamp}] : "
            f"{self.bid}:{self.bid_size} | "
            f"{self.ask}:{self.ask_size})"
        )

    @property
    def mid_price(self):
        return (self.bid + self.ask) / 2.0


class OrderBookSnapshot(TickerData):
    __slots__ = "bids", "asks"

    def __init__(
            self,
            ticker: Ticker,
            timestamp: Timestamp,
            local_timestamp: Timestamp,
            bids: SortedDict,
            asks: SortedDict,
    ):
        TickerData.__init__(self, ticker, timestamp, local_timestamp)
        self.bids = bids
        self.asks = asks

    @classmethod
    def from_dict(cls, d: dict):
        bids = SortedDict(d["bids"])
        asks = SortedDict(d["asks"])
        return OrderBookSnapshot(
            tuple(d["ticker"]),
            pd.Timestamp(d["timestamp"]),
            pd.Timestamp(d["local_timestamp"]),
            bids,
            asks,
        )

    def __str__(self):
        return (
            f"OrderBookSnapshot({self.ticker}"
            f"[{self.timestamp}|{self.local_timestamp}] : "
            f"{self.bids.peekitem(-1)} {len(self.bids)} | "
            f"{self.asks.peekitem(0)}) {len(self.asks)}"
        )

    @property
    def mid_price(self):
        return (self.bids.peekitem(-1)[0] + self.asks.peekitem(0)[0]) / 2.0

    @property
    def top_bid(self):
        return self.bids.peekitem(-1)

    @property
    def top_bid_price(self):
        return self.bids.peekitem(-1)[0]

    @property
    def top_ask(self):
        return self.asks.peekitem(0)

    @property
    def top_ask_price(self):
        return self.asks.peekitem(0)[0]

    def touch(self, side: Side):
        return self.top_ask_price if side == Side.Sell else self.top_bid_price

    def far_touch(self, side: Side):
        return self.touch(side.opposite)

    @property
    def top_of_book(self):
        bid, bid_size = self.top_bid
        ask, ask_size = self.top_ask
        return TopOfBook(
            self.ticker,
            self.timestamp,
            self.local_timestamp,
            bid,
            bid_size,
            ask,
            ask_size,
        )


class OrderBookUpdate(TickerData):
    __slots__ = "bids", "asks"

    def __init__(
            self,
            ticker: Ticker,
            timestamp: Timestamp,
            local_timestamp: Timestamp,
            bids: List[Dict[str, float]],  # list of dicts with keys "amount", "price"
            asks: List[Dict[str, float]],  # list of dicts with keys "amount", "price"
    ):
        TickerData.__init__(self, ticker, timestamp, local_timestamp)
        self.bids = bids
        self.asks = asks

    @classmethod
    def from_dict(cls, d: dict):
        return OrderBookUpdate(
            tuple(d["ticker"]),
            pd.Timestamp(d["timestamp"]),
            pd.Timestamp(d["local_timestamp"]),
            d["bids"],
            d["asks"],
        )

    def __str__(self):
        return f"OrderBookUpdate({self.ticker}[{self.timestamp}|{self.local_timestamp}] : {self.bids} | {self.asks})"


class DerivativeTicker(TickerData):
    __slots__ = (
        "last_price",
        "open_interest",
        "funding_timestamp",
        "funding_rate",
        "predicted_funding_rate",
        "index_price",
        "mark_price",
    )

    def __init__(
            self,
            ticker: Ticker,
            timestamp: Timestamp,
            local_timestamp: Timestamp,
            last_price: float,
            open_interest: float,
            funding_timestamp: Timestamp,
            funding_rate: float,
            predicted_funding_rate: float,
            index_price: float,
            mark_price: float,
    ):
        TickerData.__init__(self, ticker, timestamp, local_timestamp)
        self.last_price = last_price
        self.open_interest = open_interest
        self.funding_timestamp = funding_timestamp
        self.funding_rate = funding_rate
        self.predicted_funding_rate = predicted_funding_rate
        self.index_price = index_price
        self.mark_price = mark_price

    @classmethod
    def from_dict(cls, d: dict):
        return DerivativeTicker(
            tuple(d["ticker"]),
            d["timestamp"],
            d["local_timestamp"],
            float(d["last_price"]),
            float(d["open_interest"]),
            pd.Timestamp(d["funding_timestamp"]),
            float(d["funding_rate"]),
            float(d["predicted_funding_rate"]),
            float(d["index_price"]),
            float(d["mark_price"]),
        )

    def __str__(self):
        return (
            f"DerivativeTicker({self.ticker}[{self.timestamp}|{self.local_timestamp}] : {self.last_price}, "
            f"{self.open_interest}, {self.funding_rate}, {self.index_price}, {self.mark_price})"
        )


DerivativeTickerSet = Dict[Ticker, DerivativeTicker]


class Liquidation(TickerData):
    __slots__ = "id", "price", "side", "amount"

    def __init__(
            self,
            ticker,
            timestamp: Timestamp,
            local_timestamp: Timestamp,
            id: str,
            price: float,
            side: int,
            amount: float,
    ):
        TickerData.__init__(self, ticker, timestamp, local_timestamp)
        self.id = id
        self.price = price
        self.side = side
        self.amount = amount

    def __str__(self):
        return (
            f"Liquidation({self.ticker}[{self.timestamp}|{self.local_timestamp}] : {self.id}, "
            f"{self.price}, {self.side} {self.amount})"
        )
