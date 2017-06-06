// @file intset.hpp
// @brief 整数集合结构
// @author
// @version
// @date

#include <cstdlib>
#include <cstdint>


namespace liter
{
namespace algo
{

#define INTSET_ENC_INT16 ((sizeof(int16_t)))
#define INTSET_ENC_INT32 ((sizeof(int32_t)))
#define INTSET_ENC_INT64 ((sizeof(int64_t)))


namespace detail
{

struct Helper
{
    static uint8_t encoding(int64_t v) {
        if (v < INT32_MIN || v > INT32_MAX) {
            return INTSET_ENC_INT64;
        }
        else if (v < INT16_MIN || v > INT16_MAX) {
            return INTSET_ENC_INT32;
        }

        return INTSET_ENC_INT16;
    }
};

}

class intset {
private:
    uint32_t m_encoding;
    uint32_t m_length;
    int8_t* m_contents;
};

}
}
