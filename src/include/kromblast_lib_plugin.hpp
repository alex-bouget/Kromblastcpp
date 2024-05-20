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

        using kromlib_config_t = std::map<std::string, std::string, std::less<>>;

        /**
         * @brief Class of a kromblast library (interface)
         * @property get_version Get the version of the library
         * @property set_kromblast Set the kromblast instance
         * @property load_functions Load the functions of the library
         */
        class KromLib
        {
        private:
            ::Kromblast::Api::KromblastInterface *var_kromblast;
            const kromlib_config_t* var_config;
        protected:
            const ::Kromblast::Api::KromblastInterface &kromblast() {
                return *var_kromblast;
            }
            const kromlib_config_t &config() {
                return *var_config;
            }

        public:
            virtual std::string get_version() = 0;
            virtual void set_kromblast(::Kromblast::Api::KromblastInterface *kromblast, const kromlib_config_t &config) {
                this->var_kromblast = kromblast;
                this->var_config = &config;
                at_start();
            }
            virtual void load_functions() = 0;
            virtual void at_start() = 0;
        };

        /**
         * @brief define entry point of a kromblast library
         */
        typedef KromLib *(*kromblast_lib_get_class_t)(void);
    }
}

#endif