#include <pebble.h>
#include <time.h> // for seeding rand num generator
#include <stdlib.h>  // for generating rand nums, other stuff

Window *window;
TextLayer *text_layer;
int uniqueID = 0; // for distinguishing pebbles

static Window *s_main_window;
static TextLayer *s_output_layer;
  
// itoa, to convert num to string str with base 10
char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
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