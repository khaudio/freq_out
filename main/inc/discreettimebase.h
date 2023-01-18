#ifndef DISCREETTIMEBASE_H
#define DISCREETTIMEBASE_H

#include <cstdint>
#include <utility>
#include <vector>
#include "intfloatconversions.h"

/*  Sample rate         Overcranked         Undercranked
 *  44100               -                   -
 *  48000               48048               47952
 *  88200               -                   -
 *  96000               96096               95904
 *  176400              -                   -
 *  192000              192192              191808
 *  352800              -                   -
 *  384000              384384              383616
 */

#define SAMPLERATE_44K                      44100
#define SAMPLERATE_48K                      48000
#define SAMPLERATE_48K_OVERCRANK            48048
#define SAMPLERATE_48K_UNDERCRANK           47952
#define SAMPLERATE_88K                      88200
#define SAMPLERATE_96K                      96000
#define SAMPLERATE_96K_OVERCRANK            96096
#define SAMPLERATE_96K_UNDERCRANK           95904
#define SAMPLERATE_176K                     176400
#define SAMPLERATE_192K                     192000
#define SAMPLERATE_192K_OVERCRANK           192192
#define SAMPLERATE_192K_UNDERCRANK          191808
#define SAMPLERATE_352K                     352800
#define SAMPLERATE_384K                     384000
#define SAMPLERATE_384K_OVERCRANK           384384
#define SAMPLERATE_384K_UNDERCRANK          383616

namespace DiscreetTime
{

enum discreet_time_err
{
    INVALID_SAMPLE_RATE = -101,
    INVALID_BITS_PER_SAMPLE = -102,
    INVALID_NUM_CHANNELS = -103,
};

template <typename T>
inline bool initial_nonzero_polarity(T* buff, int length)
{
    /* Returns true if the value of the
    first nonzero sample found is positive */
    for (int i(0); i < length; ++i)
    {
        if (buff[i] != get_zero<T>())
        {
            return buff[i] > get_zero<T>();
        }
    }
}

template <typename T>
inline bool polarity_changed(T value, bool* polarity)
{
    return (
            ((!*polarity) && (value > get_zero<T>()))
            || ((*polarity) && (value < get_zero<T>()))
        );
}

template <typename T>
inline void increment_zero_crossings(
        T* buff,
        int length,
        bool* polarity,
        int* counter
    )
{
    for (int i(0); i < length; ++i)
    {
        if (polarity_changed(buff, polarity))
        {
            ++counter;
            *polarity = !*polarity;
        }
    }
}

template <typename T>
inline T mean(T* buff, int length)
{
    T sum(0);
    for (int i(0); i < length; ++i)
    {
        sum += buff[i];
    }
    return sum / static_cast<T>(length);
}

template <typename T>
inline T rms(T* buff, int length)
{
    T sum(0);
    for (int i(0); i < length; ++i)
    {
        sum += std::pow(buff[i], 2);
    }
    return std::sqrt(sum) / static_cast<T>(length);
}

template <typename T>
std::pair<T, T> filtered_low_and_high_mean(
        T* buff,
        int length,
        int thresholdLow,
        int thresholdHigh
    )
{
    T sumLow(0), sumHigh(0);
    int counterLow(0), counterHigh(0);
    for (int i(0); i < length; ++i)
    {
        if (buff[i] < thresholdLow)
        {
            sumLow += buff[i];
            ++counterLow;
        }
        else if (buff[i] > thresholdHigh)
        {
            sumHigh += buff[i];
            ++counterHigh;
        }
    }
    return std::pair<T, T>(
            sumLow / static_cast<T>(counterLow),
            sumHigh / static_cast<T>(counterHigh)
        );
}

class DiscreetTimeBase
{

protected:

    uint32_t _sampleRate = 0;
    uint16_t _bitDepth = 0, _numChannels = 0, _sampleWidth = 0;

public:

    DiscreetTimeBase() = default;
    
    DiscreetTimeBase(const DiscreetTimeBase& obj) :
    _sampleRate(obj._sampleRate),
    _bitDepth(obj._bitDepth),
    _numChannels(obj._numChannels),
    _sampleWidth(obj._sampleWidth)
    {
    }

    ~DiscreetTimeBase() = default;

    virtual void set_sample_rate(uint32_t sampleRate)
    {
        this->_sampleRate = sampleRate;
    }

    virtual void set_bit_depth(uint16_t bitsPerSample)
    {
        #if _DEBUG
        if (bitsPerSample % 8) throw INVALID_BITS_PER_SAMPLE;
        #endif
        this->_bitDepth = bitsPerSample;
        this->_sampleWidth = (this->_bitDepth / uint16_t(8));
    }

    virtual void set_channels(uint16_t channels)
    {
        #if _DEBUG
        if (channels <= 0) throw INVALID_NUM_CHANNELS;
        #endif
        this->_numChannels = channels;
    }

    virtual uint32_t sample_rate() const
    {
        return this->_sampleRate;
    }

    virtual uint16_t bit_depth() const
    {
        return this->_bitDepth;
    }

    virtual uint16_t sample_width() const
    {
        return this->_sampleWidth;
    }
    
    virtual uint16_t num_channels() const
    {
        return this->_numChannels;
    }

};

};

#endif
