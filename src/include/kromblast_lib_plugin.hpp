#ifndef KB_LIB_CLASS_H
#define KB_LIB_CLASS_H

#include <string>
#include "kromblast_api.hpp"
#include <map>

/**
 * @brief Namespace of the kromblast library
 * @class KromLib Class of a kromblast library (interface)
 */
namespace Kromblast
{

    namespace Class
    {

        typedef std::map<std::string, std::string> kromlib_config_t;

        /**
         * @brief Class of a kromblast library (interface)
         * @property get_version Get the version of the library
         * @property set_kromblast Set the kromblast instance
         * @property load_functions Load the functions of the library
         */
        class KromLib
        {
        protected:
            Kromblast::Api::KromblastInterface *kromblast;
            const kromlib_config_t* config;

        public:
            virtual std::string get_version() = 0;
            virtual void set_kromblast(::Kromblast::Api::KromblastInterface *kromblast, const kromlib_config_t &config) {
                this->kromblast = kromblast;
                this->config = &config;
                atStart();
            }
            virtual void load_functions() = 0;
            virtual void atStart() = 0;
        };

        /**
         * @brief define entry point of a kromblast library
         */
        typedef KromLib *(*kromblast_lib_get_class_t)(void);
    }
}

#endif