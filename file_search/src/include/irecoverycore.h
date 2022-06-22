/**********************************************************************
*
* Copyright (C)2015-2022 Weinbery Corporation. All rights reserved.
*
* @file irecoverycore.h
* @brief 
* @details 
* @author wd
* @version 1.0.0
* @date 2021-10-09 22:41:27.492
*
**********************************************************************/
#pragma once

#include <stdint.h>
#include <iostream>
#include <string>
#include "datatype.h"
/*
*
* @brief 通知/消息代理（UI <=> 底层调度库）
* @details 
*/
class INotifyDelegate
{
public:
	/*
	*
	* @brief 通知回调函数，type由NotifyOption定义，ptr可自定义（结构体，字符串等）
	* @details 
	*/
	virtual int32_t OnNotify(int32_t type, void* ptr = nullptr, int64_t *size = nullptr) = 0;
};
/*
*
* @brief 数据恢复核心抽象类，底层主调度库继承之
* @details 提供各数据恢复底层需要的API
*/
class IRecoveryCore
{
public:
	/*
	*
	* @brief 设置消息内转代理ITransferDelegate
	* @details 
	*/
	virtual int32_t SetDelegate(INotifyDelegate* delegate) = 0;
	/*
	*
	* @brief 初始化
	* @details 
	*/
	virtual int32_t Init() = 0;
	/*
	*
	* @brief 设备枚举
	* @details 
	*/
	virtual int32_t Explore(char* info) = 0;
	/*
	*
	* @brief 开始扫描，UI只调用该函数一次，底层库自行实现快速扫描+格式化扫/文件雕刻
	* @details 
	*/
	virtual int32_t Scan(const char* params, int32_t size) = 0;
	/*
	*
	* @brief 停止扫描
	* @details 
	*/
	virtual int32_t Stop(int32_t option = 0) = 0;
	/*
	*
	* @brief 暂停扫描
	* @details 
	*/
	virtual int32_t Pause(int32_t option = 0) = 0;
	/*
	*
	* @brief 继续扫描
	* @details 
	*/
	virtual int32_t Resume(int32_t option = 0) = 0;
	/*
	*
	* @brief 文件恢复（不是必须使用）
	* @details 
	*/
	virtual int32_t Recvoer(char* buffer, const char* params, int32_t size) = 0;
	/*
	*
	* @brief 文件预览（不是必须使用）
	* @details 
	*/
	virtual int32_t Preview(char* buffer, const char* params, int32_t size) = 0;
	/*
	*
	* @brief 控制注入，例如可注入参数，随机设置扫描的磁盘扇区偏移，修改扫描进度
	* @details 
	*/
	virtual int32_t InjectControl(char* params, int32_t* size, int32_t option = 0) = 0;
	/*
	*
	* @brief 通过底层读取纯数据
	* @details 
	*/
	virtual int32_t ReadRawData(char* buffer, int64_t offset, int32_t size, void* attach = nullptr) = 0;
	/*
	*
	* @brief 调用底层进行文件恢复，用于APFS这样的只能通过底层进行文件恢复的文件系统
	* @details 
	*/
	virtual int32_t GeneralRecover(char* filePath, int32_t index, int64_t id, int64_t size, int32_t option = 0) = 0;
	/*
	*
	* @brief 调用底层读取文件数据，用于APFS这样的只能通过底层读取文件数据的文件系统
	* @details 
	*/
	virtual int32_t GeneralReadRawData(char* buffer, int32_t index, int64_t id, int64_t offset, int64_t size, int32_t option = 0) = 0;

};
/*
*
* @brief 底层库创建的API
* @details 
*/
extern "C" int32_t __stdcall CreateRecoveryCore(IRecoveryCore** core);