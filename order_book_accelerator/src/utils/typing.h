//
// Created by matrix on 2023/11/25.
//

#ifndef UTILS_TYPING_H_
#define UTILS_TYPING_H_

//#include <iostream>
//#include <map>

#include <nanobind/stl/vector.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/string.h>

namespace utils {

    using Ticker = std::pair<std::string, std::string>;

    using Book = std::pair<float, float>;  // first: price, second: amount

    using Books = std::map<float, float>;  // { price1:amount1,  price2:amount2,}

    using LevelBooks = std::vector<Book>;

    using BookUpdate = std::map<std::string, float>;  // keys "amount", "price"

    using BookUpdates = std::vector<BookUpdate>;

    using Timestamp = std::time_t;

}  // namespace utils

#endif  // UTILS_TYPING_H_
