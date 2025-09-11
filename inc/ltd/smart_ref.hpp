#ifndef _LTD_INCLUDE_SMART_REF_HPP_
#define _LTD_INCLUDE_SMART_REF_HPP_

#include <variant>

#include "allocators.hpp"

namespace ltd
{
    /**
     * @brief 
     * Object reference representation
     */
    template <class T>
    class smart_ref
    {
    public:
        using container = std::variant<mem::allocator*,void*>;
        
    private:
        T *raw;
        container content;

    public:
        // Ctors
        smart_ref() : raw(nullptr), content(nullptr) {}

        smart_ref(smart_ref<T> &&other) {
            raw = other.raw;
            content = other.content;

            other.raw = nullptr;
            other.content = nullptr;
        }

        smart_ref(smart_ref<T>& other) = delete;
        smart_ref(const smart_ref<T>& other) = delete;
        
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

        virtual ~smart_ref()
        {
            if (raw != nullptr) {
                if (std::holds_alternative<mem::allocator*>(content)) {
                    mem::allocator *all =  std::get<mem::allocator*>(content);
                    all->destroy(raw);
                }
            }
        }
    };
}

#endif // _LTD_INCLUDE_SMART_REF_HPP_