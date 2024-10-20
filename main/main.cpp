#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "BluetoothA2DPSinkQueued.h"

I2SStream out;
BluetoothA2DPSinkQueued a2dp_sink(out);


extern "C" void app_main(void)
{
    I2SConfig i2s_config;
    i2s_config.pin_ws = 12;
    i2s_config.pin_bck = 13;
    i2s_config.pin_data = 15;
    out.begin(i2s_config);
    // out.end();

    a2dp_sink.set_task_core(0);
    a2dp_sink.start("ESP_SPEAKER");
}
