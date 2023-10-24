#ifndef KRBLST_LIB_CALLBACK_H
#define KRBLST_LIB_CALLBACK_H

#include <string>
#include <vector>
#include <functional>

/**
 * @brief Namespace of the kromblast library
 * @struct kromblast_callback_called Structure used to call a function
 * @typedef kromblast_callback_t Function type for a callback
 * @struct kromblast_callback Structure for simulating a function
 */
namespace Kromblast
{
    namespace Core
    {
        typedef struct kromblast_callback_called kromblast_callback_called_t;

        typedef std::function<std::string(kromblast_callback_called_t *)> kromblast_callback_f;

        typedef struct kromblast_callback kromblast_callback_t;

        /**
         * @brief Structure used to call a function
         * @property name Function name
         * @property args Arguments
         */
        struct kromblast_callback_called
        {
            std::string name;
            std::vector<std::string> args;
        };

        /**
         * @brief Structure for simulating a function
         * @property name Function name
         * @property args_nb Arguments number
         * @property callback Callback of the function
         */
        struct kromblast_callback
        {
            std::string name;
            int args_nb;
            kromblast_callback_f callback;
        };
    }
}
#endif