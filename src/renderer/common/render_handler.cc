// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/common/render_handler.h"
#include "glog/logging.h"
#include <png.h>
#include <mutex>
#include "include/cef_app.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"

#include "base/util.h"


int x, y;

int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;
bool finishedLoading_=false;
std::mutex finishedLoading_mutex;
//std::mutex painting_mutex;


namespace common {


RenderHandler::RenderHandler(int width, int height)
: width_(width), height_(height) {
  // empty
}

void RenderHandler::setLoading(bool bFinished) {
  //REQUIRE_UI_THREAD();
	LOG(INFO) << "RenderHandler::setLoading: " << bFinished;
	std::lock_guard<std::mutex> lock(finishedLoading_mutex);
  finishedLoading_ = bFinished;
}

bool RenderHandler::getLoading() {
  return finishedLoading_;
}

bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
  rect.Set(0, 0, width_, height_);
  return true;
}


void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                            PaintElementType type, const RectList &dirtyRects,
                            const void *buffer, int width, int height) {

  //std::lock_guard<std::mutex> lock(painting_mutex);
  LOG(INFO) << "RenderHandler::OnPaint: " << finishedLoading_;
	if (finishedLoading_ == true && !browser->IsLoading()) {
		char *file = (char *)"test.png";

		/* test image initialization */
		png_byte* pixBuffer = (png_byte*)buffer;
		png_uint_32 row;
	   	row_pointers = new png_bytep[height];
	    for (row = 0; row < (png_uint_32)height; ++row) {
	      row_pointers[row] = pixBuffer + row * width * 4;
	    }

		write_png_file(file);
    //setLoading(false);
    //CefPostTask(TID_UI, base::Bind(&RenderHandler::write_png_file, file));
		//exit(0);
    // CefQuitMessageLoop();
	}
}

void RenderHandler::write_png_file(char *filename) {
  REQUIRE_UI_THREAD();

  FILE *fp = fopen(filename, "wb");
  if(!fp) abort();
  // width = 1024;
  // height = 768;
	LOG(INFO) << "RenderHandler::write_png_file";
  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) abort();

  png_infop info = png_create_info_struct(png);
  if (!info) abort();

  if (setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);
  // Output is 8bit depth, RGBA format.
  png_set_IHDR(
    png,
    info,
    width_, height_,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  png_write_info(png, info);
  png_set_bgr(png);

  // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
  // Use png_set_filler().
  //png_set_filler(png, 0, PNG_FILLER_AFTER);

  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

  for(int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  fclose(fp);

  CefQuitMessageLoop();
}

}  // namespace common
