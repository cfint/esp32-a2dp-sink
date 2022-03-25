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


void bits_per_sample_cb(uint8_t bit_depth)
{
    ESP_LOGI(TAG, "%s: bit_depth = %d", __func__, bit_depth);

    if (bit_depth == 32) {
        a2dp_sink.set_swap_lr_channels(true);
    } else {
        a2dp_sink.set_swap_lr_channels(false);
    }
}

void setup() {
  a2dp_sink.activate_pin_code(false);

  const i2s_config_t i2s_config = i2s_config_t {
#ifdef CONFIG_A2DP_SINK_OUTPUT_INTERNAL_DAC
      .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
#else
      .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX),
#endif
      .sample_rate = 44100,
      .bits_per_sample = (i2s_bits_per_sample_t)16,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
#ifdef CONFIG_A2DP_SINK_OUTPUT_INTERNAL_DAC
      .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_STAND_MSB),
#else
      .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_STAND_I2S),
#endif
      .intr_alloc_flags = 0, // default interrupt priority
      .dma_buf_count = 8,
      .dma_buf_len = 64,
#ifdef CONFIG_A2DP_SINK_OUTPUT_INTERNAL_DAC
      .use_apll = false,
      .tx_desc_auto_clear = false,
#else
      .use_apll = true,
      .tx_desc_auto_clear = true, // avoiding noise in case of data unavailability
#endif
      .fixed_mclk = 0,
      .mclk_multiple = I2S_MCLK_MULTIPLE_DEFAULT,
      .bits_per_chan = I2S_BITS_PER_CHAN_DEFAULT,
  };
  a2dp_sink.set_i2s_config(i2s_config);

#ifdef CONFIG_A2DP_SINK_OUTPUT_EXTERNAL_I2S
  i2s_pin_config_t pin_config = i2s_pin_config_t {
      .bck_io_num = CONFIG_A2DP_SINK_I2S_BCK_PIN,
      .ws_io_num = CONFIG_A2DP_SINK_I2S_LRCK_PIN,
      .data_out_num = CONFIG_A2DP_SINK_I2S_DATA_PIN,
      .data_in_num = I2S_PIN_NO_CHANGE
  };
  a2dp_sink.set_pin_config(pin_config);
#endif

  a2dp_sink.set_bps_callback(bits_per_sample_cb);

  a2dp_sink.start("InternalDAC", false);
}

extern "C" void app_main(void)
{
    setup();
}
