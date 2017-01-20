/****************************************************************/
/* ģ����    ��PUBLIC                                         */
/* ģ������    ����������                                       */
/* �� �� ��    ��V4.3.0                                         */
/* ��	 ��    ���Ż�                                           */
/* ��������    ��1999/11/22                                     */
/* ����޸����ڣ�2001/4/6                                       */
/* ģ����;    ������ƽ̨���õĹ�������                         */
/* ��ģ���а������º���������˵����                             */
/*                        (1)  int _swVersion()                 */ 
/*			  (2)  int _swMatch()                   */
/*			  (3)  int _swTrim()                    */
/*                        (4)  int _swTrimL()                   */
/*                        (5)  int _swTrimR()                   */
/*			  (6)  int _swGetTime()                 */
/*			  (7)  int _swItoa()                    */
/*			  (8)  int _swReverse()                 */
/*			  (9)  int _swUdf_ltoa()                */
/*			  (10) int _swStrToDate()               */
/*			  (11) int _swDateToStr()               */
/*			  (12) int _swStrCmp()                  */
/*			  (13) int _swStrReplace()              */
/*			  (14) int swDberror()                  */
/*			  (15) int swDebug()                    */
/*			  (16) int swMberror()                  */
/*			  (17) int swDebugfml()                 */
/*			  (18) int swDebughex()                 */
/*			  (19) int swDebugmsghead()             */
/*			  (20) int _swMsgsend()                 */
/*			  (21) int _swOrdsend()                 */
/*			  (22) int _swGetOneField()             */
/*                        (23) int _swGet_field()               */
/*                        (24) int _swDebugflag()               */
/*                        (25) int swFmlunpack()                */
/*                        (26) int swFmlpack()                  */
/*                        (27) int swFmlget()                   */
/*                        (28) int swFmlset()                   */
/*                        (29) int _swTimeLongToChs();          */
/*                        (30) int _swFmlunion();               */
/*                        (31) int swDbopen();                  */
/*                        (32) int swDbclose();                 */
/*                        (33) int swGetlog();                  */
/*                        (34) int swPutlog();                  */
/*			  (35) int _swItou();                   */
/*			  (36) int _swUtoi();                   */
/*			  (37) int _swClrMb();                  */
/*                        (38) int _swSetdefault()              */ 
/*                        (39) int _swChan()                    */
/*                        (40) short _swChgvalue()              */
/*                        (41) short _swGetitemmode1()          */
/*                        (42) short _swGetvaluemode1()         */
/*                        (43) short _swGetitemmode2()          */ 
/*                        (44) int swPortset()                  */
/*                        (45) int swPortget()                  */
/*                        (46) short swPacksep()                */
/*                        (47) int swMacrorepl()                */
/*                        (48) FILE *_swFopen()                 */
/*                        (48) FILE *_swPopen()                 */
/*                        (48) FILE *_swFopenlist()             */
/****************************************************************/

/****************************************************************/
/* �޸ļ�¼��                                                   */
/*  2001/2/27 ����DEBUG����                                     */
/*  2001/3/24 ���V3.01,����2608��                            */
/*  2001/3/28 ��pid���͸�Ϊlong(2614)                           */
/*            swFmlunpack�������ж�������Ĺ�����ضϴ���(2622) */
/*            EBCDIC����ת���Ķ�(2713)                          */
/*  2001/4/3  ���Ӷ�swDebug�ļ����ȵ��޸�(����)(2743)           */
/*  2001/4/6  INFORMIX��ֲ(2766)                                */
/*  2002/8/20 ����_swFopen()֧��SWITCH_CFGDIR��·��             */
/****************************************************************/

#include <netdb.h>
#include <sys/socket.h>

#ifdef OS_SCO
#include <sys/netinet/in.h>
#endif

#ifdef OS_AIX
#include <netinet/in.h>
#endif

#include <regex.h>
#include <pthread.h>
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)

/* switch ���� */
#include "switch.h"


#include "swDbstruct.h"

#include "swShm.h"

static int _swReverse(char *s);
extern int _swExpN2T(char *aNstr,char *aTstr);
/**************************************************************
 ** ������: swVersion
 ** ��  ��: ȡ�ð汾��
 ** ��  ��: zzh
 ** ��������    : 2001/03/06
 ** ����޸�����:
 ** ������������:
 ** ȫ�ֱ���    :
 ** ��������    :
 ** ����ֵ: SUCCESS
***************************************************************/
int _swVersion(char *cmArgv1,char *cmArgv2)
{
   if((strcmp(cmArgv2,"-v") ==0)||(strcmp(cmArgv2,"-V")==0))
     { 
       printf("%s\n",cmArgv1);
       exit(0);
     }
   return 0;
}

/**************************************************************
 ** ������      : _swMatch
 ** ��  ��      : �����������ʽ��ƥ��У��
 ** ��  ��      : ʷ����
 ** ��������    : 1999/11/25
 ** ����޸�����: 2000/3/10
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : regexpress:�������ʽ  
		  str       :��ҪУ����ַ���
 ** ����ֵ      : TRUE      :ƥ�� 
		  FALSE     :��ƥ��
***************************************************************/
int _swMatch(char *regexpress, char *str)
{
  int ilRc;
  regex_t slComplied;
  regmatch_t pslMatch[1];

  ilRc = regcomp(&slComplied,regexpress,REG_EXTENDED);
  if ( ilRc == 0 )
  {
    ilRc = regexec(&slComplied,str,1,pslMatch,0);
    if (ilRc)
    {
      return(FALSE);
    }     
    if (pslMatch[0].rm_eo == strlen(str))
    {
      return(TRUE);
    }
    return(FALSE);
  }
  return(FALSE);
}

#if 0

/**************************************************************
 ** ������      : _swTrim
 ** ��  ��      : �ú������ַ��� s ��ǰ��ո�β�س�ȥ��
 ** ��  ��      : llx
 ** ��������    : 1999/11/4
 ** ����޸�����: 2000/3/10
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : s   :�ַ���
 ** ����ֵ      : SUCCESS
		  ת������ַ�����s����
***************************************************************/
int _swTrim(s)
char *s;
{
  short	i, l, r, len;

  for(len=0; s[len]; len++);
  for(l=0; (s[l]==' ' || s[l]=='\t' || s[l]=='\n'); l++);
  if(l==len)
  {
    s[0]='\0';
    return(SUCCESS);
  }
  for(r=len-1; (s[r]==' ' || s[r]=='\t' || s[r]=='\n'); r--);
  for(i=l; i<=r; i++) s[i-l]=s[i];
  s[r-l+1]='\0';
  return(SUCCESS);
}

#endif /**delete by wanghao move to swUtil.c 20150416**/

/**************************************************************
 ** ������      : _swTrimL
 ** ��  ��      : �ú������ַ��� s ��ǰ�ո�β�س�ȥ��
 ** ��  ��      : lijunwen
 ** ��������    : 2000/3/29
 ** ����޸�����: 2000/3/29
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : s   :�ַ���
 ** ����ֵ      : SUCCESS
		  ת������ַ�����s����
***************************************************************/
int _swTrimL(s)
char *s;
{
  short	i, l, r, len;

  for(len=0; s[len]; len++);
  for(l=0; (s[l]==' ' || s[l]=='\t' || s[l]=='\n'); l++);
  if(l==len)
  {
    s[0]='\0';
    return(SUCCESS);
  }
  r = len -1;
  /*for(r=len-1; (s[r]==' ' || s[r]=='\t' || s[r]=='\n'); r--);*/
  for(i=l; i<=r; i++) s[i-l]=s[i];
  s[r-l+1]='\0';
  return(SUCCESS);
}

/**************************************************************
 ** ������      : _swTrimR
 ** ��  ��      : �ú������ַ��� s �ĺ�ո�β�س�ȥ��
 ** ��  ��      : lijunwen
 ** ��������    : 2000/3/29
 ** ����޸�����: 2000/3/29
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : s   :�ַ���
 ** ����ֵ      : SUCCESS
		  ת������ַ�����s����
***************************************************************/
int _swTrimR(s)
char *s;
{
  short	i, l, r, len;

  for(len=0; s[len]; len++);
/*
  for(l=0; (s[l]==' ' || s[l]=='\t' || s[l]=='\n'); l++);
  if(l==len) 
  { 
    s[0]='\0'; 
    return(SUCCESS); 
  }
*/
  l = 0;
  for(r=len-1; (s[r]==' ' || s[r]=='\t' || s[r]=='\n'); r--);
  for(i=l; i<=r; i++) s[i-l]=s[i];
  s[r-l+1]='\0';

  return(SUCCESS);
}

#if 0
/**************************************************************
 ** ������      : _swGetTime
 ** ��  ��      : ȡ��ǰϵͳʱ��, ��ʽYYYY-MM-DD HH:MM:SS.
 ** ��  ��      : llx
 ** ��������    : 1999/11/4
 ** ����޸�����: 2000/3/10
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : date   :���ص��ַ���
 ** ����ֵ      : SUCCESS
***************************************************************/
int _swGetTime(char *date)
{
  long	 Time;
  struct tm *T;

  time(&Time);
  T=localtime(&Time);
  sprintf(date, "%04d-%02d-%02d %02d:%02d:%02d",
    T->tm_year+1900,T->tm_mon+1,T->tm_mday,
    T->tm_hour, T->tm_min, T->tm_sec);
  return(SUCCESS);
}

#endif /**delete by wanghao. move to swUtil.c 20150416****/

/**************************************************************
 ** ������      : _swItoa
 ** ��  ��      : ��������ת��Ϊ�ַ���
 ** ��  ��      : llx
 ** ��������    : 1999/11/5
 ** ����޸�����: 2000/3/10
 ** ������������: _swReverse
 ** ȫ�ֱ���    :
 ** ��������    : n : ��������
		  s : �ַ���
 ** ����ֵ      : SUCCESS
***************************************************************/
int _swItoa(n,s)
int n;
char s[];
{
  short i,sign;

  if ((sign = n) < 0)               /* record sign */
    n = -n; 	                    /* make positive */
  i = 0;
  do 
  {		       /* generate digits in reverse order */
    s[i++] = n % 10 + '0';          /* get next digit */
  } while ((n /= 10) > 0);	    /* delete it */
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  _swReverse(s);

  return(SUCCESS);
}

