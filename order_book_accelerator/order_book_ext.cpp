#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/string.h>

#include "./src/order_book.h"
#include "./src/objects/order_book_snapshot.h"

//using Book = std::pair<float, float>;  // first: price, second: amount

//using Books = std::map<float, float>;  // { price1:amount1,  price2:amount2,}

int add(int a, int b = 1) { return a + b; }


namespace nb = nanobind;
using namespace nb::literals;

NB_MODULE(order_book_ext, module) {
    module.def("add", &add, "a"_a, "b"_a = 1,
        "This function adds two numbers and increments if only one is provided.");

    nb::class_ <OrderBook> order_book(module, "OrderBook");
    order_book.def(nb::init<const std::string &, const std::string &, Books, Books>());
    order_book.def("key", &OrderBook::Key);
    order_book.def("set_timestamp", &OrderBook::SetTimestamp, "a"_a, "b"_a);
    order_book.def("apply_snapshot", &OrderBook::ApplySnapshot, "a"_a, "b"_a);
    order_book.def("is_valid", &OrderBook::IsValid);
    order_book.def("mid_price", &OrderBook::MidPrice);
    order_book.def("top_of_book_price", &OrderBook::TopOfBookPrice);
    order_book.def("top_bid", &OrderBook::TopBid);
    order_book.def("top_bid_price", &OrderBook::TopBidPrice);
    order_book.def("top_ask", &OrderBook::TopAsk);
    order_book.def("top_ask", &OrderBook::TopAsk);
    order_book.def("top_ask_price", &OrderBook::TopAskPrice);
//    order_book.def("cumulative_levels", &OrderBook::CumulativeLevels, "a"_a=0);
//    order_book.def("update_cumulative_levels", &OrderBook::UpdateCumulativeLevels, "a"_a=0);
//    order_book.def("vwap", &OrderBook::VWAP, "a"_a, "b"_a);
}
