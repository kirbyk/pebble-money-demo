#include <pebble.h>

#define BUFF 64
static Window* window;
static TextLayer* text_layer;

static AppSync sync;
static uint8_t sync_buffer[BUFF];

enum MsgKeys {
  MONEY = 0x0
};

static void sync_error(DictionaryResult dict_error, 
                       AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_success(const uint32_t key, const Tuple* new_tuple, 
                         const Tuple* old_tuple, void* context) {
APP_LOG(APP_LOG_LEVEL_DEBUG, "App Sync Success %s", new_tuple->value->cstring);
  text_layer_set_text(text_layer, new_tuple->value->cstring);
}


static void window_load(Window* window) {
  Layer* window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(window, GColorOxfordBlue);
  text_layer = text_layer_create((GRect) {
    .origin = {0, 62},
    .size   = {bounds.size.w, 30}
  });
  text_layer_set_text(text_layer, "Hello, World!");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorChromeYellow);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  Tuplet initial_value[] = {
    TupletCString(MONEY, "$12,345,678")
  };
  
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_value, 
                ARRAY_LENGTH(initial_value), sync_success, sync_error, NULL);
}

static void window_unload(Window* window) {
  text_layer_destroy(text_layer);
}
  
static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers){
    .load   = window_load,
    .unload = window_unload
  });
  bool animated = true;
  app_message_open(BUFF, BUFF);
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}