#include "../includes/GPS.h"
#include "../includes/UART.h"
#include "../includes/GPIO.h"
void RGB_LEDS(void);
void UART0_init();

char read_UART0();
void write_UART0(char data);
void UART2_init();

void RGB_turnONLEDs(char data);
void RGB_turnOFFLEDs(char data);

void UART_OutString(char *chs);
void getCommand(char *command , int len , char start , char end);
//NMEA $GPRMC , , , ,  *





//-------------------------------------//

int main(){	
	const int len = 10000;
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

//-------------------------------------//

double take_longitude(char *point){
	char *longitude_str = "";
	char *degrees_str = "";
	char *minutes_str = "";
	strncpy(longitude_str  , point + 1 , 9);
	strncpy(degrees_str , longitude_str , 2);
	strncpy(minutes_str , longitude_str +2 , 7);
	double longitude = strtod(degrees_str,NULL) + strtod(minutes_str , NULL) /60.0 ; 
	return longitude;
	
}

double take_latitude(char *point){
	char *latitude_str = "";
	strncpy(latitude_str  , point + 11 , 10);
	char *degrees_str = "";
	char *minutes_str = "";
	strncpy(degrees_str , latitude_str , 3);
	strncpy(minutes_str , latitude_str +3 , 7);
	double longitude = strtod(degrees_str,NULL) + strtod(minutes_str , NULL) /60.0 ; 
	return longitude;
	//return longitude;

}

//-------------------------------------//

#define R 6371000 // Radius of the Earth in meters
#define M_PI 22/7
double toRadians(double degree) {
    return degree * M_PI / 180.0;
}

double haversine(double lat1, double lon1, double lat2, double lon2) {
    // Convert degrees to radians
    lat1 = toRadians(lat1);
    lon1 = toRadians(lon1);
    lat2 = toRadians(lat2);
    lon2 = toRadians(lon2);
    
    // Haversine formula
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;
    double a = sin(dlat / 2) * sin(dlat / 2) + cos(lat1) * cos(lat2) * sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double distance = R * c;
    
    return distance;
}


   


//-------------------------------------//

/*
void systick_Wait_1ms (){
//unsigned long long delay = delay * 16000;// calcluate 
 NVIC_ST_CTRL_R = 0; // 1) disable SysTick during setup 
NVIC_ST_RELOAD_R = 16000 - 1 ; // 2) maximum reload value 
NVIC_ST_CURRENT_R = 0; // 3) any write to CURRENT clears it 
NVIC_ST_CTRL_R = 0x000005; // 4) enable SysTick with core clock and transfers the value in reload register to be decremnted in the current register
while ((NVIC_ST_CTRL_R & 0x0010000)==0);
 }

void Delay (unsigned long time ){
 unsigned long i =1 ;
 while (i<time){
	 systick_Wait_1ms() ; 
		i+=1 ;
 }
 }
*/

//-------------------------------------//

void extract_Detailed_Location_info(char *Location , char *detailed_location){
		const char delimitar[] = ",";
	
		const char Point_start_and_end[] = "#";
		const char seperator[] = ",";
	
		char *token = strtok(Location,  delimitar);
		int field_index = 0;
		while (token != NULL) {
					
					if (field_index ==3  ) { //latitude
						strcat(detailed_location, Point_start_and_end );
						strcat(detailed_location, token );
					}
					
					if (field_index ==5  ) { //longitude
						strcat(detailed_location,seperator );
						strcat(detailed_location, token );
						strcat(detailed_location, Point_start_and_end );
					}
						
					
					token = strtok(NULL, delimitar);
					field_index++;
			}

}


//-------------------------------------//

void Take_instant_location(char *point){
	
	int len = 10000;
	char *log_command = "";
	while(1){
		
		UART_OutString("Enter :\n");
		Parse_GPS_command(point , len , log_command);
		if( (strcmp(log_command,"GPRMC") == 0)){
			break;
		}			
		//UART_OutString(point );
		//UART_OutString("\n");
		
		point = "";
		log_command = "";
	}
	
}

//-------------------------------------//

void implement_GPS_fix(){
	int len = 10000;
	char *point = "";
	char *log_command = "";
	while(1){
		
		UART_OutString("Enter :\n");
		Parse_GPS_command(point , len , log_command);
		if( (strcmp(log_command,"GPRMC") == 0) && check_GPS_FIX(point) ){
			
			break;
		}			
		//UART_OutString(point );
		//UART_OutString("\n");
		
		point = "";
		log_command = "";
	}
	
}


int check_GPS_FIX(char *chs){
	//$GGGAS , 12, A,123123
	const char delimitar[] = ",";
	char *token = strtok(chs,  delimitar);
	int field_index = 0;
	 while (token != NULL) {
					if (field_index == 2) { // The Fix Quality field is typically the 7th field in a GGA sentence
						if(strcmp(token, "A") == 0){                                                                                                 
							return 1; // Convert the token to integer and return as Fix Quality
						}
						break;
					}
					token = strtok(NULL, delimitar);
					field_index++;
			}
	return 0;
			
}

void Parse_GPS_command(char *chs , int len,char *log_Name){
		getCommand(chs,len,'$','*');
		strncpy(log_Name , chs , 5);
		log_Name[5] = '\0';
		
}
//-------------------------------------//
//$GGASD
void UART_OutString(char *chs){
	while(*chs){
	write_UART0(*chs);
	chs++;
	}
}

void getCommand(char *command , int len , char start , char end){
	//
	char character;
	int i;
	//flag to indicate whether to read data or not
	int isParsed = 0;
	for(i =0 ; i<len ; i++){
		character = uart5_receive();
		
		if(character == start)
		{
			isParsed = 1;
			i = 0 ;
		}
		
		if(isParsed )  //0x0D = CR
			command[i] = character;
		
	  if (character == end)
			break;
	}
}