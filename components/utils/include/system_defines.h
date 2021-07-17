/**
 * @file system_defines.h
 * @author Steffen Breinbjerg
 * @brief 
 * @version 0.1
 * @date 2021-06-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __SYSTEM_DEFINES_H__
#define __SYSTEM_DEFINES_H__



/** Global struct to store configs etc etc */
typedef struct
{
    char DeviceName[10];
    int socket;
    int sampleFreqADXL;
    int sampleFreqMIC;
} system_defs;

#endif /* __SYSTEM_DEFINES_H__ */