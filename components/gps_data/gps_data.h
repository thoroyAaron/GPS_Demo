#ifndef GPS_DATA_H
#define GPS_DATA_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>

typedef struct {
    char latitude[16];      // 纬度
    char ns_indicator[2];   // 南北半球指示符
    char longitude[16];     // 经度
    char ew_indicator[2];   // 东西半球指示符
    char utc_time[16];      // UTC时间
    char status[2];         // 状态
    bool is_valid;          // 数据是否有效
} gps_data_t;

#ifdef __cplusplus
}
#endif

#endif // GPS_DATA_H