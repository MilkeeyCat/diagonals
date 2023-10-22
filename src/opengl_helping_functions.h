#include <GL/glew.h>
#include <signal.h>

#define ASSERT(x)                                                              \
  if (!(x))                                                                    \
  raise(SIGTRAP)
#define GL_CALL(x)                                                             \
  GLClearError();                                                              \
  x;                                                                           \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char *function_name, const char *file, int line);
