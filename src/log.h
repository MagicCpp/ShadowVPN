/**
  log.h

  Copyright (C) 2015 clowwindy

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef LOG_H
#define LOG_H

#include <string.h>
#include <stdio.h>
#include <errno.h>

extern int verbose_mode;

/*
   err:    same as perror but with timestamp
   errf:   same as printf to stderr with timestamp and \n
   logf:   same as printf to stdout with timestamp and \n,
           and only enabled when verbose is on
   debugf: same as logf but only compiles with DEBUG flag
*/

#define __LOG(o, not_stderr, ...) do {                            \
  if (not_stderr || verbose_mode) {                               \
    log_timestamp(o);                                             \
    fprintf(o, __VA_ARGS__);                                      \
    fprintf(o, "\n");                                             \
    fflush(o);                                                    \
  }                                                               \
} while (0)

#ifdef HAVE_ANDROID_LOG
#include <android/log.h>
#define logf(s...) \
      __android_log_print(ANDROID_LOG_INFO, __FILE__, s)

#define errf(s...) \
      __android_log_print(ANDROID_LOG_ERROR, __FILE__, s)

#define err(s) \
      __android_log_print(ANDROID_LOG_ERROR, __FILE__, "%s: %s", s, strerror(errno))

#else

#define logf(...) __LOG(stdout, 0, __VA_ARGS__)
#define errf(...) __LOG(stderr, 1, __VA_ARGS__)
#define err(s) perror_timestamp(s, __FILE__, __LINE__)

#endif

#ifdef DEBUG
#define debugf(...) logf(__VA_ARGS__)
#else
#define debugf(...)
#endif

void log_timestamp(FILE *out);
void perror_timestamp(const char *msg, const char *file, int line);
void print_hex_memory(void *mem, size_t len);

#endif
