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

#include "omx_expertise_enc_avc.h"
#include "omx_convert_omx_module.h"

static bool SetModuleGop(OMX_U32 bFrames, OMX_U32 pFrames, EncModule& module)
{
  auto moduleGop = module.GetGop();
  moduleGop.b = ConvertOMXToModuleBFrames(bFrames, pFrames);
  moduleGop.length = ConvertOMXToModuleGopLength(bFrames, pFrames);
  return module.SetGop(moduleGop);
}

static bool SetModuleProfileLevel(OMX_VIDEO_AVCPROFILETYPE const& profile, OMX_VIDEO_AVCLEVELTYPE const& level, EncModule& module)
{
  ProfileLevelType p;
  p.profile.avc = ConvertOMXToModuleAVCProfileLevel(profile, level).profile.avc;
  p.level = ConvertOMXToModuleAVCProfileLevel(profile, level).level;
  return module.SetProfileLevel(p);
}

static bool SetModuleEntropyCoding(OMX_BOOL entropyCoding, EncModule& module)
{
  return module.SetEntropyCoding(ConvertOMXToModuleEntropyCoding(entropyCoding));
}

static bool SetModuleConstrainedIntraPrediction(OMX_BOOL constrainedIntraPrediction, EncModule& module)
{
  return module.SetConstrainedIntraPrediction(ConvertOMXToModuleBool(constrainedIntraPrediction));
}

static bool SetModuleLoopFilter(OMX_VIDEO_AVCLOOPFILTERTYPE const& loopFilter, EncModule& module)
{
  return module.SetLoopFilter(ConvertOMXToModuleAVCLoopFilter(loopFilter));
}

bool EncExpertiseAVC::GetProfileLevelSupported(OMX_PTR param, EncModule const& module)
{
  auto supported = module.GetProfileLevelSupported();
  auto& pl = *(OMX_VIDEO_PARAM_PROFILELEVELTYPE*)param;

  if(pl.nProfileIndex >= supported.size())
    return false;

  pl.eProfile = ConvertModuleToOMXAVCProfile(supported[pl.nProfileIndex]);
  pl.eLevel = ConvertModuleToOMXAVCLevel(supported[pl.nProfileIndex]);

  return true;
}

void EncExpertiseAVC::GetProfileLevel(OMX_PTR param, Port const& port, EncModule const& module)
{
  auto& pl = *(OMX_VIDEO_PARAM_PROFILELEVELTYPE*)param;
  pl.nPortIndex = port.index;
  pl.eProfile = ConvertModuleToOMXAVCProfile(module.GetProfileLevel());
  pl.eLevel = ConvertModuleToOMXAVCLevel(module.GetProfileLevel());
}

bool EncExpertiseAVC::SetProfileLevel(OMX_PTR param, Port const& port, EncModule& module)
{
  OMX_VIDEO_PARAM_PROFILELEVELTYPE rollback;
  GetProfileLevel(&rollback, port, module);
  auto pl = *(OMX_VIDEO_PARAM_PROFILELEVELTYPE*)param;
  auto profile = static_cast<OMX_VIDEO_AVCPROFILETYPE>(pl.eProfile);
  auto level = static_cast<OMX_VIDEO_AVCLEVELTYPE>(pl.eLevel);

  if(!SetModuleProfileLevel(profile, level, module))
  {
    SetProfileLevel(&rollback, port, module);
    return false;
  }

  return true;
}

void EncExpertiseAVC::GetExpertise(OMX_PTR param, Port const& port, EncModule const& module)
{
  auto& avc = *(OMX_VIDEO_PARAM_AVCTYPE*)param;
  avc.nPortIndex = port.index;
  avc.nBFrames = ConvertModuleToOMXBFrames(module.GetGop());
  avc.nPFrames = ConvertModuleToOMXPFrames(module.GetGop());
  avc.bUseHadamard = OMX_TRUE; // XXX
  avc.nRefFrames = 1; // XXX
  avc.nRefIdx10ActiveMinus1 = 0; // XXX
  avc.nRefIdx11ActiveMinus1 = 0; // XXX
  avc.bEnableUEP = OMX_FALSE; // XXX
  avc.bEnableFMO = OMX_FALSE; // XXX
  avc.bEnableASO = OMX_FALSE; // XXX
  avc.bEnableRS = OMX_FALSE; // XXX
  avc.eProfile = ConvertModuleToOMXAVCProfile(module.GetProfileLevel());
  avc.eLevel = ConvertModuleToOMXAVCLevel(module.GetProfileLevel());
  avc.nAllowedPictureTypes = OMX_VIDEO_PictureTypeI | OMX_VIDEO_PictureTypeP | OMX_VIDEO_PictureTypeB; // XXX
  avc.bFrameMBsOnly = OMX_TRUE; // XXX
  avc.bMBAFF = OMX_FALSE; // XXX
  avc.bEntropyCodingCABAC = ConvertModuleToOMXEntropyCoding(module.GetEntropyCoding());
  avc.bWeightedPPrediction = OMX_FALSE; // XXX
  avc.nWeightedBipredicitonMode = OMX_FALSE; // XXX
  avc.bconstIpred = ConvertModuleToOMXBool(module.IsConstrainedIntraPrediction());
  avc.bDirect8x8Inference = OMX_TRUE; // XXX
  avc.bDirectSpatialTemporal = OMX_TRUE; // XXX
  avc.nCabacInitIdc = 0; // XXX
  avc.eLoopFilterMode = ConvertModuleToOMXAVCLoopFilter(module.GetLoopFilter());
}

bool EncExpertiseAVC::SetExpertise(OMX_PTR param, Port const& port, EncModule& module)
{
  OMX_VIDEO_PARAM_AVCTYPE rollback;
  GetExpertise(&rollback, port, module);
  auto avc = *(OMX_VIDEO_PARAM_AVCTYPE*)param;

  if(!SetModuleGop(avc.nBFrames, avc.nPFrames, module))
  {
    SetExpertise(&rollback, port, module);
    return false;
  }

  if(!SetModuleProfileLevel(avc.eProfile, avc.eLevel, module))
  {
    SetExpertise(&rollback, port, module);
    return false;
  }

  if(!SetModuleEntropyCoding(avc.bEntropyCodingCABAC, module))
  {
    SetExpertise(&rollback, port, module);
    return false;
  }

  if(!SetModuleConstrainedIntraPrediction(avc.bconstIpred, module))
  {
    SetExpertise(&rollback, port, module);
    return false;
  }

  if(!SetModuleLoopFilter(avc.eLoopFilterMode, module))
  {
    SetExpertise(&rollback, port, module);
    return false;
  }

  return true;
}

