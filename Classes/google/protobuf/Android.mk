#build google protobuf
LOCAL_PATH := $(call my-dir)

ifeq (,$(strip $(wildcard $(LOCAL_PATH)/obj/local/armeabi/libprotobuf_static.a)))
	PROTOBUF_REBUILD := true
endif

libprotobuf_path := $(call my-dir)

libprotobuf_src_files :=	\
		descriptor.cc \
		descriptor.pb.cc \
		descriptor_database.cc \
		dynamic_message.cc \
		extension_set.cc \
		extension_set_heavy.cc \
		generated_message_reflection.cc \
		generated_message_util.cc \
		io/coded_stream.cc \
		io/gzip_stream.cc \
		io/printer.cc \
		io/tokenizer.cc \
		io/zero_copy_stream.cc \
		io/zero_copy_stream_impl.cc \
		io/zero_copy_stream_impl_lite.cc \
		message.cc \
		message_lite.cc \
		reflection_ops.cc \
		repeated_field.cc \
		service.cc \
		stubs/common.cc \
		stubs/once.cc \
		stubs/structurally_valid.cc \
		stubs/strutil.cc \
		stubs/substitute.cc \
		text_format.cc \
		unknown_field_set.cc \
		wire_format.cc \
		wire_format_lite.cc 	
	
libprotobuf_static_libs := libstdc++	
libprotobuf_cflags := -o2
libprotobuf_cppflags := -fuse-cxa-atexit
libprotobuf_c_includes :=$(libprotobuf_path)/../.. $(libprotobuf_path)/../../../Android $(libprotobuf_path)/.. $(libprotobuf_path)/protobuf

#LOCAL_CFLAGS := -std=c99                    
#LOCAL_CXXFLAGS += -DRELEASAE -DANDROID \
#				-fexceptions -fasm-blocks -fstrict-aliasing -g -#fvisibility-inlines-hidden -std=gnu++11 -D__GXX_EXPERIMENTAL_CXX0X__ -o2

# Note: For now, this implementation depends on the system libstdc++
#       We may want to avoid that in the future, i.e. in order to
#       properly support exceptions and RTTI.
libstlport_static_libs := libstdc++

ifneq ($(PROTOBUF_REBUILD),true)
	
include $(CLEAR_VARS)
LOCAL_MODULE := protobuf_static
LOCAL_SRC_FILES := obj/local/$(TARGET_ARCH_ABI)/lib$(LOCAL_MODULE).a
LOCAL_STATIC_LIBRARIES := $(libprotobuf_static_libs)
LOCAL_EXPORT_C_INCLUDES := $(libprotobuf_c_includes)
include $(PREBUILT_STATIC_LIBRARY)

else

include $(CLEAR_VARS)
LOCAL_MODULE := protobuf_static
LOCAL_CPP_EXTENSION := .cc
LOCAL_SRC_FILES := $(libprotobuf_src_files)
LOCAL_CFLAGS := $(libprotobuf_cflags)
LOCAL_CPPFLAGS := $(libprotobuf_cppflags)
LOCAL_C_INCLUDES := $(libprotobuf_c_includes)
LOCAL_STATIC_LIBRARIES := $(libprotobuf_static_libs)
LOCAL_EXPORT_C_INCLUDES := $(libprotobuf_c_includes)
include $(BUILD_STATIC_LIBRARY)

endif

