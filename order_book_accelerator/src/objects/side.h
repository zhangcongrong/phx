//
// Created by matrix on 2023/11/25.
//

#ifndef OBJECTS_SIDE_H
#define OBJECTS_SIDE_H

#include <string>

namespace objects {

    enum class Side {
        Sell = -1,
        Buy = 1,
    };

    namespace side {

        inline std::string Opposite(Side side) {
            switch (side) {
                case Side::Sell:
                    return "sell";
                case Side::Buy:
                    return "buy";
            }
            throw std::invalid_argument("Cannot convert Side to string: " + std::to_string(static_cast<int>(side)));
        }

        inline Side Parse(const std::string &s) {
            if (s == "buy") {
                return Side::Buy;
            } else if (s == "sell") {
                return Side::Sell;
            } else {
                throw std::invalid_argument("Cannot parse Side from: " + s);
            }
        }

    } // namespace side

}  // namespace objects

#endif  // OBJECTS_SIDE_H
