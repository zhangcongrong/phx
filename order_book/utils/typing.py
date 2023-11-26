
class Ticker(tuple):
    def __new__(cls, exchange, symbol):
        return super().__new__(cls, (exchange, symbol))
