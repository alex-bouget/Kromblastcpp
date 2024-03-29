#ifndef KROMBLAST_LISTENER_HPP
#define KROMBLAST_LISTENER_HPP

#include <string>
#include <functional>
#include <vector>
#include <map>
#include "kromblast_api_dispatcher.hpp"

namespace Kromblast
{
    class Dispatcher : public Api::DispatcherInterface
    {
    private:
        std::map<std::string, std::vector<Api::SignalHandlerInterface *>> listeners;

    public:
        Dispatcher();
        ~Dispatcher();

        void listen(std::string channel, Api::SignalHandlerInterface *handler);

        void dispatch(Api::Signal signal);
        void dispatch(std::string channel, std::string message);
    };
}

#endif