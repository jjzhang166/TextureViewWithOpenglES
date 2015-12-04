/*
 * MyLog.h
 *
 *  Created on: 2014-3-4
 *      Author: pcuser
 */
#ifndef MYLOG_H_
#define MYLOG_H_
#include<android/log.h>
#define LOG(tag,message,line) __android_log_print(ANDROID_LOG_INFO, tag, "%s%d",message,line)
#define LOGI(tag,message,line,value) __android_log_print(ANDROID_LOG_INFO, tag, "%s at %d:%d",message,line,value)
#define LOGD(tag,message,value) __android_log_print(ANDROID_LOG_INFO, tag, "%s,%lf",message,value)
#endif /* MYLOG_H_ */
