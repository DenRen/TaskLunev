#ifndef PROXY_CIRCBUFFER_H
#define PROXY_CIRCBUFFER_H

struct CircBuffer {
    int m_capacity;
    int m_size;
    char *m_buf;

    char *m_reader;
};

int CircBufferInitialize (CircBuffer *circBuffer, int capacity);
void CircBufferRelease (CircBuffer *circBuffer);
bool CircBufferVerifier (CircBuffer *cirBuffer);

bool CircBufferHaveEmpty (CircBuffer *circBuffer);
bool CircBufferHaveSize  (CircBuffer *circBuffer);

int CircBufferGetEmpty (CircBuffer *circBuffer, char **pWriter);
int CircBufferGetSize  (CircBuffer *circBuffer, char **pReader);

int CircBufferGetWriter (CircBuffer *circBuffer, char **buffer);
int CircBufferGetReader (CircBuffer *circBuffer, char **buffer);

int CircBufferChangeSize (CircBuffer *circBuffer, int value);
#endif //PROXY_CIRCBUFFER_H
