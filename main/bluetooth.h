#include <stdint.h>

typedef void (*data_callback_t)(uint8_t *, uint32_t);

void start_bluetooth_register_callback(data_callback_t cb_param);