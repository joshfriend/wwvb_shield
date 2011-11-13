#define MAX_ERROR 100
#define MAX_FRAME 400
#define MAX_ONE 700
#define MAX_ZERO 1000

//define byte as data type b/c unsigned char is too long to type
typedef unsigned char byte;

byte buffer[60] = {0};

volatile byte framePosition = 0;

void main(void) {
	
}

interrupt() {
	switch(framePosition++) {
		case 0:
		case 9:
		case 19:
		case 29:
		case 39:
		case 49:
		case 59:
			//Frame marker bit
			break;
		
		case 1:
			minutes += 40 * bitValue;
			break;
		case 2:
			minutes += 20 * bitValue;
			break;
		case 3:
			minutes += 10 * bitValue;
			break;
		case 5:
			minutes += 8 * bitValue;
			break;
		case 6:
			minutes += 4 * bitValue;
			break;
		case 7:
			minutes += 2 * bitValue;
			break;
		case 8:
			minutes += 1 * bitValue;
			break;
		
		case 12:
			hours += 20 * bitValue;
			break;
		case 13:
			hours += 10 * bitValue;
			break;
		case 15:
			hours += 8 * bitValue;
			break;
		case 16:
			hours += 4 * bitValue;
			break;
		case 17:
			hours += 2 * bitValue;
			break;
		case 18:
			hours += 1 * bitValue;
			break;
		
		case 22:
			dayOfYear += 200 * bitValue;
			break;
		case 23:
			dayOfYear += 100 * bitValue;
			break;
		case 25:
			dayOfYear += 80 * bitValue;
			break;
		case 26:
			dayOfYear += 40 * bitValue;
			break;
		case 27:
			dayOfYear += 20 * bitValue;
			break;
		case 28:
			dayOfYear += 10 * bitValue;
			break;
		case 30:
			dayOfYear += 8 * bitValue;
			break;
		case 31:
			dayOfYear += 4 * bitValue;
			break;
		case 32:
			dayOfYear += 2 * bitValue;
			break;
		case 33:
			dayOfYear += 1 * bitValue;
			break;
		
		case 36:
		case 37:
		case 38:
		case 40:
		case 41:
		case 42:
		case 43:
			//DUT1 Correction not implemented yet...
			//max skew from actualy time +/- 0.7
			break;
		
		case 45:
			year += 80 * bitValue;
			break;
		case 46:
			year += 40 * bitValue;
			break;
		case 47:
			year += 20 * bitValue;
			break;
		case 48:
			year += 10 * bitValue;
			break;
		case 50:
			year += 8 * bitValue;
			break;
		case 51:
			year += 4 * bitValue;
			break;
		case 52:
			year += 2 * bitValue;
			break;
		case 53:
			year += 1 * bitValue;
			break;
		
		case 55:
			//Year is a leap year?
			leap = bitValue;
			break;
		
		case 56:
			//Leap second warning
			leapSecond = bitValue;
			break;
		
		//Daylight Savings indicator
		// 0 = DST not in effect
		// 1 = DST ends today
		// 2 = DST begins today
		// 3 = DST is in effect
		case 57:
			dst += 2* bitValue;
			break;
		case 58:
			dst += 1 * bitValue;
			break;
		
		default:
			//Don't care bit recieved
			break;
	}
}

byte bitValue() {
	byte bitVal = 0;
	
	if(pulseLength < MAX_ERROR) {
		//Pulse not recognized
		bitVal = 3;
	}
	else if(pulseLength < MAX_FRAME) {
		//Recieved frame marker pulse
		bitVal = 2;
	}
	else if(pulseLength < MAX_ONE) {
		//Recieved binary one pulse
		bitVal = 1;
	}
	else if(pulseLength < MAX_ZERO){
		//Recieved binary zero pulse
		bitVal = 0;
	}
	
	return bitVal;
}

byte getMonth() {
	if(dayOfYear < 31) 
		return 1;
	else if(dayOfYear < (59+leap)) 
		return 2;
	else if(dayOfYear < (90+leap)) 
		return 3;
	else if(dayOfYear < (120+leap)) 
		return 4;
	else if(dayOfYear < (151+leap)) 
		return 5;
	else if(dayOfYear < (181+leap)) 
		return 6;
	else if(dayOfYear < (212+leap)) 
		return 7;
	else if(dayOfYear < (243+leap)) 
		return 8;
	else if(dayOfYear < (273+leap)) 
		return 9;
	else if(dayOfYear < (304+leap)) 
		return 10;
	else if(dayOfYear < (334+leap)) 
		return 11;
	else
		return 12;
}

byte getDayOfWeek() {

	byte d = getDayOfMonth();
	byte m = getMonth();
	byte y = year;

	/*This is an alternate method which I used in my 350 project.
	http://en.wikipedia.org/wiki/Weekday_determination#Sakamoto.27s_Method
	*/
   static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
   y -= m < 3;
   return (byte)((y + y/4 - y/100 + y/400 + t[m-1] + d) % 7);

}

byte getDayOfMonth() {
	byte m = getMonth();
	byte d = dayOfYear;
	
	for(int i=1;i<m;i++){
		if(d > daysInMonth(i)){
			d -= daysInMonth(i);
		}
	}
	
	return d;
}

byte daysInMonth(int month){
	if(month == 2)
		return 28 + leap;
	else if(month==4 || month==6 || month==10 || month==11)
		return 30;
	else 
		return 31;
}
