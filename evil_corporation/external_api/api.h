#pragma once

#include <stdint.h>

using byte = unsigned char;
using func = void(*)(const byte*, const size_t);
using value = int32_t;
using size = float;

static_assert(sizeof(size) == 4, "size must equal 4");
static_assert(sizeof(value) == 4, "value must equal 4");

namespace evil
{
    class api /* export external symbols: 'api' */
    {
    public:
        virtual void get_raw_rare(func callback) = 0;
        virtual void get_test_string(char* ptr /*366 symbols*/) = 0;
        virtual ~api() {};
    };

    enum class RareType : unsigned char
    {
        Jug,
        Helm,
        Coin,
        Sword,
    };
};