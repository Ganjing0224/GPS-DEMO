#include "gnss.h"

/* string find and replace  */
char* strrpl(char *str, char* find, char *replace)
{
    int i;
    char *pt = strstr(str, find);	
	char *firstStr = NULL;
   
    if (pt == NULL) {
        printf("cannot find string \n");
        return NULL;
    }
	
	int len = strlen(str) + 1 + strlen(replace) - strlen(find);
	firstStr = (char* )malloc(len);
	memset(firstStr, 0, len);

    strncpy(firstStr, str, strlen(str) - strlen(pt)); 
    strcat(firstStr, replace);
    strcat(firstStr, pt + strlen(find));

    for(i = 0; i < strlen(firstStr); i++)
        str[i] = firstStr[i];
	
	free(firstStr);
	firstStr = NULL;
    return str;
}

void nmea_get_time(const char *buf, TIME* gnss_time)
{
    char tmpbuf[8];

    memset(tmpbuf, 0, 8);
    memcpy(tmpbuf, buf, 2);
    gnss_time->hour = atoi(tmpbuf) + 8;
    memcpy(tmpbuf, buf+2, 2);
    gnss_time->minute = atoi(tmpbuf);
    memcpy(tmpbuf, buf+4, 2);
    gnss_time->second = atoi(tmpbuf);
    memcpy(tmpbuf, buf + 7, 3);
    if (tmpbuf[2]<'0' || tmpbuf[2]>'9')
        tmpbuf[2] = '0';
    gnss_time->millisec = atoi(tmpbuf);
}

void nmea_lat_long_to_double( double *v, const char *buf, int len)
{
    char tmpbuf[16];
    double tmpf, tmpi;

    memset(tmpbuf, 0, sizeof(tmpbuf));
    memcpy(tmpbuf, buf, len);
    tmpf = strtod(buf, (char**) NULL);
    tmpi = ((int)tmpf) / 100;
    tmpf -= tmpi * 100;
    tmpf = tmpf / 60;
    tmpf += tmpi;
    *v = tmpf;
}

void nmea_get_date(const char *buf, TIME* gnss_time)
{
    char tmpbuf[8];
    memset(tmpbuf, 0, 8);
    memcpy(tmpbuf, buf, 2);
    gnss_time->day = atoi(tmpbuf);
    memcpy(tmpbuf, buf + 2, 2);
    gnss_time->month = atoi(tmpbuf);
    memcpy(tmpbuf, buf + 4, 2);
    gnss_time->year = atoi(tmpbuf) + 2000;
}

int check_nmea_message(const char* buf, int index, int maxLen)
{
	int len = 0;
	char check = 0;

	if (buf[index] != '$')
		return -1;

	// checksum
	len = 1;
	while(buf[index+len] != '*')
	{
		check ^= buf[index + len];
		len ++;
		if (index + len + 2 >= maxLen)
			return -2;
	}

	char ch[2];
	sprintf(ch, "%02X", check);

	if (memcmp(ch, buf + index + len + 1, 2) != 0)
		return -3;

	return 1;
}

 
int gps_analyse(char *buff, int buff_len, GNSS *gps_data)
{
    char *ptr = NULL;
	if (strlen(buff) < 10) {
		return -1;
	}
		
	/* 如果buff字符串中包含字符"$GPRMC"则将$GPRMC的地址赋值给ptr */
	if ( NULL == (ptr = strstr(buff, "$GPRMC")) && NULL == (ptr = strstr(buff, "$GNRMC"))) {
		return -2;
	}

	if (check_nmea_message(ptr, 0, buff_len) < 0) {
		printf("check error!\n");
		return -3;
	}
	
	char *tmpbuf;
	tmpbuf = (char *)malloc(strlen(ptr) + 100);
	memset(tmpbuf, 0, strlen(ptr) + 100);
	memcpy(tmpbuf, ptr, strlen(ptr));
	
	while (strstr(tmpbuf, ",,"))
		strrpl(tmpbuf, ",,", ",@,");
	
	printf("tmpbuf:%s \n",tmpbuf);

	char* pch = strtok(tmpbuf, ",");
	
	// 1 time
	pch = strtok(NULL, ",");
	nmea_get_time(pch, &gps_data->time);
	
	// 2 status
	pch = strtok(NULL, ",");
	gps_data->pos_state = *pch;
	
	//3 latitude
	pch = strtok(NULL, ",");
	nmea_lat_long_to_double(&gps_data->latitude, pch, strlen(pch));
	
	//4 latitude direction
	pch = strtok(NULL, ",");
	gps_data->NS = *pch;
	
	//5 longitude
	pch = strtok(NULL, ",");
	nmea_lat_long_to_double(&gps_data->longitude, pch, strlen(pch));

	//6 long direct
	pch = strtok(NULL, ",");
	gps_data->EW = *pch;
	
    //7 speed
	pch = strtok(NULL, ",");
	gps_data->speed = 1.852 * strtof(pch, (char **) NULL ) / 3.6;
	
	//8 direction
	pch = strtok(NULL, ",");
	gps_data->direction = strtof(pch, (char**)NULL);

	//9 date
	pch = strtok(NULL, ",");
	nmea_get_date(pch, &gps_data->time);
		
	//10 不处理
	pch = strtok(NULL, ",");
	
	//11 不处理
	pch = strtok(NULL, ",");
	
	//12 mode
	pch = strtok(NULL, ",");
	gps_data->pos_mode = *pch;
	
	free(tmpbuf);
	
	return 0;
}
 