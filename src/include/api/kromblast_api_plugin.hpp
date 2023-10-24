#ifndef KRBLST_API_PLUGIN_H
#define KRBLST_API_PLUGIN_H

#include <string>

namespace Kromblast
{
    namespace Api
    {
        class PluginInterface
        {
            public:
                virtual ~PluginInterface(){};
                virtual bool claim_callback(const std::string& callback_name) = 0;
        };
    }
}
#endif