#pragma once

#include "defines.hpp"
#include <cstdio>
#include <cstdlib>

#define RV_ASSERTIONS_ENABLED

// definisci sempre rv_debug_break nel caso sia richiesto fuori dalle assertions (e.g. fatal log)
// prova prima con __has_builtin 
#if defined(__has_builtin)
#   if __has_builtin(__builtin_debugtrap)
#       define rv_debug_break() __builtin_debugtrap()
#   elif __has_builtin(__debugbreak)
#      define rv_debug_break() __debugbreak()
#   endif
#endif

// defaultiamo sul classico
#ifndef rv_debug_break
#   if defined(_MSC_VER)
#       include <intrin.h>
#       define rv_debug_break() __debugbreak()
#   elif defined(__GNUC__) || defined(__clang__)
#       define rv_debug_break() __builtin_trap()
#   else
#       define rv_debug_break() std::abort()
#   endif
#endif

#ifdef RV_ASSERTIONS_ENABLED

void report_assertion_failure(const char* expr, const char* msg, const char* file, int line);

#   define RV_ASSERT(expr)                                                            \
        {                                                                             \
            if (!expr) {                                                              \
                report_assertion_failure(#expr, "", __FILE__, __LINE__); \
                rv_debug_break();                                                     \
            }                                                                         \
        }   
        
#   define RV_ASSERT_MSG(expr, msg)                                                      \
        {                                                                                \
            if (!expr) {                                                                 \
                report_assertion_failure(#expr, msg, __FILE__, __LINE__); \
                rv_debug_break();                                                        \
            }                                                                            \
        }
#   if RV_DEBUG
#       define RV_ASSERT_DEBUG(expr) RV_ASSERT(expr)
#   else
#       define RV_ASSERT_DEBUG(expr)               
#   endif
#else
#   define RV_ASSERT(expr)
#   define RV_ASSERT_MSG(expr, msg)
#   define RV_ASSERT_DEBUG(expr)
#endif
