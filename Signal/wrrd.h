#ifndef SIGNAL_WRRD_H
#define SIGNAL_WRRD_H

const int SIZE_BUF = 4096;

void bprint (unsigned char val);

int reader (int writerPID);
int writer (int readerPID, char *fileName);


void sigHandlerReaderUSR1 (int);
void sigHandlerReaderUSR2 (int);
void sigHandlerReaderCHLD (int);

void sigHandlerWriterALARM (int);
void sigHandlerWriterUSR1 (int);

#endif //SIGNAL_WRRD_H
