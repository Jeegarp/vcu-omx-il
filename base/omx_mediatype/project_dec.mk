THIS.omx_mediatype_dec:=$(call get-my-dir)

OMX_MEDIATYPE_DEC_SRCS+=\
	$(THIS.omx_mediatype_dec)/omx_mediatype_dec_avc.cpp\
	$(THIS.omx_mediatype_dec)/omx_mediatype_dec_hevc.cpp\
	$(THIS.omx_mediatype_dec)/omx_mediatype_dec_common.cpp\

UNITTESTS+=$(OMX_MEDIATYPE_DEC_SRCS)
