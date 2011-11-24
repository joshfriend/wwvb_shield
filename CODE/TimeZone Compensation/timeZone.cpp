enum UTC { 
    UTC_n12 = -12,
    UTC_n11,
    UTC_n10,
    UTC_n09,
    UTC_n08,
    UTC_n07, 
    UTC_n06, 
    UTC_n05, 
    UTC_n04,
    UTC_n03, 
    UTC_n02, 
    UTC_n01,
    UTC_00,
    UTC_p01, 
    UTC_p02, 
    UTC_p03, 
    UTC_p04,
    UTC_p05,
    UTC_p06,
    UTC_p07,
    UTC_p08,
    UTC_p09,
    UTC_p10,
    UTC_p11,
    UTC_p12
};

/*
 *Takes an integer representing the offset from UTC00
 *and sets the UTC offset accordingly
 *
 *The WWVB signal is assumed to be from Boulder, CO, USA in UTC-07
 *
 *Will se offset to zero if 
 */
void setUTC_Offset(int nOffset=0){
     if((nOffset >= UTC_n12)&&(nOffset <= UTC_p12)){
          nUTC_Offset = 7+nOffset;
     }
     else{
          nUTC_Offset = 0;
     }
}
