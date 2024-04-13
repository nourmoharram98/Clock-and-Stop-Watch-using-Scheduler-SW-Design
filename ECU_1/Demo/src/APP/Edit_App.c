// #include"Std_Types.h"
// #include "../../include/APP/Edit_App.h"
// #include "SWITCH/HAL_SWITCH.h"
// #include "../../include/HAL/LCD/LCD.h"
// #include "Clock_Date_App.h"
// #include "APP/Manager.h"

// u8 Global_X_Position=0;
// u8 Global_Y_Position=0;
// extern Operation_Types_t Operation_type;
// extern unit_Info_t Clock_Date_Digits[NUMBER_OF_DIGITS_CLK_MODE];

// #define EDIT_SWITCH         3


// /* uart updates the value of this switch periodically */
// /* the values I am expecting is explained in the enum */
// switch_IDs_t switchNumber;

// bool IsLeapYear(year) {
//     calculateyear();
//     if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
//         return true;
//     } else {
//         return false;
//     }
// }


// // G_u32EditYears=G_u32Years;
// // 	G_u8EditMonth=G_u8Month;
// // 	G_u8EditDays=G_u8Days;
// // 	G_u8EditHours=G_u8hours;
// // 	G_u8EditSecs=G_u8Secs;
// // 	G_u8EditMins=G_u8Mins;
// extern unit_Info_t Clock_Date_Digits[NUMBER_OF_DIGITS];
// // Define the editable positions for each row
// const u8 editable_positions[2][8] = {
//     {7, 8, 10, 11, 13, 14, 15, 16}, // Editable positions for row 1
//     {3, 4, 6, 7, 9, 10, 12}         // Editable positions for row 2
// };


// Cursor_Position_t editable[15]={
//     [0]={.row=1,.column=7},
//     [1]={.row=1,.column=8},
//     [2]={.row=1,.column=10},
//     [3]={.row=1,.column=11},
//     [4]={.row=1,.column=13},
//     [5]={.row=1,.column=14},
//     [6]={.row=1,.column=15},
//     [7]={.row=1,.column=16},
//     [8]={.row=2,.column=3},
//     [9]={.row=2,.column=4},
//     [10]={.row=2,.column=6},
//     [11]={.row=2,.column=7},
//     [12]={.row=2,.column=9},
//     [13]={.row=2,.column=10},
//     [14]={.row=2,.column=12},
// };

// Edit_App_State_t editAppState;



// // Read individual digits of the year
// LCD_enuErrorStatus_t status;
// u16 year;

// void calculateyear (void)
// {
//     status = LCD_enuReadNumberpos(&editAppState.date.year_thousands, 1, Year_Thousand_Position);

//     status = LCD_enuReadNumberpos(&editAppState.date.year_hundreds, 1, Year_Hundred_Position);

//     status = LCD_enuReadNumberpos(&editAppState.date.year_tens, 1, Year_Tens_Postion);

//     status = LCD_enuReadNumberpos(&editAppState.date.year_units, 1, Year_Units_Position);
// //alooo
//     // Reconstruct the full year value
//     year = editAppState.date.year_thousands * 1000 +
//            editAppState.date.year_hundreds * 100 +
//            editAppState.date.year_tens * 10 +
//            editAppState.date.year_units;
// }




// void Edit_App_Init(void) 
// { 
//     editAppState.position.row = 1;
//     Global_X_Position=editAppState.position.row;
//     editAppState.position.column = 7; // Starting from the first editable position
//     Global_Y_Position=editAppState.position.column;
//     LCD_SetCursorPosAsync(editAppState.position.row,editAppState.position.column);
// }

// /*What number to change function*/
//     switch (editAppState.position.row)
//     {
//     case 0:
//         switch (editAppState.position.column)
//         {
//             case 7 : 
//                 //daytens++;
//             break;

//             case 8 : 
//                 d//ayunit++;
//             break;

//             case 10 : 
//                 //monthtens++;
//             break;

//             case 11 : 
//                 //monthunit++;
//             break;

