/// <summary>
/// 功能说明: 通用查询功能信息查询 
/// </summary>
/// Copyright: Baosight Software LTD.co Copyright (c) 2010
/// Company: 上海宝信软件股份有限公司
/// Author:   XXXX
/// Version:  1.0
/// History:  2022/7/29 15:37:24
///	

#include "stdafx.h"
#include "Be2UserModel/SI/CFormDevConfig.h"

// Service 入口
BM2F_ENTERACE(gcpm_dataSet_inq)

int f_gcpm_dataSet_inq(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection * conn)
{
	int doFlag = 0;

	try
	{
		//低代码模式，数据源定义的SQL执行。
		//===========================
		doFlag = BE2::CFormDevConfig::QueryUtility(bcls_rec, bcls_ret, conn);

	}
	catch (CException& ex)
	{
		strcpy(s.msg, ex.GetMsg());
		s.flag = -1;
		doFlag = -1;
	}
	return doFlag;
}