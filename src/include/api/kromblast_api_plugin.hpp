#ifndef KRBLST_API_PLUGIN_H
#define KRBLST_API_PLUGIN_H

#include <string>
#include "kromblast_api.hpp"
#include "kromblast_lib_plugin_callback.hpp"
#include "kromblast_lib_config.hpp"
#include "vector"

namespace Kromblast
{
    namespace Api
    {
        class PluginInterface
        {
        public:
            virtual ~PluginInterface(){};
            virtual bool claim_callback(Core::kromblast_callback_t callback) = 0;
            virtual bool claim_callback(const std::string &name, int nb_args, const Core::kromblast_callback_f &callback) = 0;
            virtual std::string call_function(Core::kromblast_callback_called_t *function_called) = 0;
            virtual void start(const std::vector<Core::ConfigKromblastPlugin> &plugins) = 0;
            virtual void stop() = 0;
        };
    }
}
#endif