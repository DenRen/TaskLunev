#include "CircBuffer.h"
#include "DebugFunc.h"

int CircBufferInitialize (CircBuffer *circBuffer, int capacity) {
    CHECK_TRUE (circBuffer != nullptr);
    CHECK_TRUE (capacity > 0);

    circBuffer->m_buf = (char *) calloc (capacity, sizeof (char));
    CHECK_TRUE (circBuffer->m_buf != nullptr);

    circBuffer->m_capacity = capacity;
    circBuffer->m_size = 0;
    circBuffer->m_reader = circBuffer->m_buf;

    return 0;
}

void CircBufferRelease (CircBuffer *circBuffer) {
    circBuffer->m_capacity = 0;
    circBuffer->m_size     = 0;

    free (circBuffer->m_buf);

    circBuffer->m_buf    = nullptr;
    circBuffer->m_reader = nullptr;
}

bool CircBufferVerifier (CircBuffer *cirBuffer) {
    return cirBuffer->m_buf != nullptr && cirBuffer->m_capacity > 0;
}

int CircBufferGetEmpty (CircBuffer *circBuffer, char **pWriter) {
    MYASSERT (CircBufferVerifier (circBuffer));

    char *begin = circBuffer->m_buf;
    char *reader = circBuffer->m_reader;
    char *writer = reader + circBuffer->m_size;

    const int size = circBuffer->m_size;
    const int capacity = circBuffer->m_capacity;

    if (writer <= begin + capacity)
    {
        return begin + capacity - writer;
    }
    else
    {
        return capacity - size;
    }
}

int CircBufferGetSize  (CircBuffer *circBuffer, char **pReader) {
    MYASSERT (CircBufferVerifier (circBuffer));

    char *begin = circBuffer->m_buf;
    char *reader = circBuffer->m_reader;
    char *writer = reader + circBuffer->m_size;

    const int size = circBuffer->m_size;
    const int capacity = circBuffer->m_capacity;

    if (writer <= begin + capacity)
    {
        return size;
    }
    else
    {
        return begin + capacity - reader;
    }
}

int CircBufferGetWriter (CircBuffer *circBuffer, char **buffer) {
    MYASSERT (CircBufferVerifier (circBuffer));

    char *begin = circBuffer->m_buf;
    char *reader = circBuffer->m_reader;
    char *writer = reader + circBuffer->m_size;

    const int size = circBuffer->m_size;
    const int capacity = circBuffer->m_capacity;

    if (writer < begin + capacity) {
        *buffer = writer;
        return begin + capacity - writer;
    } else if (writer == begin + capacity) {
      if (reader != begin) {
          *buffer = begin;
          return reader - begin;
      } else {
          *buffer = begin;
          return 0;
      }
    } else {
        *buffer = reader + size - capacity;
        return capacity - size;
    }
}

int CircBufferGetReader (CircBuffer *circBuffer, char **buffer) {
    MYASSERT (CircBufferVerifier (circBuffer));

    char *begin = circBuffer->m_buf;
    char *reader = circBuffer->m_reader;
    char *writer = reader + circBuffer->m_size;

    const int size = circBuffer->m_size;
    const int capacity = circBuffer->m_capacity;

    if (writer <= begin + capacity) {
        *buffer = reader;
        return size;
    } else {
        *buffer = begin;
        return capacity - size;
    }
}

bool CircBufferHaveEmpty (CircBuffer *circBuffer) {
    return circBuffer->m_capacity - circBuffer->m_size;
}

bool CircBufferHaveSize (CircBuffer *circBuffer) {
    return circBuffer->m_size;
}

int CircBufferChangeSize (CircBuffer *circBuffer, int value) {
    circBuffer->m_size += value;

    if (value < 0) {
        if (circBuffer->m_reader + circBuffer->m_size == circBuffer->m_buf + circBuffer->m_capacity) {
            circBuffer->m_reader = circBuffer->m_buf;
        }
    }

    return 0;
}