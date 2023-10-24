#ifndef KROMBLAST_LISTENER_HPP
#define KROMBLAST_LISTENER_HPP

#include <string>
#include <functional>
#include <vector>
#include <map>
#include "kromblast_api_signal.hpp"

namespace Kromblast
{
    class Dispatcher : public Kromblast::Api::Dispatcher
    {
    private:
        std::map<std::string, std::vector<Kromblast::Api::SignalHandler *>> listeners;

    public:
        Dispatcher();
        ~Dispatcher();

        void listen(std::string channel, Kromblast::Api::SignalHandler *handler);

        void dispatch(Kromblast::Api::Signal signal);
        void dispatch(std::string channel, std::string message);
    };
}

#endif