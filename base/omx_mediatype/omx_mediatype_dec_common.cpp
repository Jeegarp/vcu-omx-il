/******************************************************************************
*
* Copyright (C) 2018 Allegro DVT2.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX OR ALLEGRO DVT2 BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of  Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
*
* Except as contained in this notice, the name of Allegro DVT2 shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Allegro DVT2.
*
******************************************************************************/

#include "omx_mediatype_dec_common.h"
#include "omx_mediatype_checks.h"
#include "omx_convert_module_soft.h"
#include "omx_convert_module_soft_dec.h"
#include "base/omx_utils/round.h"

extern "C"
{
#include <lib_common_enc/EncBuffers.h>
}

using namespace std;

Clock CreateClock(AL_TDecSettings settings)
{
  Clock clock;

  clock.framerate = settings.uFrameRate / 1000;
  clock.clockratio = settings.uClkRatio;

  return clock;
}

bool UpdateClock(AL_TDecSettings& settings, Clock clock)
{
  if(!CheckClock(clock))
    return false;

  settings.uFrameRate = clock.framerate * 1000;
  settings.uClkRatio = clock.clockratio;
  settings.bForceFrameRate = settings.uFrameRate && settings.uClkRatio;

  return true;
}

int CreateInternalEntropyBuffer(AL_TDecSettings settings)
{
  return settings.iStackSize;
}

bool UpdateInternalEntropyBuffer(AL_TDecSettings& settings, int internalEntropyBuffer)
{
  if(!CheckInternalEntropyBuffer(internalEntropyBuffer))
    return false;

  settings.iStackSize = internalEntropyBuffer;

  return true;
}

SequencePictureModeType CreateSequenceMode(AL_TDecSettings settings)
{
  auto stream = settings.tStream;
  return ConvertSoftToModuleSequenceMode(stream.eSequenceMode);
}

bool UpdateSequenceMode(AL_TDecSettings& settings, SequencePictureModeType sequenceMode, vector<SequencePictureModeType> sequenceModes)
{
  if(!CheckSequenceMode(sequenceMode, sequenceModes))
    return false;

  auto& stream = settings.tStream;
  stream.eSequenceMode = ConvertModuleToSoftSequenceMode(sequenceMode);
  return true;
}

Format CreateFormat(AL_TDecSettings settings)
{
  Format format;
  auto stream = settings.tStream;

  format.color = ConvertSoftToModuleColor(stream.eChroma);
  format.bitdepth = stream.iBitDepth;

  return format;
}

bool UpdateFormat(AL_TDecSettings& settings, Format format, vector<ColorType> colors, vector<int> bitdepths, int& stride, Stride strideAlignment)
{
  if(!CheckFormat(format, colors, bitdepths))
    return false;

  auto& stream = settings.tStream;
  stream.eChroma = ConvertModuleToSoftChroma(format.color);
  stream.iBitDepth = format.bitdepth;

  auto minStride = (int)RoundUp(AL_Decoder_GetMinPitch(stream.tDim.iWidth, stream.iBitDepth, settings.eFBStorageMode), strideAlignment.widthStride);
  stride = max(minStride, stride);

  return true;
}

Resolution CreateResolution(AL_TDecSettings settings, int widthStride, int heightStride)
{
  auto streamSettings = settings.tStream;
  Resolution resolution;
  resolution.width = streamSettings.tDim.iWidth;
  resolution.height = streamSettings.tDim.iHeight;
  resolution.stride.widthStride = widthStride;
  resolution.stride.heightStride = heightStride;

  return resolution;
}

DecodedPictureBufferType CreateDecodedPictureBuffer(AL_TDecSettings settings)
{
  return ConvertSoftToModuleDecodedPictureBuffer(settings.eDpbMode);
}

bool UpdateIsEnabledSubFrame(AL_TDecSettings& settings, bool isEnabledSubFrame)
{
  settings.eDecUnit = isEnabledSubFrame ? ConvertModuleToSoftDecodeUnit(DecodeUnitType::DECODE_UNIT_SLICE) : ConvertModuleToSoftDecodeUnit(DecodeUnitType::DECODE_UNIT_FRAME);
  return true;
}

bool UpdateDecodedPictureBuffer(AL_TDecSettings& settings, DecodedPictureBufferType decodedPictureBuffer)
{
  if(decodedPictureBuffer == DecodedPictureBufferType::DECODED_PICTURE_BUFFER_MAX_ENUM)
    return false;

  settings.bLowLat = decodedPictureBuffer == DecodedPictureBufferType::DECODED_PICTURE_BUFFER_LOW_REFERENCE;
  settings.eDpbMode = ConvertModuleToSoftDecodedPictureBuffer(decodedPictureBuffer);
  return true;
}

bool UpdateResolution(AL_TDecSettings& settings, int& stride, int& sliceHeight, Stride strideAlignment, Resolution resolution)
{
  if((resolution.width % 2) != 0)
    return false;

  if((resolution.height % 2) != 0)
    return false;

  auto& streamSettings = settings.tStream;
  streamSettings.tDim = { resolution.width, resolution.height };

  auto minStride = (int)RoundUp(AL_Decoder_GetMinPitch(streamSettings.tDim.iWidth, streamSettings.iBitDepth, settings.eFBStorageMode), strideAlignment.widthStride);
  stride = max(minStride, (int)RoundUp(resolution.stride.widthStride, strideAlignment.widthStride));

  auto minSliceHeight = (int)RoundUp(AL_Decoder_GetMinStrideHeight(streamSettings.tDim.iHeight), strideAlignment.heightStride);
  sliceHeight = max(minSliceHeight, (int)RoundUp(resolution.stride.heightStride, strideAlignment.heightStride));

  return true;
}

