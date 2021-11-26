
#include <stdint.h>

namespace {
using addr_type = uintptr_t;

template <typename T>
addr_type get_addr(T* p)
{
    return reinterpret_cast<addr_type>(p);
}

} // namespace
