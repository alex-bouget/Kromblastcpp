#include "signal.hpp"

namespace Kromblast
{
    Dispatcher::Dispatcher()
    {
    }

    Dispatcher::~Dispatcher()
    {
    }

    void Dispatcher::listen(std::string channel, Kromblast::Api::SignalHandler *handler)
    {
        if (this->listeners.find(channel) == this->listeners.end())
        {
            this->listeners[channel] = std::vector<Kromblast::Api::SignalHandler *>();
        }

        this->listeners[channel].push_back(handler);
    }

    void Dispatcher::dispatch(Kromblast::Api::Signal signal)
    {
        if (this->listeners.find(signal.channel) == this->listeners.end())
        {
            return;
        }

        for (auto handler : this->listeners[signal.channel])
        {
            handler->handle(signal);
        }
    }

    void Dispatcher::dispatch(std::string channel, std::string message)
    {
        Kromblast::Api::Signal signal;
        signal.channel = channel;
        signal.message = message;

        this->dispatch(signal);
    }
}