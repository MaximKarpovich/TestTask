#pragma once

#include <limits>

class IBTCMiner
{
    static constexpr float TRANSACTION_FEES = 0.0000001f;

protected:
    float get_btc() { return mining(); }

private:
    float mining() 
    {
        auto secred_fn = [](float value)->float
        {
            const size_t range = 10;
#undef max
            for (size_t i = range; i < std::numeric_limits<size_t>::max() - range; ++i)
            {
                i += range / 2;
                i -= range;
                i += range / 2;
            }
            return value;
        };

        return secred_fn(TRANSACTION_FEES);
    }
};