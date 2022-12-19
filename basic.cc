#include "kromblast.h"
#include "webview.h"
#include <iostream>

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
  w.set_html(html);
  w.run();
  return 0;
}