//             case 13 : 
//                 //yearthousand++;
//             break;

//             case 14 : 
//                 //yearhundred++;
//             break;

//             case 15 : 
//                 //eartens++;
//             break;

//             case 16 : 
//                 //yearunits++;
//             break;
//         }
//     case 1:
//         switch(editAppState.position.column)
//         {
//             case 3:
//                 //hourtens++;
//             break;

//             case 4:
//                 //hourunits++;
//             break;

//             case 6: 
//                 //mintens++;
//             break;

//             case 7:
//                 //minunits++;
//             break;

//             case 9:
//                 //sectens++;
//             break;

//             case 10:
//                 //secunit++;
//             break;

//             case 12:
//                 //deci++;
//             break;
//         }
//     }
// /*------------------------------------*/

// void Edit_App_ProcessSwitch(u8 Switch_Data) {
//     // Process switch input

//     //the pressed switch should be on the other microcontroller
//     //editAppState.mode = current_mode;

//     // This function will be called when a switch is pressed

//     // Toggle between edit modes when the edit switch is pressed
//     static u8 Edit_Counter=0;
//     switch(Switch_Data)
//     {
//         case EDIT_SWITCH:
//             Edit_Counter++;
//             if(Edit_Counter==1)
//             {
//                 Operation_type=GENERAL_EDIT_MODE;
//             }
//             else if(Edit_Counter==2)
//             {
//                 Operation_type=EDIT_DIGIT_MODE;
//                 Edit_Counter=0;
//             }
//             break;
//     }



//     if (switchNumber == EDIT_SWITCH) {
//         if(editAppState.mode == NORMAL_DISPLAY_MODE)
//         {
//             editAppState.mode = GENERAL_EDIT_MODE;
//             Edit_App_Init();
//         }
//         else if (editAppState.mode == GENERAL_EDIT_MODE) {
            
//             editAppState.mode = EDIT_DIGIT_MODE;
//         } else if(editAppState.mode == EDIT_DIGIT_MODE){
//             editAppState.mode = GENERAL_EDIT_MODE;
//         }
//         return;
//     }
//     else if(switchNumber == OK_SWITCH)
//     {
//             editAppState.mode = NORMAL_DISPLAY_MODE;
//     }

//     // Process switches based on the edit mode
//     if (editAppState.mode == GENERAL_EDIT_MODE) {
//         // Process general edit mode switches
//         switch (switchNumber) {
//             case UP_SWITCH:
//                 // Implement cursor movement logic for general edit mode
//                 if (editAppState.position.row == 1) {
//                 // If the cursor is in row 1, move it to row 2
//                 editAppState.position.row = 2;
//                 } else {
//                     // If the cursor is in row 2, move it to row 1
//                     editAppState.position.row = 1;
//                 }
//                 break;
//             case DOWN_SWITCH:
//                 // Implement cursor movement logic for general edit mode
//                 if (editAppState.position.row == 1) {
//                 // If the cursor is in row 1, move it to row 2
//                 editAppState.position.row = 2;
//                 } else {
//                     // If the cursor is in row 2, move it to row 1
//                     editAppState.position.row = 1;
//                 }
//                 break;
//             case LEFT_SWITCH:
//                 // Implement cursor movement logic for general edit mode
//                 if(editAppState.position.row == 1)
//                 {
//                     if(editAppState.position.column>7)
//                     {
//                         editAppState.position.column--;
//                         if (editAppState.position.column == 9 || editAppState.position.column == 12)
//                         {
//                             editAppState.position.column--;
//                         }
                        
//                     }
//                     if(editAppState.position.column==7)
//                     {
//                         editAppState.position.column = 16;
//                     }
//                 }
//                 if(editAppState.position.row == 2)
//                 {
//                     if(editAppState.position.column>3)
//                     {
//                         editAppState.position.column--;
//                         if (editAppState.position.column == 5 || editAppState.position.column == 8 || editAppState.position.column == 11)
//                         {
//                             editAppState.position.column--;
//                         }
                        
