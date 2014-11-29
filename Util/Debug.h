#ifndef __DEBUG_H__
#define __DEBUG_H__

/*
========================================================================
	
	This file involves debug functionality. Define DEBUG_ENABLE for 
	using this functionality. 

========================================================================
*/

#include <Windows.h>
#include "Journal.h"
#include "TypeDefinitions.h"

using std::ofstream;
using std::ios;

#ifdef DEBUG_ENABLE
#define log_msg(msg) { \
	Journal::openJournal()->writeMsg(__FILE__, __LINE__, msg); \
	}

#define error_msg(msg) { \
	Journal::openJournal()->writeErrorMsg(__FILE__, __LINE__, msg); \
	}

#define debug_assert(x, comment) { \
			if(!(x)) \
			{\
				error_msg(comment)\
				_asm{int 3};\
			} \
			}

#else
#define debug_assert(x, comment)
#define log_msg(msg)
#define error_msg(msg);
#endif

#define debug_error(x, comment) { \
			if(!(x)) \
			{\
				error_msg(comment) \
				exit(EXIT_SUCCESS); \
			}\
			}

#endif