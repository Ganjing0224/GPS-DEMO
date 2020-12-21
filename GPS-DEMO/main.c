#include "uart.h"
#include "gnss.h"
#define GPS_LEN 10000

int print_gps (GNSS *gps_data)
{
	printf("Date:%04d-%02d-%02d\n", gps_data->time.year, gps_data->time.month, gps_data->time.day);
	printf("Time:%02d:%02d:%02d:%03d\n", gps_data->time.hour, gps_data->time.minute, gps_data->time.second, gps_data->time.millisec);
	printf("pos_state:%c\n", gps_data->pos_state);
	printf("latitude:%lf\n", gps_data->latitude);
	printf("NS:%c\n", gps_data->NS);
	printf("longitude:%lf\n", gps_data->longitude);
	printf("EW:%c\n", gps_data->EW);
	printf("speed:%f\n", gps_data->speed);
	printf("direction:%f\n", gps_data->direction);
	printf("pos_mode:%c\n", gps_data->pos_mode);
	printf("************************************************\n");
 
    return 0;
}
 
int main (int argc, char **argv)
{
    int fd = 0;
    int n = 0;
    GNSS gnss;
    char buff[GPS_LEN];
    //TODO
    char *dev_name = "/dev/tty*"; //得具体看gps对应的/dev路径
 
    if ((fd = open(dev_name, O_RDWR|O_NOCTTY|O_NDELAY)) < 0) {
            perror("Can't Open the tty* Serial Port");
            return -1;
    }
    set_serial(fd, 9600, 8, 'N', 1);
 
    while(1)
    {
       sleep(1);
       if ((n = read(fd, buff, sizeof(buff))) < 0) {
           perror("read error");
           return -1;
       }
       printf("buff len:%d;%s\n", n, buff);
       memset(&gnss, 0 , sizeof(gnss));
       gps_analyse(buff, GPS_LEN, &gnss);
	   memset(buff, 0, GPS_LEN);
       print_gps(&gnss);
 
    }
    close(fd);
    return 0;
}
