#ifndef _LTD_INCLUDE_STDLTD_HPP_
#define _LTD_INCLUDE_STDLTD_HPP_

#include <atomic>
#include <variant>

#include "err.hpp"
#include "stddef.hpp"
#include "cli.hpp"

namespace ltd
{
    /**
     * Construct object of type T on the given memory address.
     */
    template<typename T, typename... Args>
    static void construct(T *instance, Args&&... args) noexcept
    {
        new (instance) T(std::forward<Args>(args)...);
    }

    /**
     * Destruct the object by calling its destructor.
     * This does not release the memory.
     */
    template<typename T>
    void destruct(T *instance) noexcept
    {
        (*instance).~T();
    }   

    /**
     * 
     */
    struct memory_pool
    {
        virtual multi_ret<void*,memory_pool*,err> allocate(size_t allocation_size) noexcept = 0;
        virtual err deallocate(void *allocated_ptr) noexcept = 0;
    };

    /**
     * 
     */
    template<class T>
    struct default_dltr
    {
        void operator ()(T *ptr, memory_pool *deallocator) noexcept
        {
            destruct(ptr);
            deallocator->deallocate(ptr);
        }
    };

    /**
     * 
     */
    template<class T>
    struct cpp_dltr
    {
        void operator ()(T *ptr, memory_pool *deallocator = nullptr) noexcept
        {
            delete ptr;
        }
    };

    /**
     * 
     */
    struct ref_counter
    {
        /**
         * @brief
         * Default constructor (deleted)
         */
        ref_counter() = delete;
        
        /**
             * @brief
             * Parameterized constructor
             */
        ref_counter(memory_pool *pool) : deallocator(pool) {}

        /**
         * @broef
         * Increments the reference counter.
         */
        inline int32_t increment() noexcept
        {
            return counter.fetch_add(1, std::memory_order_relaxed) + 1;
        }

        /**
         * @brief 
         * Decrements the reference counter.
         */
        inline int32_t decrement() noexcept
        {
            return counter.fetch_sub(1, std::memory_order_acq_rel) - 1;
        }


        /**
         * @brief
         * Checks is the refernce counter is still unique.
         */
        inline bool is_unique() const noexcept
        {
            return counter.load(std::memory_order_acquire) == 1;
        }

        /**
         * @brief
         * Checks whether the pointer is still valid, i.e. the owner still exist.
         */
        inline bool is_valid() const noexcept
        {
            return flag == 0;
        }

        /**
         * @brief
         * Marks the pointers as an invalid pointer because the ownser is dstroyed.
         */
        inline void invalidate() noexcept
        {
            flag = 1;
        }

        /**
         * @brief
         * Get the referene count number.
         */
        inline uint32_t count() const noexcept
        {
            return counter.load(std::memory_order_acquire);
        }

    private:
        memory_pool * deallocator;        
        std::atomic<int32_t> counter{0};
        uint32_t flag{0};
    };

    struct rc_factory
    {
        static ref_counter* create_ref_counter(memory_pool *pool) noexcept;
        static void destroy_ref_counter(ref_counter *rc) noexcept;
    };

    /**
     * @brief 
     * Smart pointer's representation
     */ 
    template <class T, class D = default_dltr<T>>
    class pointer
    {
        private:
            T *raw_ptr;
            ref_counter *ref_cnt;       

        public:
            /**
             * @brief
             * Default constructor
             */
            pointer() noexcept : raw_ptr(nullptr), ref_cnt(nullptr) {}      
            
            /**
             * @brief
             * Parameterized constructor
             */
            pointer(T *pointer, ref_counter *rc) noexcept : raw_ptr(pointer), ref_cnt(rc) 
            {
                if (ref_cnt != nullptr) {
                    ref_cnt->increment();
                }
            }

            /**
             * @brief
             * Move constructor
             */
            pointer(pointer<T> &&other) noexcept {
                raw_ptr       = other.raw_ptr;
                other.raw_ptr = nullptr;

                ref_cnt        = other.ref_cnt;
                other.ref_cnt  = nullptr;
            }   

            /**
             * @brief
             * Copy constructor
             */
            pointer(pointer<T> &other) noexcept {
                if (ref_cnt != nullptr) {
                    ref_cnt->increment();

                    raw_ptr = other.raw_ptr;
                    ref_cnt = other.ref_cnt;                
                }                
            }
            
            /**
             * @brief
             * Member access operator 
             */
            inline T* operator->() const noexcept { return raw_ptr; }
            
