#ifndef AIRSYNC_H
#define AIRSYNC_H

#include "debugmacros.h"
#include "discreettimebase.h"
#include "espdelay.h"
#include "espi2s.h"
#include "DigitalFilters.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <future>
#include <iomanip>
#include <numeric>

#include <esp_system.h>
#include <esp_log.h>
#include <esp_task_wdt.h>
#include <esp_timer.h>
#include <esp_types.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <driver/i2s_std.h>
#include <driver/i2s_common.h>
#include <driver/gpio.h>
#include <nvs.h>
#include <nvs_flash.h>
#include <rtc_wdt.h>
#include <soc/timer_group_struct.h>
#include <soc/timer_group_reg.h>
#include <soc/rtc_cntl_reg.h>
#include <soc/rtc.h>
#include <soc/syscon_reg.h>
#include <soc/rtc_cntl_struct.h>
#include <soc/gpio_pins.h>
#include <soc/gpio_reg.h>
#include <soc/io_mux_reg.h>

#endif
