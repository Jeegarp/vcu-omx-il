/******************************************************************************
*
* Copyright (C) 2017 Allegro DVT2.  All rights reserved.
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

#include "omx_settings_dec_hevc.h"
#include "omx_settings_dec_common.h"
#include "base/omx_mediatype/omx_mediatype_common_hevc.h"
#include "base/omx_mediatype/omx_mediatype_common.h"

#include <cstring>

using namespace std;

DecSettingsHEVC::DecSettingsHEVC()
{
  Reset();
}

DecSettingsHEVC::~DecSettingsHEVC()
{
}

static void resetStreamSettings(AL_TStreamSettings& stream, bool& highTier)
{
  stream.tDim = { 176, 144 };
  stream.eChroma = CHROMA_4_2_0;
  stream.iBitDepth = 8;
  stream.iLevel = 10;
  stream.iProfileIdc = AL_PROFILE_HEVC_MAIN;
  highTier = false;
}

ErrorSettingsType DecSettingsHEVC::Reset()
{
  alignments.stride = 64;
  alignments.sliceHeight = 64;

  bufferHandles.input = BufferHandleType::BUFFER_HANDLE_CHAR_PTR;
  bufferHandles.output = BufferHandleType::BUFFER_HANDLE_CHAR_PTR;
  bufferBytesAlignments.input = 0;
  bufferBytesAlignments.output = 0;
  bufferContiguities.input = false;
  bufferContiguities.output = false;

  memset(&settings, 0, sizeof(settings));
  settings.iStackSize = 5;
  settings.uFrameRate = 15000;
  settings.uClkRatio = 1000;
  settings.uDDRWidth = 32;
  settings.eDecUnit = AL_AU_UNIT;
  settings.eDpbMode = AL_DPB_NORMAL;
  settings.eFBStorageMode = AL_FB_RASTER;
  settings.eCodec = AL_CODEC_HEVC;

  resetStreamSettings(settings.tStream, isHighTier);
  return ERROR_SETTINGS_NONE;
}

ErrorSettingsType DecSettingsHEVC::Set(string index, void const* settings)
{
  if(!settings)
    return ERROR_SETTINGS_BAD_PARAMETER;

  if(index == SETTINGS_INDEX_RESOLUTION)
  {
    if(!UpdateResolution(this->settings, this->alignments, *(static_cast<Resolution const*>(settings))))
      return ERROR_SETTINGS_BAD_PARAMETER;
    return ERROR_SETTINGS_NONE;
  }

  if(index == SETTINGS_INDEX_FORMAT)
  {
    if(!UpdateFormat(this->settings, *(static_cast<Format const*>(settings))))
      return ERROR_SETTINGS_BAD_PARAMETER;
    return ERROR_SETTINGS_NONE;
  }

  if(index == SETTINGS_INDEX_PROFILE_LEVEL)
    return ERROR_SETTINGS_NOT_IMPLEMENTED;

  return ERROR_SETTINGS_BAD_INDEX;
}

ErrorSettingsType DecSettingsHEVC::Get(string index, void* settings)
{
  if(!settings)
    return ERROR_SETTINGS_BAD_PARAMETER;

  if(index == SETTINGS_INDEX_RESOLUTION)
  {
    *(static_cast<Resolution*>(settings)) = CreateResolution(this->settings, this->alignments);
    return ERROR_SETTINGS_NONE;
  }

  if(index == SETTINGS_INDEX_FORMAT)
  {
    *(static_cast<Format*>(settings)) = CreateFormat(this->settings);
    return ERROR_SETTINGS_NONE;
  }

  if(index == SETTINGS_INDEX_FORMATS_SUPPORTED)
  {
    *(static_cast<vector<Format>*>(settings)) = CreateFormatsSupported(this->colors, this->bitdepths);
    return ERROR_SETTINGS_NONE;
  }

  if(index == SETTINGS_INDEX_PROFILE_LEVEL)
  {
    *(static_cast<ProfileLevelType*>(settings)) = this->isHighTier ? CreateHEVCHighTierProfileLevel(static_cast<AL_EProfile>(this->settings.tStream.iProfileIdc), this->settings.tStream.iLevel) : CreateHEVCMainTierProfileLevel(static_cast<AL_EProfile>(this->settings.tStream.iProfileIdc), this->settings.tStream.iLevel);
    return ERROR_SETTINGS_NONE;
  }

  if(index == SETTINGS_INDEX_BUFFER_MODE)
  {
    *(static_cast<BufferModeType*>(settings)) = CreateBufferMode(this->settings);
    return ERROR_SETTINGS_NONE;
  }

  if(index == SETTINGS_INDEX_BUFFER_HANDLES)
  {
    *(static_cast<BufferHandles*>(settings)) = this->bufferHandles;
    return ERROR_SETTINGS_NONE;
  }

  if(index == SETTINGS_INDEX_BUFFER_SIZES)
  {
    *(static_cast<BufferSizes*>(settings)) = CreateBufferSizes(this->settings, this->alignments);
    return ERROR_SETTINGS_NONE;
  }

  if(index == SETTINGS_INDEX_BUFFER_BYTES_ALIGNMENTS)
  {
    *(static_cast<BufferBytesAlignments*>(settings)) = this->bufferBytesAlignments;
    return ERROR_SETTINGS_NONE;
  }

  if(index == SETTINGS_INDEX_BUFFER_CONTIGUITIES)
  {
    *(static_cast<BufferContiguities*>(settings)) = this->bufferContiguities;
    return ERROR_SETTINGS_NONE;
  }

  return ERROR_SETTINGS_BAD_INDEX;
}