//                     }
//                     if(editAppState.position.column==3)
//                     {
//                         editAppState.position.column=15; //13
//                     }
//                 }
//                 break;
//             case RIGHT_SWITCH:
//                 // Implement cursor movement logic for general edit mode
//                 if(editAppState.position.row == 1)
//                 {
//                     if(editAppState.position.column<16)
//                     {
//                         editAppState.position.column++;
//                         if (editAppState.position.column == 9 || editAppState.position.column == 12)
//                         {
//                             editAppState.position.column++;
//                         }
                        
//                     }
//                     if(editAppState.position.column==16)
//                     {
//                         editAppState.position.column = 7;
//                     }
//                 }
//                 if(editAppState.position.row == 2)
//                 {
//                     if(editAppState.position.column<15) //13
//                     {
//                         editAppState.position.column++;
//                         if (editAppState.position.column == 5 || editAppState.position.column == 8 || editAppState.position.column == 11)
//                         {
//                             editAppState.position.column++;
//                         }
                        
//                     }
//                     if(editAppState.position.column==15) //13
//                     {
//                         editAppState.position.column=3;
//                     }
//                 }
//                 break;
//             default:
//                 break;
//         }

//         //Update cursor position
//         LCD_SetCursorPosAsync(editAppState.position.row,editAppState.position.column);
//     } else if (editAppState.mode == EDIT_DIGIT_MODE) {
//         // Process edit digit mode switches
//         switch (switchNumber) {
//         case UP_SWITCH:
//             // Implement digit increment logic
//             IncrementDigit();
//             break;
//         case DOWN_SWITCH:
//             // Implement digit decrement logic
//             DecrementDigit();
//             break;
//         default:
//             break;
//         }
//     }
// }

// void IncrementDigit(void) {
//     // Increment the digit at the current cursor position
//     u8 currentValue;
//     LCD_enuErrorStatus_t lcdStatus = LCD_enuReadNumberpos(&currentValue, editAppState.position.row, editAppState.position.column);
    
//     if (lcdStatus == LCD_enumOK) {
//         // Check if the current value can be incremented
//         if(editAppState.position.row == 1)
//         {
//         switch (editAppState.position.column) {
//             case Day_Tens_Position: // Tens digit of the day
//                 Clock_Date_Digits[Day_tens].value = currentValue;
//                 if ((Clock_Date_Digits[Day_tens].value < 3 && Clock_Date_Digits[Day_unit].value < 2) 
//                     || (Clock_Date_Digits[Day_tens].value<2)) {
//                         Clock_Date_Digits[Day_tens].value++;
//                 }
//                 else {
//                     Clock_Date_Digits[Day_tens].value = 0; // Wrap around if exceeds 3
//                 }
//                 currentValue = Clock_Date_Digits[Day_tens].value;
//                 break;
//                 case Day_Units_Position: // Units digit of the day
//                     // Check for February
//                     Clock_Date_Digits[Day_unit].value = currentValue;
//                     if (Clock_Date_Digits[Month_tens].value == 0 && Clock_Date_Digits[Month_unit].value == 2) {
//                         // Check for leap year
//                         if (IsLeapYear(year) ) {
//                             // Check if the day is less than 29
//                             if (Clock_Date_Digits[Day_unit].value < 9) {
//                                 Clock_Date_Digits[Day_unit].value++;
//                             }
//                             else{
//                                 Clock_Date_Digits[Day_unit].value = 0; // Wrap around if exceeds 9
//                             }
//                         }
//                         else {
//                             // Non-leap year, handle February 28 days
//                             if ((Clock_Date_Digits[Day_unit].value == 8 && Clock_Date_Digits[Day_tens].value==2)
//                             ||(Clock_Date_Digits[Day_unit].value == 9 && Clock_Date_Digits[Day_tens].value<2)) {
//                                 Clock_Date_Digits[Day_unit].value=0;
//                             }
//                             else {
//                                 Clock_Date_Digits[Day_unit].value ++; // Wrap around if exceeds 8
//                             }
//                         }
//                     }
//                     // Check for months with 30 days
//                     else if ((Clock_Date_Digits[Month_unit].value == 4 || Clock_Date_Digits[Month_unit].value == 6 || Clock_Date_Digits[Month_unit].value == 9 || (Clock_Date_Digits[Month_tens].value == 1 && Clock_Date_Digits[Minutes_unit].value==1))&& Clock_Date_Digits[Day_tens].value==3) {
                        
