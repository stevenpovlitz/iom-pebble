#include <pebble.h>
#include <time.h> // for seeding rand num generator
#include <stdlib.h>  // for generating rand nums, other stuff

Window *window;
TextLayer *text_layer;
int uniqueID = 0; // for distinguishing pebbles
int dataKey = 0; // for use in sendData() function

static Window *s_main_window;
static TextLayer *s_output_layer;

// create dictionary, send data to pebblekit JS
void sendData(int x, int y, int z){
  // Prepare dictionary
  DictionaryIterator *iterator;
  app_message_outbox_begin(&iterator);
  
  // Write data
  dict_write_int(iterator, dataKey, &x, sizeof(int), true /* signed */);
  dict_write_int(iterator, dataKey, &y, sizeof(int), true /* signed */);
  dict_write_int(iterator, dataKey, &z, sizeof(int), true /* signed */);
  
  // Send the data!
  dataKey++;
  app_message_outbox_send();
}

// generate user's ID, (very) roughly in range 1 - 2**30
void generateID() {
  srand(time(NULL));
  uniqueID = rand();
  printf("Unique ID is %d", uniqueID);
}

// create window
static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_output_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 10, window_bounds.size.h));
  text_layer_set_font(s_output_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text(s_output_layer, "IOM\n");
  text_layer_set_overflow_mode(s_output_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
  
  // show the unique ID to users
  static char s_buffer[128];
  snprintf(s_buffer, sizeof(s_buffer), 
      "Pebble ID is %10d", uniqueID
  );
  //Show the data
  text_layer_set_text(s_output_layer, s_buffer);
}

// destroy window
static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_output_layer);
}

// init, set it all up
void handle_init(void) {
  // generate our ID
  generateID();
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
}

// free up memory
void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(text_layer);
	
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}