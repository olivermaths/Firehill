#pragma once
#include <stdint.h>
#include <signal.h>
#include <stdbool.h>
#include <vulkan/vulkan.h>


#ifndef __RELEASE__
#define COLOR_RED "\033[1;31m"
#define COLOR_RESET "\033[0m"

#define c_assert(value)                                                        \
  if (!(value)) {                                                              \
    cc_assert(#value, __FILE__, __LINE__);                                     \
  }

#define cc_assert(value, file, line)                                           \
  printf(COLOR_RED "assertion failed: " value                                  \
                   " in %s : Line %i\n" COLOR_RESET,                           \
         file, line);                                                          \
  raise(SIGINT);

#else
#define c_assert(value)
#endif
