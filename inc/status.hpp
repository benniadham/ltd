#ifndef _LTD_INCLUDE_STATUS_HPP_
#define _LTD_INCLUDE_STATUS_HPP_

#include <string>

namespace ltd
{
    enum status_type
    {
        APPLICATION,
        SYSTEM,
        HTTP
    };

    enum app_error
    {
        app_unknown,            null_pointer,               index_out_of_bound,
        invalid_argument,       type_conversion_failure,    not_found,
        allocation_failure,     deallocation_failure,       invalid_operation,
        not_implemented,        invalid_state,              end_of_input
    };

    /**
     * @brief
     * Provides ltd's status reporting and error handling.
     * 
     * @details
     * Class `status` provide simple representation of status reporting and 
     * declaration of some standards response code for error reporting.
     *      * 
     * Predefined standard errors is vailable to represent an error.
     * ```C++
     *      status perform_operation(int value)
     *      {
     *          // ... perform some action
     *          if (value < 0)
     *              return status::invalid_operation.clone("Not enough data");
     *          
     *          return status::ok;
     *      }
     * ```
     * 
     * Equality operator can be used to check the kind of the error.
     * ```C++
     *      auto err = perform_operation(0);
     *      if ( err != status::ok)
     *          return err;
     * ``` 
     */
    class status 
    {
    private:
        std::string message;
        const char *description;
        
        int  code;
        char type;
        bool success;

    public:
        // ctors
        status();
        status(bool error, int type, int code, const char *desc, const char* msg);
        status(bool error, int type, int code, const char *desc);
        status(const status& other);
        status(status&& other);

        /**
         * @brief clone this status.
         * 
         * Clone this status with a new user defined message. This is useful to 
         * use standard statuses with custom message. 
         * 
         * ```C++
         *      if (value < 0)
     *              return status::invalid_operation.clone("Not enough data");
         * ```
         */
        status clone(const std::string& msg);

        /**
         * @brief Check if the status is success. 
         */
        bool   is_success() const;

        void set_status(bool error);
        void set_type(int type);
        void set_code(int code);
        void set_description(const char *description);
        void set_message(const char *message);

        bool get_status() const;
        int get_type() const;
        int get_code() const;
        const char * get_description() const;
        const char * get_message() const;

        bool operator==(const status& other) const;
        bool operator!=(const status& other) const;

        void operator=(const status& other);

    public:
        static status make(bool success, int type=0, int code=0, const char* desc=0); 
        static status make_app_status(bool success, int code=0, const char* desc=0); 

    public:
        static status ok;
        static status null_pointer;

        static status index_out_of_bound;
        static status invalid_argument;     
        static status type_conversion_failure;    
        static status not_found;
        static status allocation_failure;     
        static status deallocation_failure;       
        static status invalid_operation;
        static status not_implemented;        
        static status invalid_state;              
        static status end_of_input;
    };
} // namespace ltd

#endif // _LTD_INCLUDE_STATUS_HPP_