            /**
             * @brief
             * Check whether this pointer is null
             */     
            
            inline bool is_null() const noexcept { return raw_ptr == nullptr; }            

            /**
             * @brief
             * Destructor
             */
            ~pointer()
            {
                if (ref_cnt != nullptr) {
                    if (ref_cnt->decrement() == 0) {
                        static_assert(ref_cnt->is_valid()==false, "Invalid pointer<T> state");

                        D deleter;
                        deleter(raw_ptr, ref_cnt->deallocator);

                        rc_factory::destroy_ref_counter(ref_cnt);

                        raw_ptr = nullptr;
                        ref_cnt = nullptr;
                    }
                }
            }            
    };

    /**
     * @brief 
     * Smart pointer's representation
     */
    template <class T, class D = default_dltr<T>>
    class object
    {
    private:
        T *raw_ptr;
        std::variant<std::monostate,ref_counter*,memory_pool*> holder;

    public:
        /**
         * @brief
         * Default constructor
         */
        object() noexcept : raw_ptr(nullptr), holder(std::monostate{}) {}

        /**
         * @brief
         * Parameterized constructor
         */
        object(T *pointer, memory_pool *d) noexcept : raw_ptr(pointer), holder(d) {}

        /**
         * @brief
         * Move constructor
         */
        object(object<T> &&other) noexcept {
            raw_ptr       = other.raw_ptr;
            other.raw_ptr = nullptr;

            holder        = other.holder;
            other.holder  = std::monostate{};
        }

        /**
         * @brief
         * Copy constructor (deleted)
         */
        object(const object<T> &other) = delete;

        /**
         * @brief
         * Copy constructors are deleted because this class is not copy-able.
         * 
         * @param other
         */
        // object(const object& other) = delete;

        /**
         * @brief
         * Assignment operator is deleted because `object` cannot be copied.
         * 
         * @param other
         * @return ptr&
         */
        object& operator=(const object& other) = delete;

        /**
         * @brief
         * Member access operator 
         */
        inline T* operator->() const noexcept { return raw_ptr; }

        /**
         * @brief
         * Check whether this pointer is null
         */
        inline bool is_null() const noexcept { return raw_ptr == nullptr; }

        /**
         * @brief
         * Check whether this object is being referenced by other pointers.
         */
        bool is_referenced() const noexcept
        {
            return std::holds_alternative<ref_counter*>(holder);
        }

        /**
         * @brief
         * Get the number of references to this object.
         */
        uint32_t get_reference_count() const noexcept
        {
            if(is_referenced()) {
                std::get<ref_counter*>(holder)->count();
            }

            return 0;
        }

        /**
         * @brief
         * Get a reference pointer to this object.
         */
        pointer<T> get_reference() noexcept
        {
            ref_counter* rc = nullptr;

            if(is_referenced()) 
            {
                rc = std::get<ref_counter*>(holder);
                rc->increment();                
            } 
            else 
            {
                auto pool = std::get<memory_pool*>(holder);
                rc = rc_factory::create_ref_counter(pool);

                rc->increment();                
                holder = rc;
            }

            pointer<T> ptr(raw_ptr, rc);
            return ptr;
        }

        /**
         * @brief
         * Destructor
         */
        ~object()
        {
            if (raw_ptr != nullptr) {
                if(is_referenced()) {
                    std::get<ref_counter*>(holder)->invalidate();
                    std::get<ref_counter*>(holder)->decrement();
                } else {
                    D deleter;                    
                    deleter(raw_ptr, std::get<memory_pool*>(holder));                    
                }
                                
                raw_ptr = nullptr;
            }
        }
    };

    template<typename T, typename... Args>
    multi_ret<object<T>,err> create_object(memory_pool *pool, Args&&... args) noexcept
    {
        auto [allocated_ptr, mem_pool, allocation_err] = pool->allocate(sizeof(T));     
        
        if (allocation_err != err::no_error) {
            return {object<T>(), allocation_err};
        }

        T *obj_ptr = static_cast<T*>(allocated_ptr);

        construct<T, Args...>(obj_ptr, std::forward<Args>(args)...);   

        object<T> obj(obj_ptr, mem_pool);

        return {std::move(obj), err::no_error}; 
    }

    template<typename T, typename... Args>
    object<T,cpp_dltr<T>> attach_object(T *ptr) noexcept
    {
        object<T,cpp_dltr<T>> obj(ptr,nullptr);
        return obj; 
    }
} // namespace ltd

#endif // _LTD_INCLUDE_STDLTD_HPP_