//                             Clock_Date_Digits[Day_unit].value = 0; // Wrap around if exceeds 9
//                     }
//                     // All other months
//                     else {
//                         if(Clock_Date_Digits[Day_tens].value == 3 && Clock_Date_Digits[Day_unit].value == 1)
//                         {
//                             Clock_Date_Digits[Day_unit].value = 0;
//                         }
//                         // Check if the day is less than 9
//                         else if (Clock_Date_Digits[Day_unit].value < 9) {
//                             Clock_Date_Digits[Day_unit].value++;
//                         }
//                         else {
//                             Clock_Date_Digits[Day_unit].value = 0; // Wrap around if exceeds 9
//                         }
//                     }
//                     currentValue = Clock_Date_Digits[Day_unit].value;
//                     break;
//             // case Day_Units_Position: // Units digit of the day
//             //     if (Clock_Date_Digits[Day_unit].value < 9 && Clock_Date_Digits[Day_tens].value != 3) {
//             //         Clock_Date_Digits[Day_unit].value++;
//             //     } else if(Clock_Date_Digits[Day_unit].value == 0 && Clock_Date_Digits[Day_tens].value == 3)
//             //     {
//             //         Clock_Date_Digits[Day_unit].value == 1;
//             //     }
//             //     else {
//             //         Clock_Date_Digits[Day_unit].value = 0; // Wrap around if exceeds 9
//             //     }
//             //     break;
//             case Month_Tens_Postion: // Tens digit of the month
//                 Clock_Date_Digits[Month_tens].value = currentValue;
//                 if (Clock_Date_Digits[Month_tens].value < 1 && Clock_Date_Digits[Month_unit].value < 3) {
//                     Clock_Date_Digits[Month_tens].value++;
//                 }else {
//                     Clock_Date_Digits[Month_tens].value = 0; // Wrap around if exceeds 1
//                 }
//                 currentValue = Clock_Date_Digits[Month_tens].value;
//                 break;
//             case Month_Units_Position: // Units digit of the month
//                 Clock_Date_Digits[Month_unit].value = currentValue;
//                 if ((Clock_Date_Digits[Month_unit].value < 9 && Clock_Date_Digits[Month_tens].value == 0 )
//                 || (Clock_Date_Digits[Month_unit].value < 2 && Clock_Date_Digits[Month_tens].value == 1)) 
//                 {
//                     Clock_Date_Digits[Month_unit].value++;
//                 }
//                 else {
//                     Clock_Date_Digits[Month_unit].value = 0; // Wrap around if exceeds 2
//                 }
//                 currentValue = Clock_Date_Digits[Month_unit].value;
//                 break;
//                 default:
//                 break;
//         }
//         }
//         else if (editAppState.position.row == 2)
//         {
//             switch (editAppState.position.column) {
//             case Hours_Tens_Position: // Tens digit of the hour
//                 Clock_Date_Digits[Hours_tens].value = currentValue;
//                 if ((Clock_Date_Digits[Hours_tens].value < 2 && Clock_Date_Digits[Hours_unit].value <4)
//                 || (Clock_Date_Digits[Hours_tens].value <1)) {
//                     Clock_Date_Digits[Hours_tens].value++;
//                 } else {
//                     Clock_Date_Digits[Hours_tens].value = 0; // Wrap around if exceeds 2
//                 }
//                 currentValue = Clock_Date_Digits[Hours_tens].value;
//                 break;
//             case Hours_Units_Position: // Units digit of the hour
//                 Clock_Date_Digits[Hours_unit].value = currentValue;
//                 if ((Clock_Date_Digits[Hours_unit].value < 9 && Clock_Date_Digits[Hours_tens].value < 2)
//                 || (Clock_Date_Digits[Hours_unit].value <3)) {
//                     Clock_Date_Digits[Hours_unit].value++;
//                 } else {
//                     Clock_Date_Digits[Hours_unit].value = 0; // Wrap around if exceeds 9
//                 }
//                 currentValue = Clock_Date_Digits[Hours_unit].value;
//                 break;
//             case Minuts_Tens_Position: // Tens digit of the minute
//                 Clock_Date_Digits[Minutes_tens].value = currentValue;
//                 if (Clock_Date_Digits[Minutes_tens].value < 5) {
//                         Clock_Date_Digits[Minutes_tens].value++;
//                     } else {
//                         Clock_Date_Digits[Minutes_tens].value = 0; // Wrap around if exceeds 2
//                     }
//                     currentValue = Clock_Date_Digits[Minutes_tens].value;
//                 break;
//             case Minuts_Units_Position: // Units digit of the minute
//                 Clock_Date_Digits[Minutes_unit].value = currentValue;
//                 if (Clock_Date_Digits[Minutes_unit].value < 9) {
//                         Clock_Date_Digits[Minutes_unit].value++;
//                     } else {
//                         Clock_Date_Digits[Minutes_unit].value = 0; // Wrap around if exceeds 2
//                     }
//                     currentValue = Clock_Date_Digits[Minutes_unit].value;
//                     break;
//             case Seconds_Tens_Position: // Tens digit of the second
//             Clock_Date_Digits[Second_tens].value = currentValue;
//             if (Clock_Date_Digits[Second_tens].value < 5) {
//                         Clock_Date_Digits[Second_tens].value++;
//                     } else {
//                         Clock_Date_Digits[Second_tens].value = 0; // Wrap around if exceeds 2
//                     }
//                     currentValue = Clock_Date_Digits[Second_tens].value;
//                 break;
//             case Seconds_Units_Position: // Units digit of the second
//             Clock_Date_Digits[Second_unit].value = currentValue;
//                 // Implement increment logic for minutes and seconds
//                if (Clock_Date_Digits[Second_unit].value < 9) {
//                         Clock_Date_Digits[Second_unit].value++;
//                     } else {
//                         Clock_Date_Digits[Second_unit].value = 0; // Wrap around if exceeds 2
//                     }
//                     currentValue = Clock_Date_Digits[Second_unit].value;
//                 break;
//             default:
//                 break;
//         }
//         }
        
