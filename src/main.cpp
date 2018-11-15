/* quirc -- QR-code recognition library
 * Copyright (C) 2010-2012 Daniel Beer <dlbeer@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <quirc.h>
#include <setjmp.h>
#include <time.h>
#include "quirc_internal.h"
#include "opencv2/opencv.hpp"

using namespace cv;

#define IMG_WIDTH 640
#define IMG_HEIGHT 480

int main(void)
{
	VideoCapture frame(0);
	struct quirc *qr;
	qr = quirc_new();
	if (!qr) {
		perror("Failed to allocate memory");
		abort();
	}
	// qrÄÚ´æ³õÊ¼»¯
	qr->image = (uint8_t*)calloc(IMG_WIDTH, IMG_HEIGHT);
	if (!qr->image)
		goto fail;
	qr->pixels = (quirc_pixel_t*)calloc(IMG_WIDTH*IMG_HEIGHT, sizeof(quirc_pixel_t));
	if (!qr->pixels)
		goto fail;
	qr->row_average = (int*)calloc(IMG_WIDTH, sizeof(int));
	if (!qr->row_average)
		goto fail;

	while (1)
	{
		Mat src = imread("code.bmp");
		frame >> src;
		if (src.empty())
		{
			printf("image is empty");
			return 0;
		}
		Mat srcImg;
		resize(src, srcImg, Size(640, 480));
		Mat grayImg;
		cvtColor(srcImg, grayImg, COLOR_BGR2GRAY);
		memcpy(qr->image, grayImg.data, IMG_WIDTH*IMG_HEIGHT);

		qr->w = grayImg.size().width;
		qr->h = grayImg.size().height;
		qr->num_regions = QUIRC_PIXEL_REGION;
		qr->num_capstones = 0;
		qr->num_grids = 0;

		//if (quirc_resize(qr, 640, 480) < 0) {
		//	perror("Failed to allocate video memory");
		//	std::cout << "err";
		//	abort();
		//}
		//
		/* ... */
		//Mat debung(480, 640, CV_8UC1, qr->image);
		//imshow("debug", debung);

		/* Fill out the image buffer here.
		 * image is a pointer to a w*h bytes.
		 * One byte per pixel, w pixels per line, h lines in the buffer.
		 */

		quirc_end(qr);

		//debug info
		Mat debugImg;
		srcImg.copyTo(debugImg);
		if (qr->num_capstones >= 3)
		{
			Point2i upLeftCenter, upRightCenter, downLeftCenetr;
			upLeftCenter.x = qr->capstones[0].center.x;
			upLeftCenter.y = qr->capstones[0].center.y;
			upRightCenter.x = qr->capstones[1].center.x;
			upRightCenter.y = qr->capstones[1].center.y;
			downLeftCenetr.x = qr->capstones[2].center.x;
			downLeftCenetr.y = qr->capstones[2].center.y;

			circle(debugImg, upLeftCenter, 3, Scalar(0, 0, 255), 2);
			circle(debugImg, upRightCenter, 3, Scalar(0, 0, 255), 2);
			circle(debugImg, downLeftCenetr, 3, Scalar(0, 0, 255), 2);
		}
		imshow("debugImg", debugImg);
		int c = waitKey(30);
		if (c == 27 || c == 'q' || c == 'Q')
		{
			std::cout << "exit" << std::endl;
			break;
		}
	}

	quirc_destroy(qr);
	std::cout << "free memory" << std::endl;
	return 0;
fail:
	free(qr->image);
	free(qr->pixels);
	free(qr->row_average);
	std::cerr << "get memorry error" << std::endl;
	return -1;
}