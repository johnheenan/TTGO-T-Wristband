// Created by John Heenan 2020
#pragma once

#define TZ_OFFSET 600 // timezone offset in minutes, this is only used if DST_NONE below is defined, it still must be defined
//only one set of timezone data below should be left defined
//#define DST_NONE // Do not use daylight savings time, make sure TZ_offset above is correct
//#define DST_CEST_CET  // Northern Hemisphere example
//#define DST_AEDT_AEST   // Southern Hemisphere example

#if !defined DST_NONE && !defined DST_CEST_CET && !defined DST_AEDT_AEST
#define DST_CEST_CET
#endif

#ifdef DST_NONE
#define TZ_USES_DST false
#else
#define TZ_USES_DST true
#endif

#ifdef DST_NONE
//Example when daylight savings time is not used
//The next 10 defines are not used if TZ_USES_DST is false but they must be defined to avoid compile errors
#define TZ_WEEK First
#define TZ_WDAY Sun
#define TZ_DST_NAME "UNUSED"
#define TZ_DST_MONTH Jan
#define TZ_DST_HOUR 0
#define TZ_DST_OFFSET 0 // unused when TZ_USES_DST is false
#define TZ_STD_NAME "UNUSED"
#define TZ_STD_MONTH Jan
#define TZ_STD_HOUR 0
#define TZ_STD_OFFSET 0 // unused when TZ_USES_DST is false
#endif

// enum week_t {Last, First, Second, Third, Fourth};
// enum dow_t {Sun, Mon, Tue, Wed, Thu, Fri, Sat};
// enum month_t {Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};

#ifdef DST_CEST_CET
//Northern Hemisphere Time Zone setting example
#define TZ_WEEK Last
#define TZ_WDAY Sun
//CEST
#define TZ_DST_NAME "CEST"
#define TZ_DST_MONTH Mar
#define TZ_DST_HOUR 2     // 2AM
#define TZ_DST_OFFSET 120 // 2 hours in minutes
//CET
#define TZ_STD_NAME "CET"
#define TZ_STD_MONTH Oct
#define TZ_STD_HOUR 3    // 3AM
#define TZ_STD_OFFSET 60 // 1 hour in minutes
#endif

#ifdef DST_AEDT_AEST
#define TZ_WEEK First
#define TZ_WDAY Sun
//AEDT Australian Eastern Daylight Time
#define TZ_STD_NAME "AEDT"
#define TZ_STD_MONTH Apr
#define TZ_STD_HOUR 3     // 3AM
#define TZ_STD_OFFSET 600 // 10 hours in minutes
//Austrlian Eastern Summer Time
#define TZ_DST_NAME "AEST"
#define TZ_DST_MONTH Oct
#define TZ_DST_HOUR 2     // 2AM
#define TZ_DST_OFFSET 660 // 11 hours in minutes
#endif