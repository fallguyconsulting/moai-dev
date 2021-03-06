#================================================================#
# Copyright (c) 2010-2011 Zipline Games, Inc.
# All Rights Reserved.
# http://getmoai.com
#================================================================#

include $(CLEAR_VARS)

LOCAL_MODULE 		:= moai-http-client
LOCAL_ARM_MODE 		:= $(MY_ARM_MODE)
LOCAL_CFLAGS		:= $(MY_LOCAL_CFLAGS) -include $(MOAI_SDK_HOME)/src/zl-vfs/zl_replace.h -fvisibility=hidden

LOCAL_C_INCLUDES 	:= $(MY_HEADER_SEARCH_PATHS)
LOCAL_C_INCLUDES 	+= '$(MOAI_SDK_HOME)/3rdparty/openssl-1.0.0m'

LOCAL_SRC_FILES 	+= $(wildcard $(MOAI_SDK_HOME)/src/moai-http-client/*.cpp)

include $(BUILD_STATIC_LIBRARY)
