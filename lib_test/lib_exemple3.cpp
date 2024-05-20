#include "kromblast_api.hpp"
#include "kromblast_lib_plugin.hpp"
#include "kromblast_lib_plugin_callback.hpp"
#include <functional>

class Exemple : public Kromblast::Class::KromLib
{
public:
    std::string get_version() {
        return "0.1.0";
    }

    void atStart() {
        kromblast->get_logger()->log("Exemple", "Start");
        if (config->contains("joke")) {
            kromblast->get_logger()->log("Exemple", "Joke: " + config->at("joke"));
        }
    }

    std::string increment(Kromblast::Core::kromblast_callback_called_t *parameters) {
        kromblast->get_logger()->log("Exemple", "Increment function called");
        kromblast->get_logger()->log("Exemple", "Parameters: " + parameters->args.at(0));
        int nb = std::stoi(parameters->args.at(0));
        std::string result_str = "{\"count\": " + std::to_string(++nb) + "}";
        return result_str;
    }

    void load_functions() {
        kromblast->get_logger()->log("Exemple", "Load functions");
        Kromblast::Core::kromblast_callback_t callback = {
            "libtest.secondexemple.increment",
            1,
            std::bind(&Exemple::increment, this, std::placeholders::_1)
        };
        kromblast->get_plugin()->claim_callback(callback);

    }
};

extern "C" Kromblast::Class::KromLib *kromblast_lib_get_class()
{
    return (new Exemple());
}