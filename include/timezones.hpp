// Created by John Heenan 2020
#pragma once
#include <NTP.h> // for enums


#define DST_CODE_STRLEN_MAX 4
#define DST_REGION_STRLEN_MAX 20
#define DST_MAX 100


typedef struct
{
  char region[DST_REGION_STRLEN_MAX + 1];
  week_t week;
  dow_t wday;
  char dst_code[DST_CODE_STRLEN_MAX + 1];
  month_t dst_month;
  int8_t dst_hour;
  int dst_offset;
  char std_code[DST_CODE_STRLEN_MAX + 1];
  month_t std_month;
  int8_t std_hour;
  int std_offset;

} dst_t;

extern int dst_index;
extern int dst_length;
extern dst_t dst_array[];


