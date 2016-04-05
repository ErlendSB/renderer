// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/common/render_handler.h"
#include "glog/logging.h"
#include <png.h>

int x, y;

int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;


namespace common {


RenderHandler::RenderHandler(int width, int height)
: width_(width), height_(height) {
  // empty
}


bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
  rect.Set(0, 0, width_, height_);
  return true;
}


void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                            PaintElementType type, const RectList &dirtyRects,
                            const void *buffer, int width, int height) {
  // empty
	LOG(INFO) << "RenderHandler::OnPaint";
	//row_pointers = (png_bytep*)buffer;
	char *file = (char *)"test.png";
	// width = width;
	// height = height;

	LOG(INFO) << "Width:" << width;
	LOG(INFO) << "Height:" << height;
	/* test image initialization */
	png_byte* pixBuffer = (png_byte*)buffer;
	png_uint_32 row;
   	row_pointers = new png_bytep[height];
    for (row = 0; row < (png_uint_32)height; ++row) {
      row_pointers[row] = pixBuffer + row * width * 4;
    }

	//png_set_rows(png_ptr, info_ptr, (png_bytepp)row_pointers);

	write_png_file(file);
	//encodeOneStep(file,row_pointers,width,height);
}

void RenderHandler::write_png_file(char *filename) {

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

	LOG(INFO) << "Width2:" << width_;
	LOG(INFO) << "Height2:" << height_;
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
}

}  // namespace common
