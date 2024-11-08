#ifndef _LTD_INCLUDE_ERR_HPP_
#define _LTD_INCLUDE_ERR_HPP_

enum class err
{
    no_error, 
    app_unknown,            null_pointer,               index_out_of_bound,
    invalid_argument,       type_conversion_failure,    not_found,
    allocation_failure,     deallocation_failure,       invalid_operation,
    not_implemented,        invalid_state,              end_of_input
};

#endif // _LTD_INCLUDE_ERR_HPP_