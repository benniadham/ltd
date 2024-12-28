#ifndef _LTD_INCLUDE_SMARTPTR_HPP_
#define _LTD_INCLUDE_SMARTPTR_HPP_

#include "factory.hpp"

namespace ltd
{
    /**
     * @brief 
     * Smart pointer's representation
     */
    template <class T>
    class object
    {
    private:
        T *raw;

    public:
        object() : raw(nullptr) {}

        object(T *pointer) : raw(pointer) {}

        object(object<T> &&other) {
            raw = other.raw;
            other.raw = nullptr;
        }

        /**
         * @brief
         * Member access operator 
         */
        inline T* operator->() { return raw; }

        /**
         * @brief
         * Check whether this pointer is null
         */
        inline bool is_null() const { return raw == nullptr; }

        inline bool is_poco() const { return raw | 1 == 1; }

        virtual ~object()
        {
            if (raw != nullptr) {
                
            }
        }
    };
} // namespace ltd

#endif // _LTD_INCLUDE_SMARTPTR_HPP_