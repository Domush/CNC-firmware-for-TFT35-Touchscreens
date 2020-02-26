#ifndef _LEDCOLOR_H_
#define _LEDCPLOR_H_

#include "variants.h"

//24bit                           // G  R  B
#define LED_OFF 0x00000000
#define LED_WHITE 0x00FFFFFF
#define LED_RED 0x0000FF00
#define LED_ORANGE 0x00FF8C00
#define LED_YELLOW 0x00FFFF00
#define LED_GREEN 0x00FF0000
#define LED_BLUE 0x000000FF
#define LED_INDIGO 0x004B0082D
#define LED_VIOLET 0x0000FEFE

/*颜色宏定�?*/                               /*R G B*/
#define COLOR_LIGHTPINK 0xFFB6C1             //	Light powder?
#define COLOR_PINK 0xFFC0CB                  //	Pink
#define COLOR_CRIMSON 0xDC143C               //	scarlet
#define COLOR_LAVENDERBLUSH 0xFFF0F5         //	Blush?
#define COLOR_PALEVIOLETRED 0xDB7093         //	Pale violet red
#define COLOR_HOTPINK 0xFF69B4               //	Love Pink
#define COLOR_DEEPPINK 0xFF1493              //	Deep powder?
#define COLOR_MEDIUMVIOLETRED 0xC71585       //	Moderate Violet Red
#define COLOR_ORCHID 0xDA70D6                //	Orchid purple
#define COLOR_THISTLE 0xD8BFD8               //	�?
#define COLOR_PLUM 0xDDA0DD                  //	Plum
#define COLOR_VIOLET 0xEE82EE                //	Roland
#define COLOR_MAGENTA 0xFF00FF               //	Magenta
#define COLOR_FUCHSIA 0xFF00FF               //	Cage Sea (red)
#define COLOR_DARKMAGENTA 0x8B008B           //	Dark magenta
#define COLOR_PURPLE 0x800080                //	�?�?
#define COLOR_MEDIUMORCHID 0xBA55D3          //	Moderate Orchid Purple
#define COLOR_DARKVOILET 0x9400D3            //	Deep violet?
#define COLOR_DARKORCHID 0x9932CC            //	Deep Orchid Purple
#define COLOR_INDIGO 0x4B0082                //	indigo
#define COLOR_BLUEVIOLET 0x8A2BE2            //	Deep violet blue?
#define COLOR_MEDIUMPURPLE 0x9370DB          //	Moderate purple?
#define COLOR_MEDIUMSLATEBLUE 0x7B68EE       //	Moderate slate dark blue gray?
#define COLOR_SLATEBLUE 0x6A5ACD             //	Slate dark blue grey
#define COLOR_DARKSLATEBLUE 0x483D8B         //	Dark Rock Dark Blue Grey
#define COLOR_LAVENDER 0xE6E6FA              //	The lightness of lavender flowers?
#define COLOR_GHOSTWHITE 0xF8F8FF            //	White ghost?
#define COLOR_BLUE 0x0000FF                  //	�?�?
#define COLOR_MEDIUMBLUE 0x0000CD            //	Moderate blue?
#define COLOR_MIDNIGHTBLUE 0x191970          //	Noon blue
#define COLOR_DARKBLUE 0x00008B              //	Cyan?
#define COLOR_NAVY 0x000080                  //	Navy?
#define COLOR_ROYALBLUE 0x4169E1             //	Emperor Blue
#define COLOR_CORNFLOWERBLUE 0x6495ED        //	Cornflower blue
#define COLOR_LIGHTSTEELBLUE 0xB0C4DE        //	Light steel
#define COLOR_LIGHTSLATEGRAY 0x778899        //	Light slate gray
#define COLOR_SLATEGRAY 0x708090             //	Slate?
#define COLOR_DODERBLUE 0x1E90FF             //	Road? Blue
#define COLOR_ALICEBLUE 0xF0F8FF             //	Alice Blue
#define COLOR_STEELBLUE 0x4682B4             //	Steel blue
#define COLOR_LIGHTSKYBLUE 0x87CEFA          //	Light blue?
#define COLOR_SKYBLUE 0x87CEEB               //	Sky blue?
#define COLOR_DEEPSKYBLUE 0x00BFFF           //	Deep sky?
#define COLOR_LIGHTBLUE 0xADD8E6             //	Light blue
#define COLOR_POWDERBLUE 0xB0E0E6            //	�?�?�?
#define COLOR_CADETBLUE 0x5F9EA0             //	Military school?
#define COLOR_AZURE 0xF0FFFF                 //	Azure?
#define COLOR_LIGHTCYAN 0xE1FFFF             //	Light green?
#define COLOR_PALETURQUOISE 0xAFEEEE         //	Pale emerald
#define COLOR_CYAN 0x00FFFF                  //	Blue
#define COLOR_AQUA 0x00FFFF                  //	Water green?
#define COLOR_DARKTURQUOISE 0x00CED1         //	Dark Emerald
#define COLOR_DARKSLATEGRAY 0x2F4F4F         //	Deep slate gray
#define COLOR_DARKCYAN 0x008B8B              //	Deep green?
#define COLOR_TEAL 0x008080                  //	Water duck?
#define COLOR_MEDIUMTURQUOISE 0x48D1CC       //	Moderate Emerald
#define COLOR_LIGHTSEAGREEN 0x20B2AA         //	Light ocean green
#define COLOR_TURQUOISE 0x40E0D0             //	Emerald
#define COLOR_AUQAMARIN 0x7FFFAA             //	Green Jade \ Blue Green?
#define COLOR_MEDIUMAQUAMARINE 0x00FA9A      //	Moderate turquoise
#define COLOR_MEDIUMSPRINGGREEN 0x00FF7F     //	Moderate spring green
#define COLOR_MINTCREAM 0xF5FFFA             //	Mint cream
#define COLOR_SPRINGGREEN 0x3CB371           //	Green of spring?
#define COLOR_SEAGREEN 0x2E8B57              //	Ocean?
#define COLOR_HONEYDEW 0xF0FFF0              //	honey
#define COLOR_LIGHTGREEN 0x90EE90            //	Light green?
#define COLOR_PALEGREEN 0x98FB98             //	Pale green?
#define COLOR_DARKSEAGREEN 0x8FBC8F          //	Deep Ocean Green
#define COLOR_LIMEGREEN 0x32CD32             //	Acid green
#define COLOR_LIME 0x00FF00                  //	Acid color
#define COLOR_FORESTGREEN 0x228B22           //	Forest green
#define COLOR_GREEN 0x008000                 //	�?�?
#define COLOR_DARKGREEN 0x006400             //	Dark green?
#define COLOR_CHARTREUSE 0x7FFF00            //	Chateau Green
#define COLOR_LAWNGREEN 0x7CFC00             //	Lawn?
#define COLOR_GREENYELLOW 0xADFF2F           //	Green yellow?
#define COLOR_OLIVEDRAB 0x556B2F             //	Olive brown
#define COLOR_BEIGE 0xF5F5DC                 //	Beige (light color)
#define COLOR_LIGHTGOLDENRODYELLOW 0xFAFAD2  //	Shallow? Unicorn yellow
#define COLOR_IVORY 0xFFFFF0                 //	ivory
#define COLOR_LIGHTYELLOW 0xFFFFE0           //	Light yellow?
#define COLOR_YELLOW 0xFFFF00                //	�?�?
#define COLOR_OLIVE 0x808000                 //	Olive
#define COLOR_DARKKHAKI 0xBDB76B             //	Deep khaki
#define COLOR_LEMONCHIFFON 0xFFFACD          //	Lime
#define COLOR_PALEGODENROD 0xEEE8AA          //	Gray?
#define COLOR_KHAKI 0xF0E68C                 //	Khaki?
#define COLOR_GOLD 0xFFD700                  //	�?
#define COLOR_CORNISLK 0xFFF8DC              //	Corn?
#define COLOR_GOLDENROD 0xDAA520             //	Qiu Qi?
#define COLOR_FLORALWHITE 0xFFFAF0           //	Flower white
#define COLOR_OLDLACE 0xFDF5E6               //	Old decoration?
#define COLOR_WHEAT 0xF5DEB3                 //	Wheat?
#define COLOR_MOCCASIN 0xFFE4B5              //	Buckskin?
#define COLOR_ORANGE 0xFFA500                //	Orange
#define COLOR_PAPAYAWHIP 0xFFEFD5            //	Papaya
#define COLOR_BLANCHEDALMOND 0xFFEBCD        //	Bleached apricot?
#define COLOR_NAVAJOWHITE 0xFFDEAD           //	Navajo white
#define COLOR_ANTIQUEWHITE 0xFAEBD7          //	Ancient white?
#define COLOR_TAN 0xD2B48C                   //	Tan
#define COLOR_BRULYWOOD 0xDEB887             //	Strong tree
#define COLOR_BISQUE 0xFFE4C4                //	(Thick soup) cream, eggplant, etc.
#define COLOR_DARKORANGE 0xFF8C00            //	Deep color
#define COLOR_LINEN 0xFAF0E6                 //	Linen?
#define COLOR_PERU 0xCD853F                  //	Peru
#define COLOR_PEACHPUFF 0xFFDAB9             //	Pink
#define COLOR_SANDYBROWN 0xF4A460            //	Sand
#define COLOR_CHOCOLATE 0xD2691E             //	Chocolate
#define COLOR_SADDLEBROWN 0x8B4513           //	Saddle color
#define COLOR_SEASHELL 0xFFF5EE              //	Haibei?
#define COLOR_SIENNA 0xA0522D                //	Loess?
#define COLOR_LIGHTSALMON 0xFFA07A           //	Shallow fresh (salmon)?
#define COLOR_CORAL 0xFF7F50                 //	coral
#define COLOR_ORANGERED 0xFF4500             //	Orange red?
#define COLOR_DARKSALMON 0xE9967A            //	Deep Fresh (Salmon)?
#define COLOR_TOMATO 0xFF6347                //	�?�?
#define COLOR_MISTYROSE 0xFFE4E1             //	Haze
#define COLOR_SALMON 0xFA8072                //	Fresh meat (salmon)?
#define COLOR_SNOW 0xFFFAFA                  //	�?
#define COLOR_LIGHTCORAL 0xF08080            //	Light coral
#define COLOR_ROSYBROWN 0xBC8F8F             //	Color
#define COLOR_INDIANRED 0xCD5C5C             //	India?
#define COLOR_RED 0xFF0000                   //	�?�?
#define COLOR_BROWN 0xA52A2A                 //	brown
#define COLOR_FIREBRICK 0xB22222             //	Fireproof
#define COLOR_DARKRED 0x8B0000               //	Crimson?
#define COLOR_MAROON 0x800000                //	maroon
#define COLOR_WHITE 0xFFFFFF                 //	�?�?
#define COLOR_WHITESMOKE 0xF5F5F5            //	White smoke
#define COLOR_GAINSBORO 0xDCDCDC             //	Gray
#define COLOR_LIGHTGREY 0xD3D3D3             //	Light gray
#define COLOR_SILVER 0xC0C0C0                //	Silver white?
#define COLOR_DARKGRAY 0xA9A9A9              //	Dark gray?
#define COLOR_GRAY 0x808080                  //	gray
#define COLOR_DIMGRAY 0x696969               //	Dim gray?
#define COLOR_BLACK 0x000000                 //	�?�?

void led_color_Init(uint16_t psc, uint16_t arr);  //
void LED_color_PIN_IPN(void);
void ws2812_send_DAT(uint32_t ws2812_dat);
#endif
