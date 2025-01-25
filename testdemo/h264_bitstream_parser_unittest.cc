/*
 *  Copyright (c) 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "common_video/h264/h264_bitstream_parser.h"

#include <stdio.h>

using namespace webrtc;

#define EXPECT_EQ(a, b)         do { if ((a) != (b)) { printf("error: expert eq\n"); } } while (false)
#define EXPECT_FALSE(a, b)      do { if ((a) == (b)) { printf("error: expert false\n"); } } while (false)

// SPS/PPS part of below chunk.
uint8_t kH264SpsPps[] = {0x00, 0x00, 0x00, 0x01, 0x67, 0x42, 0x80, 0x20, 0xda,
                         0x01, 0x40, 0x16, 0xe8, 0x06, 0xd0, 0xa1, 0x35, 0x00,
                         0x00, 0x00, 0x01, 0x68, 0xce, 0x06, 0xe2};

// Contains enough of the image slice to contain slice QP.
uint8_t kH264BitstreamChunk[] = {
    0x00, 0x00, 0x00, 0x01, 0x67, 0x42, 0x80, 0x20, 0xda, 0x01, 0x40, 0x16,
    0xe8, 0x06, 0xd0, 0xa1, 0x35, 0x00, 0x00, 0x00, 0x01, 0x68, 0xce, 0x06,
    0xe2, 0x00, 0x00, 0x00, 0x01, 0x65, 0xb8, 0x40, 0xf0, 0x8c, 0x03, 0xf2,
    0x75, 0x67, 0xad, 0x41, 0x64, 0x24, 0x0e, 0xa0, 0xb2, 0x12, 0x1e, 0xf8,
};

uint8_t kH264BitstreamChunkCabac[] = {
    0x00, 0x00, 0x00, 0x01, 0x27, 0x64, 0x00, 0x0d, 0xac, 0x52, 0x30,
    0x50, 0x7e, 0xc0, 0x5a, 0x81, 0x01, 0x01, 0x18, 0x56, 0xbd, 0xef,
    0x80, 0x80, 0x00, 0x00, 0x00, 0x01, 0x28, 0xfe, 0x09, 0x8b,
};

// Contains enough of the image slice to contain slice QP.
uint8_t kH264BitstreamNextImageSliceChunk[] = {
    0x00, 0x00, 0x00, 0x01, 0x41, 0xe2, 0x01, 0x16, 0x0e, 0x3e, 0x2b, 0x86,
};

// Contains enough of the image slice to contain slice QP.
uint8_t kH264BitstreamNextImageSliceChunkCabac[] = {
    0x00, 0x00, 0x00, 0x01, 0x21, 0xe1, 0x05, 0x11, 0x3f, 0x9a, 0xae, 0x46,
    0x70, 0xbf, 0xc1, 0x4a, 0x16, 0x8f, 0x51, 0xf4, 0xca, 0xfb, 0xa3, 0x65,
};


int main(void)
{
    {
        H264BitstreamParser h264_parser;
        printf("ret=%d\n", h264_parser.GetLastSliceQp().has_value());
    }

    {
        H264BitstreamParser h264_parser;
        h264_parser.ParseBitstream(kH264SpsPps);
        printf("ret=%d\n", h264_parser.GetLastSliceQp().has_value());
    }

    {
        H264BitstreamParser h264_parser;
        h264_parser.ParseBitstream(kH264BitstreamChunk);
        absl::optional<int> qp = h264_parser.GetLastSliceQp();
        printf("ret=%d\n", qp.has_value());
        // EXPECT_EQ(35, *qp);
        printf("qp=%d\n", *qp);


        // Parse an additional image slice.
        h264_parser.ParseBitstream(kH264BitstreamNextImageSliceChunk);
        qp = h264_parser.GetLastSliceQp();
        printf("ret=%d\n", qp.has_value());
        // EXPECT_EQ(37, *qp);
        printf("qp=%d\n", *qp);
    }

    {
        H264BitstreamParser h264_parser;
        h264_parser.ParseBitstream(kH264BitstreamChunkCabac);
        printf("ret=%d\n", h264_parser.GetLastSliceQp().has_value());

        // Parse an additional image slice.
        h264_parser.ParseBitstream(kH264BitstreamNextImageSliceChunkCabac);
        absl::optional<int> qp = h264_parser.GetLastSliceQp();
        printf("ret=%d\n", qp.has_value());
        // EXPECT_EQ(24, *qp);
        printf("qp=%d\n", *qp);
    }

    return 0;
}