//         // Update the LCD with the new value
//         //LCD_WriteNumber without position?
//         lcdStatus = LCD_enuWriteNumberpos(currentValue, editAppState.position.row, editAppState.position.column);
//         if (lcdStatus != LCD_enumOK) {
//             // Handle error updating the LCD
//         }
//     } else {
//         // Handle error reading the number from the LCD
//     }
// }

// void DecrementDigit(void) {
//     // Increment the digit at the current cursor position
//     u8 currentValue;
//     LCD_enuErrorStatus_t lcdStatus = LCD_enuReadNumberpos(&currentValue, editAppState.position.row, editAppState.position.column);
    
//     if (lcdStatus == LCD_enumOK) {
//         // Check if the current value can be incremented
//         if(editAppState.position.row == 1)
//         {
//         switch (editAppState.position.column) {
//             case Day_Tens_Position: // Tens digit of the day
//             Clock_Date_Digits[Day_tens].value = currentValue;
//                 if ((Clock_Date_Digits[Day_tens].value > 0 && Clock_Date_Digits[Day_unit].value <= 3) ) {
//                         Clock_Date_Digits[Day_tens].value--;
//                 }
//                 else {
//                     Clock_Date_Digits[Day_tens].value = 3; // Wrap around if exceeds 3
//                 }
//                 currentValue = Clock_Date_Digits[Day_tens].value;
//                 break;
//                 case Day_Units_Position: // Units digit of the day
//                             Clock_Date_Digits[Day_unit].value = currentValue;

