#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "tinygps.h"

// properties
uint32_t      _time, _new_time;
uint32_t      _date, _new_date;
uint32_t      _speed, _new_speed;
uint32_t      _course, _new_course;
uint32_t      _hdop, _new_hdop;
uint32_t      _last_time_fix, _new_time_fix;
uint32_t      _last_position_fix, _new_position_fix;

uint16_t      _numsats, _new_numsats;

int32_t       _latitude, _new_latitude;
int32_t       _longitude, _new_longitude;
int32_t       _altitude, _new_altitude;

unsigned char _parity           = 0;
unsigned char _sentence_type    = 0;
unsigned char _term_number      = 0;
unsigned char _term_offset      = 0;
char          _term[15];
bool          _is_checksum_term = 0;
bool          _is_gps_data_good = 0;

#ifndef GPS_NO_STATS
  uint32_t    _encoded_characters;
  uint16_t    _good_sentences;
  uint16_t    _failed_checksum;
  uint16_t    _passed_checksum;
#endif

// verify is character is a digit
bool gpsisdigit(char c) 
{ 
  return c >= '0' && c <= '9'; 
}

// signed altitude in centimeters (from GPGGA sentence)
int32_t altitude() 
{ 
  return _altitude; 
}

// course in last full GPRMC sentence in 100th of a degree
uint32_t course() 
{ 
  return _course; 
}

// speed in last full GPRMC sentence in 100ths of a knot
uint32_t speed() 
{
  return _speed; 
}

// satellites used in last full GPGGA sentence
uint16_t gps_satellites() 
{ 
  return _numsats; 
}

// horizontal dilution of precision in 100ths
uint32_t gps_hdop() 
{ 
  return _hdop; 
}

clock_t uptime()
{
	return clock() / (CLOCKS_PER_SEC / 1000);
}

float radians(float deg)
{
	return deg * (PI/180);
}

float degrees(float rad)
{
	return rad * (180/PI);
}

bool gps_encode(char c)
{
  bool valid_sentence = false;

#ifndef GPS_NO_STATS
  _encoded_characters++;
#endif

  switch(c)
  {
    case ',': // term terminators
      _parity ^= c;
    case '\r':
    case '\n':
    case '*':
      if (_term_offset < sizeof(_term))
      {
        _term[_term_offset] = 0;
        valid_sentence = gps_term_complete();
      }

      ++_term_number;
      _term_offset = 0;
      _is_checksum_term = c == '*';

      return valid_sentence;
  
    case '$': // sentence begin
      _term_number = 0;
      _term_offset = 0;
      _parity = 0;
      _sentence_type = GPS_SENTENCE_OTHER;
      _is_checksum_term = false;
      _is_gps_data_good = false;
      
      return valid_sentence;
  }

  // ordinary characters
  if (_term_offset < sizeof(_term) - 1)
  {
    _term[_term_offset++] = c;
  }
  if (!_is_checksum_term)
  {
    _parity ^= c;
  }

  return valid_sentence;
}

#ifndef GPS_NO_STATS
void gps_stats(uint32_t *chars, uint16_t *sentences, uint16_t *failed_cs)
{
  if (chars)
  {
	  *chars = _encoded_characters;
  }
  if (sentences)
  {
	  *sentences = _good_sentences;
  }
  if (failed_cs)
  {
	  *failed_cs = _failed_checksum;
  }
}
#endif

int32_t from_hex(char a) 
{
  if (a >= 'A' && a <= 'F')
  {
    return a - 'A' + 10;
  }
  else if (a >= 'a' && a <= 'f')
  {
    return a - 'a' + 10;
  }
  else
  {
    return a - '0';
  }
}

uint32_t gps_parse_decimal()
{
  char     *p;
  bool     isneg;
  uint32_t ret;

  p     = _term;
  isneg = (*p == '-');
  if (isneg)
  {
	  ++p;
  }

  ret = 100UL * gpsatol(p);

  while (gpsisdigit(*p))
  {
	  ++p;
  }

  if (*p == '.')
  {
    if (gpsisdigit(p[1]))
    {
      ret += 10 * (p[1] - '0');
      if (gpsisdigit(p[2]))
        ret += p[2] - '0';
    }
  }

  return isneg ? -ret : ret;
}

uint32_t gps_parse_degrees()
{
  char     *p;
  uint32_t left;
  uint32_t tenk_minutes;

  left = gpsatol(_term);
  tenk_minutes = (left % 100UL) * 10000UL;

  for (p=_term; gpsisdigit(*p); ++p);

  if (*p == '.')
  {
    uint32_t mult = 1000;
    while (gpsisdigit(*++p))
    {
      tenk_minutes += mult * (*p - '0');
      mult /= 10;
    }
  }

  return (left / 100) * 100000 + tenk_minutes / 6;
}

#define COMBINE(sentence_type, term_number) (((unsigned)(sentence_type) << 5) | term_number)

