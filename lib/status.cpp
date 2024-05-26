#include "../inc/status.hpp"

namespace ltd
{
    status::status()
    {
        success = false;
        type    = 0;
        code    = 0;

        description = nullptr;
    }

    status::status(bool success, int type, int code, const char *description, const char* message)
    {
        this->success     = success;
        this->type        = type;
        this->code        = code;
        this->description = description;
        this->message     = message;
    }

    status::status(bool success, int type, int code, const char *description)
    {
        this->success     = success;
        this->type        = type;
        this->code        = code;
        this->description = description;
    }

    status::status(const status& other)
    {
        success     = other.success;
        type        = other.type;
        code        = other.code;
        description = other.description;
        message     = other.message;
    }

    status::status(status&& other)
    {
        success     = other.success;
        type        = other.type;
        code        = other.code;
        description = other.description;
        message     = std::move(other.message);
    }

    status status::clone(const std::string& msg) 
    {
        message = msg;
        return std::move(*this);
    }

    bool status::is_success() const
    {
        return success;
    }

    void status::set_status(bool success)
    {
        this->success = success;
    }

    void status::set_type(int type)
    {
        this->type = type;
    }

    void status::set_code(int code)
    {
        this->code = code;
    }

    void status::set_description(const char *description)
    {
        this->description = description;
    }

    void status::set_message(const char *message)
    {
        this->message = message;
    }

    bool status::get_status() const
    {
        return success;
    }

    int status::get_type() const
    {
        return type;
    }

    int status::get_code() const
    {
        return code;
    }

    const char * status::get_description() const
    {
        return description;
    }

    const char * status::get_message() const
    {
        return message.c_str();
    }

    bool status::operator==(const status& other) const
    {
        return success == other.success && 
               type == other.type &&
               code == other.code;
    }

    bool status::operator!=(const status& other) const
    {
        return success != other.success || 
               type != other.type ||
               code != other.code;
    }

    void status::operator=(const status& other)
    {
        success     = other.success;
        type        = other.type;
        code        = other.code;
        description = other.description;
        message     = other.message;
    }

    status status::make(bool success, int type, int code, const char* description)
    {
        status s(success, type, code, description);
        return std::move(s);
    }

    status status::make_app_status(bool success, int code, const char* description)
    {
        return status::make(success, status_type::APPLICATION, code, description);
    }

    status status::ok           = 
            status::make_app_status(true, 0, "Ok");

    status status::null_pointer = 
            status::make_app_status(false, app_error::null_pointer, "Null pointer exception");

    status status::index_out_of_bound = 
            status::make_app_status(false, app_error::null_pointer, "Index out-of-bound");

    status status::invalid_argument = 
            status::make_app_status(false, app_error::null_pointer, "Invalid argument");

    status status::type_conversion_failure = 
            status::make_app_status(false, app_error::null_pointer, "Type conversion failure");
   
    status status::not_found = 
            status::make_app_status(false, app_error::null_pointer, "Not found");

    status status::allocation_failure= 
            status::make_app_status(false, app_error::null_pointer, "Null pointer exception");
   
    status status::deallocation_failure= 
            status::make_app_status(false, app_error::null_pointer, "Allocation failure");
      
    status status::invalid_operation= 
            status::make_app_status(false, app_error::null_pointer, "Invalid operation");

    status status::not_implemented= 
            status::make_app_status(false, app_error::null_pointer, "Not implemented");
       
    status status::invalid_state= 
            status::make_app_status(false, app_error::null_pointer, "Invalid state");
            
    status status::end_of_input= 
            status::make_app_status(false, app_error::null_pointer, "End of input");

} // namespace ltd