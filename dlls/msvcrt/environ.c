/*
 * msvcrt.dll environment functions
 *
 * Copyright 1996,1998 Marcus Meissner
 * Copyright 1996 Jukka Iivonen
 * Copyright 1997,2000 Uwe Bonnes
 * Copyright 2000 Jon Griffiths
 */
#include "wine/unicode.h"
#include "msvcrt.h"

DEFAULT_DEBUG_CHANNEL(msvcrt);

LPWSTR wcsrchr( LPWSTR str, WCHAR ch );

/*********************************************************************
 *		getenv (MSVCRT.@)
 */
char *MSVCRT_getenv(const char *name)
{
  char *environ = GetEnvironmentStringsA();
  char *pp,*pos = NULL;
  unsigned int length;

  for (pp = environ; (*pp); pp = pp + strlen(pp) +1)
  {
    pos =strchr(pp,'=');
    if (pos)
      length = pos -pp;
    else
      length = strlen(pp);
    if (!strncmp(pp,name,length)) break;
  }
  if ((pp)&& (pos))
  {
     pp = pos+1;
     TRACE("got %s\n",pp);
  }
  FreeEnvironmentStringsA( environ );
  return pp;
}

/*********************************************************************
 *		_wgetenv (MSVCRT.@)
 */
WCHAR *_wgetenv(const WCHAR *name)
{
  WCHAR* environ = GetEnvironmentStringsW();
  WCHAR* pp,*pos = NULL;
  unsigned int length;

  for (pp = environ; (*pp); pp = pp + strlenW(pp) + 1)
  {
    pos =wcsrchr(pp,'=');
    if (pos)
      length = pos -pp;
    else
      length = strlenW(pp);
    if (!strncmpW(pp,name,length)) break;
  }
  if ((pp)&& (pos))
  {
     pp = pos+1;
     TRACE("got %s\n",debugstr_w(pp));
  }
  FreeEnvironmentStringsW( environ );
  return pp;
}

/*********************************************************************
 *		_putenv (MSVCRT.@)
 */
int _putenv(const char *str)
{
 char name[256], value[512];
 char *dst = name;

 TRACE("%s\n", str);

 if (!str)
   return -1;
 while (*str && *str != '=')
  *dst++ = *str++;
 if (!*str++)
   return -1;
 *dst = '\0';
 dst = value;
 while (*str)
  *dst++ = *str++;
 *dst = '\0';

 return !SetEnvironmentVariableA(name, value[0] ? value : NULL);
}

/*********************************************************************
 *		_wputenv (MSVCRT.@)
 */
int _wputenv(const WCHAR *str)
{
 WCHAR name[256], value[512];
 WCHAR *dst = name;

 TRACE("%s\n", debugstr_w(str));

 if (!str)
   return -1;
 while (*str && *str != (WCHAR)L'=')
  *dst++ = *str++;
 if (!*str++)
   return -1;
 *dst = (WCHAR)L'\0';
 dst = value;
 while (*str)
  *dst++ = *str++;
 *dst = (WCHAR)L'\0';

 return !SetEnvironmentVariableW(name, value[0] ? value : NULL);
}