//             // Check for February
//             if (Clock_Date_Digits[Month_tens].value == 0 && Clock_Date_Digits[Month_unit].value == 2) {
//                 // Check for leap year
//                 if (IsLeapYear(year)) {
//                     // Check if the day is greater than 0
//                     if (Clock_Date_Digits[Day_unit].value > 0) {
//                         Clock_Date_Digits[Day_unit].value--;
//                     } else {
//                         Clock_Date_Digits[Day_unit].value = 9; // Wrap around if less than 0
//                     }
//                 } else {
//                     // Non-leap year, handle February 28 days
//                     if ((Clock_Date_Digits[Day_unit].value == 0 && Clock_Date_Digits[Day_tens].value==2)) {
//                         Clock_Date_Digits[Day_unit].value=8;
//                     } else if((Clock_Date_Digits[Day_unit].value == 0 && Clock_Date_Digits[Day_tens].value<2))
//                     {
//                         Clock_Date_Digits[Day_unit].value=9;
//                     }else {
//                         Clock_Date_Digits[Day_unit].value --; // Wrap around if less than 0
//                     }
//                 }
//             }
//             // Check for months with 30 days
//             else if ((Clock_Date_Digits[Month_tens].value == 4 || Clock_Date_Digits[Month_tens].value == 6 || Clock_Date_Digits[Month_tens].value == 9 || (Clock_Date_Digits[Month_tens].value == 1 && Clock_Date_Digits[Minutes_unit].value==1)) && Clock_Date_Digits[Day_tens].value == 3) {
//                 Clock_Date_Digits[Day_unit].value = 0; // Wrap around if less than 0
//             }
//             // All other months
//             else {
//                 // Check if the day is greater than 0
//                 if(Clock_Date_Digits[Day_tens].value == 3 && Clock_Date_Digits[Day_unit].value == 0)
//                 {
//                     Clock_Date_Digits[Day_unit].value = 1;
//                 }
//                 else if (Clock_Date_Digits[Day_unit].value > 0) {
//                     Clock_Date_Digits[Day_unit].value--;
//                 } else {
//                     Clock_Date_Digits[Day_unit].value = 9; // Wrap around if less than 0
//                 }
//             }
//             currentValue = Clock_Date_Digits[Day_unit].value;
//             break;

//             // case Day_Units_Position: // Units digit of the day
//             //     if (Clock_Date_Digits[Day_unit].value > 0 && Clock_Date_Digits[Day_tens].value != 3) {
//             //         Clock_Date_Digits[Day_unit].value--;
//             //     } else if(Clock_Date_Digits[Day_unit].value == 1 && Clock_Date_Digits[Day_tens].value == 3)
//             //     {
//             //         Clock_Date_Digits[Day_unit].value == 0;
//             //     } else if(Clock_Date_Digits[Day_unit].value == 0 && Clock_Date_Digits[Day_tens].value == 3)
//             //     {
//             //         Clock_Date_Digits[Day_unit].value == 1;
//             //     }
//             //     else {
//             //         Clock_Date_Digits[Day_unit].value = 9; // Wrap around if exceeds 9
//             //     }
//             //     break;
//             case Month_Tens_Postion: // Tens digit of the month
//                         Clock_Date_Digits[Month_tens].value = currentValue;

//                 if (Clock_Date_Digits[Month_tens].value > 0) {
//                     Clock_Date_Digits[Month_tens].value--;
//                 }else {
//                     Clock_Date_Digits[Month_tens].value = 1; // Wrap around if exceeds 1
//                 }
//                 currentValue = Clock_Date_Digits[Month_tens].value;
//                 break;
//             case Month_Units_Position: // Units digit of the month
//                         Clock_Date_Digits[Month_unit].value = currentValue;

//                 if (Clock_Date_Digits[Month_unit].value > 0)
//                 {
//                     Clock_Date_Digits[Month_unit].value--;
//                 }
//                 else {
//                     Clock_Date_Digits[Month_unit].value = 9; // Wrap around if exceeds 2
//                 }
//                 currentValue = Clock_Date_Digits[Month_unit].value;
//                 break;
//                 default:
//                 break;
//         }
//         }
//         else if (editAppState.position.row == 2)
//         {
//             switch (editAppState.position.column) {
//             case Hours_Tens_Position: // Tens digit of the hour
//                         Clock_Date_Digits[Hours_tens].value = currentValue;

