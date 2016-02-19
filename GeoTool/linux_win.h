#ifndef  _LINUX_WIN_H_
#define _LINUX_WIN_H_

#ifdef LINUX
#define feek64 fseeko64
#define ftell64 fseeko64
#else 
#define fseek64 _fseeki64
#define ftell64 _ftelli64
#endif

#endif