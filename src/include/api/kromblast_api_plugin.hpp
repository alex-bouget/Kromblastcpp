#ifndef KRBLST_API_PLUGIN_H
#define KRBLST_API_PLUGIN_H

#include <string>
#include "kromblast_api.hpp"
#include "kromblast_lib_plugin_callback.hpp"
#include "vector"

namespace Kromblast
{
    namespace Api
    {
        class PluginInterface
        {
        public:
            virtual ~PluginInterface(){};
            virtual bool claim_callback(Kromblast::Core::kromblast_callback_t callback) = 0;
            virtual std::string call_function(Kromblast::Core::kromblast_callback_called_t *function_called) = 0;
            virtual void start(std::vector<std::string> plugins) = 0;
            virtual void stop() = 0;
        };
    }
}
#endif