/*
 * SPDX-FileCopyrightText: 2022 Chris Johnson <tinfc@protonmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "BluetoothA2DPSink.h"

static const char *TAG = "main";

BluetoothA2DPSink a2dp_sink;


void setup() {
  a2dp_sink.activate_pin_code(false);

  const i2s_config_t i2s_config = {
      .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
      .sample_rate = 44100, // corrected by info from bluetooth
      .bits_per_sample = (i2s_bits_per_sample_t) 16, /* the DAC module will only take the 8bits from MSB */
      .channel_format =  I2S_CHANNEL_FMT_RIGHT_LEFT,
#if (ESP_IDF_VERSION_MAJOR == 4 && ESP_IDF_VERSION_MINOR >= 4) || ESP_IDF_VERSION_MAJOR > 4
      .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_STAND_MSB),
#else
      .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_STAND_I2S),
#endif
      .intr_alloc_flags = 0, // default interrupt priority
      .dma_buf_count = 8,
      .dma_buf_len = 64,
      .use_apll = false,
      .tx_desc_auto_clear = true,
      .fixed_mclk = 0,
      .mclk_multiple = I2S_MCLK_MULTIPLE_DEFAULT,
      .bits_per_chan = I2S_BITS_PER_CHAN_DEFAULT,
  };

  a2dp_sink.set_i2s_config(i2s_config);
  a2dp_sink.start("InternalDAC", false);
}

extern "C" void app_main(void)
{
    setup();
}
