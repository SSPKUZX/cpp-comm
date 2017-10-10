/////////////////////////////////////////////////////////////////////
//          Copyright Yibo Zhu 2017
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
/////////////////////////////////////////////////////////////////////
#pragma once

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <stdlib.h>
#include <windows.h>

static size_t cacheline_size() {
  size_t line_size = 0;
  DWORD buffer_size = 0;
  DWORD i = 0;
  SYSTEM_LOGICAL_PROCESSOR_INFORMATION *buffer = 0;
  GetLogicalProcessorInformation(0, &buffer_size);
  buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(buffer_size);
  GetLogicalProcessorInformation(&buffer[0], &buffer_size);

  for (i = 0; i != buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
       ++i) {
    if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1) {
      line_size = buffer[i].Cache.LineSize;
      break;
    }
  }

  free(buffer);
  return line_size;
}

#elif defined(__linux__)
#include <unistd.h>
static size_t cacheline_size() {
  size_t line_size = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
  return line_size;
}
#elif defined(__APPLE__)
#include <sys/sysctl.h>
static size_t cacheline_size() {
  size_t line_size = 0;
  size_t size_of_linesize = sizeof(line_size);
  sysctlbyname("hw.cachelinesize", &line_size, &size_of_linesize, 0, 0);
  return line_size;
}
#else
#error unsupported platform
#endif
