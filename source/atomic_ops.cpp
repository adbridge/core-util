// Copyright (C) 2015 ARM Limited. All rights reserved.

#include "mbed-util/atomic_ops.h"

namespace mbed {
namespace util {

/* For ARMv7-M and above, we use the load/store-exclusive instructions to
 * implement atomic_cas, so we provide three template specializations
 * corresponding to the byte, half-word, and word variants of the instructions.
 */
#if (__CORTEX_M >= 0x03)
template <>
bool atomic_cas<uint8_t>(uint8_t *ptr, uint8_t *expectedCurrentValue, uint8_t desiredValue)
{
    uint8_t currentValue = __LDREXB(ptr);
    if (currentValue != *expectedCurrentValue) {
        *expectedCurrentValue = currentValue;
        __CLREX();
        return false;
    }

    return !__STREXB(desiredValue, ptr);
}

template<>
bool atomic_cas<uint16_t>(uint16_t *ptr, uint16_t *expectedCurrentValue, uint16_t desiredValue)
{
    uint16_t currentValue = __LDREXH(ptr);
    if (currentValue != *expectedCurrentValue) {
        *expectedCurrentValue = currentValue;
        __CLREX();
        return false;
    }

    return !__STREXH(desiredValue, ptr);
}

template<>
bool atomic_cas<uint32_t>(uint32_t *ptr, uint32_t *expectedCurrentValue, uint32_t desiredValue)
{
    uint32_t currentValue = __LDREXW(ptr);
    if (currentValue != *expectedCurrentValue) {
        *expectedCurrentValue = currentValue;
        __CLREX();
        return false;
    }

    return !__STREXW(desiredValue, ptr);
}
#endif /* #if (__CORTEX_M >= 0x03) */

} // namespace util
} // namespace mbed