bool gps_term_complete()
{
  if (_is_checksum_term)
  {
    unsigned char checksum;
    checksum = 16 * from_hex(_term[0]) + from_hex(_term[1]);

    if (checksum == _parity)
    {
      if (_is_gps_data_good)
      {
        #ifndef GPS_NO_STATS
          ++_good_sentences;
        #endif

        _last_time_fix = _new_time_fix;
        _last_position_fix = _new_position_fix;

        switch(_sentence_type)
        {
          case GPS_SENTENCE_GPRMC:
            _time      = _new_time;
            _date      = _new_date;
            _latitude  = _new_latitude;
            _longitude = _new_longitude;
            _speed     = _new_speed;
            _course    = _new_course;
            break;
          case GPS_SENTENCE_GPGGA:
            _altitude  = _new_altitude;
            _time      = _new_time;
            _latitude  = _new_latitude;
            _longitude = _new_longitude;
            _numsats   = _new_numsats;
            _hdop      = _new_hdop;
            break;
        }

        return true;
      }
    }

    #ifndef GPS_NO_STATS
      else
        ++_failed_checksum;
    #endif

    return false;
  }

  // the first term determines the sentence type
  if (_term_number == 0)
  {
    if (!gpsstrcmp(_term, GPRMC_TERM))
    {
      _sentence_type = GPS_SENTENCE_GPRMC;
    }
    else if (!gpsstrcmp(_term, GPGGA_TERM))
    {
      _sentence_type = GPS_SENTENCE_GPGGA;
    }
    else
    {
      _sentence_type = GPS_SENTENCE_OTHER;
    }

    return false;
  }

  if (_sentence_type != GPS_SENTENCE_OTHER && _term[0])
  {
    switch(COMBINE(_sentence_type, _term_number))
    {
      case COMBINE(GPS_SENTENCE_GPRMC, 1): // Time in both sentences
      case COMBINE(GPS_SENTENCE_GPGGA, 1):
        _new_time = gps_parse_decimal();
        _new_time_fix = uptime();
        break;
      case COMBINE(GPS_SENTENCE_GPRMC, 2): // GPRMC validity
        _is_gps_data_good = (_term[0] == 'A');
        break;
      case COMBINE(GPS_SENTENCE_GPRMC, 3): // Latitude
      case COMBINE(GPS_SENTENCE_GPGGA, 2):
        _new_latitude = gps_parse_degrees();
        _new_position_fix = uptime();
        break;
      case COMBINE(GPS_SENTENCE_GPRMC, 4): // N/S
      case COMBINE(GPS_SENTENCE_GPGGA, 3):
        if (_term[0] == 'S')
          _new_latitude = -_new_latitude;
        break;
      case COMBINE(GPS_SENTENCE_GPRMC, 5): // Longitude
      case COMBINE(GPS_SENTENCE_GPGGA, 4):
        _new_longitude = gps_parse_degrees();
        break;
      case COMBINE(GPS_SENTENCE_GPRMC, 6): // E/W
      case COMBINE(GPS_SENTENCE_GPGGA, 5):
        if (_term[0] == 'W')
          _new_longitude = -_new_longitude;
        break;
      case COMBINE(GPS_SENTENCE_GPRMC, 7): // Speed (GPRMC)
        _new_speed = gps_parse_decimal();
        break;
      case COMBINE(GPS_SENTENCE_GPRMC, 8): // Course (GPRMC)
        _new_course = gps_parse_decimal();
        break;
      case COMBINE(GPS_SENTENCE_GPRMC, 9): // Date (GPRMC)
        _new_date = gpsatol(_term);
        break;
      case COMBINE(GPS_SENTENCE_GPGGA, 6): // Fix data (GPGGA)
        _is_gps_data_good = (_term[0] > '0');
        break;
      case COMBINE(GPS_SENTENCE_GPGGA, 7): // Satellites used (GPGGA)
        _new_numsats = (unsigned char)atoi(_term);
        break;
      case COMBINE(GPS_SENTENCE_GPGGA, 8): // HDOP
        _new_hdop = gps_parse_decimal();
        break;
      case COMBINE(GPS_SENTENCE_GPGGA, 9): // Altitude (GPGGA)
        _new_altitude = gps_parse_decimal();
        break;
    }
  }

  return false;
}

int32_t gpsatol(const char *str)
{
  int32_t ret = 0;

  while (gpsisdigit(*str))
  {
    ret = 10 * ret + *str++ - '0';
  }

  return ret;
}

int32_t gpsstrcmp(const char *str1, const char *str2)
{
  while (*str1 && *str1 == *str2)
  {
    ++str1, ++str2;
  }

  return *str1;
}

