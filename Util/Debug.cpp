#include "Debug.h"

void OGE::writeMsg(const char* condition, const char* description, const char* fileName, int lineNumber)
{
	ofstream out("LOG.txt", ios::app);

	const int MAX_BUFFER_SIZE = 1024;
	char buffer[MAX_BUFFER_SIZE];

	int size = _snprintf_s(	buffer, 
							MAX_BUFFER_SIZE, 
							_text(
									 "File : %s\n"      \
									 "Line : %d\n"      \
									 "Error: %s\n"		\
									 "Comment: %s\n"	\
									),
							fileName,
							lineNumber,
							condition,
							description
							);

	out.write(const_cast<const char*>(buffer), size);
	out.close();
}
