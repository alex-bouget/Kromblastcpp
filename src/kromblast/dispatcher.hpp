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
        void dispatch(const std::string &channel, const Api::Signal &signal);

    public:
        Dispatcher();
        ~Dispatcher();

        void listen(const std::string &channel, Api::SignalHandlerInterface *handler) override;
        void listen(Api::SignalHandlerInterface *handler) override;

        void dispatch(const Api::Signal &signal) override;
        void dispatch(const std::string &channel, const std::string &message) override;
    };
}

#endif