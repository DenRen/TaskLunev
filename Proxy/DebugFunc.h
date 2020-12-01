#pragma once

#include <cerrno>
#include <stdexcept>

void NOP ();
void PrintLogInfo (const char file[], int line, bool inFile, bool end = false);
void DebugEndMain ();

#define TURN_DEBUG

#define RELEASE(res)        \
    if (res != nullptr)     \
    {                       \
        res->Release ();    \
        res = nullptr;      \
    }

#ifdef TURN_DEBUG
    #define DUMP_DEBUG_INFO     PrintLogInfo (__FILE__, __LINE__, false, false)
    #define END_DUMP_DEBUG_INFO	PrintLogInfo (__FILE__, __LINE__, false, true)
#else
    #define DUMP_DEBUG_INFO     PrintLogInfo (__FILE__, __LINE__, true, false)
    #define END_DUMP_DEBUG_INFO	PrintLogInfo (__FILE__, __LINE__, true, true)
#endif

#define RETURN_FALSE    \
{                       \
    DUMP_DEBUG_INFO;    \
    return false;       \
} NOP ()

#define CHECK_FAILED(val)   \
    if (FAILED (val))       \
    {                       \
        RETURN_FALSE;       \
    }

#define CHECK_NULLPTR(val)  \
    if (val == nullptr)     \
    {                       \
        RETURN_FALSE;       \
    }

#define CHECK_ERROR(ret)                    \
    if (ret == -1) {                        \
        DUMP_DEBUG_INFO;                    \
        return -1;                          \
    } else {                                \
        errno = 0;                          \
    } NOP ()

#define CHECK_TRUE(val)                     \
    if (val == false) {                     \
        DUMP_DEBUG_INFO;                    \
        return -1;                          \
    } else {                                \
        errno = 0;                          \
    } NOP ()

#define RETURN_THROW                \
{                                   \
    DUMP_DEBUG_INFO;                \
    DebugEndMain ();                \
    throw std::runtime_error ("");  \
} NOP ()

#ifdef TURN_DEBUG
    #define MYASSERT(val)               \
    {                                   \
        if ((val) == false)             \
        {                               \
            DUMP_DEBUG_INFO;            \
            END_DUMP_DEBUG_INFO;        \
            printf ("%s\n", #val);      \
            RETURN_THROW;               \
        }                               \
    } NOP ()
#else
    #define MYASSERT(val) NOP ();
#endif


#define CHECK_THIS() MYASSERT (this != nullptr)