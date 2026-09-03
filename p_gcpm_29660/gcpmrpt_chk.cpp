/*========================================================================*/
/*== [service名  ]:  gcpmrpt_chk      ||  [对应VC#画面 ]:  ALL          ==*/
/*== [程序编制人 ]:  张颖             ||  [程序定稿日期]:2016-10-14 0:01:31==*/ 
/*== [程序修改人 ]：                  ||  [程序修改日期]:               ==*/
/*========================================================================*/
/*== [数据库表   ]： tgcpmsi01                                          ==*/
/*== [调用函数   ]： 无				                                          ==*/
/*== [service功能]： 报表打印_功能键校验                                ==*/
/*========================================================================*/ 
/***** C/C++ 的标准头文件部分 *****/ 
#include "stdafx.h"



/*<remark>=========================================================
/// <summary>
///  报表打印_功能键校验
/// <para>
/// //用于判断：
    //是否有该权限，
    //是否调用指定service .
/// </para> 
===========================================================</remark>*/

// service入口
BM2F_ENTERACE(gcpmrpt_chk)
//-EP_SYSTEM_HEAD_END                                                  
int f_gcpmrpt_chk(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);  // 系统日志，必须在代码段开始处定义

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmrpt_chk";                //定义函数英文名称  
	CString FunctionCname = "报表打印_功能键校验";              //定义函数中文名称

	//程序用变量
	int   doFlag = 0;
	int   i = 0;
	int   fetchRowCount = 0;


	//获得系统时间，当前用户代码
	CString systime = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString userid = s.userid;
	 
	CString  sqlstr = "";  //SQL 信息。 

	CString v_rpt_code = "";//表名称。
	CString v_moid = ""; //模块代码
	CString v_func_id = "";


	try
	{
		 
		//CTGCPMSI01 tgcpmsi01(conn);

		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString  c_sql_orderBy = "";



		//从2#BLK 中获取静态表的表名称。
		if (bcls_rec->Tables[1].Columns.Contains("RPT_CODE"))
			v_rpt_code = bcls_rec->Tables[1].Rows[0]["RPT_CODE"].ToString();
		Log::Trace("", __FUNCTION__, "报表代码 =[{0}] ", v_rpt_code);

		if (v_rpt_code.Trim() == "")
		{
			sprintf(s.msg, "【报表代码】不允许为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		}

		

		//===========
		//根据[报表代码]，进行对应责任者的校验，确认有权限，才允许操作。
		//=============================
		//根据 报表代码 ,获取是否已经维护[GCP6]的信息。
		CString v_rpt_id = "xx";
		CString v_service_name = "xx";
		c_sql_condition = "select t.code_desc_4_content,t.code_desc_5_content "
			" from tgcpmsi00 t "
			" where t.code_class   ='GCP6' "
			" and   t.code = @code " //指定的报表代码
			;
		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("code", v_rpt_code);//报表代码
		cmd_sql.SetCommandText(c_sql_condition);
		cmd_sql.ExecuteReader();
		if (cmd_sql.Read())
		{
			v_rpt_id = cmd_sql.GetString(1);
			v_service_name = cmd_sql.GetString(2);
		}
		cmd_sql.Close();

		if (v_rpt_id.Trim() == "")
		{// 
			sprintf(s.msg, "没有报表代码[%s]对应的【报表ID】，\n详见控制代码[GCP6],\n当前操作失败。"
				, (const char*)v_rpt_code);
			throw CApplicationException(-1, s.msg, log.Location);
		}

		if (v_service_name.Trim() == "")
		{// 
			sprintf(s.msg, "没有报表代码[%s]对应的【service】，\n详见控制代码[GCP6],\n当前操作失败。"
				, (const char*)v_rpt_code);
			throw CApplicationException(-1, s.msg, log.Location);
		}






		 
		//设置返回信息。

		/// <summary>
		/// 返回总记录数
		/// </summary>     
		CString v_rpt_code = "SERVICE";
		bcls_ret->Tables.Add(v_rpt_code);
		bcls_ret->Tables[v_rpt_code].Columns.Add(DT_STRING, "RPT_ID");
		bcls_ret->Tables[v_rpt_code].Columns.Add(DT_STRING, "SERVICE_NAME");
		bcls_ret->Tables[v_rpt_code].Rows.Add();
		bcls_ret->Tables[v_rpt_code].Rows[0]["RPT_ID"] = v_rpt_id;
		bcls_ret->Tables[v_rpt_code].Rows[0]["SERVICE_NAME"] = v_service_name;
 


	}



	/*捕获数据库操作异常*/
	catch (CDbException& ex)  //捕获数据库操作异常
	{
		CString str = "DB error:[" + sqlstr + "]\r\n" + ex.GetMsg();
		sprintf(s.msg, "%s,sqlCode[%d]", (const char*)str, ex.GetCode());
		doFlag = -1;                 //数据库异常时返回-1，事务将被回滚
	}
	catch (CApplicationException& ex)  //捕获应用错误
	{
		s.flag = ex.GetCode();
		doFlag = -1;
	}
	catch (CException& ex) //其他错误。
	{
		strncpy(s.msg, (const char*)ex.GetMsg(), sizeof(s.msg) - 1);
		s.flag = ex.GetCode();
		doFlag = -1;
	}

	//此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;


}
