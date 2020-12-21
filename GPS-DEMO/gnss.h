#ifndef __GNSS_H__
#define __GNSS_H__

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
 
typedef unsigned int UINT;
typedef int BYTE;
typedef struct _time_
{
	short	year;
	short month;
	short	day;
	short	hour;
	short	minute;
	short	second;
	short	millisec;
 }TIME; 

typedef struct __gnss__
{
   TIME time; /* 时间 */
   char pos_state; /*gps状态位*/
   char	NS; /*North/South*/
   char	EW; /*East/West*/
   double latitude; /*纬度 */
   double longitude; /* 经度 */
   float speed; /* 速度,单位m/s */
   float direction; /*航向 */
   char pos_mode; /* GPS模式位 */
}GNSS;

extern int gps_analyse(char *buff, int buff_len, GNSS *gps_data);
#endif