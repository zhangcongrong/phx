cmake_minimum_required(VERSION 3.26)
project(order_book)

set(CMAKE_CXX_STANDARD 17)

add_executable(order_book main.cpp
        utils/typing.h
        objects/side.h
        objects/ticker_data.h
        objects/top_of_book.h
        objects/order_book_snapshot.h
        objects/order_book_update.h
        order_book.h
        order_book.cpp)
