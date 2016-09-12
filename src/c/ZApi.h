#pragma once
#include <pebble.h>

//////////////////////////////////////
//////////image resource
//////////////////////////////////////
#define BUFFER_SIZE 8192
#define IMAGE_OPTIMAZED false
static uint32_t poolResourceId[100];
static GBitmap* poolResourceBitmap[100];
static bool poolResourceIsInited = false;

static void ZAPI_ImageINIT(){
	poolResourceIsInited = true;
	for(int i=0;i<100;i++){
		poolResourceId[i]=0;
		poolResourceBitmap[i] = NULL;
	}
}

static void ZAPI_ImageDestryAll(){
	for(int i=0;i<100;i++){
		if(poolResourceId[i]!=0){
			poolResourceId[i] = 0;
			gbitmap_destroy(poolResourceBitmap[i]);
			poolResourceBitmap[i] = NULL;
		}
	}
}

static void ZAPI_ImageDestroy(uint32_t resourceId){
	if(poolResourceId[resourceId]!=0){
		poolResourceId[resourceId] = 0;
		gbitmap_destroy(poolResourceBitmap[resourceId]);
		poolResourceBitmap[resourceId] = NULL;
	}
}

static GBitmap* ZAPI_ImageResource(uint32_t resourceId){
	if(!poolResourceIsInited){
		ZAPI_ImageINIT();
	}
	//have
	for(int i=0;i<100;i++){
		if(poolResourceId[i]==resourceId){
			return poolResourceBitmap[i];
		}
	}
	//no have
#if IMAGE_OPTIMAZED
// 	APP_LOG(APP_LOG_LEVEL_WARNING, "IMAGE_OPTIMAZED");
	uint8_t* load_buffer = malloc(BUFFER_SIZE);
	size_t png_size = resource_load(resource_get_handle(resourceId),load_buffer, BUFFER_SIZE);
	GBitmap* bitmap = gbitmap_create_from_png_data(load_buffer, png_size);
	free(load_buffer);
#else
// 	APP_LOG(APP_LOG_LEVEL_WARNING, "IMAGE_NORMAL");
	GBitmap* bitmap = gbitmap_create_with_resource(resourceId);
#endif
	for(int i=0;i<100;i++){
		if(poolResourceId[i]==0){
			poolResourceId[i] = resourceId;
			poolResourceBitmap[i] = bitmap;
			break;
		}
	}
	return bitmap;
}

static BitmapLayer* ZAPI_ImageCreate(uint32_t resourceId, GRect rect, bool isTransparent){
	APP_LOG(APP_LOG_LEVEL_WARNING, "image %d",(int)resourceId);
	
// 	GBitmap* bitmap = gbitmap_create_with_resource(resourceId);
	
	GBitmap* bitmap = ZAPI_ImageResource(resourceId);
	
	BitmapLayer* layer = bitmap_layer_create(rect);
	if(isTransparent){
		bitmap_layer_set_compositing_mode(layer, GCompOpSet);
	}
	bitmap_layer_set_bitmap(layer, bitmap);
	
	return layer;
}

static void ZAPI_ImageChange(BitmapLayer* bitmapLayer, uint32_t resourceId){
	APP_LOG(APP_LOG_LEVEL_WARNING, "image %d",(int)resourceId);
	GBitmap* bitmap = ZAPI_ImageResource(resourceId);
	bitmap_layer_set_bitmap(bitmapLayer, bitmap);
}

//////////////////////////////////////
//////////atoi
//////////////////////////////////////
static int ZAPI_atoi(char* str){
	if(!str){
		APP_LOG(APP_LOG_LEVEL_WARNING, "Enter valid string");
	}
	int number = 0;
	char* p = str;
	while((*p >= '0') && (*p <= '9')){
		number = number * 10 + (*p - '0');
		p++;
	} 
	return number;
}

static char* ZAPI_strtok(s, delim)register char *s;register const char *delim;{
	register char *spanp;
	register int c, sc;
	char *tok;
	static char *last;
	if (s == NULL && (s = last) == NULL)
		return (NULL);
	cont:
	c = *s++;
	for (spanp = (char *)delim; (sc = *spanp++) != 0;) {
		if (c == sc)
			goto cont;
	}
	if (c == 0) {		/* no non-delimiter characters */
		last = NULL;
		return (NULL);
	}
	tok = s - 1;
	for (;;) {
		c = *s++;
		spanp = (char *)delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				last = s;
				return (tok);
			}
		} while (sc != 0);
	}
}


//////////////////////////////////////
//////////Store
//////////////////////////////////////

#define SaveChar(typeCheck,value)\
if(savePool[typeCheck]){\
	savePool[typeCheck] = false;\
	ZAPI_Store_SaveString(typeCheck, value);\
}
#define SaveInt(typeCheck,value)\
if(savePool[typeCheck]){\
	savePool[typeCheck] = false;\
	ZAPI_Store_SaveInt(typeCheck, value);\
}
#define SaveBool(typeCheck,value)\
if(savePool[typeCheck]){\
	savePool[typeCheck] = false;\
	ZAPI_Store_SaveBool(typeCheck, value);\
}

#define LoadChar(typeCheck,defaultValue,value)\
ZAPI_Store_LoadString(typeCheck, defaultValue, value, sizeof(value))
#define LoadInt(typeCheck,defaultValue,value)\
ZAPI_Store_LoadInt(typeCheck, defaultValue, &value);
#define LoadBool(typeCheck,defaultValue,value)\
ZAPI_Store_LoadBool(typeCheck, defaultValue, &value);

static void ZAPI_Store_LoadString(int key, char* defaultValue, char* value,int valueSize){
	if (persist_exists(key) == true) {
		persist_read_string(key, value, valueSize);
	}else{
		strcpy(value, defaultValue);
	}
}

static void ZAPI_Store_LoadInt(int key, int defaultValue, int* value){
	if (persist_exists(key) == true) {
		*value = persist_read_int(key);
	}else{
		*value = defaultValue;
	}
}

static void ZAPI_Store_LoadBool(int key, bool defaultValue, bool* value){
	if (persist_exists(key) == true) {
		*value = persist_read_bool(key);
	}else{
		*value = defaultValue;
	}
}

static void ZAPI_Store_SaveString(int key, char* value){
	persist_write_string(key, value);
}

static void ZAPI_Store_SaveInt(int key, int value){
	persist_write_int(key, value);
}


static void ZAPI_Store_SaveBool(int key, bool value){
	persist_write_bool(key, value);
}

static void ZAPI_Store_Clear(int key){
	if (persist_exists(key) == true) {
		persist_delete(key);
	}
}

//////////////////////////////////////
//////////screen colorful check
//////////////////////////////////////
static bool __isScreenSupportColorfull = false;
static bool __isScreenSupportColorfullInit = false;
static bool ZAPI_IsScreenColorful(){
	if(__isScreenSupportColorfullInit){
		return __isScreenSupportColorfull;
	}
	__isScreenSupportColorfullInit = true;
	switch(watch_info_get_model()){
		case WATCH_INFO_MODEL_PEBBLE_ORIGINAL:
		case WATCH_INFO_MODEL_PEBBLE_STEEL :
		__isScreenSupportColorfull = false;
		break;
		default:
		__isScreenSupportColorfull = true;
		break;
	}
	return __isScreenSupportColorfull;
}