/* static */
float gps_distance_between (float lat1, float int32_t1, 
                            float lat2, float int32_t2) 
{
  float delta = radians(int32_t1-int32_t2);
  float sdint32_t = (float)sin(delta);
  float cdint32_t = (float)cos(delta);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  float slat1 = sin(lat1);
  float clat1 = cos(lat1);
  float slat2 = sin(lat2);
  float clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdint32_t); 
  delta = sq(delta); 
  delta += sq(clat2 * sdint32_t); 
  delta = sqrt(delta); 
  float denom = (slat1 * slat2) + (clat1 * clat2 * cdint32_t); 
  delta = atan2(delta, denom); 
  return delta * 6372795; 
}

float gps_course_to (float lat1, float int32_t1, 
                     float lat2, float int32_t2) 
{
  float dlon = radians(int32_t2-int32_t1);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  float a1 = sin(dlon) * cos(lat2);
  float a2 = sin(lat1) * cos(lat2) * cos(dlon);
  a2 = cos(lat1) * sin(lat2) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0)
  {
    a2 += TWO_PI;
  }
  return degrees(a2);
}

const char *gps_cardinal (float course)
{
  static const char* directions[] = {"N", "NNE", "NE", "ENE", 
                                     "E", "ESE", "SE", "SSE", 
                                     "S", "SSW", "SW", "WSW", 
                                     "W", "WNW", "NW", "NNW"};

  int32_t direction = (int32_t)((course + 11.25f) / 22.5f);

  return directions[direction % 16];
}

void gps_get_position(int32_t  *latitude, 
                      int32_t  *longitude, 
                      uint32_t *fix_age)
{
  if (latitude)
  {
	  *latitude = _latitude;
  }
  if (longitude)
  {
	  *longitude = _longitude;
  }
  if (fix_age)
  {
	  *fix_age = (_last_position_fix == GPS_INVALID_FIX_TIME) ? 
		                        GPS_INVALID_AGE : uptime() - _last_position_fix;
  }
}

// date as ddmmyy, time as hhmmsscc, and age in milliseconds
void gps_get_datetime(uint32_t *date, uint32_t *time, uint32_t *age)
{
  if (date)
  {
	  *date = _date;
  }
  if (time)
  {
	  *time = _time;
  }
  if (age)
  {
	  *age = _last_time_fix == GPS_INVALID_FIX_TIME ? 
		                            GPS_INVALID_AGE : uptime() - _last_time_fix;
  }
}

void gps_f_get_position(float   *latitude, 
                        float   *longitude, 
                        uint32_t *fix_age)
{
  int32_t lat, lon;

  gps_get_position(&lat, &lon, fix_age);

  *latitude = lat == GPS_INVALID_ANGLE ? GPS_INVALID_F_ANGLE : (lat / 100000.0);
  *longitude = lon == GPS_INVALID_ANGLE ? GPS_INVALID_F_ANGLE : (lon / 100000.0);
}

void gps_crack_datetime(int32_t       *year, 
                        unsigned char *month, 
                        unsigned char *day, 
                        unsigned char *hour, 
                        unsigned char *minute, 
                        unsigned char *second, 
                        unsigned char *hundredths, 
                        uint32_t      *age)
{
  uint32_t date, time;

  gps_get_datetime(&date, &time, age);

  if(year) 
  {
    *year = date % 100;
    *year += *year > 80 ? 1900 : 2000;
  }
  if(month) 
  {
    *month = (date / 100) % 100;
  }
  if(day) 
  {
    *day = date / 10000;
  }
  if(hour) 
  {
    *hour = time / 1000000;
  }
  if(minute) 
  {
    *minute = (time / 10000) % 100;
  }
  if(second) 
  {
    *second = (time / 100) % 100;
  }
  if(hundredths) 
  {
    *hundredths = time % 100;
  }
}

float gps_f_altitude()    
{
  return _altitude == 
            GPS_INVALID_ALTITUDE ? GPS_INVALID_F_ALTITUDE : _altitude / 100.0;
}

float gps_f_course()
{
  return _course == GPS_INVALID_ANGLE ? GPS_INVALID_F_ANGLE : _course / 100.0;
}

float gps_f_speed_knots() 
{
  return _speed == GPS_INVALID_SPEED ? GPS_INVALID_F_SPEED : _speed / 100.0;
}

float gps_f_speed_mph()   
{ 
  float sk = gps_f_speed_knots();
  return sk == GPS_INVALID_F_SPEED ? GPS_INVALID_F_SPEED 
                                      : GPS_MPH_PER_KNOT * gps_f_speed_knots(); 
}

float gps_f_speed_mps()   
{ 
  float sk = gps_f_speed_knots();
  return sk == GPS_INVALID_F_SPEED ? GPS_INVALID_F_SPEED 
                                      : GPS_MPS_PER_KNOT * gps_f_speed_knots(); 
}

float gps_f_speed_kmph()  
{ 
  float sk = gps_f_speed_knots();
  return sk == GPS_INVALID_F_SPEED ? GPS_INVALID_F_SPEED 
                                      : GPS_KMPH_PER_KNOT * gps_f_speed_knots(); 
}
