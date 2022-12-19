#include "kromblast.h"
#include "webview.h"
#include <iostream>
#include <filesystem>

constexpr const auto html =
    R"html(<button id="increment">Tap me</button>
<div>You tapped <span id="count">0</span> time(s).</div>
<script>
  const [incrementElement, countElement] =
    document.querySelectorAll("#increment, #count");
  document.addEventListener("DOMContentLoaded", () => {
    incrementElement.addEventListener("click", () => {
      window.kromblast().then(result => {
        countElement.textContent = result.count;
      });
    });
  });
</script>)html";


#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInt, HINSTANCE hPrevInst, LPSTR lpCmdLine,
                   int nCmdShow) {
#else
int main() {
#endif
    webview::webview w = create_kromblast_window("Kromblast", 800, 600, true, 2);
    char* cwd = getcwd(NULL, 0);
    std::cout << cwd << std::endl;
    KromblastLib::KromLib* lib = get_lib(std::string(cwd) + "/build/myclass.so");
    w.set_html(html);
    w.run();
    return 0;
}