#ifndef _LTD_INCLUDE_FACTORY_HPP_
#define _LTD_INCLUDE_FACTORY_HPP_

namespace ltd
{
    namespace factory
    {
        /**
         * Construct object of type T on the given memory address.
         */
        template<typename T, typename... Args>
        static void construct(T *instance, Args&&... args)
        {
            new (instance) T(std::forward<Args>(args)...);
        }

        /**
         * Destruct the object by calling its destructor.
         * This does not release the memory.
         */
        template<typename T>
        void destruct(T *instance)
        {
            (*instance).~T();
        }
    }
}
#endif // _LTD_INCLUDE_FACTORY_HPP_