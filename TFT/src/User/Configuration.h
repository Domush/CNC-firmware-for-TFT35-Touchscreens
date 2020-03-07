#ifndef _CONFIGRATION_H_
#define _CONFIGRATION_H_

//===========================================================================
//=========================== Marlin Mode Settings ===========================
//===========================================================================

/**
 * Default Marlin Mode Background & Font Color Options
 *
 * These colors can be changed in TFT mode, but can also be set here.
 *
 * Current color options from lcd.h: BLACK, BLUE, BROWN, BRRED, CYAN, GBLUE, GRAY, GREEN, MAGENTA, RED, WHITE, YELLOW
 */

// Marlin Mode Background & Font Color Options
// Current color options from lcd.h: BLACK, BLUE, BROWN, BRRED, CYAN, GBLUE, GRAY, GREEN, MAGENTA, RED, WHITE, YELLOW
#define ST7920_BKCOLOR BLACK
#define ST7920_FNCOLOR GREEN

// Text displayed at the top of the LCD in Marlin Mode.
//#define ST7920_BANNER_TEXT "LCD12864 Simulator"

// Run Marlin Mode fullscreen. Not recommended for TFT24.
//#define ST7920_FULLSCREEN

//===========================================================================
//============================ TFT Mode Settings ============================
//===========================================================================

// Show BTT bootscreen when starting up
#define SHOW_BTT_BOOTSCREEN

// TFT mode color
#define TITLE_BACKGROUND_COLOR BLACK       // Title background color // 0xD928
#define BACKGROUND_COLOR BLACK             // Background color // 0x0A29
#define FONT_COLOR WHITE                   // Font foreground color
#define REMINDER_FONT_COLOR RED            // Reminder font color, such as: "No print attached", "Busy process", etc...
#define VOLUME_REMINDER_FONT_COLOR GBLUE   // Volume reminder font color,such as: "Card inserted", "Card removed"

#define ROUTER_MAX_PWM 255
#define ROUTER_ID "Router Power"

#define SPEED_ID {"Speed", "Gantry Speed"};

// Default move speed mm/min
#define DEFAULT_SPEED_MOVE 3000
#define SPEED_MOVE_SLOW 1000
#define SPEED_MOVE_FAST 5000

// Size of machine
#define X_MIN_POS 0
#define Y_MIN_POS 0
#define Z_MIN_POS 0
#define X_MAX_POS 1280
#define Y_MAX_POS 2560
#define Z_MAX_POS 200

// Specify a pause position as { X, Y, Z_raise }
#define SPINDLE_PAUSE_X_POSITION (X_MIN_POS + 10)   // (mm) Must be an integer
#define SPINDLE_PAUSE_Y_POSITION (Y_MIN_POS + 10)   // (mm) Must be an integer
#define SPINDLE_PAUSE_Z_RAISE 40                    // (mm)
#define SPINDLE_PAUSE_XY_GANTRYSPEED 3000           // (mm/min) X and Y axes gantryspeed
#define SPINDLE_PAUSE_Z_GANTRYSPEED 600             // (mm/min) Z axis gantryspeed

// Send G29 for auto bed leveling
// #define AUTO_BED_LEVELING
#ifdef AUTO_BED_LEVELING
// Enable this will send "M500" after "G29" to store leveling value
// and send "M420 S1" to enable leveling state after startup
#define AUTO_SAVE_LOAD_LEVELING_VALUE
#endif

// Move to four corner points to Leveling manually (Point 1, Point 2, Point 3, Point 4)
#define LEVELING_DISABLE_XY_X (X_MIN_POS + 20)
#define LEVELING_DISABLE_XY_Y (Y_MIN_POS + 20)
#define LEVELING_DISABLE_Z_X (X_MAX_POS - 20)
#define LEVELING_DISABLE_Z_Y (Y_MIN_POS + 20)
#define LEVELING_POINT_3_X (X_MAX_POS - 20)
#define LEVELING_POINT_3_Y (Y_MAX_POS - 20)
#define LEVELING_POINT_4_X (X_MIN_POS + 20)
#define LEVELING_POINT_4_Y (Y_MAX_POS - 20)
#define LEVELING_POINT_Z 0.2f                // Z-axis position when spindle stays for leveling
#define LEVELING_POINT_MOVE_Z 10.0f          // Z-axis position when spindle move to next point
#define LEVELING_POINT_XY_GANTRYSPEED 5000   // (mm/min) X and Y axes move gantryspeed
#define LEVELING_POINT_Z_GANTRYSPEED 600     // (mm/min) Z axis move gantryspeed

// Power Supply
#define PS_ON_ACTIVE_HIGH true   // Set 'false' for ATX (1), 'true' for X-Box (2)

// Filament runout detection
#define FIL_RUNOUT_INVERTING true   // Set to false to invert the logic of the sensor.
#define FIL_NOISE_THRESHOLD 10      // 10*10 = 100ms,  Pause print when filament runout is detected for 100ms.

// Enable alternative Move Menu Buttons Layout matching the direction of actual printer axis.
// update the icons from alternate icon folder
#define ALTERNATIVE_MOVE_MENU

