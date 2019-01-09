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

#pragma once

#include <OMX_Core.h>
#include "base/omx_component/omx_component_interface.h"

OMXComponentInterface* GetThis(OMX_IN OMX_HANDLETYPE hComponent);
OMX_ERRORTYPE WrapSendCommand(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_COMMANDTYPE Cmd, OMX_IN OMX_U32 nParam1, OMX_IN OMX_PTR pCmdData);
OMX_ERRORTYPE WrapGetState(OMX_IN OMX_HANDLETYPE hComponent, OMX_OUT OMX_STATETYPE* pState);
OMX_ERRORTYPE WrapSetCallbacks(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_CALLBACKTYPE* pCallbacks, OMX_IN OMX_PTR pAppData);
OMX_ERRORTYPE WrapGetParameter(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_INDEXTYPE nParamIndex, OMX_INOUT OMX_PTR pParam);
OMX_ERRORTYPE WrapSetParameter(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_INDEXTYPE nParamIndex, OMX_IN OMX_PTR pParam);
OMX_ERRORTYPE WrapUseBuffer(OMX_IN OMX_HANDLETYPE hComponent, OMX_OUT OMX_BUFFERHEADERTYPE** ppBufferHdr, OMX_IN OMX_U32 nPortIndex, OMX_IN OMX_PTR pAppPrivate, OMX_IN OMX_U32 nSizeBytes, OMX_IN OMX_U8* pBuffer);
OMX_ERRORTYPE WrapAllocateBuffer(OMX_IN OMX_HANDLETYPE hComponent, OMX_INOUT OMX_BUFFERHEADERTYPE** ppBufferHdr, OMX_IN OMX_U32 nPortIndex, OMX_IN OMX_PTR pAppPrivate, OMX_IN OMX_U32 nSizeBytes);
OMX_ERRORTYPE WrapFreeBuffer(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_U32 nPortIndex, OMX_IN OMX_BUFFERHEADERTYPE* pBufferHdr);
OMX_ERRORTYPE WrapEmptyThisBuffer(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_BUFFERHEADERTYPE* pBufferHdr);
OMX_ERRORTYPE WrapFillThisBuffer(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_BUFFERHEADERTYPE* pBufferHdr);
OMX_ERRORTYPE WrapGetComponentVersion(OMX_IN OMX_HANDLETYPE hComponent, OMX_OUT OMX_STRING pComponentName, OMX_OUT OMX_VERSIONTYPE* pComponentVersion, OMX_OUT OMX_VERSIONTYPE* pSpecVersion, OMX_OUT OMX_UUIDTYPE pComponentUUID[128]);
OMX_ERRORTYPE WrapGetConfig(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_INDEXTYPE nIndex, OMX_INOUT OMX_PTR pComponentConfigStructure);
OMX_ERRORTYPE WrapSetConfig(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_INDEXTYPE nIndex, OMX_IN OMX_PTR pComponentConfigStructure);
OMX_ERRORTYPE WrapGetExtensionIndex(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_STRING cParameterName, OMX_OUT OMX_INDEXTYPE* pIndexType);
OMX_ERRORTYPE WrapComponentTunnelRequest(OMX_IN OMX_HANDLETYPE hComponent, OMX_IN OMX_U32 nPort, OMX_IN OMX_HANDLETYPE hTunneledComp, OMX_IN OMX_U32 nTunneledPort, OMX_INOUT OMX_TUNNELSETUPTYPE* pTunnelSetup);
OMX_ERRORTYPE WrapUseEGLImage(OMX_IN OMX_HANDLETYPE hComponent, OMX_INOUT OMX_BUFFERHEADERTYPE** ppBufferHdr, OMX_IN OMX_U32 nPortIndex, OMX_IN OMX_PTR pAppPrivate, OMX_IN void* eglImage);
OMX_ERRORTYPE WrapComponentRoleEnum(OMX_IN OMX_HANDLETYPE hComponent, OMX_OUT OMX_U8* cRole, OMX_IN OMX_U32 nIndex);

