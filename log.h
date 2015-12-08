#ifndef _LOG_H_
#define _LOG_H_

#include "bool.h"

BOOL log_open    (const char *filename);
BOOL log_close   (void);
int  log_message (const char *fmt, ...);

#endif /* _LOG_H_ */