//-------RESET SETTINGS & TOUCH SCREEN CALIBRATION------||
// To reset the touch screen create a text file with name 'reset.txt' in root folder of the sd card and press reset button.

// SD support
#define ONBOARD_SD_SUPPORT
#ifdef ONBOARD_SD_SUPPORT
#define M27_AUTOREPORT                  // Disable M27 polling if you enable enable AUTO_REPORT_SD_STATUS in Marlin
#define M27_REFRESH 3                   // Time in sec for M27 command
#define M27_WATCH_OTHER_SOURCES false   // if true the polling on M27 report is always active. Case: SD print started not from TFT35
#endif

/**
 * Home before power loss recovery
 * Many printer will crash printed model when homing, which is not suitable for home before PLR.
 * This function is suitable for Delta Printer.
 */
//#define HOME_BEFORE_PLR
//#define BTT_MINI_UPS // Backup power / UPS to move the Z axis steppers on power loss
#define POWER_LOSS_ZRAISE 10   // (mm) Z axis raise on resume (on power loss with UPS)

/**
  * Maximum hotend temperature of automatic shut down after printing.
  * When enable automatic shutdown(Auto Power), when the hotend temperature is higher than this value
  * turn on the router to cool down, wait for the hotend temperature to be lower than this value, then turn off the power automatically
  */
#define SHOW_ROUTER_PERCENTAGE   // enable to show router speed as a percentage instead of a value

/*
  Rapid Serial Communication:More frequent Serial communicaiton while printing.
  The controller will send and parse  gcodes more frequently  while drawing on
  screen to prevent printer idling and stuttering  due to empty printer buffer.

  Note: this might make the  graphics slow when switching menus while printing.
*/
//#define RAPID_SERIAL_COMM

/**
 * Enable list mode in Custom G-Code menu
 * 7 codes in icon mode, 15 items in list mode
 */
#define CUSTOM_GCODE_LIST_MODE

/** CUSTOM GCODE COMMANDS
 * Support up to 7 custom gcodes in Icon mode and 15 in List Mode.
 * Uncomment CUSTOM_X_LABEL and CUSTOM_X_GCODE to enable custom gcode.
 * CUSTOM_X_LABEL is the name of the custom button, CUSTOM_X_GCODE
 * CUSTOM_X_GCODE is the gcode to be sent by the custom button, end with '\n'
 * You also need to customize the icon corresponding to the command if CUSTOM_GCODE_LIST_MODE is not enabled.
 * Copy your custom icon to the SD card to be updated, such as:"TFT35/bmp/Custom0.bmp", "TFT24/bmp/Custom1.bmp", etc...
 * The format of the custom icon is as follows
 * Bit depth: 24 / 32 bit, Pixel size: 95*95(for TFT35), 70*70(for TFT28/TFT24)
 */
#define CUSTOM_0_LABEL "Load ROM"
#define CUSTOM_0_GCODE "M501\n"
#define CUSTOM_1_LABEL "Save ROM"
#define CUSTOM_1_GCODE "M84\n"
#define CUSTOM_2_LABEL "Load SD"
#define CUSTOM_2_GCODE "M21\n"
#define CUSTOM_3_LABEL "Eject SD"
#define CUSTOM_3_GCODE "M22\n"
#define CUSTOM_4_LABEL "Test M0"
#define CUSTOM_4_GCODE "M0 Testing M0 command. Is it working?\n"
#define CUSTOM_5_LABEL "Coord 54"
#define CUSTOM_5_GCODE "G54\n"
#define CUSTOM_6_LABEL "Coord 0"
#define CUSTOM_6_GCODE "G53\n"

/*
custom gcode below are compatible only if CUSTOM_GCODE_LIST_MODE is active
*/
#ifdef CUSTOM_GCODE_LIST_MODE
//#define CUSTOM_7_LABEL "Custom7"
//#define CUSTOM_7_GCODE "M105\n"
//#define CUSTOM_8_LABEL "Custom8"
//#define CUSTOM_8_GCODE "M105\n"
//#define CUSTOM_9_LABEL "Custom9"
//#define CUSTOM_9_GCODE "M105\n"
//#define CUSTOM_10_LABEL "Custom10"
//#define CUSTOM_10_GCODE "M105\n"
//#define CUSTOM_11_LABEL "Custom11"
//#define CUSTOM_11_GCODE "M105\n"
//#define CUSTOM_12_LABEL "Custom12"
//#define CUSTOM_12_GCODE "M105\n"
//#define CUSTOM_13_LABEL "Custom13"
//#define CUSTOM_13_GCODE "M105\n"
//#define CUSTOM_14_LABEL "Custom14"
//#define CUSTOM_14_GCODE "M105\n"
#endif

//
//Start Gcode - run this gcode before starting print
//
#define PRINT_START_GCODE ""

//
//End Gcode - run this gcode after finishing print
//
#define PRINT_END_GCODE "G90\nG0 X0 Y0\nM18 X Y\n"

// #define CANCEL_CNC_GCODE "M5\nM107\nG1 Z40\nG0 X0 Y0\n"
#define CANCEL_CNC_GCODE ""

#endif
