
#include "../includes/GPIO.h"
#include "../includes/UART.h"
#include "tm4c123gh6pm.h"
#include "../includes/Delay.h"
#include "../includes/GPS.h"
# define Red_Led 0X02
# define Blue_Led 0x04
# define  Green_Led 0x08


// int IsButtonPressed() ;
void Init_UART(void) ;




int main ()
{const int len = 10000;
	char command[len] = {0};
	char *start_point_command = "";
	char *start_point = "";
	char *point_command = "";
	char *point = "";
	char *trajectory = "";
	char *previous_point = "";
	double total_distance = 0;
	uart5_init();
	portF_init();
	implement_GPS_fix();
	
	//
	Take_instant_location(start_point_command );
	extract_Detailed_Location_info(start_point_command  , start_point );
	

	strcpy(previous_point , start_point);
	
	
	while(1){
		
		Take_instant_location(point_command );
		extract_Detailed_Location_info(point_command  , point );
		strcat(trajectory , point);
	
		double point1_lat = take_latitude(previous_point); 
    double point1_lon = take_longitude(previous_point);
		
		
    double point2_lat = take_latitude(point); 
    double point2_lon = take_longitude(point);
		
    // Calculate distance
		
    double distance = haversine(point1_lat, point1_lon, point2_lat, point2_lon);
		total_distance += distance;
		
    //printf("Distance between the two points: %.2f meters\n", distance);
		
    strcpy(previous_point,point);
		point_command = "";
		point = "";
		
	}
	 // Example coordinates in degrees and minutes
  
	
}
	
 


