#include <pebble.h>

//WINDOW
Window* window;
//LAYER
Layer* layer;
//IMAGES
GBitmap* digit_1 = NULL;
GBitmap* digit_2 = NULL;

uint32_t images[] = {RESOURCE_ID_IMAGE_0, RESOURCE_ID_IMAGE_1, RESOURCE_ID_IMAGE_2, RESOURCE_ID_IMAGE_3, RESOURCE_ID_IMAGE_4, RESOURCE_ID_IMAGE_5, RESOURCE_ID_IMAGE_6, RESOURCE_ID_IMAGE_7, RESOURCE_ID_IMAGE_8, RESOURCE_ID_IMAGE_9};

void destroy_digits()
{
	if(digit_1 != NULL)
	{
		gbitmap_destroy(digit_1);
		digit_1 = NULL;
	}
	if(digit_2 != NULL)
	{
		gbitmap_destroy(digit_2);
		digit_2 = NULL;
	}
}

void update_layer(Layer *me, GContext* ctx) 
{
	destroy_digits();

	//get time
	time_t temp = time(NULL); 
  	struct tm *tick_time = localtime(&temp);
	
	//get date
	char date[3];
	strftime(date, 3, "%d", tick_time);
	
	//big date numbers
	if(date[0] != '0'){
		
		APP_LOG(APP_LOG_LEVEL_DEBUG, "%d %d", date[0], date[1]);
		//digit 1
		digit_1 = gbitmap_create_with_resource(images[(int)date[0]-48]);
		graphics_draw_bitmap_in_rect(ctx, digit_1, GRect(22,72,50,77));
		//digit 2
		digit_2 = gbitmap_create_with_resource(images[(int)date[1]-48]);
		graphics_draw_bitmap_in_rect(ctx, digit_2, GRect(72,72,50,77));
	}else{ //one digit
		//digit 2 only
		digit_2 = gbitmap_create_with_resource(images[(int)date[1]-48]);
		graphics_draw_bitmap_in_rect(ctx, digit_2,GRect(47,72,50,77));
	}

  	//black bar
	graphics_context_set_fill_color(ctx, GColorBlack);
	graphics_fill_rect(ctx, GRect(0,0,144,55), 0, GCornerNone);	
	
	//text
	char text[10];
	graphics_context_set_text_color(ctx, GColorWhite);
	
	//weekday
	strftime(text, 10, "%A", tick_time);
	graphics_draw_text(ctx, text,  fonts_get_system_font(FONT_KEY_GOTHIC_18), GRect(5,5,144,100), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);
	
	//month	
	strftime(text, 10, "%B", tick_time);
	graphics_draw_text(ctx, text,  fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD), GRect(5,20,144,60), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);	
	
}

void tick(struct tm *tick_time, TimeUnits units_changed)
{
	layer_mark_dirty(layer);
}

void init() 
{
	//create window
	window = window_create();
	window_stack_push(window, true);
	Layer* window_layer = window_get_root_layer(window);	
	
	//create layer
	layer = layer_create(GRect(0,0,144,168));
	layer_set_update_proc(layer, update_layer);
	layer_add_child(window_layer, layer);
	
	/*
	
	//tick event
	tick_timer_service_subscribe(DAY_UNIT, (TickHandler) tick);
*/
}

void deinit() 
{
	window_destroy(window);
	layer_destroy(layer);
	destroy_digits();
}

int main()
{
	init();
	app_event_loop();
	deinit();
}
