#ifndef KRBLST_API_PLUGIN_H
#define KRBLST_API_PLUGIN_H

#include <string>
#include "kromblast_api.hpp"
#include "vector"

namespace Kromblast
{
    namespace Api
    {
        class PluginInterface
        {
            public:
                virtual ~PluginInterface(){};
                virtual bool claim_callback(const std::string& callback_name) = 0;
                virtual void start(std::vector<std::string> plugins) = 0;
                virtual void stop() = 0;
        };
    }
}
#endif