#include "webview.h"

int main() {
  webview::webview w(false, nullptr);
  w.set_title("Basic Example");
  w.set_size(480, 320, WEBVIEW_HINT_NONE);
  w.run();
  return 0;
}
