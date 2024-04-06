#include "dispatcher.hpp"

namespace Kromblast
{
    Dispatcher::Dispatcher()
    {
    }

    Dispatcher::~Dispatcher()
    {
    }

    void Dispatcher::listen(const std::string &channel, Api::SignalHandlerInterface *handler)
    {
        if (!this->listeners.contains(channel))
        {
            this->listeners[channel] = std::vector<Api::SignalHandlerInterface *>();
        }

        this->listeners[channel].push_back(handler);
    }

    void Dispatcher::listen(Api::SignalHandlerInterface *handler)
    {
        this->listen("", handler);
    }

    void Dispatcher::dispatch(const std::string &channel, const Api::Signal &signal)
    {
        if (!this->listeners.contains(channel))
        {
            return;
        }

        for (auto handler : this->listeners[channel])
        {
            handler->handle(signal);
        }
    }

    void Dispatcher::dispatch(const Api::Signal &signal)
    {
        dispatch(signal.channel, signal);
        dispatch("", signal);
    }

    void Dispatcher::dispatch(const std::string &channel, const std::string &message)
    {
        Api::Signal signal = {
            channel,
            message,
        };

        this->dispatch(signal);
    }
}