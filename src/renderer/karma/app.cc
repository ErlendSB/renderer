// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/karma/app.h"

#include <string>
#include <sstream>

#include "glog/logging.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/cef_parser.h"

#include "base/util.h"
#include "renderer/karma/client.h"

DEFINE_string(url, "", "url to load");

namespace karma {

std::string App::GetDataURI(const std::string& data) {
  return "data:text/html;base64," +
      CefURIEncode(CefBase64Encode(data.data(), data.size()), false).ToString();
}

void App::OnContextInitialized() {
  REQUIRE_UI_THREAD();

  std::string url(FLAGS_url);
  if (url.empty()) {
    LOG(FATAL) << "the url flag cannot be empty";
  }

  CefBrowserSettings browser_settings;

  CefWindowInfo window_info;
  //window_info.windowless_rendering_enabled = true;
  window_info.SetAsWindowless(0L,false);
  std::stringstream ss;
  ss << "<html><head><title>Hei på deg</title></head>"
        "<body bgcolor=\"white\">"
        "<span style='background-color:wheat;padding:30px;height:100px;width:100px;font-family:Courier New;font-size:40pt;'>Jadda</span></body></html>";

  url = GetDataURI(ss.str());
  LOG(INFO) << "opening: " << url;
  CefRefPtr<Client> client(new Client());
  CefBrowserHost::CreateBrowser(window_info, client.get(), url.c_str(),
      browser_settings, NULL);
}

}  // namespace karma
