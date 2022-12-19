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
      window.libtest.increment().then(result => {
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
    std::string cwd =  std::string(getcwd(NULL, 0));
    std::string lib_name[] = {std::string(cwd) + "/build/myclass.so"};
    
    webview::webview w = create_kromblast_window("Kromblast", 800, 600, true, lib_name, 1);
    std::cout << cwd << std::endl;
    w.set_html(html);
    w.run();
    return 0;
}