/**************************************************************
 ** ������      : _swReverse
 ** ��  ��      : ���ַ�����ת
 ** ��  ��      : llx
 ** ��������    : 1999/11/5
 ** ����޸�����: 2000/3/10
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : s : �ַ���
 ** ����ֵ      : SUCCESS
		  ת����Ĵ���s����
***************************************************************/
static int _swReverse(char *s)
{
  short c, i, j;
  for (i=0, j=strlen(s)-1; i<j; i++, j--)
  {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
  return(SUCCESS);
}

/**************************************************************
 ** ������      : _swUdf_ltoa
 ** ��  ��      : ��������ת��Ϊ�ַ���
 ** ��  ��      : llx
 ** ��������    : 1999/11/5
 ** ����޸�����: 2000/3/10
 ** ������������: reverse
 ** ȫ�ֱ���    :
 ** ��������    : n : ��������
		  s : �ַ���
 ** ����ֵ      : SUCCESS
***************************************************************/
int _swUdf_ltoa(n,s)
long n;
char s[];
{
  long i,sign;

  if ((sign = n) < 0)               /* record sign */
    n = -n; 		            /* make positive */
  i = 0;
  do 
  {			/* generate digits in reverse order */
    s[i++] = n % 10 + '0';          /* get next digit */
  } while ((n /= 10) > 0);	    /* delete it */
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  _swReverse(s);
  return(SUCCESS);
}

/**************************************************************
 ** ������      : _swStrToDate
 ** ��  ��      : ��λ����(YYYYMMDD)ת��Ϊʮλ����(YYYY/MM/DD)
 ** ��  ��      : llx
 ** ��������    : 1999/11/5
 ** ����޸�����: 2000/3/10
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : date1: ��λ����
		  date2: ʮλ����
 ** ����ֵ      : SUCCESS :ת���ɹ�
                  -1:������벻��
                  -2:�µ����벻�� 
                  -3:�յ����벻�� 
***************************************************************/
int _swStrToDate(char * date1, char * date2)
{
  char Year[5], Month[3], Day[3];

  memcpy(Year, date1, 4);
  Year[4] = '\0';
  if (atoi(Year) < 1900 || atoi(Year) > 9999)
  {
    return (-1);
  }

  memcpy(Month, date1 + 4, 2);
  Month[2] = '\0';
  if (atoi(Month) < 1 || atoi(Month) > 12)
  {
    return (-2);
  }

  memcpy(Day, date1 + 6, 2);
  Day[2] = '\0';
  if (atoi(Day) < 1 || atoi(Day) > 31)
  {
    return (-3);
  }
  sprintf(date2, "%4s/%2s/%2s", Year, Month, Day);
  return (SUCCESS);
}

/**************************************************************
 ** ������      : _swDateToStr
 ** ��  ��      : ʮλ����(YYYY/MM/DD)ת��Ϊ��λ����(YYYY/MM/DD)
 ** ��  ��      : llx
 ** ��������    : 1999/11/5
 ** ����޸�����: 2000/3/10
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : date1: ʮλ����
		  date2: ��λ����
 ** ����ֵ      : SUCCESS :ת���ɹ�
                  -1:������벻��
                  -2:�µ����벻�� 
                  -3:�յ����벻�� 
***************************************************************/
int _swDateToStr(char * date1, char * date2)
{
  char Year[5], Month[3], Day[3];

  if (date1[0] == ' ')
  {
    sprintf(date2, "%8s", "19900101");
    return (SUCCESS);
  }
  memcpy(Year, date1, 4);
  Year[4] = '\0';
  if (atoi(Year) < 1900 || atoi(Year) > 9999)
  {
    return (-1);
  }

  memcpy(Month, date1 + 5, 2);
  Month[2] = '\0';
  if (atoi(Month) < 1 || atoi(Month) > 12)
  {
    return (-2);
  }

  memcpy(Day, date1 + 8, 2);
  Day[2] = '\0';
  if (atoi(Day) < 1 || atoi(Day) > 31)
  {
    return (-3);
  }

  sprintf(date2, "%4s%2s%2s", Year, Month, Day);
  return (SUCCESS);
}

/**************************************************************
 ** ������      : _swStrCmp
 ** ��  ��      : ���ַ���s1�в���s2
 ** ��  ��      : llx
 ** ��������    : 1999/11/6
 ** ����޸�����: 2000/3/10
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : s1 : �ַ���
		  s2 : �ַ���
 ** ����ֵ      : -1,0:�ַ���s1��û��s2	
                  �ɹ��򷵻�s2��s1�еĸ���.
***************************************************************/
int _swStrCmp(s1,s2)
char *s1,*s2;
{
  short i,j,k,l,sub;
  char ch[2],*p,*begin;
  char s10[200];

  i = strlen(s1);
  j = strlen(s2);
  strncpy(s10,s1,i);
  s10[i] = '\0';
  l     = 0;
  sub   = 0;
  p     = s1;
  begin = s1;
  if ( i < j ) return(-1);

  if ( i == j )
  {
    k = strcmp(s1,s2);
    if  (k == 0) return(1);
    if  (k != 0) return(0);
  }

  if (i > j )
  {
    ch[0] = s2[0];
    ch[1] = '\0';
    while ( *p != '\0' )
    {
      if (*p != ch[0] )
      {
        p++;
        continue;
      }
      if (*p == ch[0] )
      {
        k = strncmp(p,s2,strlen(s2));
        if ( k == 0 )
        {
          sub++;
          p=p+strlen(s2);
          continue;
        }
        else 
        {
          p++;
          continue;
        }
      }
    }
  }
  return(sub);
}

/**************************************************************
 ** ������      : _swStrReplace
 ** ��  ��      : ��s1�е�s2�滻��ָ����s3.,�������ַ���s4
 ** ��  ��      : llx
 ** ��������    : 1999/11/6
 ** ����޸�����: 2000/3/10
 ** ������������: StrCmp
 ** ȫ�ֱ���    :
 ** ��������    : s1 : ԭ��
		  s2 : Ҫ�滻���ַ���
		  s3 : �滻����ַ���
		  s4 : ����ַ���
 ** ����ֵ      : �ɹ��򷵻��滻�ĸ���
                  -1: s1��û��s2	
                  -2: strlen(s2) != strlen(s3)
***************************************************************/
int  _swStrReplace(s1,s2,s3,s_back)
char *s1,*s2,*s3,*s_back;
{
  char *p,*begin;
  char buffer[200];
  char ch[2];
  short have,sub,k,wz;

  if (strlen(s2) != strlen(s3) ) return(-2);

  p = s1;
  begin = s1;
  have = _swStrCmp(p,s2);

  if ( have <= 0  ) return(-1);

  sub = 0;
  ch[0]=s2[0];
  ch[1] = '\0';
  while ( *p != '\0' )
  {
    if (*p != ch[0] )
    {
      wz=p-begin;
      buffer[wz]=*p;
      p++;
      continue;
    }
    if (*p == ch[0] )
    {
      k = strncmp(p,s2,strlen(s2));
      if ( k == 0 )
      {
        strcat(buffer,s3);
        p=p+strlen(s3);
        continue;
      }
      else 
      {
        wz=p-begin;
        buffer[wz]=*p;
        p++;
        continue;
      }
    }
  }
  memcpy(s_back,buffer,strlen(s1));
  return(have);
}


#if 0
/**************************************************************
 ** ������      : swDebug
 ** ��  ��      : �Ǹ�����־:��ӡָ����Ϣ
 ** ��  ��      : lv lixiao
 ** ��������    : 1999/11/19
 ** ����޸�����:
 ** ������������: GetTime
 ** ȫ�ֱ���    : agDebugfile
 ** ��������    : frm,va_alist :�÷���ʽͬprintf
 ** ����ֵ      : -1:������־�ļ�����. 0:�ɹ�
***************************************************************/
/*"ifdef" modified by qh 20070416*/
#ifdef OS_LINUX
/* del by gengling at 2015.03.31 one line PSBC_V1.0 */
/* int swVdebug(short iDebug,char *frm, ...) */
/* add by gengling at 2015.03.31 one line PSBC_V1.0 */
int _swVdebug(short iDebug,char *filename,int line,char *frm, ...)
#endif
#ifdef OS_AIX
/* del by gengling at 2015.03.31 three lines PSBC_V1.0 */
/* int swVdebug(frm,va_list)
char *frm
va_dcl */
/* add by gengling at 2015.03.31 one line PSBC_V1.0 */
int _swVdebug(iDebug,filename,line,frm,va_list);
short iDebug
char *filename
int line
char *frm
va_dcl
#endif
#ifdef OS_HPUX
/* del by gengling at 2015.03.31 one line PSBC_V1.0 */
/* int swVdebug(short iDebug,char *frm, ...) */
/* add by gengling at 2015.03.31 one line PSBC_V1.0 */
int _swVdebug(short iDebug,char *filename,int line,char *frm, ...)
#endif
#ifdef OS_SCO
/* del by gengling at 2015.03.31 one line PSBC_V1.0 */
/* int swVdebug(short iDebug,char *frm, ...) */
/* add by gengling at 2015.03.31 one line PSBC_V1.0 */
int _swVdebug(short iDebug,char *filename,int line,char *frm, ...)
#endif
#ifdef OS_SOLARIS
/* del by gengling at 2015.03.31 one line PSBC_V1.0 */
/* int swVdebug(short iDebug,char *frm, ...) */
/* add by gengling at 2015.03.31 one line PSBC_V1.0 */
int _swVdebug(short iDebug,char *filename,int line,char *frm, ...)
#endif
{
  static struct swt_sys_config sSwt_sys_config;
  static short ilTrace_flag = 0;
  static long llLogfilesize = 0;
  FILE *fp;
  /*modify by pc 2008-3-19 10:36*/
  char buf[iMSGMAXLEN],fname1[100],buf1[iMSGMAXLEN],alCmdbuf[200];
  /*modify by pc 2008-3-19 10:36*/
  /*char buf[1024],fname1[100],buf1[1024],alCmdbuf[200];*/
  va_list  ap;
  struct tm  *sttm;
  int ilRc;
  long llDatetime;
  short ilRn;
  struct stat sgbuf;
  int rc;
  
  if ( cgDebug < iDebug ) return(0);

  memset(buf,0x00,sizeof(buf));
  memset(buf1,0x00,sizeof(buf1));
  memset(fname1,0x00,sizeof(fname1));
  if (*frm) 
  {
/*"ifdef" modified by qh 20070416*/
#ifdef OS_LINUX
    va_start( ap, frm );
#endif
#ifdef OS_AIX
    va_start(ap);
#endif
#ifdef OS_HPUX
    va_start( ap, frm );
#endif
#ifdef OS_SCO
    va_start( ap, frm );
#endif
#ifdef OS_SOLARIS
    va_start( ap, frm );
#endif
    vsprintf(buf, frm, ap);
    va_end(ap);
  }

  if (!llLogfilesize)
  {
    ilRc = 0; 
    if ((ilRc == FAIL) || (ilRc == 0)) llLogfilesize = 8388608L;
    else llLogfilesize = ilRc * 1048576L;
  }

  /*0����־ͳһ�����ϵͳ��־�ļ���add by cjh 20150317, PSBC_V1.0*/
  if( iDebug == 0 )
  	sprintf(fname1,"%s/%s",getenv("SWITCH_DEBUG_PATH"),"swSyslog.debug");
  else
  	sprintf(fname1,"%s/%s",getenv("SWITCH_DEBUG_PATH"),agDebugfile);
	
  rc=stat(fname1, &sgbuf);
  if (sgbuf.st_size > llLogfilesize)
  {
    time(&llDatetime);
    sttm = localtime( &llDatetime );

    sprintf(alCmdbuf,"%s.%02d%02d%02d%02d%02d",
      fname1,
      sttm->tm_mon+1,
      sttm->tm_mday,
      sttm->tm_hour,
      sttm->tm_min,
      sttm->tm_sec );
    if ((access(fname1,F_OK)) != -1)
      rename(fname1,alCmdbuf);
  }

  if (( fp = fopen(fname1,"a+")) == NULL)  return(-1);
  _swGetTime(buf1);
/*add by pc*/
        struct timeval curtime;
//char buf3[36];

/*** modify by wanghao 20150330 PSBC_V1.0***/
char buf3[80];
memset(buf3,0x00, sizeof(buf3));
/****end modify****/
gettimeofday(&curtime,NULL);
/*== begin ==add by cjh, 2015.03.26, PSBC_V1.0*/
/* mod by gengling at 2015.03.31 one line PSBC_V1.0 */
/* snprintf(buf3,sizeof(buf3),".%06d|%16s|%08s|%05d",curtime.tv_usec, procName, __FILE__, __LINE__); */
snprintf(buf3,sizeof(buf3),".%06d|%s|%ld|%s|%05d",curtime.tv_usec, procName, gettid(), filename, line);
/*== begin ==add by cjh, 2015.03.26, PSBC_V1.0*/
strcat(buf1,buf3);
/*add by pc*/
  strcat(buf1,": ");
  strcat(buf1,buf);
  fprintf(fp,"%s\n",buf1 + 5);
  fclose(fp);
  if ( iDebug == 0 )
  {
    if ( ilTrace_flag == 0 )
    {
      ilTrace_flag = 1;
      memset((char *)&sSwt_sys_config,0x00,sizeof(struct swt_sys_config));
      ilRn = swShmcfgload(&sSwt_sys_config);
      if (ilRn)
      {
        swDebug("S6291: [����/��������] swShmcfgload()����,������=%d",ilRn);
        return(FAIL);
      }
      signal(SIGTTOU,SIG_IGN);
      if (freopen(sSwt_sys_config.trace_term,"a+",stderr)==NULL)
      {
        swDebug("S6292: [����/����] %s δ���ӻ�Ȩ�޲���,���޸ĸ��ն˵�Ȩ��.",
          sSwt_sys_config.trace_term);
      }
    }
    fprintf(stderr,"%s\n",buf1 + 5);
  }

  return(0);
}

/*"ifdef" modified by qh 20070416*/
#ifdef OS_LINUX
int swDebug(char *frm, ...)
#endif
#ifdef OS_AIX
int swDbug(frm,va_list)
char *frm
va_dcl
#endif
#ifdef OS_HPUX
int swDebug(char *frm, ...)
#endif
#ifdef OS_SCO
int swDebug(char *frm, ...)
#endif
#ifdef OS_SOLARIS
int swDebug(char *frm, ...)
#endif
{
  static long llLogfilesize = 0;
  FILE *fp;
  char buf[1024],fname1[100],buf1[1024],alCmdbuf[200];
  va_list  ap;
  struct tm  *sttm;
  int ilRc;
  long llDatetime;
  struct stat sgbuf;
  int rc;
  char alFilename2[100]; 

  memset(buf,0x00,sizeof(buf));
  memset(buf1,0x00,sizeof(buf1));
  memset(fname1,0x00,sizeof(fname1));
  if (*frm) 
  {
/*"ifdef" modified by qh 20070416*/
#ifdef OS_LINUX
    va_start( ap, frm );
#endif
#ifdef OS_AIX
    va_start(ap);
#endif
#ifdef OS_HPUX
    va_start( ap, frm );
#endif
#ifdef OS_SCO
    va_start( ap, frm );
#endif
#ifdef OS_SOLARIS
    va_start( ap, frm );
#endif
    vsprintf(buf, frm, ap);
    va_end(ap);
  }

  if (!llLogfilesize)
  {
    ilRc = 0; 
    if ((ilRc == FAIL) || (ilRc == 0)) llLogfilesize = 8388608L;
    else llLogfilesize = ilRc * 1048576L;
  }

  sprintf(fname1,"%s/%s",getenv("SWITCH_DEBUG_PATH"),agDebugfile);
  rc=stat(fname1, &sgbuf);
  if (sgbuf.st_size > llLogfilesize)
  {
    time(&llDatetime);
    sttm = localtime( &llDatetime );
    sprintf(alFilename2,"%s.%02d%02d%02d%02d%02d",
      fname1,
      sttm->tm_mon+1,
      sttm->tm_mday,
      sttm->tm_hour,
      sttm->tm_min,
      sttm->tm_sec );
    if ((access(fname1,F_OK)) != -1)
      rename(fname1,alFilename2);  /* �Ȳ��ж�ִ�н�� */      
  }

  if (( fp = fopen(fname1,"a+")) == NULL)  return(-1);
  _swGetTime(buf1);
  strcat(buf1,": ");
  strcat(buf1,buf);
  fprintf(fp,"%s\n",buf1 + 5);
  fclose(fp);
  return(0);
}

#endif
/**************************************************************
 ** ������      : swMberror
 ** ��  ��      : ���������.�Ѵ�����Ϣ����debug�ļ�,ͬʱ���ط�����
 **               ��û�д���,�򲻼�debug�ļ�,������ر���.
 ** ��  ��      : lv lixiao
 ** ��������    : 1999/11/19
 ** ����޸�����: 2000/3/10
 ** ������������: swDebug,qwrite
 ** ȫ�ֱ���    : iMSGMESSAGE
 ** ��������    : rc:��������ķ�����. msgpack:����
 ** ����ֵ      : rc
***************************************************************/
int swMberror(int rc,char *msgpack)
{
  struct msgpack slMsgpack;
  struct msghead *pslMsghead;
  unsigned int ilMsglen;

  if ( rc == 0 )  
    return(0);

  pslMsghead = (struct msghead *)msgpack;

  memset(&slMsgpack,0x00,sizeof(struct msgpack));
  if ( msgpack )
  {
    ilMsglen = pslMsghead->iBodylen + sizeof(struct msghead);
    if (ilMsglen > sizeof(struct msgpack))
      memcpy(&slMsgpack,msgpack,sizeof(struct msgpack));
    else
      memcpy(&slMsgpack,msgpack,ilMsglen);
  }
  else 
  {
    slMsgpack.sMsghead.iBodylen = 0;
  }

  slMsgpack.sMsghead.iMsgtypeorg = slMsgpack.sMsghead.iMsgtype;
  slMsgpack.sMsghead.iMsgtype = iMSGMESSAGE;
  slMsgpack.sMsghead.lCode = 351099;
  _swMsgsend(351099,(char *)&slMsgpack);

  return(rc);
}

/**************************************************************
 ** ������      : swDebugfml
 ** ��  ��      : �ڸ����ļ��д�ӡFML���ĵ�����
 ** ��  ��      : ʷ����
 ** ��������    : 1999/11/24
 ** ����޸�����: 1999/11/24
 ** ������������: GetTime,swFmlunpack
 ** ȫ�ֱ���    :
 ** ��������    : msgpack     ����ı���(������ͷ)
 ** ����ֵ      : SUCCESS/FAIL
***************************************************************/
int swDebugfml(char *msgpack)
{
  FILE *fp;
  char fname1[80],buf[100];
  struct msgpack *pslMsgpack;
  union preunpackbuf prebuf[iBUFFLDNUM];
  short i = 0,ilRc;

  pslMsgpack = (struct msgpack *)msgpack;
  ilRc = swFmlunpack(pslMsgpack->aMsgbody,pslMsgpack->sMsghead.iBodylen,prebuf);
  if (ilRc != SUCCESS) return(FAIL);
  _swGetTime(buf);
  sprintf(fname1,"%s/%s",getenv("SWITCH_DEBUG_PATH"),agDebugfile);
  if ( (fp=fopen(fname1,"a+")) == NULL) return(FAIL);
  fprintf(fp,"%s *** FML����(��ʼ) ***\n",buf + 5);
  while(prebuf[i].sImfbuf.aFldname[0] != '\0')
  {
    /* next add by nh 2002.3.19*/
    /* �ؼ������ */
/*    if(!strcmp(prebuf[i].sImfbuf.aFldname+strlen(prebuf[i].sImfbuf.aFldname)-1,"*")) */
    if(prebuf[i].sImfbuf.aFldname[strlen(prebuf[i].sImfbuf.aFldname)-1]=='*')
    {
      fprintf(fp,"%10s(%03d) = %s\n",prebuf[i].sImfbuf.aFldname,6,"******"); 
    }
    else
    {
      fprintf(fp,"%10s(%03d) = %s\n",prebuf[i].sImfbuf.aFldname, 
		    prebuf[i].sImfbuf.iFldlen, 
		    prebuf[i].sImfbuf.aFldvalue);
    }
    /* end add by nh */
    i ++;
  }
  fprintf(fp,"               *** FML����(����) ***\n");
  fclose(fp);
  return(SUCCESS);
}

/**************************************************************
 ** ������      : swDebughex
 ** ��  ��      : ��ӡʮ�����Ƶ���־�ļ� 
 ** ��  ��      : lv lixiao
 ** ��������    : 1999/11/19
 ** ����޸�����: 2000/3/10
 ** ������������: GetTime
 ** ȫ�ֱ���    :
 ** ��������    : buff  :����ı���
 **               buflen:���ĳ���
 ** ����ֵ      : -1:�ļ��򿪳���  0:�ɹ�
***************************************************************/
int swDebughex(char * buff,unsigned int buflen)
{
  char fname1[80], buf[100];
  FILE *fp;
  short i,g,k;
  unsigned char *abuf,*hbuf;

  _swGetTime(buf);
  memset(fname1,0x00,80);
  sprintf(fname1,"%s/%s",getenv("SWITCH_DEBUG_PATH"),agDebugfile);
  if ( (fp=fopen(fname1,"a+")) == NULL) return(-1);

  hbuf=(unsigned char *)buff;
  abuf=(unsigned char *)buff;
  fprintf(fp,"%s *** ʮ�����Ʊ���(��ʼ) ***\n",buf + 5);
  for(i=0, g=buflen/16; i < g; i++)
  {
    fprintf(fp, "M(%6.6d)=< ",i*16);
    for(k=0; k<16; k++) fprintf(fp, "%02x ",*hbuf++);
    fprintf(fp,"> ");
    for(k=0; k<16; k++, abuf++)
      fprintf(fp, "%c",(*abuf>32) ? ((*abuf<128) ? *abuf : '*') : '.');
    fprintf(fp,"\n");
  }
  if((i=buflen%16) > 0)
  {
    fprintf(fp,"M(%6.6d)=< ",buflen-buflen%16);
    for(k=0; k < i; k++) fprintf(fp, "%02x ",*hbuf++);
    for(k=i; k < 16; k++) fprintf(fp, "   ");
    fprintf(fp, "> ");
    for(k=0; k < i; k++, abuf++)
      fprintf(fp, "%c",(*abuf>32) ? ((*abuf<128) ? *abuf : '*') : '.');
    fprintf(fp, "\n");
  }
  fprintf(fp,"                    *** ʮ�����Ʊ���(����) ***\n");
  fflush(fp);
  fclose(fp);
  return(0);
}

/**************************************************************
 ** ������      : swDebugmsghead
 ** ��  ��      : ��ӡ����ͷ����־�ļ�.
 ** ��  ��      : lv lixiao
 ** ��������    : 1999/11/19
 ** ����޸�����: 2000/3/10
 ** ������������: GetTime
 ** ȫ�ֱ���    :
 ** ��������    : bufhead:����ͷ
 ** ����ֵ      : -1:�ļ��򿪳���  0:�ɹ�
***************************************************************/
int swDebugmsghead(char *bufhead)
{
  FILE *fp;
  char fname1[80],buf[100],buf1[100];
  struct msghead slMsghead;
  memcpy(&slMsghead,bufhead,sizeof(struct msghead));

  _swGetTime(buf);
  memset(fname1,0x00,80);
  sprintf(fname1,"%s/%s",getenv("SWITCH_DEBUG_PATH"),agDebugfile);
  if ( (fp=fopen(fname1,"a+")) == NULL) return(-1);
  fprintf(fp,"%s *** ����ͷ(��ʼ) ***\n",buf + 5);
  fprintf(fp,"lTranid         [%ld]\n",slMsghead.lTranid);
  fprintf(fp,"lBegintime      [%ld]\n",slMsghead.lBegintime);
  fprintf(fp,"iBegin_q_id     [%d]\n",slMsghead.iBegin_q_id);
  memset(buf1,0x00,100);
  memcpy(buf1,slMsghead.aBegin_trancode,sizeof(slMsghead.aBegin_trancode));
  fprintf(fp,"aBegin_trancode [%s]\n",buf1);
  memset(buf1,0x00,100);
  memcpy(buf1,slMsghead.aTrancode,sizeof(slMsghead.aTrancode));
  fprintf(fp,"aTrancode       [%s]\n",buf1);
  fprintf(fp,"iTranstep       [%d]\n",slMsghead.iTranstep);
  fprintf(fp,"iMsgtype        [%d]\n",slMsghead.iMsgtype);
  fprintf(fp,"iMsgtypeorg     [%d]\n",slMsghead.iMsgtypeorg);
  fprintf(fp,"iOrg_q          [%d]\n",slMsghead.iOrg_q);
  fprintf(fp,"iDes_q          [%d]\n",slMsghead.iDes_q);
  fprintf(fp,"cFormatter      [%c]\n",slMsghead.cFormatter);
  fprintf(fp,"cRouter         [%c]\n",slMsghead.cRouter);
  fprintf(fp,"cCodetype       [%c]\n",slMsghead.cCodetype);
  fprintf(fp,"iMsgformat      [%d]\n",slMsghead.iMsgformat);
  fprintf(fp,"cEndflag        [%c]\n",slMsghead.cEndflag);
  fprintf(fp,"iRouteline      [%d]\n",slMsghead.iRouteline);
  fprintf(fp,"lCode           [%ld]\n",slMsghead.lCode);
  fprintf(fp,"iBodylen        [%d]\n",slMsghead.iBodylen);
  fprintf(fp,"cSafflag        [%c]\n",slMsghead.cSafflag);
  fprintf(fp,"lSafid          [%ld]\n",slMsghead.lSafid);
  /*modify by zcd 20141223
  fprintf(fp,"iFmtgroup       [%d]\n",slMsghead.iFmtgroup);*/
  fprintf(fp,"iFmtgroup       [%ld]\n",slMsghead.iFmtgroup);
  fprintf(fp,"iRevmode        [%d]\n",slMsghead.iRevmode);
  /*modify by zcd 20141223
  fprintf(fp,"iRevfmtgroup    [%d]\n",slMsghead.iRevfmtgroup);
  fprintf(fp,"iSwitchgroup    [%d]\n",slMsghead.iSwitchgroup);*/
  fprintf(fp,"iRevfmtgroup    [%ld]\n",slMsghead.iRevfmtgroup);
  fprintf(fp,"iSwitchgroup    [%ld]\n",slMsghead.iSwitchgroup);
  fprintf(fp,"lSwitchtranid   [%ld]\n",slMsghead.lSwitchtranid);
  fprintf(fp,"aSysattr        [%s]\n",slMsghead.aSysattr);/* add by nh 20020923*/
  fprintf(fp,"call_flag       [%c]\n",slMsghead.call_flag);/* add by baiqj 20150319,PSBC_V1.0*/
  memset(buf1,0x00,100);
  memcpy(buf1,slMsghead.timeconsum,sizeof(slMsghead.timeconsum));
  fprintf(fp,"timeconsum      [%s]\n",buf1);/* add by baiqj20150319,PSBC_V1.0*/
  fprintf(fp,"log_level       [%d]\n",slMsghead.log_level);/* add by baiqj20150402,PSBC_V1.0*/
  memset(buf1,0x00,100);
  memcpy(buf1,slMsghead.aMemo,sizeof(slMsghead.aMemo));
  fprintf(fp,"aMemo           [%s]\n",buf1);
  fprintf(fp,"                    *** ����ͷ(����) ***\n");
  fclose(fp);
  return(0);
}

/**************************************************************
 ** ������      : _swMsgsend
 ** ��  ��      : ��������������ģ�鷢��Ϣ����               
 ** ��  ��      : llx
 ** ��������    : 1999/11/22
 ** ����޸�����: 2000/3/10
 ** ������������: qwrite,swMberror
 ** ȫ�ֱ���    :
 ** ��������    : code : ��Ϣ����
		  msg  : ����(������ͷ)
 ** ����ֵ      : SUCCESS,FAIL
***************************************************************/
int  _swMsgsend(long code,char *msg)
{
  struct msghead *pslMsghead;
  struct msgpack slMsgpack;
  unsigned int ilMsglen,ilRc;
  int ilSockfd;
  static char alAddr[16];
  static int ilPort = -1;

  pslMsghead = (struct msghead *)msg;

  if ( msg )
  {
    ilMsglen = pslMsghead->iBodylen + sizeof(struct msghead);
    if (ilMsglen > sizeof(struct msgpack))
      memcpy(&slMsgpack,msg,sizeof(struct msgpack));
    else
      memcpy(&slMsgpack,msg,ilMsglen);
  }
  else 
  {
    slMsgpack.sMsghead.iBodylen = 0;
  }
  slMsgpack.sMsghead.iMsgtypeorg = slMsgpack.sMsghead.iMsgtype;
  slMsgpack.sMsghead.iMsgtype = iMSGMESSAGE;
  slMsgpack.sMsghead.lCode = code;
  if (slMsgpack.sMsghead.lBegintime == 0) time(&slMsgpack.sMsghead.lBegintime);
  ilMsglen = slMsgpack.sMsghead.iBodylen + sizeof(struct msghead);

  if (ilPort == -1)
  {        
    ilRc = swShmselect_swt_sys_config(&sgSwt_sys_config);
    if (ilRc) return(FAIL);
    strcpy(alAddr,sgSwt_sys_config.sysmonaddr);
    ilPort = sgSwt_sys_config.sysmonport;
  }
   
  if ((ilSockfd = _swUDPCreate(0)) == -1) return(FAIL);
  
  ilRc = _swUDPPut(ilSockfd,alAddr,ilPort,(char *)&slMsgpack,ilMsglen);
  _swUDPClose(ilSockfd);
   
  if (ilRc)
    return(FAIL);
  else
    return(SUCCESS); 
}

/**************************************************************
 ** ������      : _swOrdsend
 ** ��  ��      : ����������������ģ�鷢����� 
 ** ��  ��      : llx
 ** ��������    : 1999/11/22
 ** ����޸�����: 2000/3/10
 ** ������������: qwrite,swMberror
 ** ȫ�ֱ���    :
 ** ��������    : code : ��Ϣ����
		  msg  : ����(������ͷ)
		  q_id : ����ķ��������
 ** ����ֵ      : SUCCESS,FAIL
***************************************************************/
int  _swOrdsend(long code,char *msg,short q_id)
{
  struct msghead *pslMsghead;
  struct msgpack slMsgpack;
  unsigned int ilMsglen,ilRc;

  pslMsghead = (struct msghead *)msg;

  memset(&slMsgpack,0x00,sizeof(struct msgpack));
  if ( msg )
  {
    ilMsglen = pslMsghead->iBodylen + sizeof(struct msghead);
    if (ilMsglen > sizeof(struct msgpack))
      memcpy(&slMsgpack,msg,sizeof(struct msgpack));
    else
      memcpy(&slMsgpack,msg,ilMsglen);
  }
  else 
  {
    slMsgpack.sMsghead.iBodylen = 0;
  }
  slMsgpack.sMsghead.iMsgtypeorg = slMsgpack.sMsghead.iMsgtype;
  slMsgpack.sMsghead.iMsgtype = iMSGORDER;
  slMsgpack.sMsghead.lCode = code;
  if (slMsgpack.sMsghead.lBegintime == 0) time(&slMsgpack.sMsghead.lBegintime);
  ilRc = qwrite((char *)&slMsgpack,slMsgpack.sMsghead.iBodylen + sizeof(struct msghead),q_id);
  if (ilRc != SUCCESS)
  {
    swMberror(ilRc,NULL);
    return (FAIL);
  }
  return (SUCCESS);
}

/**************************************************************
 ** ������      : _swGetOneField
 ** ��  ��      : �� Buffer ��ȡ��n���ֶη���Field�С�
                  Buffer�е�ÿ���ֶ���Seperator Ϊ�ָ��ޡ�
                  n �� 1 ��ʼ��
 ** ��  ��      : llx
 ** ��������    : 1999/11/22
 ** ����޸�����: 2000/3/10
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : Buffer   : ԭ��
		  n        : ���
                  Field    : Ŀ����
		  Seperator: �ָ���
 ** ����ֵ      : SUCCESS
***************************************************************/
int _swGetOneField(char *Buffer,int n,char * Field,char Seperator)
{
  int i, len;
  char *p, *begin;

  i = 0 ;
  p = Buffer;

  while ( *p != '\0' &&  i != n-1) 
  {
    if ( *p == Seperator ) i++;
		p++;
  }
  if ( *p == '\0') 
  {
    Field[0] ='\0';

    return(0);
  }
  begin = p;
  while ( *p != '\0' &&  *p != Seperator) p++;
  len = p - begin;
  if ( len != 0 )
  {
    memcpy(Field, begin, len);
    Field[len] = '\0';
    return(len);
  }

  else 
  {
    Field[0] ='\0';
    return(0);
  }
}

/***************************************************************
 ** ������      : _swGet_field
 ** ��  ��      : �������ļ��ж�ȡָ����ֵ��ֵ
 ** ��  ��      : 
 ** ��������    : 2001/05/26
 ** ����޸�����: 2001/05/26
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : 
 ** ����ֵ      : 0:�ɹ� / -1:ʧ��
***************************************************************/
short _swGet_field(char *aFile, char *aKey, char *aValue)
{
  char alBuf[257],alKey[20],*clTmp;
  FILE *fp;
  
  if ((fp = fopen(aFile, "r")) == NULL) return -1;
  alBuf[0] = '\0';
  while (fgets(alBuf, sizeof(alBuf), fp) != NULL)
  {
    _swTrim(alBuf);
    if (strlen(alBuf) == 0 || alBuf[0] == '#') continue;
    if ((clTmp = strchr(alBuf,'#')) != NULL) *clTmp = '\0';
    sscanf(alBuf,"%s %s",alKey,aValue);  
    if (strcmp(alKey,aKey) == 0)
    {
      fclose(fp); 
      return 0;
    }
    alBuf[0] = '\0';
  }
  fclose(fp);
  return -1;
}
/***************************************************************
 ** ������      : _swDebugflag
 ** ��  ��      : ��ȡTrace����
 ** ��  ��      : 
 ** ��������    : 2001/05/26
 ** ����޸�����: 2002/08/20 ֧��SWITCH_CFGDIR��·��
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : 
 ** ����ֵ      : >=0:�ɹ� / -1:ʧ��
***************************************************************/
int _swDebugflag(char *flag)
{
  FILE *fp;
  int ilDebug;
  char alBuf[256],alKey[20],*p;

  cgDebug=0;
  if(!(fp=_swFopen("debug.ini","r"))) return(-1);
  while(fgets(alBuf,sizeof(alBuf),fp)!=NULL)
  {
    _swTrim(alBuf);
    if((strlen(alBuf)==0)||(alBuf[0]=='#')) continue;
    /*if(p = strchr(alBuf,'#'))  modiby nh 20020928 */
    if((p = strchr(alBuf,'#')) != NULL)  
      *p = '\0'; 
    sscanf(alBuf,"%s %d",alKey,&ilDebug);
    if(!strcmp(alKey,flag))
    {
      fclose(fp);
      return(ilDebug);
    }
  }
  fclose(fp);
  return -1;
}

/**************************************************************
 ** ������: swFmlunpack
 ** ��  ��: ��FML���Ĳ����Ԥ���
 ** ��  ��: ʷ����
 ** ��������: 1999.11.22
 ** ����޸�����: 1999.11.22
 ** ������������:
 ** ȫ�ֱ���:
 ** ��������: msgbody	����׼������ı�����(��������ͷ)
 **           msglen	���뱨�ĵĳ���
 **           prebuf	Ԥ����洢buf
 ** ����ֵ: �ɹ�	0
 **         ʧ��(���ȳ���)	-1
***************************************************************/
int swFmlunpack(char *msgbody,unsigned int msglen,union preunpackbuf *prebuf)
{
  int i = 0,j;
  /*modify by zcd 20141230
  short ilPos = 0,ilLen;*/
  unsigned int ilPos = 0; 
  short ilLen;

  /* memset(prebuf,0x00,sizeof(union preunpackbuf) * iBUFFLDNUM);*/ 
  while(ilPos < msglen)
  {
    for(j=0;j <= iFLDNAMELEN;j++)
    {
      if (msgbody[ilPos] == '\0') 
      {
        ilPos++;
        break;
      }
      prebuf[i].sImfbuf.aFldname[j] = msgbody[ilPos];
      ilPos++;
    }
    if(msgbody[ilPos-1] != '\0')
    {
      swVdebug(0,"S6020: [����/����] ��FML����ʱ��������>[%d],���� %s",iFLDNAMELEN,
        prebuf[i].sImfbuf.aFldname);
      return(FAIL);
    }
    prebuf[i].sImfbuf.aFldname[j] = '\0';

/*   ilLen = *(short *)(msgbody + ilPos); */ /*delete by zjj 2004 */

    memcpy(&ilLen,msgbody + ilPos,sizeof(short));
    
    /*modify by zcd 20141230
    ilPos = ilPos + sizeof(short);*/
    ilPos = ilPos + sizeof(short);
    
/* added by fzj at 2003.07.04 */    
    if((ilLen<0)||(ilPos>msglen)||(ilPos+ilLen >msglen))
    {
      swVdebug(0,"S6025: FML���Ĳ�����"); 
      return(FAIL);
    }
/**/    
    for (;;)
    {
      if (ilLen <= iPERFLDVALUELEN)
      {
        prebuf[i].sImfbuf.iFldlen = ilLen;
        memcpy(prebuf[i].sImfbuf.aFldvalue,msgbody + ilPos,ilLen);
        prebuf[i].sImfbuf.aFldvalue[ilLen]='\0';
        break;
      }
      strcpy(prebuf[i + 1].sImfbuf.aFldname,prebuf[i].sImfbuf.aFldname);
      prebuf[i].sImfbuf.iFldlen = iPERFLDVALUELEN;
      memcpy(prebuf[i].sImfbuf.aFldvalue,msgbody + ilPos,iPERFLDVALUELEN);
      prebuf[i].sImfbuf.aFldvalue[iPERFLDVALUELEN]='\0';
      ilPos = ilPos + iPERFLDVALUELEN;
      ilLen = ilLen - iPERFLDVALUELEN;
      i++;
    }
    ilPos = ilPos + prebuf[i].sImfbuf.iFldlen;
    i++;
  }
  prebuf[i].sImfbuf.aFldname[0] = '\0';
  return(SUCCESS);
}

/**************************************************************
 ** ������: swFmlpack
 ** ��  ��: ��Ԥ��������FML����
 ** ��  ��: ʷ����
 ** ��������: 1999.11.22
 ** ����޸�����: 1999.11.22
 ** ������������:
 ** ȫ�ֱ���:
 ** ��������: prebuf	Ԥ���
 **           msgbody	��ɵ�FML����
 **           msglen	FML���ĵĳ���
 ** ����ֵ: �ɹ�	0
 **         ʧ��(���ȳ���)	-1
***************************************************************/
int swFmlpack(union preunpackbuf *prebuf,char *msgbody,unsigned int *msglen)
{
  int i = 0;
  /*modify by zcd 20141230
  short ilPos = 0,ilTmp = 0,ilLen,ilCount = 0;*/
  unsigned int ilPos = 0,ilTmp = 0,ilLen;
  short ilCount = 0;

  while((prebuf[i].sImfbuf.aFldname[0] != '\0') && (i <= iBUFFLDNUM))
  {
    ilLen = strlen(prebuf[i].sImfbuf.aFldname);
    /*  2001.03.07  add by */ 
    if (ilLen > iFLDNAMELEN)
    {
      swVdebug(0,"S6030:[����/����] �����������̫��,���� %s",prebuf[i].sImfbuf.aFldname);
       return(FAIL);
    }
  /* add end */

    memcpy(msgbody + ilPos,prebuf[i].sImfbuf.aFldname,ilLen);
    ilPos = ilPos + ilLen;
    msgbody[ilPos] = '\0';
    ilPos ++;

    ilCount = 0;
    ilTmp = ilPos + sizeof(short);
    for(;;)
    {
      if (strcmp(prebuf[i].sImfbuf.aFldname,prebuf[i+1].sImfbuf.aFldname) != 0)
      {
        ilLen = prebuf[i].sImfbuf.iFldlen + ilCount * iPERFLDVALUELEN;
        /*del by zcd 20141230
        memcpy(msgbody + ilPos, (char *)(&ilLen),sizeof(short));
        ilPos = ilPos + sizeof(short);
        ***end of del by zcd 20141230*/

        /*add by zcd 20141230*/
        memcpy(msgbody + ilPos, (char *)(&ilLen),sizeof(short));
        ilPos = ilPos + sizeof(short);
        /*end of add  by zcd 20141230*/

        ilPos = ilPos + ilCount * iPERFLDVALUELEN;
        memcpy(msgbody + ilPos,prebuf[i].sImfbuf.aFldvalue, 
          prebuf[i].sImfbuf.iFldlen);
        break;
      }
      
      memcpy(msgbody + ilTmp,prebuf[i].sImfbuf.aFldvalue, iPERFLDVALUELEN);
      ilTmp = ilTmp + iPERFLDVALUELEN;
      ilCount++;
      
      i++;
    }

    ilPos = ilPos + prebuf[i].sImfbuf.iFldlen;
    i++;
  }
  *msglen = ilPos;
  return(SUCCESS);
}

/**************************************************************
 ** ������: swFmlget
 ** ����: �õ�FMLԤ����ָ�������ĳ��Ⱥ���ֵ
 ** ����: ʷ����
 ** ��������: 1999.11.23
 ** ����޸�����: 1999.11.23
 ** ������������:
 ** ȫ�ֱ���:
 ** ��������: prebuf    Ԥ���
 **           fldname   ָ��������
 **           fldlen    ������ĳ���
 **           fldval    ���ص���ֵ
 ** ����ֵ:
***************************************************************/
int swFmlget(union preunpackbuf *prebuf,char *fldname,
  short *fldlen,char *fldval)
{
  int i = 0;
  short ilFldlen, ilFlag = 0;

  ilFldlen = 0;
  *fldlen = 0;
  
  while((prebuf[i].sImfbuf.aFldname[0] != '\0') && (i <= iBUFFLDNUM))
  {
    if(strcmp(fldname,prebuf[i].sImfbuf.aFldname) != 0)
    {
      i ++;
      if (ilFlag == 1)
      {
      	break;
      }
      else
      {
        continue;
      }  
    }
    ilFlag = 1;
    ilFldlen = prebuf[i].sImfbuf.iFldlen;
    memcpy(fldval+(*fldlen),prebuf[i].sImfbuf.aFldvalue,ilFldlen);
    *fldlen = *fldlen + ilFldlen;
    i ++;
  }
  
  fldval[*fldlen] = '\0';
  if (ilFlag == 0)
    return(FAIL);
  else
    return(SUCCESS);
}
/**************************************************************
 ** ������: swFmlset
 ** ��  ��: ����FMLԤ����ָ�������ĳ��Ⱥ���ֵ
 ** ��  ��: ʷ����
 ** ��������: 1999.11.23
 ** ����޸�����: 1999.11.23
 ** ������������:
 ** ȫ�ֱ���:
 ** ��������: fldname   ָ��������
 **           fldlen    ��ĳ���
 **           fldval    ��ֵ
 **           prebuf    Ԥ���
 ** ����ֵ:
***************************************************************/
int swFmlset(char *fldname,short fldlen,char *fldval,union preunpackbuf *prebuf)
{
  int i = 0,j=0,m,ilNeed=0,ilStart=-1;  
  short ilLen,ilOrglen,ilPos=0,ilRestlen=0;
  short ilCount=0;

  ilLen = strlen(fldname);
  
  if (ilLen > iFLDNAMELEN)
  {
    swVdebug(0,"S6040: [����/����] ����Fml��������̫��,���� %s",prebuf[i].sImfbuf.aFldname);
    return(FAIL);
  }
  ilRestlen = fldlen;
  ilOrglen = fldlen;
  ilCount = (fldlen - 1)/iPERFLDVALUELEN + 1;
  
  /* ����λ�� */
  while((prebuf[i].sImfbuf.aFldname[0] != '\0') && (i <= iBUFFLDNUM))
  {
    if (strcmp(fldname,prebuf[i].sImfbuf.aFldname) != 0)
    {
      i ++;
      continue;
    }
    if (j==0) ilStart = i;
    j ++;
    i ++;
  }
  if (ilStart == -1) ilStart = i;
  
  /* ����λ */
  ilNeed = ilCount - j;
  
  if ((i+ilNeed) > iBUFFLDNUM)
  {
    swVdebug(0,"S6050: [����/����] ��������������[%d]",iBUFFLDNUM);
    return(FAIL);
  }
  
  if (ilNeed>0)
  {
    for(m=i;m>=ilStart+j;m--)
    {
      memcpy((char *)&(prebuf[m+ilNeed].sImfbuf),(char *)&(prebuf[m].sImfbuf),
        sizeof(struct imfbuf));
    }
  }
  else if (ilNeed<0)
  {
    for(m=ilStart+j;prebuf[m].sImfbuf.aFldname[0] != '\0';m++)
    {
      memcpy((char *)&(prebuf[m+ilNeed].sImfbuf),(char *)&(prebuf[m].sImfbuf),
        sizeof(struct imfbuf));
    }
    prebuf[m+ilNeed].sImfbuf.aFldname[0] = '\0';
  }
  
  /* �������� */
  for(;;)
  {
    if (ilRestlen <= iPERFLDVALUELEN)
    {
      strcpy(prebuf[ilStart].sImfbuf.aFldname,fldname);
      prebuf[ilStart].sImfbuf.iFldlen = ilRestlen;
      memcpy(prebuf[ilStart].sImfbuf.aFldvalue,fldval+ilPos,ilRestlen);
      prebuf[ilStart].sImfbuf.aFldvalue[ilRestlen] = '\0';
      break;
    }
    strcpy(prebuf[ilStart].sImfbuf.aFldname,fldname);
    prebuf[ilStart].sImfbuf.iFldlen = iPERFLDVALUELEN;
    memcpy(prebuf[ilStart].sImfbuf.aFldvalue,fldval+ilPos,iPERFLDVALUELEN);
    prebuf[ilStart].sImfbuf.aFldvalue[iPERFLDVALUELEN] = '\0';
    ilRestlen = ilRestlen - iPERFLDVALUELEN;
    ilPos = ilPos + iPERFLDVALUELEN;
    ilStart ++;
  }
  return(SUCCESS);
}

/**************************************************************
 ** ������: swFmlpackget
 ** ��  ��: �õ�FML������ָ�������ĳ��Ⱥ���ֵ
***************************************************************/
/*modify by zcd 20141230
int swFmlpackget(char *aMsgbody, short iMsglen, char *aFldname, short *piFldlen, char *aFldval)*/
int swFmlpackget(char *aMsgbody, int iMsglen, char *aFldname, short  *piFldlen, char *aFldval)
{
 /*modify by zcd 20141230
  short  ilPos=0,ilLen;*/
  unsigned int ilPos=0;
  short ilLen;  /*change by dyw*/
  char * alFldname;
  /* swVdebug(0,"s99999");*/
  while (ilPos < iMsglen)
  {
    alFldname = aMsgbody + ilPos;
    /*swVdebug(0,"alFldname=[%s],aFldname=[%s]",alFldname,aFldname); */
    if (strlen(alFldname) > iFLDNAMELEN)
    {
      swVdebug(0,"S6060: [����/����] FML������������[%s]",alFldname);
       return(FAIL);
    }
    if (strcmp(alFldname, aFldname) == 0)
    {
      ilPos = ilPos + strlen(alFldname) + 1;
      memcpy(piFldlen,aMsgbody + ilPos,iSHORTLEN);
      /* modify by nh 20040412
      *piFldlen = *(short *)(aMsgbody + ilPos); */
      ilPos = ilPos + sizeof(short);
      memcpy(aFldval, aMsgbody + ilPos, *piFldlen);
      return(SUCCESS);
    }
    else
    {
      ilPos = ilPos + strlen(alFldname) + 1;
      memcpy(&ilLen,aMsgbody + ilPos,iSHORTLEN);
      /* modify by nh 20040412
      ilLen = *(short *)(aMsgbody + ilPos); */
      ilPos = ilPos + sizeof(short) + ilLen;
     }
  }
  return FAIL;
} 

/**************************************************************
 ** ������: swFmlpackset
 ** ��  ��: ����FML������ָ�������ĳ��Ⱥ���ֵ
***************************************************************/
/*modify by zcd 20141230
int swFmlpackset(char *aMsgbody, short *piMsglen, char *aFldname, short iFldlen, char *aFldval)*/
int swFmlpackset(char *aMsgbody, int *piMsglen, char *aFldname, short iFldlen, char *aFldval)
{
  
  /*modify by zcd 20141230
  short ilPos=0,ilLen,ilSize;*/
  unsigned int ilPos=0,ilSize;
  short ilLen; /*change by dyw*/
  char *alFldname;
  char alTmp[iMSGMAXLEN - sizeof(struct msghead)];
 
  if(*piMsglen < 0)
  {
    swVdebug(0,"S6070: [����/����] ��Чƫ����[%d]",*piMsglen);
    return(FAIL);
  }
  
  if(strlen(aFldname) > iFLDNAMELEN)
  {
    swVdebug(0,"S6080: [����/����] FML������������[%s]",aFldname);
    return(FAIL);
  }
  while (ilPos < *piMsglen)
  {
    alFldname = aMsgbody + ilPos;
    if(strlen(alFldname) > iFLDNAMELEN)
    {
      swVdebug(0,"S6090: [����/����] FML������������[%s]",alFldname);
       return(FAIL);
    }   
    if(strcmp(alFldname, aFldname) == 0)
    {
      ilPos = ilPos + strlen(alFldname) + 1;
      
      /*modify by pc 2008-3-7 9:28
      ilLen = *(short *)(aMsgbody + ilPos);
      */
      memcpy(&ilLen,aMsgbody + ilPos,iSHORTLEN);
      
      ilSize = *piMsglen - ilPos - sizeof(short) - ilLen;
      memcpy(alTmp, (aMsgbody + ilPos + sizeof(short) + ilLen), ilSize);
      memcpy(aMsgbody + ilPos, (char *)&iFldlen, sizeof(short));
      ilPos = ilPos + sizeof(short);
      memcpy(aMsgbody + ilPos, aFldval, iFldlen);
      ilPos = ilPos + iFldlen;
      memcpy(aMsgbody + ilPos,alTmp,ilSize);
      *piMsglen = *piMsglen + iFldlen - ilLen;
      return(SUCCESS);
    }
    else
    {
      ilPos = ilPos + strlen(alFldname) + 1;
      /*modify by pc 2008-3-7 9:28
      ilLen = *(short *)(aMsgbody + ilPos);
      */
      memcpy(&ilLen,aMsgbody + ilPos,iSHORTLEN);
      ilPos = ilPos + sizeof(short) + ilLen;
    }
  }
  memcpy((aMsgbody + *piMsglen), aFldname, strlen(aFldname));
  aMsgbody[*piMsglen + strlen(aFldname)] = '\0';

  memcpy(aMsgbody + *piMsglen + strlen(aFldname) + 1, 
    (char *)&iFldlen, sizeof(short)); 
  memcpy(aMsgbody + *piMsglen + strlen(aFldname) + sizeof(short) + 1,
    aFldval, iFldlen); 
  *piMsglen = *piMsglen +  strlen(aFldname) + 1 + sizeof(short) + iFldlen; 

  return(SUCCESS) ;
}

/*******************************************************************/
/* �� �� ��       :  _swTimeLongToChs                               */
/* ��    ��       :  �������͵�ʱ��ת��Ϊ�ַ�����                  */
/* ��������       :  iDateTime �����͵�ʱ��                        */
/*                   pcFormat ʱ��ĸ�ʽ                           */
/* �� �� ֵ       :  pcDateTime �ַ����͵�ʱ��                     */
/*******************************************************************/
int _swTimeLongToChs( lDateTime , pcDateTime , pcFormat )
long    lDateTime;
char   *pcDateTime;
char   *pcFormat;
{
  struct tm  *sttm;

  memset( pcDateTime, 0x00, sizeof( pcDateTime ) );

  sttm = localtime( &lDateTime );

  if( strcmp( pcFormat, "YYMMDD" ) == 0 )
  {
    if (lDateTime)
    {
      sprintf( pcDateTime,"%02d%02d%02d",
/* ======== delete by dgm 2001/10/15 ========== 
		    sttm->tm_year,
===== end of delete by dgm 2001/10/15 ====== */

/* ======== add by dgm 2001/10/15 ========== */
		    sttm->tm_year > 99 ? sttm->tm_year - 100 : sttm->tm_year,
/* ===== end of add by dgm 2001/10/15 ====== */
		    sttm->tm_mon+1,
		    sttm->tm_mday);
    }
    else
    {
      strcpy(pcDateTime,"------");
    }
  }
  else if ( strcmp( pcFormat, "YYYYMMDD" ) == 0 )
  {
    if (lDateTime)
    {
      sprintf( pcDateTime,"%04d%02d%02d",
		    sttm->tm_year+1900,
		    sttm->tm_mon+1,
		    sttm->tm_mday);
    }
    else
    {
      strcpy(pcDateTime,"--------");
    }
  }
  else if ( strcmp( pcFormat, "YYYYMMDDHHMMSS" ) == 0 )
  {
    if (lDateTime)
    {
      sprintf( pcDateTime,"%04d%02d%02d%02d%02d%02d",
		    sttm->tm_year+1900,
		    sttm->tm_mon+1,
		    sttm->tm_mday,
		    sttm->tm_hour,
		    sttm->tm_min,
		    sttm->tm_sec );
    }
    else
    {
      strcpy(pcDateTime,"--------------");
    }
  }
  else if ( strcmp( pcFormat, "YYYY-MM-DD HH:MM:SS" ) == 0 )
  {
    if (lDateTime)
    {
      sprintf( pcDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		    sttm->tm_year+1900,
		    sttm->tm_mon+1,
		    sttm->tm_mday,
		    sttm->tm_hour,
		    sttm->tm_min,
		    sttm->tm_sec );
    }
    else
    {  
      strcpy(pcDateTime,"----/--/-- --:--:--");
    }
  }
  else if ( strcmp( pcFormat, "HHMMSS" ) == 0 )
  {
    if (lDateTime)
    {
      sprintf( pcDateTime,"%02d%02d%02d",
		    sttm->tm_hour,
		    sttm->tm_min,
		    sttm->tm_sec );
    }
    else
    {
      strcpy(pcDateTime,"------");
    }
  }
  else if ( strcmp( pcFormat, "HH:MM:SS" ) == 0 )
  {
    if (lDateTime)
    {
      sprintf( pcDateTime,"%02d:%02d:%02d",
		    sttm->tm_hour,
		    sttm->tm_min,
		    sttm->tm_sec );
    }
    else
    {
      strcpy(pcDateTime,"--:--:--");
    }
  }
  else if ( strcmp( pcFormat, "MMDD" ) == 0 )
  {
    if (lDateTime)
    {
      sprintf( pcDateTime,"%02d%02d",
		    sttm->tm_mon+1,
		    sttm->tm_mday);
    }
    else
    {
      strcpy(pcDateTime,"----");
    }
  }
  else if ( strcmp( pcFormat, "MMDDHHMMSS" ) == 0 )
  {
    if (lDateTime)
    {
      sprintf( pcDateTime,"%02d%02d%02d%02d%02d",
		    sttm->tm_mon+1,
		    sttm->tm_mday,
		    sttm->tm_hour,
		    sttm->tm_min,
		    sttm->tm_sec );
    }
    else
    {
      strcpy(pcDateTime,"----------");
    }
  }
  else
  {
    if (lDateTime)
    {
      sprintf( pcDateTime,"%04d-%02d-%02d %02d:%02d:%02d",
		    sttm->tm_year+1900,
		    sttm->tm_mon+1,
		    sttm->tm_mday,
		    sttm->tm_hour,
		    sttm->tm_min,
		    sttm->tm_sec );
    }
    else
    {
      strcpy(pcDateTime,"----/--/-- --:--:--");
    }
  }
  return(0);
}


/**************************************************************
 ** ������: _swFmlunion
 ** ��  ��: ��2��FMLԤ���ȡ����,���Ե�2��Ԥ���Ϊ��
 ** ��  ��: ʷ����
 ** ��������: 1999.11.23
 ** ����޸�����: 1999.11.23
 ** ������������:
 ** ȫ�ֱ���:
 ** ��������: prebuf1	Ԥ���1(��Ž��)
 **           prebuf2	Ԥ���2(Ϊ��)
 ** ����ֵ:
***************************************************************/
int _swFmlunion(union preunpackbuf *prebuf1,union preunpackbuf *prebuf2)
{
  short ilRc,i = 0;
  short ilFldlen,ilCount=0;
  char  alValue[iFLDVALUELEN+1];

  while((prebuf2[i].sImfbuf.aFldname[0] != '\0') && (i <= iBUFFLDNUM))
  {
    swFmlget(prebuf2,prebuf2[i].sImfbuf.aFldname,&ilFldlen,alValue);
    ilCount = (ilFldlen - 1)/iFLDVALUELEN + 1;
    ilRc = swFmlset(prebuf2[i].sImfbuf.aFldname,ilFldlen,alValue,prebuf1);
    if (ilRc != SUCCESS) return(FAIL);
    i = i + ilCount;
  }
  return(SUCCESS);
}

/**************************************************************
 ** ������: swGetlog
 ** ��  ��: ���ݱ����ڽ�����־�ļ��е���ʼƫ�����ͱ��ĵĳ���,
 **         �õ�������ˮ��־�еı���
 ** ��  ��: ʷ����
 ** ��������:     1999.12.19
 ** ����޸�����: 2000.02.25 ��4KΪһ���ļ���λ,�����ļ��ֽ��������Ϊ��׺�Ķ��С�ļ�,���Ч��
 **              �ļ����Ķ�̬��ʽ ,ÿ���ļ����ڳ��Ȳ�����8K
 ** �޸���: ����
 ** ������������:
 ** ȫ�ֱ���:
 ** ��������: logfile   ��������ŵ��ļ���,����·��   [char *]
 **           logoffset	��������־�ļ��е���ʼƫ����  [short]
 **           loglen	���ĵĳ���                    [long]
 **           msgpack	���ȡ�õı��ķ���ֵ          [char *]
 ** ����ֵ:   SUCCESS	�ɹ�
 **           FAIL	ʧ��
***************************************************************/

int swGetlog(char * logfile,unsigned int loglen,long logoffset,char *msgpack)
{
  FILE *fp;
  long llLen;
  char amFp_name[80];

  memset(amFp_name,0x00,80);
  _swTrim(logfile);
  sprintf(amFp_name,"%s/%s",getenv("SWITCH_LOG_PATH"),logfile);
  fp=fopen(amFp_name,"r");
  if (fp==NULL) return(FAIL);
  llLen = fseek(fp,logoffset,SEEK_SET);
  if (llLen) return(FAIL);
  llLen = fread(msgpack,sizeof(char),loglen,fp);
  fclose(fp);
  if (llLen > 0) 
    return (SUCCESS);
  else 
    return (FAIL);
}

/**************************************************************
 ** ������: �潻����ˮ��־���ĺ���
 ** ��  ��: ������ı������ݴ�ŵ�������־�ļ���,�����ر��Ĵ��
 **         ����ʼƫ��λ��
 ** ��  ��: ʷ����
 ** ��������:     1999.12.19
 ** ����޸�����:2000.02.25 ��4MΪһ���ļ���λ,
 ** �����ļ��ֽ��������Ϊ��׺�Ķ��С�ļ�,���Ч��
 ** �ļ����Ķ�̬��ʽ, ǰ׺Ϊ���̺�,��׺Ϊ����,�ɸú����Զ��ж�׷�ӻ��´���
 ** �޸���: ����
 ** ������������: 
 ** ȫ�ֱ���:
 ** ��������: msgpack	����ı������ݼ�׼��д��־������             [char *]
 **           loglen    ����ı������ݳ��ȼ�׼��д��־�����ݳ���     [long]
 **           logoffset ���ر��Ĵ���ڽ�����־�ļ��е���ʼƫ��λ��   [long]
 **           logfile   ���ر��Ĵ�ŵĽ�����־�ļ���                 [char *]
 ** ����ֵ:   SUCCESS    �ɹ�
 **           FAIL       ʧ��
***************************************************************/
int swPutlog(char *msgpack,unsigned int loglen,long *logoffset,char * logfile)
{

  static unsigned short ilFileid = 1;  /*��̬�ļ�����׺��ֵ��ʼֵ*/
  long llLen;
  FILE *fp;
  unsigned long ilPid;
  char amFp_name[80];
  static short ilFlag=0; /*  add by nh 20021102 */

  ilPid = getpid();

  memset(amFp_name,0x00,80);
  /* modify by  nh 20021102
  sprintf(amFp_name,"%s/%ld.%d",getenv("SWITCH_LOG_PATH"),ilPid,ilFileid);
  */
  sprintf(amFp_name,"%s/%06lx%04x",getenv("SWITCH_LOG_PATH"),ilPid,ilFileid);

  while (TRUE)
  {
    if(ilFlag == 0)
    {
      fp=fopen(amFp_name,"w+");
    }
    else
      fp=fopen(amFp_name,"a+");
    if(fp==NULL)
    {
      swVdebug(0,"S6100: [����/����] �޷����ļ�[%s]",amFp_name);
      return(FAIL);
    }

    llLen=fseek(fp, 0, SEEK_END);
    if ( llLen )
    {
      swVdebug(0,"S6110: [����/ϵͳ����] fseek()����,errno=%ld",llLen);
      return(FAIL);
    }
    *logoffset = ftell(fp);
    if ((*logoffset) >= LOGFILESIZE)
    {
      ilFileid++;
      ilFlag = 0;
      /* modify by  nh 20021102 */
      if(ilFileid >= 65530)
      {
        ilFileid = 1;
        swVdebug(2,"S6120: [������Ϣ]����ƽ̨������־��¼�Ѿ��ﵽ���,������!");
      }
      fclose(fp);
      memset(amFp_name,0x00,80);
      sprintf(amFp_name,"%s/%06lx%04x",getenv("SWITCH_LOG_PATH"),
                          ilPid,ilFileid);
    }
    else
    {
      ilFlag = 1;
      break;
    }
  }

  memset(logfile,0x00,11);
  /*  modify by  nh 20021102
  sprintf(logfile,"%ld.%d",ilPid,ilFileid);
  */
  sprintf(logfile,"%06lx%04x",ilPid,ilFileid);

  llLen=fwrite(msgpack, 1, loglen, fp);
  if ( llLen <= 0)
  {
    swVdebug(0,"S6130: [����/ϵͳ����] fwrite()����,errno=%ld",llLen);
    return(FAIL);
  }
  fclose(fp);

  return(SUCCESS);
}

/**************************************************************
 ** ������      : _swItou
 ** ��  ��      : ��������ת��Ϊ�޷��Ŷ�����
 ** ��  ��      : zzh
 ** ��������    : 2001/03/06
 ** ����޸�����: 
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : ism : ��������
                  usm : �޷��Ŷ�����
 ** ����ֵ      : SUCCESS
***************************************************************/
/* ============ delete by dgm 2001/10/25 ==========
int _swItou(short ism,unsigned short usm)
{
  if ( ism < 0 ) 
    usm = ism + 65536;
  else 
    usm = ism;
                   
  return(SUCCESS);
}
========= end of delete by dgm 2001/10/25 ======== */

/**************************************************************
 ** ������: _swUtoi
 ** ��  ��: ���޷��Ŷ�����ת��Ϊ������
 ** ��  ��: zzh
 ** ��������    : 2001/03/06
 ** ����޸�����:
 ** ������������:
 ** ȫ�ֱ���    :
 ** ��������    : ism : ��������
                  usm : �޷��Ŷ�����
 ** ����ֵ      : SUCCESS
***************************************************************/
/* ============ delete by dgm 2001/10/25 ==========
int _swUtoi(unsigned short usm,short ism)
{
        if (usm > 32767) 
          ism = usm - 65536;
        else 
          ism = usm;

        return(SUCCESS);
}
========= end of delete by dgm 2001/10/25 ======== */

/**************************************************************
 ** ������      : _swClrMb
 ** ��  ��      : ������
 ** ��  ��      : byz
 ** ��������    : 2001/03/06
 ** ����޸�����:
 ** ������������:
 ** ȫ�ֱ���    :
 ** ��������    :
 ** ����ֵ:
***************************************************************/
int  _swClrMb(int iQid)
{
  int  iRet=0;
  short  ilOrgqid;
  short  ilPriority = 0;
  short  ilClass = 0;
  short  ilType = 0;
  unsigned int ilMsglen=iMSGMAXLEN;
  char slMsgpack[iMSGMAXLEN];

  while(1)
  {
    ilMsglen = iMSGMAXLEN;
    ilClass = 0;
    ilType = 0;
    ilPriority = 0;
    iRet = qreadnw( (char *)&slMsgpack, &ilMsglen, &ilOrgqid,&ilPriority, &ilClass, &ilType, 1 );

    if(iRet == 0 ) continue;
    else if( (iRet == BMQ__NOMOREMSG) || (iRet == BMQ__TIMEOUT)) break;
    else 
    {
      swVdebug(0,"S6140:read mailbox failed =  %d",iRet);
      return(FAIL);
    }
  }
  return( SUCCESS );
}

/**************************************************************
 ** ������      : _swSetdefault
 ** ��  ��      : ���ֶ����ó�ȱʡֵ
 ** ��  ��      : 
 ** ��������    : 
 ** ����޸�����:
 ** ������������:
 ** ȫ�ֱ���    :
 ** ��������    : 1. fld_buf: �����õ��ֶ�ָ��
 **             : 2. fld_len: �ֶγ��� 
 ** ����ֵ:
***************************************************************/
int _swSetdefault(fld_buf, fld_len, fld_type)
unsigned char *fld_buf;
short fld_len;
char fld_type;
{
  int i;
  char tmpbuf[iFLDVALUELEN];
  
  memset(tmpbuf,'\0',sizeof(tmpbuf));

  switch(fld_type)
  {
    case 'A':
    case 'O':
      for(i=0;i<fld_len;i++) fld_buf[i]=' ';
      break;
    case 'P':
	if ( strlen( (char *)fld_buf ) == 0)
	   {
	   memset( fld_buf, '0', fld_len);
	   break;
	   }
	for ( i=0;i< fld_len;i++)
	    {
		if ( fld_buf[i] == ' ')
		    fld_buf[i] = '0';
	    }
	break;
    case 'S': 
        if ( strlen( (char *)fld_buf ) == 0 )
	   {
      	    for(i=0;i<fld_len;i++) fld_buf[i]='0';
	    break;
	   }
        for ( i=0; i< fld_len; i++)
	    {
	     if ( fld_buf[i] == ' ' || fld_buf[i] == 0 )
		  fld_buf[i] = '0';
            }
	break;
    case 'M':
	for ( i=0;i< fld_len-strlen((char *)fld_buf);i++)
	    tmpbuf[i] = '0';
	memcpy( tmpbuf+i, fld_buf, strlen((char *)fld_buf));
	memcpy( fld_buf, tmpbuf, fld_len);
	break;
  }
  return(0);
}

/**************************************************************
 ** ������      : _swChan
 ** ��  ��      :
 ** ��  ��      : 
 ** ��������    : 
 ** ����޸�����:
 ** ������������:
 ** ȫ�ֱ���    :
 ** ��������    :
 ** ����ֵ:
**************************************************************/
int _swChan(char *pp,int len)
{
  int i,num,lab,ilLen;
  int sum;

  num=0;
  sum=0;
  lab=0;
  for(i=0;i<len;i++)
  {
    if((pp[i]==0x0e)||(pp[i]==0x0f))
    {
      sum++;lab=i;	
    }
  }
  if((sum==0)||((sum%2)==0)) 
  {
    ilLen=len;
    goto END;
  }
  num=len-1-lab;

  if(num==1)
  {
    pp[len-2]='\0';
    ilLen=len-2;
  }
  else
  {	
    if(num==2) {pp[len-3]='\0';ilLen=len-3;}
    else
    {	
      if((num%2)==0) 
      {
	pp[len-2]=0x0f;
	pp[len-1]='\0';
	ilLen=len-1;
      }
      else {pp[len-1]=0x0f;ilLen=len;}
    }
  }
  END: pp[len]='\0';

  return(ilLen);
}

/* qinyong 06/05/2001 add */


#if 0
/***************************************************************
 ** ������      : _swChgvalue
 ** ��  ��      : ����Լ���Ĺ���ȡ�ַ�����ֵ
 ** ��  ��      :
 ** ��������    : 2001/05/26
 ** ����޸�����: 2001/05/26
 ** ������������:
 ** ȫ�ֱ���    :
 ** ��������    :
 ** ����ֵ      : 0:�ɹ� / -1:ʧ��
***************************************************************/
short _swChgvalue(char *aValue)
{
  char alBuf[257];

  _swTrim(aValue);
  if ((aValue[0] == '"') && (aValue[strlen(aValue) - 1] == '"'))
  {
    strcpy(alBuf,aValue);
    strcpy(aValue,alBuf+1);
    aValue[strlen(aValue) - 1] = '\0';
  }
  return 0;
}

#endif /***delete by wanghao move to swUtil.c 20150416***/
/***************************************************************
 ** ������      : _swGetitemmode1
 ** ��  ��      : ��һ����¼(���ڸ�ʽһ��ŵ������ļ�)
 ** ��  ��      : 
 ** ��������    : 2001/05/26
 ** ����޸�����: 2001/05/26
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : 
 ** ����ֵ      : 0:�ɹ� / -1:ʧ��
***************************************************************/
short _swGetitemmode1(FILE *fp, char paKey[][iFLDVALUELEN], 
  char paFldname[][iFLDNAMELEN], char paFldvalue[][iFLDVALUELEN])
{
  static char palKey[iKEYNUMBER][iFLDVALUELEN];
  static int ilRec = 0,k = 0;
  short i,j,ilPos,ilPos_old;
  char alBuf[iFLDVALUELEN];
  char *alTmp;
  int ilRc;

  i = 0;
  while (fgets(alBuf, sizeof(alBuf), fp) != NULL)
  {
/* ========= begin of add by dgm 2002.3.19 ========== */
    ilRc = swMacrorepl(alBuf);
    if(ilRc)
    {
      swVdebug(0,"S6150: [����/����] �궨���滻ʧ��! [buf=%s]",alBuf);
      return(FAIL);
    }
/* ========== end of add by dgm 2002.3.19 =========== */
    _swTrim(alBuf);
    if (strlen(alBuf) == 0 || alBuf[0] == '#')
    {
      continue;
    }
    if ((alTmp = strchr(alBuf, '#')) != NULL) *alTmp='\0';
    if ((alBuf[0] == '[') && (alBuf[strlen(alBuf) - 1] == ']'))
    {
      for (j=0; j<iKEYNUMBER; j++)
        strcpy(paKey[j],palKey[j]);      
        
      /* Key field */
      alBuf[strlen(alBuf) - 1] = ',';
      j = 0;
      ilPos = ilPos_old = 1;
      while (alBuf[ilPos] != '\0')
      {
        if (alBuf[ilPos] == ',' )
        {
          memcpy(palKey[j],alBuf + ilPos_old,ilPos - ilPos_old);
          palKey[j][ilPos - ilPos_old] = '\0';
          _swTrim(palKey[j]);
          ilPos_old = ilPos + 1;
          j ++;
        }  
        ilPos ++;
      }  
      if (ilRec > 0)
      {
        ilRec ++;
        return 0;
      }
      ilRec ++;
    }

    if ((alTmp = strchr(alBuf, '=')) == NULL) continue;
    strcpy(paFldvalue[i],alTmp + 1);
    *alTmp = '\0';
    strcpy(paFldname[i],alBuf);

    _swChgvalue(paFldvalue[i]);
    _swChgvalue(paFldname[i]);

    _swTrim(paFldvalue[i]);
    _swTrim(paFldname[i]);
    paFldname[i+1][0] = '\0';
    i ++;
  }
  k ++;
  if (k == 2)
  {
    k = 0;
    return (-1);
  }
  else
  {
    for (j=0; j<iKEYNUMBER; j++)
      strcpy(paKey[j],palKey[j]);
        
    ilRec = 0;
    return 0;
  }  
}

/***************************************************************
 ** ������      : _swGetvaluemode1
 ** ��  ��      : ��һ����¼��ĳ����ֵ(���ڸ�ʽһ��ŵ������ļ�)
 ** ��  ��      : 
 ** ��������    : 2001/05/26
 ** ����޸�����: 2001/05/26
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    : 
 ** ����ֵ      : 0:�ɹ� / -1:ʧ��
***************************************************************/
short _swGetvaluemode1(char paFldname[][iFLDNAMELEN],
  char paFldvalue[][iFLDVALUELEN], const char *aFldname, char *aFldvalue)
{
  int i;
  char alTmp[iFLDVALUELEN];

  strcpy(alTmp, aFldname);
  _swTrim(alTmp);

  for (i = 0;;i++)
  {
    if (paFldname[i][0] == '\0') return -1;
    _swTrim(paFldname[i]);
    if (strcmp(alTmp,paFldname[i]) == 0)
    {
      strcpy(aFldvalue,paFldvalue[i]);
      return 0;
    }
  }
}

/***************************************************************
 ** ������      : _swGetitemmode2
 ** ��  ��      : ��һ����¼(���ڸ�ʽ����ŵ������ļ�)
 ** ��  ��      : 
 ** ��������    : 2001/05/26
 ** ����޸�����: 2001/05/26
 ** ������������: swGetitemmode2
 ** ȫ�ֱ���    :
 ** ��������    : 
 ** ����ֵ      : 0:�ɹ� / -1:ʧ��
***************************************************************/
short _swGetitemmode2(FILE *fp, char paFldvalue[][iFLDVALUELEN], short iFldmaxnum)
{
  char alBuf[257];
  char *alTmp;
  short ilPos1, ilPos, j, ilFlag;
  int ilRc;

  while (1)
  {
    if (fgets(alBuf, sizeof(alBuf), fp) != NULL)
    {
/* ========= begin of add by dgm 2002.3.19 ========== */
      ilRc = swMacrorepl(alBuf);
      if(ilRc)
      {
        swVdebug(0,"S6160: [����/����] �궨���滻ʧ��! [buf=%s]",alBuf);
        return(FAIL);
      }
/* ========== end of add by dgm 2002.3.19 =========== */
      _swTrim(alBuf);
      if (strlen(alBuf) == 0 || alBuf[0] == '#') continue;
      if ((alTmp = strchr(alBuf,'#')) != NULL) *alTmp = '\0';
      ilPos = ilPos1 = j = ilFlag = 0;
      while (1)
      {
        if (alBuf[ilPos] == '"')
        {
          if (ilFlag == 0)
            ilFlag = 1;
          else
            ilFlag = 0;
        }
        if (ilFlag == 0 && 
          (alBuf[ilPos] == ' ' || alBuf[ilPos] == '\t' ||
           alBuf[ilPos] == '\n' || alBuf[ilPos] == '\0'))
        {
           if (j >= iFldmaxnum)
           {
             swVdebug(0,"S6170: [����/����] ���������Ԥ���ռ�[%d]", iFldmaxnum);
             return(0);
           }
           strncpy(paFldvalue[j],alBuf + ilPos1,ilPos - ilPos1);
           if (paFldvalue[j][0] == '-')
             paFldvalue[j][0] = '\0';
           paFldvalue[j][ilPos - ilPos1] = '\0';
           if (strcmp(paFldvalue[j],"-") == 0)
             paFldvalue[j][0] = '\0';
           _swChgvalue(paFldvalue[j]);    /* add by szhengye */
           ilFlag = 0;
           j ++;
           if (alBuf[ilPos] == '\0') break;
           while ((alBuf[ilPos] == ' ') || (alBuf[ilPos] == '\t')
             || (alBuf[ilPos] == '\n'))
           {
             ilPos ++;
           }
           ilPos1 = ilPos;
           continue;
        }
        else
        {
          ilPos ++;
        }
      }
      if (j < iFldmaxnum)
        paFldvalue[j][0] = '\0';
      else
        swVdebug(0,"S6180: [����/����] ���������Ԥ���ռ�[%d]", iFldmaxnum);
      return 0;
    }
    else
      return (-1);
  }
}

int _swUDPCreate(int iPort)
{
  int sockfd;
  struct sockaddr_in slAddr;

  if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
  {
    return -1;
  }

  slAddr.sin_family = AF_INET;
  slAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  slAddr.sin_port = htons(iPort); 

  if(bind(sockfd,(struct sockaddr *)&slAddr,sizeof(struct sockaddr_in)) == -1)
  {
    _swUDPClose(sockfd);
    return -1;
  }

  return sockfd;
}

int _swUDPClose(int sock)
{
  close(sock);
  return(0);
}

int _swUDPGet(int sock,char *aAddr,int *iPort,char *aMsgpack,unsigned int *iMsglen)
{
  struct sockaddr_in slSockaddr;
  int ilLen = *iMsglen;
  #ifdef OS_SCO
    int ilFromlen = sizeof(struct sockaddr_in);
  #else
    socklen_t ilFromlen = sizeof(struct sockaddr_in);
  #endif

  *iMsglen = recvfrom(sock,aMsgpack,ilLen,
         0,(struct sockaddr *)&slSockaddr,&ilFromlen);

  if(*iMsglen == -1)
    return(-1);

  strcpy(aAddr,(char *)inet_ntoa(slSockaddr.sin_addr));
  *iPort = ntohs(slSockaddr.sin_port);
  
  return(0);
}

/*int _swUDPPut(int sock,char *aAddr,short iPort,char *aMsgpack,short iMsglen)*/
int _swUDPPut(int sock,char *aAddr,int iPort,char *aMsgpack,unsigned int iMsglen)
{
  struct sockaddr_in slSockaddr;
  struct hostent *host;
  int ilTolen = sizeof(struct sockaddr_in);
  int ilRc;

  if(!(host = gethostbyname(aAddr)))
  {
    if(!(host = gethostbyaddr(aAddr,strlen(aAddr),AF_INET)))
      return -1;
  }

  slSockaddr.sin_family = AF_INET;
  memcpy((char*)&slSockaddr.sin_addr,host->h_addr,host->h_length);
  slSockaddr.sin_port = htons(iPort);
  ilRc = sendto(sock,aMsgpack,iMsglen,0,
        (struct sockaddr *)&slSockaddr,ilTolen);
  if(ilRc == -1)
    return -1;

  return(0);
}

/***************************************************************
 ** ������: _swPresql
 ** ��  ��: ��̬SQL������ 
 ** ��  ��:
 ** ��������    : 2001/05/26
 ** ����޸�����: 2001/05/26
 ** ������������: _swExstr
 ** ȫ�ֱ���    :
 ** ��������    :
 ** ����ֵ: 0:�ɹ� / -1:ʧ��
***************************************************************/
int _swPresql(char *aSqlstr,char *aSqltext,char paSqlval[10][iFLDVALUELEN + 1],short *iFldnum,char *alSqlflag)
{
  char *alPos1;
  char *alPos2;
  char alSqlval[iFLDVALUELEN + 1];
  char alSqlstr[iFLDVALUELEN + 1];
  char alFldexpress[2*iFLDVALUELEN + 1],alTmpexp[iFLDVALUELEN+1];
  char alFldvalue[iFLDVALUELEN + 1];
  char alBuf[iFLDVALUELEN + 1];
  short ilFldlen,ilRc;
  short i = 0;

  strcpy(alSqlstr,aSqlstr);
  for(;;)
  {
    /* ȥ�����������ȡSQL��� */
    alPos1 = strrchr(alSqlstr,'{');
    if (!alPos1) break;
    alPos2 = strchr(alPos1,'}');
    if (!alPos2) break;
    memset(alFldexpress,0x00,sizeof(alFldexpress));
    /* strncpy(alFldexpress,alPos1 + 1,alPos2 - alPos1 - 1); */
    /* delete   by wangpan 2001/11/29                        */
    /* modified by wangpan 2001/11/29                        */
    memset(alTmpexp,0x00,sizeof(alTmpexp));
    strncpy(alTmpexp,alPos1 + 1,alPos2 - alPos1 - 1);
    ilRc = _swExpN2T(alTmpexp,alFldexpress);
    if(ilRc!=0) return(ilRc);
    /* end modify by wangpan 2001/11/29                      */

    ilRc = _swExpress(alFldexpress,alFldvalue,&ilFldlen);
    if (ilRc) 
    {
      swVdebug(3,"S6190: �������ʽ[%s]����",alFldexpress);
        return(ilRc);
    }
    if (alPos1 && alPos2)
    {
      strcpy(alBuf,alPos2 + 1);
      strncpy(alPos1,alFldvalue,ilFldlen);
      strcpy(alPos1 + ilFldlen,alBuf);
    }
  }
  ilRc = _swExstr(alSqlstr,alSqlflag);
  if (ilRc)
  {
    swVdebug(3,"S6200: SQL���[%s]���Ϸ�",alSqlstr);
    return(ilRc);
  }

  switch (alSqlflag[0])
  {
    case 'S':	/* SELECT */
      alPos1 = strstr(alSqlstr,"INTO ");
      if (!alPos1)
        return (FAIL);
      if ( alSqlflag[0] == 'S')
      {
        alPos2 = strstr(alPos1,"FROM ");
        if (!alPos1)
          return (FAIL);
        strcpy(aSqltext,alSqlstr);
        strcpy(aSqltext+(alPos1-alSqlstr),alPos2); 
        memset(alSqlval,0x00,sizeof(alSqlval));
        strncpy(alSqlval,alPos1 + 5 ,alPos2 - alPos1 - 5);
        _swTrim(aSqltext);
        _swTrim(alSqlval);
        alPos1 = alSqlval;
      }
      for (;;)
      {
        alPos2 = strchr(alPos1,',');
        if (!alPos2) break;
        memset(paSqlval[i],0x00,iFLDVALUELEN + 1);
        strncpy(paSqlval[i],alPos1,alPos2 - alPos1);
        _swTrim(paSqlval[i]);
        i++;
        alPos1 = alPos2 + 1;
      }
      if (strlen(alPos1) >= 0)
      {
        memset(paSqlval[i],0x00,iFLDVALUELEN + 1);
        strcpy(paSqlval[i],alPos1);
        _swTrim(paSqlval[i]);
        i++;
      }
      *iFldnum = i ;
      break;

    case 'O':	/* OPEN */	
      alPos1 = strstr(alSqlstr,"SELECT ");
      if (!alPos1)
        return (FAIL);
      alPos1 = alPos1 + 8;
      alPos2 = strstr(alPos1,"FROM ");
      if (!alPos2)
        return (FAIL);
      for (;;)
      {
        alPos2 = strchr(alPos1,',');
         if (!alPos2) break;
        i++;
        alPos1 = alPos2 + 1;
      }
      if (strlen(alPos1) >= 0)
          i = i + 1;
      *iFldnum = i ;
      strcpy(aSqltext,alSqlstr);
      break;

    case 'F':	/* FETCH */
      alPos1 = strstr(alSqlstr,"INTO ");
      if (!alPos1)
        return(FAIL);
      alPos1 = alPos1 + 5 ;
      for (;;)
      {
        alPos2 = strchr(alPos1,',');
        if (!alPos2) break;
        memset(paSqlval[i],0x00,iFLDVALUELEN + 1);
        strncpy(paSqlval[i],alPos1,alPos2 - alPos1);
        _swTrim(paSqlval[i]);
        i++;
        alPos1 = alPos2 + 1;
      }
      if (strlen(alPos1) >= 0)
      {
        memset(paSqlval[i],0x00,iFLDVALUELEN + 1);
        strcpy(paSqlval[i],alPos1);
        _swTrim(paSqlval[i]);
        i++;
      }
      *iFldnum = i ;
      strcpy(aSqltext,alSqlstr);
      break;

    default:
      _swTrim(alSqlstr);
      strcpy(aSqltext,alSqlstr);
      break;
  } 
  return(SUCCESS);
}

/***************************************************************
 ** ������: _swExstr
 ** ��  ��: ��̬SQL������
 ** ��  ��:
 ** ��������    : 2001/05/26
 ** ����޸�����: 2001/05/26
 ** ������������: 
 ** ȫ�ֱ���    :
 ** ��������    :
 ** ����ֵ: 0:�ɹ� / -1:ʧ��
***************************************************************/
int _swExstr(char *alStr,char *cFlag)
{
  char *alPos;
  int i;
 
  /* ��SQL���ؼ���Сдת���ɴ�д */
  alPos = strstr(alStr,"select"); 
  if (alPos)
  {
    for( i=0;i<6;i++)
      alPos[i] = alPos[i] - 32;
  }
  alPos = strstr(alStr,"update"); 
  if (alPos)
  {
    for( i=0;i<6;i++)
      alPos[i] = alPos[i] - 32;
  }
  alPos = strstr(alStr,"insert"); 
  if (alPos)
  {
    for( i=0;i<6;i++)
      alPos[i] = alPos[i] - 32;
  }
  alPos = strstr(alStr,"delete"); 
  if (alPos)
  {
    for( i=0;i<6;i++)
      alPos[i] = alPos[i] - 32;
  }
  alPos = strstr(alStr,"from"); 
  if (alPos)
  {
    for( i=0;i<4;i++)
      alPos[i] = alPos[i] - 32;
  }
  alPos = strstr(alStr,"into"); 
  if (alPos)
  {
    for( i=0;i<4;i++)
      alPos[i] = alPos[i] - 32;
  }
  alPos = strstr(alStr,"where"); 
  if (alPos)
  {
    for( i=0;i<5;i++)
      alPos[i] = alPos[i] - 32;
  }
  alPos = strstr(alStr,"open"); 
  if (alPos)
  {
    for( i=0;i<4;i++)
      alPos[i] = alPos[i] - 32;
  }
  alPos = strstr(alStr,"declare"); 
  if (alPos)
  {
    for( i=0;i<7;i++)
      alPos[i] = alPos[i] - 32;
  }
  alPos = strstr(alStr,"cursor"); 
  if (alPos)
  {
    for( i=0;i<6;i++)
      alPos[i] = alPos[i] - 32;
  }
  alPos = strstr(alStr,"for"); 
  if (alPos)
  {
    for( i=0;i<3;i++)
      alPos[i] = alPos[i] - 32;
  }
  alPos = strstr(alStr,"fetch"); 
  if (alPos)
  {
    for( i=0;i<5;i++)
      alPos[i] = alPos[i] - 32;
  }
  alPos = strstr(alStr,"close"); 
  if (alPos)
  {
    for( i=0;i<5;i++)
      alPos[i] = alPos[i] - 32;
  }

  /* ȡSQL����ʶ */
  if( !strncmp(alStr,"SELECT ",7) )
  {
    cFlag[0]='S';
    return(SUCCESS);
  }
  if( !strncmp(alStr,"UPDATE ",7) )
  {
    cFlag[0]='U';
    return(SUCCESS);
  }
  if( !strncmp(alStr,"INSERT ",7) ) 
  {
    cFlag[0]='I';
    return(SUCCESS);
  }
  if( !strncmp(alStr,"DELETE ",7) )
  {
    cFlag[0]='D';
    return(SUCCESS);
  }
  if( !strncmp(alStr,"OPEN ",5) )
  {
    cFlag[0]='O';
    return(SUCCESS);
  }
  if( !strncmp(alStr,"FETCH " ,6) )
  {
    cFlag[0]='F';
    return(SUCCESS);
  }
  if( !strncmp(alStr,"CLOSE " ,6) )
  {
    cFlag[0]='C';
    return(SUCCESS);
  }
  else
     return(FAIL);
}

/* ========= add by qy 2001/08/22 ============= */

/**************************************************************
 ** ������      ��swPortset
 ** ��  ��      �����ö˿�״̬
 ** ��  ��      ��
 ** ��������    :  2001/08/09
 ** ����޸����ڣ�
 ** ��������������
 ** ȫ�ֱ���    ��
 ** ��������    ��
 ** ����ֵ      ��SUCCESS - ƥ�� FAIL - ��ƥ��
***************************************************************/
int swPortset(short iPort, short iLevel, short iStatus)
{
  short ilRc, ilMask = 1;
  unsigned char alTmp1, alTmp2;
  struct swt_sys_queue slSwt_sys_queue;
  
  if (iLevel > 8 || iLevel < 1)
  {
    swVdebug(0,"S6210: [����/����] ���[%d]����ȷ", iLevel);
     return(FAIL);
  }
    
  ilRc = swShmselect_swt_sys_queue(iPort, &slSwt_sys_queue);
  if (ilRc != SUCCESS)
  {
    swVdebug(0,"S6220: [����/����] �˿�[%d]������", iPort);
    return(FAIL);
  }
  
  alTmp1 = slSwt_sys_queue.port_status;
    
  ilMask = ilMask << (8-iLevel);  
  
  if (iStatus == 1)
  {
    alTmp2 = ilMask;
    slSwt_sys_queue.port_status = alTmp1 | alTmp2;
  }  
  else
  {
    alTmp2 = ilMask;
    alTmp2 = ~alTmp2;
    slSwt_sys_queue.port_status = alTmp1 & alTmp2;
  }

  ilRc = swShmupdate_swt_sys_queue(iPort, slSwt_sys_queue);
  if (ilRc != SUCCESS)
  {
    swVdebug(0,"S6230:[����/����] ���¶˿�[%d]״̬ʧ��", iPort);
    return(FAIL);
  }
  return(SUCCESS);
}  

/**************************************************************
 ** ������      ��swPortget
 ** ��  ��      ����ȡ�˿�״̬
 ** ��  ��      ��
 ** ��������    : 2001/08/09
 ** ����޸����ڣ�
 ** ��������������
 ** ȫ�ֱ���    ��
 ** ��������    ��
 ** ����ֵ      ��SUCCESS - ƥ�� FAIL - ��ƥ��
***************************************************************/  
int swPortget(short iPort, short iLevel, short *iStatus)
{
  short ilRc, ilMask = 1;
  unsigned char alTmp1, alTmp2;
  struct swt_sys_queue slSwt_sys_queue;
  
  if (iLevel > 8 || iLevel < 1)
  {
    swVdebug(0,"S6240: [����/����] ���[%d]����ȷ", iLevel);
     return(FAIL);
  }
    
  ilRc = swShmselect_swt_sys_queue(iPort, &slSwt_sys_queue);
  if (ilRc != SUCCESS)
  {
    swVdebug(0,"S6250: [����/����] �˿�[%d]������", iPort);
     return(FAIL);
  }
  
  alTmp1 = slSwt_sys_queue.port_status;
    
  ilMask = ilMask << (8-iLevel);  
  alTmp2 = ilMask;
  
  if (alTmp1 & alTmp2)
    *iStatus = 1;
  else
    *iStatus = 0;
  
  return(SUCCESS);
}  

/* ========= end of add by qy 2001/08/22 ====== */


/* ========= add by dgm 2001/09/07 ====== */

/**************************************************************
 ** ������      ��swPacksep
 ** ��  ��      ��ȡ�зָ�������ָ������ֵ
 ** ��  ��      ��
 ** ��������    ��2001.9.7
 ** ����޸����ڣ�
 ** ��������������
 ** ȫ�ֱ���    ��
 ** ��������    ��
 ** ����ֵ      ��0:�ɹ� / -1:ʧ��
***************************************************************/
short swPacksep(char *aMsgbody, unsigned int iMsglen, char *aRecs, char *aGrps, char *aFlds, short iRecid, short iGrpid, short iFldid, char *aFldval)
{
  short i,j=0,ilPos,ilPosbegin=0,ilRs,ilGs,ilFs,ilRsid=0,ilGsid=0,ilFsid=0;
  unsigned int ilLen;

  for (i = 0; i <= 5; i++)
  {
    if (aRecs[i] == '\0') break;
  }
  ilRs = i;
  
  for (i = 0; i <= 5; i++)
  {
    if (aGrps[i] == '\0') break;
  }
  ilGs = i;
  
  for (i = 0; i <= 5; i++)
  {
    if (aFlds[i] == '\0') break;
  }

  ilFs = i;
  ilPos = 0;

  while (ilPos < iMsglen)
  {
    if ((memcmp(aMsgbody + ilPos,aRecs,ilRs) == 0) && (ilRs > 0))
    {
      ilLen = ilRs;
      if ((ilRsid == iRecid) && (ilGsid == iGrpid) && (ilFsid == iFldid))
      {
        memset(aFldval, 0x00, iFLDVALUELEN +1);
        memcpy(aFldval, aMsgbody + ilPosbegin, ilPos - ilPosbegin);
        return(SUCCESS);
      }
      
      ilPos = ilPos + ilLen;
      ilPosbegin = ilPos;
      j ++;
      ilRsid ++;
      ilGsid = 0;
      ilFsid = 0;
    }
    else if ((memcmp(aMsgbody + ilPos,aGrps,ilGs) == 0) && (ilGs > 0))
    {
      ilLen = ilGs;
      if ((ilRsid == iRecid) && (ilGsid == iGrpid) && (ilFsid == iFldid))
      {
        memset(aFldval, 0x00, iFLDVALUELEN +1);
        memcpy(aFldval, aMsgbody + ilPosbegin, ilPos - ilPosbegin);
        return(SUCCESS);
      }
      
      ilPos = ilPos + ilLen;
      ilPosbegin = ilPos;
      j ++;
      ilGsid ++;
      ilFsid = 0;
    }
    else if ((memcmp(aMsgbody + ilPos,aFlds,ilFs) == 0) && (ilFs > 0))
    {
      ilLen = ilFs;
      if ((ilRsid == iRecid) && (ilGsid == iGrpid) && (ilFsid == iFldid))
      {
        memset(aFldval, 0x00, iFLDVALUELEN +1);
        memcpy(aFldval, aMsgbody + ilPosbegin, ilPos - ilPosbegin);
        return(SUCCESS);
      }
      
      ilPos = ilPos + ilLen;
      ilPosbegin = ilPos;
      j ++;
      ilFsid ++;
    }
    else ilPos ++;
  }

  if ((ilRsid == iRecid) && (ilGsid == iGrpid) && (ilFsid == iFldid))
  {
    memset(aFldval, 0x00, iFLDVALUELEN +1);
    memcpy(aFldval, aMsgbody + ilPosbegin, ilPos - ilPosbegin);
    return(SUCCESS);
  }
  
  return(FAIL);
}

/* ========= end of add by dgm 2001/09/07 ====== */

/**************************************************************
 ** ������: swSepget
 ** ����: �õ�SEPԤ����ָ�������ĳ��Ⱥ���ֵ
 ** ����: 
 ** ��������: 
 ** ����޸�����:
 ** ������������:
 ** ȫ�ֱ���:
 ** ��������: prebuf    Ԥ���
              iRecid    ��¼ID
              iGrpid    ��ID
              iFldid    ��ID
 ** ����ֵ:
***************************************************************/
int swSepget(union preunpackbuf *prebuf,short iRecid,short iGrpid,short iFldid,
  short *fldlen,char *fldval)
{
  int i = 0;
  short ilFldlen, ilFlag = 0;

  ilFldlen = 0;
  *fldlen = 0;
  
  while((prebuf[i].sSepbuf.iRecid != -1) && (i <= iBUFFLDNUM))
  {
    if (prebuf[i].sSepbuf.iRecid != iRecid ||
        prebuf[i].sSepbuf.iGrpid != iGrpid ||
        prebuf[i].sSepbuf.iFldid != iFldid )
    {
      i ++;
      if (ilFlag == 1)
      {
      	break;
      }
      else
      {
        continue;
      }  
    }
    ilFlag = 1;
    ilFldlen = prebuf[i].sSepbuf.iFldlen;
    memcpy(fldval+(*fldlen),prebuf[i].sSepbuf.aFldvalue,ilFldlen);
    *fldlen = *fldlen + ilFldlen;
    i ++;
  }
  
  fldval[*fldlen] = '\0';
  if (ilFlag == 0)
    return(FAIL);
  else
    return(SUCCESS);
}
/**************************************************************
 ** ������: swSepset
 ** ��  ��: ����SEPԤ����ָ�������ĳ��Ⱥ���ֵ
 ** ��  ��:
 ** ��������:
 ** ����޸�����:
 ** ������������:
 ** ȫ�ֱ���:
 ** ��������: iRecid    ��¼ID
              iGrpid    ��ID
              iFldid    ��ID
 **           fldlen    ��ĳ���
 **           fldval    ��ֵ
 **           prebuf    Ԥ���
 ** ����ֵ:
***************************************************************/
int swSepset(short iRecid,short iGrpid,short iFldid,
  short fldlen,char *fldval,union preunpackbuf *prebuf)
{
  int i = 0,j=0,m,ilNeed=0,ilStart=-1;
  short ilOrglen,ilPos=0,ilRestlen=0;
  short ilCount=0;

  ilRestlen = fldlen;
  ilOrglen = fldlen;
  ilCount = (fldlen - 1)/iPERFLDVALUELEN + 1;
  
  /* ����λ�� */
  while((prebuf[i].sSepbuf.iRecid != -1) && (i <= iBUFFLDNUM))
  {
    if (prebuf[i].sSepbuf.iRecid != iRecid ||
        prebuf[i].sSepbuf.iGrpid != iGrpid ||
        prebuf[i].sSepbuf.iFldid != iFldid )
    {
      i ++;
      continue;
    }
    if (j==0) ilStart = i;
    j ++;
    i ++;
  }
  if (ilStart == -1) ilStart = i;
  
  /* ����λ */
  ilNeed = ilCount - j;
  
  if ((i+ilNeed) > iBUFFLDNUM)
  {
    swVdebug(0,"S6260: [����/����] ��������������[%d]",iBUFFLDNUM);
    return(FAIL);
  }
  
  if (ilNeed>0)
  {
    for(m=i;m>=ilStart+j;m--)
    {
      memcpy((char *)&(prebuf[m+ilNeed].sSepbuf),(char *)&(prebuf[m].sSepbuf),
        sizeof(struct sepbuf));
    }
  }
  else if (ilNeed<0)
  {
    for(m=ilStart+j;prebuf[m].sSepbuf.iRecid != -1;m++)
    {
      memcpy((char *)&(prebuf[m+ilNeed].sSepbuf),(char *)&(prebuf[m].sSepbuf),
        sizeof(struct sepbuf));
    }
    prebuf[m+ilNeed].sSepbuf.iRecid = -1;
  }
  
  /* �������� */
  for(;;)
  {
    if (ilRestlen <= iPERFLDVALUELEN)
    {
      prebuf[ilStart].sSepbuf.iRecid = iRecid;
      prebuf[ilStart].sSepbuf.iGrpid = iGrpid;
      prebuf[ilStart].sSepbuf.iFldid = iFldid;
      
      prebuf[ilStart].sSepbuf.iFldlen = ilRestlen;
      
      memcpy(prebuf[ilStart].sSepbuf.aFldvalue,fldval+ilPos,ilRestlen);
      prebuf[ilStart].sSepbuf.aFldvalue[ilRestlen] = '\0';
      break;
    }
    prebuf[ilStart].sSepbuf.iRecid = iRecid;
    prebuf[ilStart].sSepbuf.iGrpid = iGrpid;
    prebuf[ilStart].sSepbuf.iFldid = iFldid;    
    
    prebuf[ilStart].sSepbuf.iFldlen = iPERFLDVALUELEN;
    
    memcpy(prebuf[ilStart].sSepbuf.aFldvalue,fldval+ilPos,iPERFLDVALUELEN);
    prebuf[ilStart].sSepbuf.aFldvalue[iPERFLDVALUELEN] = '\0';
    ilRestlen = ilRestlen - iPERFLDVALUELEN;
    ilPos = ilPos + iPERFLDVALUELEN;
    ilStart ++;
  }
  return(SUCCESS);
}

/**************************************************************
 ** ������: swIsoget
 ** ����: �õ�ISOԤ����ָ�������ĳ��Ⱥ���ֵ
 ** ����: 
 ** ��������: 
 ** ����޸�����:
 ** ������������:
 ** ȫ�ֱ���:
 ** ��������: prebuf    Ԥ���
              iFldid    ��ID
 ** ����ֵ:
***************************************************************/
/*int swIsoget(union preunpackbuf *prebuf,short iFldid,
  short *fldlen,char *fldval) del by baiqj20150413 PSBC_V1.0*/
int swIsoget(union preunpackbuf *prebuf,long iFldid,short *fldlen,char *fldval) /*add by baiqj20150413 PSBC_V1.0*/
{
  int i = 0;
  short ilFldlen,ilFlag = 0;

  ilFldlen = 0;
  *fldlen = 0;
  
  while((prebuf[i].sIsobuf.iFldid != -1) && (i <= iBUFFLDNUM))
  {
    if (prebuf[i].sIsobuf.iFldid != iFldid)
    {
      i ++;
      if (ilFlag == 1)
      {
      	break;
      }
      else
      {
        continue;
      }  
    }
    ilFlag = 1;
    ilFldlen = prebuf[i].sIsobuf.iFldlen;
    if (ilFldlen>0)
    {
      memcpy(fldval+(*fldlen),prebuf[i].sIsobuf.aFldvalue,ilFldlen);
      *fldlen = *fldlen + ilFldlen;
    }
    i ++;
  }
  
  fldval[*fldlen] = '\0';
  if (ilFlag == 0)
    return(FAIL);
  else
    return(SUCCESS);
}
/**************************************************************
 ** ������: swIsoset
 ** ��  ��: ����ISOԤ����ָ�������ĳ��Ⱥ���ֵ
 ** ��  ��:
 ** ��������:
 ** ����޸�����:
 ** ������������:
 ** ȫ�ֱ���:
 ** ��������:
              iFldid    ��ID
 **           fldlen    ��ĳ���
 **           fldval    ��ֵ
 **           orglenval    ԭʼ������ֵ
 **           orglen       ԭʼ��ֵ�ĳ���
 **           orgfldval    ԭʼ��ֵ
 **           prebuf    Ԥ���
 ** ����ֵ:
***************************************************************/
/*int swIsoset(short iFldid,short fldlen,char *fldval,
  char *orglenval,short orglen,char *orgfldval,union preunpackbuf *prebuf) del by baiqj20150413 PSBC_V1.0*/
int swIsoset(long iFldid,short fldlen,char *fldval,
  char *orglenval,short orglen,char *orgfldval,union preunpackbuf *prebuf) /*add by baiqj20150413 PSBC_V1.0*/
{
  int i = 0,j=0,m,ilNeed=0,ilStart=-1;
  short ilPos_fin=0,ilPos_org=0;
  short ilRestlen_fin=0,ilRestlen_org=0;
  short ilCount=0,ilCount_fin=0,ilCount_org=0;

  ilCount_fin = (fldlen - 1)/iPERFLDVALUELEN + 1;
  ilCount_org = (orglen - 1)/iPERFLDVALUELEN + 1;
  if (ilCount_fin > ilCount_org)
    ilCount = ilCount_fin;
  else
    ilCount = ilCount_org;
  
  
  /* ����λ�� */
  while((prebuf[i].sIsobuf.iFldid != -1) && (i <= iBUFFLDNUM))
  {
    if (prebuf[i].sIsobuf.iFldid != iFldid )
    {
      i ++;
      continue;
    }
    if (j==0) ilStart = i;
    j ++;
    i ++;
  }
  if (ilStart == -1) ilStart = i;
  
  /* ����λ */
  ilNeed = ilCount - j;
  
  if ((i+ilNeed) > iBUFFLDNUM)
  {
    swVdebug(0,"S6270: [����/����] �����[%d][%d]���������[%d]",i,ilNeed,iBUFFLDNUM);
    return(FAIL);
  }
  
  if (ilNeed>0)
  {
    for(m=i;m>=ilStart+j;m--)
    {
      memcpy((char *)&(prebuf[m+ilNeed].sIsobuf),(char *)&(prebuf[m].sIsobuf),
        sizeof(struct isobuf));
    }
  }
  else if (ilNeed<0)
  {
    for(m=ilStart+j;prebuf[m].sIsobuf.iFldid != -1;m++)
    {
      memcpy((char *)&(prebuf[m+ilNeed].sIsobuf),(char *)&(prebuf[m].sIsobuf),
        sizeof(struct isobuf));
    }
    prebuf[m+ilNeed].sIsobuf.iFldid = -1;
  }
  
  /* �������� */

  for (i=0;i<ilCount;i++)
  {
    prebuf[ilStart+i].sIsobuf.iFldlen = -1;
    prebuf[ilStart+i].sIsobuf.iOrgfldlen = -1;
  }
  ilRestlen_fin = fldlen;
  for(;;)
  {
    if (ilRestlen_fin <= iPERFLDVALUELEN)
    {
      prebuf[ilStart].sIsobuf.iFldid = iFldid;
      prebuf[ilStart].sIsobuf.iFldlen = ilRestlen_fin;
      
      memcpy(prebuf[ilStart].sIsobuf.aFldvalue,fldval+ilPos_fin,ilRestlen_fin);
      prebuf[ilStart].sIsobuf.aFldvalue[ilRestlen_fin] = '\0';
      break;
    }
    prebuf[ilStart].sIsobuf.iFldid = iFldid;    
    
    prebuf[ilStart].sIsobuf.iFldlen = iPERFLDVALUELEN;
    memcpy(prebuf[ilStart].sIsobuf.aFldvalue,fldval+ilPos_fin,iPERFLDVALUELEN);
    prebuf[ilStart].sIsobuf.aFldvalue[iPERFLDVALUELEN] = '\0';
    ilRestlen_fin = ilRestlen_fin - iPERFLDVALUELEN;
    ilPos_fin = ilPos_fin + iPERFLDVALUELEN;
    ilStart ++;
  }
  swVdebug(4,"S6280: swIsoset() �ɹ�");
  swVdebug(4,"S6290: id=[%d],len=[%d],value=[%s],orglen[%d],orgval[%s]",
      iFldid,fldlen,fldval,orglen,orgfldval);
  /**
  swVdebug(3,"S6300: swIsoset()����8583Ԥ�ñ�����...");
    for(i=0;prebuf[i].sIsobuf.iFldid!=-1;i++)
  {
    swVdebug(3,"S6310: id=[%ld],len=[%d],value=[%s]orglen[%d]org_c[%d]",
      prebuf[i].sIsobuf.iFldid,
      prebuf[i].sIsobuf.iFldlen,
      prebuf[i].sIsobuf.aFldvalue,orglen,ilCount_org);
  }
  **/
  /* ԭʼ���� */
  ilStart = ilStart - ilCount_fin + 1;
  ilRestlen_org = orglen;

  memcpy(prebuf[ilStart].sIsobuf.aOrgfldlen,orglenval,10);

  for(;;)
  {
    if (ilRestlen_org <= iPERFLDVALUELEN)
    {
      prebuf[ilStart].sIsobuf.iFldid = iFldid;
      prebuf[ilStart].sIsobuf.iOrgfldlen = ilRestlen_org;

      memcpy(prebuf[ilStart].sIsobuf.aOrgfldvalue,orgfldval+ilPos_org,
        ilRestlen_org);
      prebuf[ilStart].sIsobuf.aOrgfldvalue[ilRestlen_org] = '\0';
      break;
    }
    prebuf[ilStart].sIsobuf.iFldid = iFldid;
    prebuf[ilStart].sIsobuf.iOrgfldlen = iPERFLDVALUELEN;

    memcpy(prebuf[ilStart].sIsobuf.aOrgfldvalue,orgfldval+ilPos_org,
      iPERFLDVALUELEN);
    prebuf[ilStart].sIsobuf.aOrgfldvalue[iPERFLDVALUELEN] = '\0';

    ilRestlen_org = ilRestlen_org - iPERFLDVALUELEN;
    ilPos_org = ilPos_org + iPERFLDVALUELEN;
    ilStart ++;
  }
/**
  swVdebug(3,"S6320: swIsoset()����8583Ԥ�ñ�����...");
    for(i=0;prebuf[i].sIsobuf.iFldid!=-1;i++)
  {
    swVdebug(3,"S6330: id=[%ld],len=[%d],value=[%s]",
      prebuf[i].sIsobuf.iFldid,
      prebuf[i].sIsobuf.iFldlen,
      prebuf[i].sIsobuf.aFldvalue);
  }
**/
  return(SUCCESS);
}

/*******************************************************************/
/* �� �� ��       :  swHextoasc                                    */
/* ��    ��       :  ��HEX(ASCII��)ת��ΪASCII                     */
/* ��    ��       :  ʷ����                                        */
/* ��������       :  2000��2��19��                                 */
/* ����޸�����   :  2000��2��19��                                 */
/* ������������   :                                                */
/* ȫ�ֱ���       :                                                */
/* ��������       :  char	*InBuf	HEX(ASCII��)��             */ 
/*                   int	Len	ASCII�ַ�������            */
/*                   unsigned char *OutBuf ASCII�ַ���             */
/* �� �� ֵ       :  -1 �ɹ�/0 ʧ��                                */
/*******************************************************************/
/* �޸ļ�¼       :                                                */
/*                                                                 */
/*******************************************************************/
int swHextoasc( char *InBuf, char *OutBuf, short * Outlen)
{
  int      rc;                 /* Return Value */
  int     ActiveNibble;        /* Active Nibble Flag */
  char     CharIn;             /* Character from source buffer */
  unsigned char  CharOut;      /* Character from target buffer */
  rc = 0;                      /* Assume everything OK. */
  ActiveNibble = 0;            /* Set Most Sign Nibble (MSN) */
                               /* to Active Nibble. */

  *Outlen = 0;
  for ( ; ; InBuf++ )
  {
    CharIn = *InBuf;
    
    if ( !isxdigit ( CharIn ) )    /* validate character */
    {
      break;
    }
    else
    {
      if ( CharIn > '9')
      {
        CharIn += 9;      /* Adjust Nibble for A-F */
      }
    }

    CharOut = (unsigned char)*OutBuf;
    if ( ActiveNibble )    
    {
      *OutBuf++ = ( CharOut & 0xF0) | ( CharIn  & 0x0F);
    }
    else
    {
      *OutBuf = ( CharOut & 0x0F) | ( (CharIn & 0x0F) << 4);
      (*Outlen) ++;
    }
    ActiveNibble ^= 1;      /* Change Active Nibble */
  }

  return rc;
}

/* ========= begin of add by dgm 2002.3.19 ========== */
/**************************************************************
 ** ������      ��swMacrorepl
 ** ��  ��      �����滻
 ** ��  ��      ��
 ** ��������    : 2002/03/19
 ** ����޸����ڣ�
 ** ��������������
 ** ȫ�ֱ���    ��
 ** ��������    ��
 ** ����ֵ      ��SUCCESS - �ɹ� FAIL - ʧ��
***************************************************************/
int swMacrorepl(char *aBuf)
{
  FILE *fp;
  short ilRepl_flag = 0;
  unsigned int ilLen;
  char alBuf[iFLDVALUELEN], alResult[iFLDVALUELEN], *alTmp;
  char alBuf_tmp[iFLDVALUELEN];
  char alMacro[iFLDVALUELEN];
  char *alTmp1, *alTmp2, *alTmp3;

  if ((fp = _swFopenlist("config/SWMACRO.CFG", "r")) == NULL)
  {
    if (errno != ENOENT)
      return(FAIL);
    else
      return(SUCCESS);
  }
  while (fp)
  {
    while((alTmp1 = strstr(aBuf,"$(")) != NULL)
    {
      alTmp2 = alTmp1 + 1;
      if((alTmp3 = strchr(alTmp2 + 1,')')) == NULL)
      {
        fclose(fp);
        return(FAIL);
      }

      memcpy(alMacro, alTmp2 + 1 , alTmp3 - alTmp2);
      alMacro[alTmp3 - alTmp2 - 1] = 0;

      ilRepl_flag = FALSE;

      while (fgets(alBuf, sizeof(alBuf), fp) != NULL)
      {
        _swTrim(alBuf);
        if (strlen(alBuf) == 0 || alBuf[0] == '#') continue;
        if ((alTmp = strchr(alBuf,'#')) != NULL) *alTmp = '\0';
        _swTrim(alBuf);

        alTmp = alBuf;
        while(*alTmp != 0)
        {
          if((*alTmp == ' ') || (*alTmp == '\t'))
            break;

          alTmp ++ ;
        }     
        if(*alTmp == 0) continue;


        strcpy(alResult,alTmp + 1);
        _swChgvalue(alResult);
        *alTmp = '\0';
        _swTrim(alBuf);

        if (strcmp(alBuf,alMacro) == 0)
        {
          ilRepl_flag = TRUE;
          ilLen = strlen(alResult);
          break;
        }
      }
      if(ilRepl_flag == TRUE)
      {
        strcpy(alBuf_tmp,alTmp3 + 1);
        memcpy(alTmp1,alResult,ilLen);
        memcpy(alTmp1 + ilLen,alBuf_tmp,strlen(alBuf_tmp));
        *(alTmp1 + ilLen + strlen(alBuf_tmp)) = 0;
      }
      else
      {
        fclose(fp);
        return(FAIL);
      }
    }
    fclose(fp);
    fp = _swFopenlist(NULL, "r");
  }  
  return(SUCCESS);
}

/* fzj:2002/8/20 ֧��SWITCH_CFGDIR��·�� */
#if 0
FILE *_swFopen(const char *file, const char *mode)
{
  char alEnv[256],alFile[256],*p,*s=alEnv;
  FILE *fp;

  if(*file=='/')
  {
    fp=fopen(file,mode);
    return(fp);
  }
  
  if(getenv("SWITCH_CFGDIR")==NULL)
  {
    swVdebug(0,"��������SWITCH_CFGDIRû������!");
    return(NULL);
  }  
  else
    strcpy(alEnv,getenv("SWITCH_CFGDIR"));
    
  while(*s)
  {
    if((p=strchr(s,':'))!=NULL)
    {
      *p=0;
      sprintf(alFile,"%s/%s",s,file);
      s=p+1;
      swVdebug(5,"S6340: _swFopen(): %s",alFile);
      if((fp=fopen(alFile,mode))!=NULL) return(fp);
      if(errno!=ENOENT) return(NULL);
    }
    else
    {
      sprintf(alFile,"%s/%s",s,file);
      fp=fopen(alFile,mode);
      swVdebug(5,"S6350: _swFopen(): %s",alFile);
      return(fp);
    }
  }
  return(NULL);
}

#endif /**delete by wanghao move to swUtil.c 20150416**/

FILE *_swPopen(const char *path, const char *mode)
{
  char alEnv[256],alPath[256],alCmd[1024],*p,*s=alEnv;
  FILE *pp;
  
  if(getenv("SWITCH_CFGDIR")==NULL)
  {
    swVdebug(0,"��������SWITCH_CFGDIRû������!");
    return(NULL);
  }  
  else
    strcpy(alEnv,getenv("SWITCH_CFGDIR"));
  
  strcpy(alCmd,"ls");
    
  while(*s)
  {
    if((p=strchr(s,':'))!=NULL)
    {
      *p=0;
      sprintf(alPath,"%s/%s",s,path);
      s=p+1;
      swVdebug(4,"S6360: _swPopen(): %s",alPath);
      strcat(alCmd," ");
      strcat(alCmd,alPath);
    }
    else
    {
      sprintf(alPath,"%s/%s",s,path);
      swVdebug(4,"S6370: _swPopen(): %s",alPath);
      strcat(alCmd," ");
      strcat(alCmd,alPath);
      break;
    }
  }
  pp = popen(alCmd,mode);
  return(pp);
}

FILE *_swFopenlist(const char *file, const char *mode)
{
  char alFile[256],*p;
  static char alEnv[256],alPath[256],*s; 
  FILE *fp;

  if(file)
  {
    if(getenv("SWITCH_CFGDIR")==NULL)
    {
      swVdebug(0,"��������SWITCH_CFGDIRû������!");
      return(NULL);
    }  
    else
      strcpy(alEnv,getenv("SWITCH_CFGDIR"));
    s=alEnv;
    strcpy(alPath,file);
  }    
  
  if(*s==0) return(NULL);
    
  while(*s)
  {
    if((p=strchr(s,':'))!=NULL)
    {
      *p=0;
      sprintf(alFile,"%s/%s",s,alPath);
      s=p+1;
      swVdebug(4,"S6380: _swFopenlist(): %s",alFile);
      if((fp=fopen(alFile,mode))!=NULL) return(fp);
      if(errno!=ENOENT) return(NULL);
    }
    else
    {
      sprintf(alFile,"%s/%s",s,alPath);
      s=alEnv+strlen(alEnv);
      fp=fopen(alFile,mode);
      swVdebug(4,"S6390: _swFopenlist(): %s",alFile);
      return(fp);
    }
  }
  return(NULL);
}

