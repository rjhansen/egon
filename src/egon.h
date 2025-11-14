//
// Created by Robert J. Hansen on 11/14/25.
//

#ifndef EGON_FETCH_H
#define EGON_FETCH_H

#include <stdexcept>
#include <string>

class egon_error : public std::runtime_error {
public:
    egon_error() noexcept
        : std::runtime_error("")
    {
    }
    explicit egon_error(const char* what) noexcept
        : std::runtime_error(what)
    {
    }
    explicit egon_error(const std::string& what) noexcept
        : std::runtime_error(what.c_str())
    {
    }
    ~egon_error() noexcept override { }
};

void fetch_key_for(const char* addr);

#endif // EGON_FETCH_H