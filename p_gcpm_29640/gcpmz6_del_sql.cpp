/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2017-10-11 14:26:16
功能: 设置表数据_删除SQL脚本
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/
// New Include
#include "stdafx.h" 


/*<remark>=========================================================
/// <summary>
/// 设置表数据_删除SQL脚本
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz6_del_sql)

int f_gcpmz6_del_sql(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);


	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz6_del_sql";                //定义函数英文名称  
	CString FunctionCname = "设置表数据_删除SQL脚本";              //定义函数中文名称 


	//程序用变量
	int   i = 0;
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  v_dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  v_userid = s.userid;
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0;


	try
	{

		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where = "  WHERE   1 = 1 "; //批量修改条件。
		CString    c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString    c_order_by = " order by t.order_no ";


		/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2 = "  WHERE   1 = 1 "; //批量修改条件。
		CString    c_sql_condition2 = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";


		/// <summary>
		/// 返回 sql脚本信息。
		/// </summary>  
		bcls_ret->Tables[0].Clear();//先CLEAR
		bcls_ret->Tables[0].Columns.Add(DT_STRING, "SQL_MSG");
		/*bcls_ret->Tables[0].Rows.Add();
		bcls_ret->Tables[0].Rows[i++]["SQL_MSG"] = v_sql_msg;*/


		CString v_sql_msg = ""; //sql脚本信息。。。
		CString v_table_ename = ""; //业务表名称。
		CString v_table_cname = ""; //业务表中文。

		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{
			//T.TABLE_ENAME 
			if (bcls_rec->Tables[0].Columns.Contains("TABLE_ENAME"))
				v_table_ename = bcls_rec->Tables[0].Rows[i]["TABLE_ENAME"].ToString().TrimOrBlank();

			if (bcls_rec->Tables[0].Columns.Contains("TABLE_CNAME"))
				v_table_cname = bcls_rec->Tables[0].Rows[i]["TABLE_CNAME"].ToString().TrimOrBlank();


			Log::Trace("", __FUNCTION__, "in 第[{0}]个，表[{1}]-[{2}]"
				, i + 1, v_table_ename, v_table_cname);

			//生成N行的'删除脚本',并返回前台。
			//DELETE FROM TPMOF01 ; --表的中文说明,
			v_sql_msg = " DELETE FROM  " + v_table_ename + "  ;     --" + v_table_cname;

			bcls_ret->Tables[0].Rows.Add();
			bcls_ret->Tables[0].Rows[i]["SQL_MSG"] = v_sql_msg; 
		}


		/*设置系统返回参数*/
		strcpy(s.msg, "恭喜，处理成功。");//处理成功。 

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


