#include "plugin.hpp"

namespace Kromblast
{
    Plugin::Plugin(Api::KromblastInterface *kromblast)
    {
        this->kromblast = kromblast;
    }
    Plugin::~Plugin()
    {
    }
    bool Plugin::claim_callback(const std::string &callback_name)
    {
    }
    void Plugin::start(std::vector<std::string> plugins)
    {
        for (std::string plugin : plugins)
        {
            Class::kromblast_lib_get_class_t *callback library_loader.get_lib(plugin, "kromblast_lib_get_class", kromblast->get_logger());
            if (callback == nullptr)
            {
                kromblast->get_logger()->log("Plugin", "Cannot load plugin: " + plugin);
                continue;
            }
            Class::KromLib *lib = callback();
            if (lib == nullptr)
            {
                kromblast->get_logger()->log("Plugin", "Cannot callback the plugin: " + plugin);
                continue;
            }
            lib->set_kromblast(kromblast);
            lib->load_functions();
        }
    }

    void Plugin::stop()
    {
    }
}