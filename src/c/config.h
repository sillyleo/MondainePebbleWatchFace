#include "pebble.h"
  
static const GPathInfo MINUTE_HAND_POINTS = {
  4,
  (GPoint []) {
    { -4, 16 },
    { 6, 16 },
    { 4, -66 },
    { -2, -66 }
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
  4,
  (GPoint []) {
    { -5, 16 },
    { 7, 16 },
    { 5, -45 },
    { -3, -45 }
  }
};

typedef enum ResId{
	ResId_BluetoothConnected = RESOURCE_ID_IMAGE_BLUETOOTHCONNECTED,
	ResId_BluetoothDisconnected = RESOURCE_ID_IMAGE_BLUETOOTHDISCONNECTED,
	ResId_BgWhite = RESOURCE_ID_IMAGE_MONDAINE_WHITEBG,
	ResId_BgWhiteLogo = RESOURCE_ID_IMAGE_MONDAINE_WHITENLBG,
	ResId_BgBlack = RESOURCE_ID_IMAGE_MONDAINE_BLACKBG,
	ResId_BgBlackLogo = RESOURCE_ID_IMAGE_MONDAINE_BLACKNLBG
}ResId;


// Number of updates per second, must be < 1000
#define STOP2GO_TICK_RESOLUTION 4

/*
// Force the users to shake X times to dismiss the bluetooth disconnection screen
#define BLUETOOTH_DISCONNECTION_SHAKE_TIMES 2
static uint8_t s_bluetooth_disconnection_dismissal_attempt = 0;
*/

#define CONFIGS_KEY 0

static int s_persist_value_read, s_persist_value_written;
static AppSync s_sync;
static uint8_t s_sync_buffer[128];
static Layer *main_layer;

typedef enum WatchfaceType{
	WatchfaceType_WhiteLogo,
	WatchfaceType_White,
	WatchfaceType_BlackLogo,
	WatchfaceType_Black,
}WatchfaceType;

typedef enum SecondType{
	SecondType_1Sec,
	SecondType_5Sec,
	SecondType_Hidden,
}SecondType;


typedef struct Persist {
	WatchfaceType watchfaceType;
	SecondType secondType;
	char dateoption[10];
	char hourlyvibration[10];
	uint8_t bluetoothstatusdetection;
	int cHourColor,cMinuteColor,cSecondColor;
} __attribute__((__packed__)) Persist;

Persist configs = {
// 	.dialcolor = "white",
	.watchfaceType = WatchfaceType_WhiteLogo,
// 	.secondhandoption = "quartz",
	.secondType = SecondType_1Sec,
	.dateoption = "nodate",
	.hourlyvibration = "off",
	.bluetoothstatusdetection = 0,
	.cHourColor = 0x000000,
	.cMinuteColor = 0x000000,
	.cSecondColor = 0xff000000
};

enum {
	DialColor_KEY = 0x0,
	SecondHandOption_KEY = 0x1,
	DateOption_KEY = 0x2,
	HourlyVibration_KEY = 0x3,
	BluetoothStatusDetection_KEY = 0x4
};

static Window *s_window;
static Layer *s_hands_layer;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

static GPath *s_minute_arrow, *s_hour_arrow;

static GFont s_res_gothic_18_bold;
static TextLayer *s_textlayer_date;

Window *bluetooth_connected_splash_window;
static BitmapLayer *s_bluetoothconnected_layer;
static GBitmap *s_bluetoothconnected_bitmap;

Window *bluetooth_disconnected_splash_window;
static BitmapLayer *s_bluetoothdisconnected_layer;
static GBitmap *s_bluetoothdisconnected_bitmap;