/* -*- Mode: C; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
   Copyright (C) 2009 Red Hat, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _H_CANVAS_UTILS
#define _H_CANVAS_UTILS

#include <spice/types.h>

#include "pixman_utils.h"
#include "lz.h"

typedef struct PixmanData {
#ifdef WIN32
    HBITMAP bitmap;
    HANDLE mutex;
#endif
    uint8_t *data;
} PixmanData;

#ifdef WIN32
pixman_image_t *surface_create(HDC dc, pixman_format_code_t format,
                               int width, int height, int top_down);
#else
pixman_image_t *surface_create(pixman_format_code_t format, int width, int height, int top_down);
#endif

#ifdef WIN32
pixman_image_t *surface_create_stride(HDC dc, pixman_format_code_t format, int width, int height,
                                      int stride);
#else
pixman_image_t *surface_create_stride(pixman_format_code_t format, int width, int height,
                                      int stride);
#endif


typedef struct LzDecodeUsrData {
#ifdef WIN32
    HDC dc;
#endif
    pixman_image_t       *out_surface;
} LzDecodeUsrData;


pixman_image_t *alloc_lz_image_surface(LzDecodeUsrData *canvas_data, LzImageType type, int width,
                                       int height, int gross_pixels, int top_down);
#endif