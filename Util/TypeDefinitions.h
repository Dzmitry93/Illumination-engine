#ifndef __TYPE_DEFINITIONS_H__
#define __TYPE_DEFINITIONS_H__

namespace OGE
{
	typedef signed long long int64;
	typedef signed long int32;
	typedef signed short int16;
	typedef signed char int8;

	typedef unsigned long long uint64;
	typedef unsigned long uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;
}

#define _text
#define _unicode_text(a) L ## a

#define safe_release(pointer) { \
	if(pointer != nullptr) { \
		delete pointer; \
		pointer = nullptr; \
	}\
}\

#define safe_array_release(pointer) { \
	if(pointer != nullptr) { \
		delete[] pointer; \
		pointer = nullptr; \
	}\
}\

#define safe_dx_resource_release(resource) { \
	if(resource != nullptr) { \
		resource->Release(); \
		resource = nullptr; \
	}\
}\

#define safe_resource_release(resource) { \
	if(resource != nullptr) { \
		resource->release(); \
		resource = nullptr; \
	}\
}\

#endif