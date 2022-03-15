# ESP32 A2DP sink with extra codecs

Supports aptX, aptX-HD, aptX-LL, LDAC codecs.

Use `esp-idf` branch with extra A2DP sink codecs https://github.com/cfint/esp-idf

## Measuring codec latency

Measure latency with `jack_iodelay`.

### Setup

List jack ports

    jack_lsp

Latency measure script

    # cat io-delay-test.sh
    jack_iodelay &
    sleep 1
    jack_connect jack_delay:out system:playback_1
    jack_connect system:capture_1 jack_delay:in
    sleep 5
    killall jack_iodelay

Measure latency command

    PIPEWIRE_LATENCY=256/48000 ./io-delay-test.sh

Reference:
https://gitlab.freedesktop.org/pipewire/pipewire/-/wikis/Performance#latency


### Results

Source: arch linux + libpipewire 0.3.47 + bluez 5.63 Intel 9560 Wifi/Bluetooth  
Sink: ESP32 internal DAC

| Codec | Latency (ms) |
|---|---|
| SBC | 67.056 |
| SBC-XQ | 66.452 |
| aptX | 72.438 |
| aptX-HD | 75.102 |
| aptX-LL | 42.820 |
| LDAC | 60.657 |


## Pipewire A2DP configuration

Get template `bluez-monitor.conf` from `/etc/pipewire/media-session.d/`. Copy it to `~/.config/pipewire/media-session.d/`.

Sample rate:

    #bluez5.default.rate     = 48000

LDAC quality:

    #bluez5.a2dp.ldac.quality = auto

Apply configs by restarting pipewire:

    systemctl --user restart pipewire.service
