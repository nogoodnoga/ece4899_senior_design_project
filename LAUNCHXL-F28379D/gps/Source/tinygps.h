#ifndef tinygps_h
#define tinygps_h

#define false                  0
#define true                   1

#define PI                     3.14159265
#define TWO_PI                 2*PI

#define GPRMC_TERM             "GPRMC"
#define GPGGA_TERM             "GPGGA"

#define GPS_INVALID_F_ANGLE    1000.0
#define GPS_INVALID_F_ALTITUDE 1000000.0
#define GPS_INVALID_F_SPEED    -1.0

#define GPS_MPH_PER_KNOT       1.15077945
#define GPS_MPS_PER_KNOT       0.51444444
#define GPS_KMPH_PER_KNOT      1.852
#define GPS_MILES_PER_METER    0.00062137112
#define GPS_KM_PER_METER       0.001

#define sq(x) ((x)*(x))

enum 
{
   GPS_INVALID_AGE             = 0xFFFFFFFF,
   GPS_INVALID_ANGLE           = 999999999, 
   GPS_INVALID_ALTITUDE        = 999999999,
   GPS_INVALID_DATE            = 0,
   GPS_INVALID_TIME            = 0xFFFFFFFF,
   GPS_INVALID_SPEED           = 999999999, 
   GPS_INVALID_FIX_TIME        = 0xFFFFFFFF,
   GPS_INVALID_SATELLITES      = 0xFF,
   GPS_INVALID_HDOP            = 0xFFFFFFFF
};

// process one character received from GPS
bool gps_encode(char c);

void gps_get_position(int32_t *latitude, int32_t *longitude, uint32_t *fix_age);

void gps_get_datetime(uint32_t *date, uint32_t *time, uint32_t *age);

void gps_f_get_position(float *latitude, float *longitude, uint32_t *fix_age);

void gps_crack_datetime(int32_t       *year, 
                        unsigned char *month, 
                        unsigned char *day, 
                        unsigned char *hour, 
                        unsigned char *minute, 
                        unsigned char *second, 
                        unsigned char *hundredths, 
                        uint32_t      *fix_age);

float  gps_f_altitude();

float  gps_f_course();

float  gps_f_speed_knots();

float  gps_f_speed_mph();

float  gps_f_speed_mps();

float  gps_f_speed_kmph();

float gps_distance_between(float lat1, float long1, 
                           float lat2, float long2);

float gps_course_to(float lat1, float long1, 
                    float lat2, float long2);

const char *gps_cardinal(float course);

#ifndef GPS_NO_STATS
  void gps_stats(uint32_t *chars, 
                 uint16_t *good_sentences, 
                 uint16_t *failed_cs);
#endif

enum 
{
   GPS_SENTENCE_GPGGA,
   GPS_SENTENCE_GPRMC,
   GPS_SENTENCE_OTHER
};

uint32_t gps_parse_decimal();

uint32_t gps_parse_degrees();

int32_t  from_hex(char a);

int32_t  gpsatol(const char *str);

int32_t  gpsstrcmp(const char *str1, const char *str2);

bool     gps_term_complete();

bool     gpsisdigit(char c);

#endif
