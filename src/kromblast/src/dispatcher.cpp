#include "dispatcher.hpp"

namespace Kromblast
{
    Dispatcher::Dispatcher()
    {
    }

    Dispatcher::~Dispatcher()
    {
    }

    void Dispatcher::listen(std::string channel, Api::SignalHandlerInterface *handler)
    {
        if (this->listeners.find(channel) == this->listeners.end())
        {
            this->listeners[channel] = std::vector<Api::SignalHandlerInterface *>();
        }

        this->listeners[channel].push_back(handler);
    }

    void Dispatcher::listen(Api::SignalHandlerInterface *handler)
    {
        this->listen("", handler);
    }


    void Dispatcher::dispatch(std::string channel, Api::Signal signal)
    {
        if (this->listeners.find(channel) == this->listeners.end())
        {
            return;
        }

        for (auto handler : this->listeners[channel])
        {
            handler->handle(signal);
        }
    }

    void Dispatcher::dispatch(Api::Signal signal)
    {
        dispatch(signal.channel, signal);
        dispatch("", signal);
    }

    void Dispatcher::dispatch(std::string channel, std::string message)
    {
        Api::Signal signal;
        signal.channel = channel;
        signal.message = message;

        this->dispatch(signal);
    }
}