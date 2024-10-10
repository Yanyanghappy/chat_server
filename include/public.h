#ifndef PUBLIC_H
#define PUBLIC_H

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <limits.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define CLIENT_NUM 128

/**
 * @brief set nonblocking
 * @param fd 
 **/
int setnonblocking(int fd);

/**
 * @brief achieve some universal functions 
 */

typedef struct tagPrintParameterInfoHead//信息头
{
    uint32_t magic;//魔术标识:0x01112141
    uint16_t version;//协议版本号，默认1000
    uint32_t length;//数据部分字节长度
}PrintParameterInfoHead;


typedef struct tagPrintParameterInfo//数据信息,打印开始时间发送一次
{
    char sMachineID[64];//机器ID(机器序列号)
    char sTaskName[1024];//任务名称
    char sRipVersion[64];//rip版本
    uint16_t BiPrint;//单双向，0:单向;1:双向
    uint16_t FeatherLevel;//羽化等级，0关闭羽化,1小波浪羽化,2大波浪羽化,3自定义羽化
    uint16_t FeatherLevelCustom;//自定义羽化等级:0-100
    uint16_t BiSkipBlank;//是否跳白，0:不跳白;1:跳白
    uint16_t PrintSpeed;//打印速度，0:低速;1:高速
    uint16_t PrintDirection;//开始打印方向，0:出小车方向(向右);1:回小车方向(向左)
    float dDisLeft;//左边彩条离图的间距
    float dWidthLeft;//左边彩条宽度
    float dDisRight;//右边彩条离图的间距
    float dWidthRight;//右边彩条宽度
    uint16_t PercentColorbarInk;//彩条墨量百分比:0-100
} PrintParameterInfo, * pPrintParameterInfo;


typedef struct tagPrinterStatusInfo//打印机实时状态，维护最新状态
{
    char sMachineID[64];//机器ID(机器序列号)
    uint16_t status;//0:空闲;1:离线(PC无法跟下位机通讯);2:正在打印;3:故障(errMsg为具体的错误信息);4:busy;5:Pause;空闲和正在打印时只要ATQ返回打印完成或者异常删除后都能下发订单
    uint16_t version;//协议版本号，默认1000
    char sErrMsg[64];//3故障时具体错误信息
}PrinterStatusInfo;


typedef struct tagJobStatusInfo
{
    char sMachineID[64];//机器ID(机器序列号)
    char sTaskName[1024];//任务名称
    uint16_t statusTask;//任务状态0:等待打印;1:打印中;2:打印完成;3:打印暂停;4:打印取消;5故障;6错误删除；7添加成功
    uint16_t statusSoftware;//软件(ATQ)状态；0:未启动；1：启动，点击打印才是真正意义上的启动
    uint16_t version;//协议版本号，默认1000
    char sErrMsg[64];//5:故障时具体的错误信息(目前不提供)
}JobStatusInfo;


typedef struct tagPrintdataInfoHead//信息头
{
    uint32_t magic;//魔术标识:0x02122242
    uint16_t version;//协议版本号，默认1000
    uint32_t length;//数据部分字节长度
}PrintdataInfoHead;


typedef struct tagPrintdataInfo//数据信息
{
    char sMachineID[64];//机器ID(机器序列号)
    char sTaskName[1024];//任务名称
    uint16_t progress;//打印进度:0-100
    float dLenPrinted;//当前作业已打印长度
    float dTimeRemaining;//当前作业剩余时间(分钟)
    float dTimePrinted;//当前作业已消耗的时间(分钟)
    float dMeterTotal;//总米数(m)
    float dInkTotal;//总墨量(ml)
    float dPaperManualTotal;//手动走纸统计(m)
    float dInkRemaining[16];//机器剩余墨量(ml)
    float dPaperRemaining;//机器剩余纸量(m)，卷径，具体米数需要自己算
    //打印完成代表可以下发新任务
    uint16_t statusTask;//任务状态0:等待打印;1:打印中;2:打印完成;3:打印暂停;4:打印取消;5故障；6错误删除；7添加成功
    uint16_t statusSoftware;//软件(ATQ)状态；0:未启动；1：启动成功; 2:暂停
    uint16_t statusPrinter;//打印机状态,0:空闲;1:离线(PC无法跟下位机通讯);2:正在打印;3:故障(errMsg为具体的错误信息);4:busy;5:Pause
} PrintdataInfo, * pPrintdataInfo;


typedef struct tagReinforceInfoHead//信息头
{
    uint32_t magic;//魔术标识0x08182848
    uint16_t version;//协议版本号，默认1000
    uint32_t length;//数据部分字节长度
}ReinforceInfoHead;


typedef struct tagRecordInfo//记录信息每次任务结束时发送
{
    char sMachineID[64];//机器ID(机器序列号)
    char sBeginTime[64];//开始时间
    char sEndTime[64];//结束时间
    char sFilename[256];//文件名
    double dLen;//长度
    double dArea;//面积
    double dInk;//墨水
} RecordInfo, * pRecordInfo;

typedef struct tagThumbnailsInfoHead//信息头
{
    uint32_t magic;//魔术标识:0x02122448
    uint16_t version;//协议版本号，默认1000
    uint16_t type;//类型,默认0,jpg文件
    char sMachineID[64];//机器ID(机器序列号)
    char filename[PATH_MAX];//文件名
    uint32_t length;//文件部分字节长度
}ThumbnailsInfoHead;




#endif