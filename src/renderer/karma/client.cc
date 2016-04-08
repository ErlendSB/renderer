// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/karma/client.h"

#include "glog/logging.h"
#include "include/cef_app.h"
#include "include/cef_render_handler.h"

#include "base/util.h"

namespace karma {

Client::Client() {
  render_handler_ = new common::RenderHandler(600, 800);
}

void Client::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                   bool isLoading, bool canGoBack,
                                   bool canGoForward) {
  REQUIRE_UI_THREAD();

  if (!isLoading) {
    browser->GetMainFrame()->ExecuteJavaScript("document.body.style.overflow='hidden';","",0);
    render_handler_->setLoading(true);
    //usleep(300000);
    LOG(INFO) << "OnLoadingStateChange" <<
        browser->GetMainFrame()->GetURL().ToString();

    //browser->GetHost()->Invalidate(CefRenderHandler::PaintElementType::PET_VIEW);

  }
}

void Client::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame, int httpStatusCode){

  REQUIRE_UI_THREAD();
  LOG(INFO) << "OnLoadEnd";
  // render_handler_->setLoading(true);
  // usleep(500000);
  // browser->GetHost()->Invalidate(CefRenderHandler::PaintElementType::PET_VIEW);
}

void Client::OnLoadError(CefRefPtr<CefBrowser> browser,
                          CefRefPtr<CefFrame> frame,
                          ErrorCode errorCode, const CefString& errorText,
                          const CefString& failedUrl) {
  REQUIRE_UI_THREAD();

  if (errorCode == ERR_CONNECTION_REFUSED) {
    LOG(FATAL) << "cannot connect to " << failedUrl.ToString();
  }

  LOG(FATAL) << "error loading (" << errorCode << "): " <<
      errorText.ToString();
}

}  // namespace karma