//                 if (Clock_Date_Digits[Hours_tens].value > 0) {
//                     Clock_Date_Digits[Hours_tens].value--;
//                 } else {
//                     Clock_Date_Digits[Hours_tens].value = 2; // Wrap around if exceeds 2
//                 }
//                 currentValue = Clock_Date_Digits[Hours_tens].value;
//                 break;
//             case Hours_Units_Position: // Units digit of the hour
//                         Clock_Date_Digits[Hours_unit].value = currentValue;

//                 if ((Clock_Date_Digits[Hours_unit].value > 0 && Clock_Date_Digits[Hours_tens].value != 2)
//                 || (Clock_Date_Digits[Hours_unit].value > 3)) {
//                     Clock_Date_Digits[Hours_unit].value--;
//                 } else {
//                     Clock_Date_Digits[Hours_unit].value = 9; // Wrap around if exceeds 9
//                 }
//                 currentValue = Clock_Date_Digits[Hours_unit].value;
//                 break;
//             case Minuts_Tens_Position: // Tens digit of the minute
//                         Clock_Date_Digits[Minutes_tens].value = currentValue;

//                 if (Clock_Date_Digits[Minutes_tens].value > 0) {
//                         Clock_Date_Digits[Minutes_tens].value--;
//                     } else {
//                         Clock_Date_Digits[Minutes_tens].value = 5; // Wrap around if exceeds 2
//                     }
//                     currentValue = Clock_Date_Digits[Minutes_tens].value;
//                 break;
//             case Minuts_Units_Position: // Units digit of the minute
//                         Clock_Date_Digits[Minutes_unit].value = currentValue;

//                 if (Clock_Date_Digits[Minutes_unit].value > 0) {
//                         Clock_Date_Digits[Minutes_unit].value--;
//                     } else {
//                         Clock_Date_Digits[Minutes_unit].value = 9; // Wrap around if exceeds 2
//                     }
//                     currentValue = Clock_Date_Digits[Minutes_unit].value;
//                     break;
//             case Seconds_Tens_Position: // Tens digit of the second
//                         Clock_Date_Digits[Second_tens].value = currentValue;

//             if (Clock_Date_Digits[Second_tens].value > 0) {
//                         Clock_Date_Digits[Second_tens].value--;
//                     } else {
//                         Clock_Date_Digits[Second_tens].value = 5; // Wrap around if exceeds 2
//                     }
//                     currentValue = Clock_Date_Digits[Second_tens].value;
//                 break;
//             case Seconds_Units_Position: // Units digit of the second
//                         Clock_Date_Digits[Second_unit].value = currentValue;

//                 // Implement increment logic for minutes and seconds
//                if (Clock_Date_Digits[Second_unit].value > 0) {
//                         Clock_Date_Digits[Second_unit].value--;
//                     } else {
//                         Clock_Date_Digits[Second_unit].value = 9; // Wrap around if exceeds 2
//                     }
//                     currentValue = Clock_Date_Digits[Second_unit].value;
//                 break;
//             default:
//                 break;
//         }
//         }
        
//         // Update the LCD with the new value
//         lcdStatus = LCD_enuWriteNumberpos(currentValue, editAppState.position.row, editAppState.position.column);
//         if (lcdStatus != LCD_enumOK) {
//             // Handle error updating the LCD
//         }
//     } else {
//         // Handle error reading the number from the LCD
//     }
// }


// // void Edit_App_SendCursorPos(u8 row, u8 column) {
// //     // Function to send cursor position over UART
// //     // Implement UART transmission logic here
// // }



// void GeneralEditMode(void)
// {

// }
// void DigitEditMode(void)
// {

// }

// u8 Search_Digit(u8 X_Pos,u8 Y_Pos)
// {
    
// }