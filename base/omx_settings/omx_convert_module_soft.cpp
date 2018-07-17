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

#include "omx_convert_module_soft.h"

AL_EChromaMode ConvertModuleToSoftChroma(ColorType const& color)
{
  switch(color)
  {
  case ColorType::COLOR_400: return CHROMA_4_0_0;
  case ColorType::COLOR_420: return CHROMA_4_2_0;
  case ColorType::COLOR_422: return CHROMA_4_2_2;
  case ColorType::COLOR_444: return CHROMA_4_4_4;
  case ColorType::COLOR_MAX_ENUM: return CHROMA_MAX_ENUM;
  default: return CHROMA_MAX_ENUM;
  }

  return CHROMA_MAX_ENUM;
}

ColorType ConvertSoftToModuleColor(AL_EChromaMode const& chroma)
{
  switch(chroma)
  {
  case CHROMA_4_0_0: return ColorType::COLOR_400;
  case CHROMA_4_2_0: return ColorType::COLOR_420;
  case CHROMA_4_2_2: return ColorType::COLOR_422;
  case CHROMA_4_4_4: return ColorType::COLOR_444;
  case CHROMA_MAX_ENUM: return ColorType::COLOR_MAX_ENUM;
  default: return ColorType::COLOR_MAX_ENUM;
  }

  return ColorType::COLOR_MAX_ENUM;
}

AL_EEntropyMode ConvertModuleToSoftEntropyCoding(EntropyCodingType const& entropy)
{
  switch(entropy)
  {
  case EntropyCodingType::ENTROPY_CODING_CABAC: return AL_MODE_CABAC;
  case EntropyCodingType::ENTROPY_CODING_CAVLC: return AL_MODE_CAVLC;
  case EntropyCodingType::ENTROPY_CODING_MAX_ENUM: return AL_MODE_MAX_ENUM;
  default: return AL_MODE_MAX_ENUM;
  }

  return AL_MODE_MAX_ENUM;
}

EntropyCodingType ConvertSoftToModuleEntropyCoding(AL_EEntropyMode const& entropy)
{
  switch(entropy)
  {
  case AL_MODE_CABAC: return EntropyCodingType::ENTROPY_CODING_CABAC;
  case AL_MODE_CAVLC: return EntropyCodingType::ENTROPY_CODING_CAVLC;
  case AL_MODE_MAX_ENUM: return EntropyCodingType::ENTROPY_CODING_MAX_ENUM;
  default: return EntropyCodingType::ENTROPY_CODING_MAX_ENUM;
  }

  return EntropyCodingType::ENTROPY_CODING_MAX_ENUM;
}

