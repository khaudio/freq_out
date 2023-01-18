/*----------------------------------------------------------------------------
 * Copyright 2022 K Hughes Production, LLC
 * All Rights Reserved.
 *----------------------------------------------------------------------------

ESP32 I2S
    I2S_WS                          I               26                  I2S input word select
    I2S_BCK                         I               14                  I2S input serial (bit) clock
    I2S_DI                          I               35                  I2S input data
    I2S_DO                          O               25                  I2S output data
    I2S_MCK                         O               0                   I2S master clock output
    I2S_SHUTDOWN                    O               21                  I2S shutdown signal */

#include "main.h"

/*                              Macros                              */

/* I2S master clock */
#define I2S_MCK                             (GPIO_NUM_0)

/* I2S word select */
#define I2S_WS                              (GPIO_NUM_26)

/* I2S bit clock */
#define I2S_BCK                             (GPIO_NUM_14)

/* I2S data */
#define I2S_DI                              (GPIO_NUM_35)

/* I2S output data */
#define I2S_DO                              (GPIO_NUM_25)

/* Shutdown signal output */
#define I2S_SHUTDOWN                        (GPIO_NUM_21)

#ifndef I2S_BUFFER_LENGTH
#define I2S_BUFFER_LENGTH                   128
#endif

/* Audio sample rate */
#ifndef SAMPLE_RATE
#define SAMPLE_RATE                         48000
#endif

/* Desired level of floating point precision */
#ifndef FLOAT_DATATYPE
#define FLOAT_DATATYPE                      double
#endif

/* Desired level of integer precision */
#ifndef INT_DATATYPE
#define INT_DATATYPE                        int
#endif

#define INT_BIT_DEPTH                       (sizeof(INT_DATATYPE) * 8)

/*                             Variables                            */

/* I2S I/O Bus */
static I2S::Bus i2s;

/*                           Declarations                           */

/* Low-pass filter */
void low_pass_filter(FLOAT_DATATYPE* buff);

/* Loop to ingest and set LTC using I2S */
void i2s_input_output_loop(void);

/* Entry point */
extern "C" void app_main(void);

/*                            Definitions                           */

void low_pass_filter(FLOAT_DATATYPE* buff)
{
    /* Do filtering... */
}

void i2s_input_output_loop(void)
{
    DEBUG_OUT("Starting i/o loop...\n");

    /* Delay counter for use in debug mode */
    DELAY_COUNTER_INT(0);

    /* int buffer for interfacing with i2s */
    INT_DATATYPE intBuffer[I2S_BUFFER_LENGTH];

    /* float buffer for processing */
    FLOAT_DATATYPE floatBuffer[I2S_BUFFER_LENGTH];

    // /* Signal polarity */
    // bool polarity(1);

    // /* Number of samples per zero-crossing (2x frequency) */
    // int numSamples(0);

    while (true)
    {
        i2s.read_bytes(intBuffer, I2S_BUFFER_LENGTH * sizeof(INT_DATATYPE));
        int_to_float<INT_DATATYPE, FLOAT_DATATYPE>(
                floatBuffer,
                intBuffer,
                I2S_BUFFER_LENGTH
            );
        
        // /* Detect zero-crossing */
        // for (int i(0); i < I2S_BUFFER_LENGTH; ++i)
        // {
        //     ++numSamples;
        //     if (
        //             (polarity && floatBuffer[i] < 0)
        //             || (!polarity && floatBuffer[i] > 0)
        //         )
        //     {
        //         polarity = !polarity;
        //         numSamples = 0;
        //     }
        // }

        /* Filter float values */
        low_pass_filter(floatBuffer);

        /* Convert processed samples back to int */
        float_to_int<FLOAT_DATATYPE, INT_DATATYPE>(
                intBuffer,
                floatBuffer,
                I2S_BUFFER_LENGTH
            );
        
        /* Write processed signal to output */
        i2s.write_bytes(intBuffer, I2S_BUFFER_LENGTH * sizeof(INT_DATATYPE));

        /* Delay periodically to feed the watchdog when in debug mode */
        DELAY_TICKS_AT_COUNT(100);
    }
}

extern "C" void app_main(void)
{
    DEBUG_OUT("Starting freq_out...");

    i2s.set_pin_master_clock(I2S_MCK);
    i2s.set_pin_bit_clock(I2S_BCK);
    i2s.set_pin_word_select(I2S_WS);
    i2s.set_pin_data_out(I2S_DO);
    i2s.set_pin_data_in(I2S_DI);
    i2s.set_bit_depth(INT_BIT_DEPTH);
    i2s.set_sample_rate(SAMPLE_RATE);
    i2s.set_channels(1);
    i2s.set_buffer_length(I2S_BUFFER_LENGTH, 2);
    i2s.set_auto_clear(false);
    i2s.start();

    DEBUG_OUT("freq_out initialized\n");

    i2s_input_output_loop();
}

