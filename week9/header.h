#ifndef HEADER_H
# define HEADER_H

#ifdef __cplusplus
#include <iostream>

using namespace std;

extern "C" {

#else

typedef struct ostream ostream;

#endif

void cpp_function(const char *s);
ostream *return_cout(void);
void use_cout(ostream *o);

#ifdef __cplusplus
}
#endif

#endif
