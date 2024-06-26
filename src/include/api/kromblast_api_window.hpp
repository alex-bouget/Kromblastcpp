#ifndef KRBLST_API_WINDOW_H
#define KRBLST_API_WINDOW_H

#include <string>

namespace Kromblast
{
    namespace Api
    {
        class WindowInterface
        {
        public:
            virtual ~WindowInterface(){};
            virtual void set_fullscreen(bool fullscreen) = 0;
            virtual void set_size(int width, int height) = 0;
            virtual void set_frameless(bool frameless) = 0;
            virtual void set_html(const std::string html) = 0;
            virtual void navigate(const std::string url) = 0;
            virtual void run() = 0;
            virtual void init_inject(const std::string js) = 0;
            virtual void inject(const std::string js) = 0;

            /**
             * @attention This method is not implemented yet for windows and macos
             */
            virtual std::string get_current_url() = 0;

            /**
             * @attention This method is not implemented yet for windows and macos
             */
            virtual std::string get_title() = 0;
        };
    }
}

#endif