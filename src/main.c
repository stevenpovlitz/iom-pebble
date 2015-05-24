#include <pebble.h>
#include <time.h> // for seeding rand num generator
#include <stdlib.h>  // for generating rand nums, other stuff

Window *window;
TextLayer *text_layer;
int uniqueID = 0; // for distinguishing pebbles
#define KEY_DATA 55 // for use in sendData() function

static Window *s_main_window;
static TextLayer *s_output_layer;

int accelCounter = 0;

// create dictionary, send data to pebblekit JS
void sendData(int x){
  
  // trying to get things working here
  // printf("%d,%d", (int)app_message_inbox_size_maximum(), (int)app_message_outbox_size_maximum());
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  // Prepare dictionary
  DictionaryIterator *iterator;
  app_message_outbox_begin(&iterator);
  
  // Write data
//   printf("we got to just before dict_write_int"); // ghetto debugging
  dict_write_int(iterator, KEY_DATA, &x, sizeof(int), false);
  //dict_write_int(iterator, dataKey, &y, sizeof(int), true /* signed */);
  //dict_write_int(iterator, dataKey, &z, sizeof(int), true /* signed */);
  
  // Send the data!
  app_message_outbox_send();
  dict_write_end(iterator);
}

static void data_handler(AccelData *data, uint32_t num_samples) {
  if (accelCounter % 10 == 5 && ((accelCounter / 10) % 2 == 0)){ // removed " accelCounter % 10 == 0 || "
    
    // format all accel data into one int, loose a little precision
    // WON'T WORK - NEGATIVES CAN'T BE ADDED TO POS WITHOUT LOOSING THEIR SIGN!!!!! (fuqqqqq)
    int result = (data[0].x / 10 * 1000000) + (data[0].y / 10 * 1000) + (data[0].z / 10);
    printf("%d, %d, %d" ,(data[0].x / 10 * 1000000), (data[0].y / 10 * 1000), (data[0].z / 10));
    printf("Result, formatted: %10d", result);
    sendData(result);
  }
  accelCounter++;
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
  
  // Subscribe to the accelerometer data service
  int num_samples = 1;
  accel_data_service_subscribe(num_samples, data_handler);
  
  // Choose update rate
  accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
}

// free up memory
void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(text_layer);
  
	accel_data_service_unsubscribe(); // testing with tap event
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}