#include "kb_lib_class.hpp"
#include "kb_lib_core.hpp"
#include "kb_lib_kromblast.hpp"
#include <functional>

class Exemple : public KromblastCore::Class::KromLib
{
public:
    KromblastCore::KromblastInterface *kromblast;

    std::string get_version() {
        return "0.1.0";
    }

    void set_kromblast(void *kromblast) {
        this->kromblast = (KromblastCore::KromblastInterface *)kromblast;
    }

    std::string increment(struct KromblastCore::kromblast_callback_called *parameters) {
        kromblast->log("Exemple", "Increment function called");
        kromblast->log("Exemple", "Parameters: " + parameters->args.at(0));
        int nb = std::stoi(parameters->args.at(0));
        std::string result_str = "{\"count\": " + std::to_string(++nb) + "}";
        return result_str;
    }

    void load_functions() {
        kromblast->log("Exemple", "Load functions");
        struct KromblastCore::kromblast_callback callback = {
            "libtest.secondexemple.increment",
            1,
            std::bind(&Exemple::increment, this, std::placeholders::_1)
        };
        kromblast->claim_callback(&callback);

    }
};

extern "C" KromblastCore::Class::KromLib *kromblast_lib_get_class()
{
    return (new Exemple());
}