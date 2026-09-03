/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2017-6-23 10:57:01
功能: server号统计_查询
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/
// New Include
#include "stdafx.h"

//从字符串中根据指定分隔符拆分数据
// 入口字符，分隔字符，函数是返回字符信息。
CString f_get_multi_value(CString v_in_str, CString v_spilit_flag, CDbConnection * conn);

/*<remark>=========================================================
/// <summary>
/// server号统计_查询
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz2_inqs_id)

int f_gcpmz2_inqs_id(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);


	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz2_inqs_id";                //定义函数英文名称  
	CString FunctionCname = "server号统计_查询";              //定义函数中文名称
	//LogTrace(1, 1, " **************%s begin*****************", (const char*)FunctionEname);


	//程序用变量
	int   i = 0;
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString v_code_name = "";
	int     v_total_count = 0;
	CString sqlstr = "";  //SQL 信息。 


	try
	{ 


		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where = "  WHERE   1 = 1 "; //查询条件。
		CString    c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString    c_order_by = " ORDER BY t.order_no ";


		/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2 = "  WHERE   1 = 1 "; //查询条件。
		CString    c_sql_condition2 = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";




		CString v_moid = "";   //一级代码
		CString v_srv_name = "";   //服务名称  

		if (bcls_rec->Tables[0].Columns.Contains("MOID"))
			v_moid = bcls_rec->Tables[0].Rows[0]["MOID"].ToString().Trim();

		Log::Trace("", __FUNCTION__, "in ==v_moid[{0}]  ", v_moid);


		//查询条件；
		c_sql_where = "  WHERE   1 = 1 "; //查询条件。
		if (v_moid.Trim() != "")
		{
			c_sql_where += " AND sub_system_ename like @sub_system_ename || '%' ";//二级模块.

		}

		////二级模块范围==v_moid 
		////=================== 
		//CString v_tmp = ""; 
		//if (v_moid.Trim() != "")
		//{
		//	//根据指定分隔符，拆分字符信息。
		//	v_tmp = f_get_multi_value(v_moid, ",", conn);

		//	if (v_tmp.Trim() != "")
		//	{//返回的信息，不为空。

		//		//进行字符拆分处理。 ===f_get_multi_value() 
		//		c_sql_where += " AND sub_system_ename in ( " + v_tmp + " ) ";

		//	}

		//}



		/*
		SELECT  T.SRV_ID,T.SRV_NAME,T.SUB_SYSTEM_ENAME
		FROM TEA00 T

		*/
		c_sql_condition = "SELECT SRV_ID,SRV_NAME,SUB_SYSTEM_ENAME"
			" , 0   AS  SRV_SUM " //service总数
			" , ' ' AS  SRV_MX  " //service明细
			" from     TEA00  "
			//" where    sub_system_ename like @sub_system_ename || '%' " //二级模块.
			//" order    by sub_system_ename,srv_id "//二级模块，server号。
			;

		c_order_by = " order    by sub_system_ename,srv_id ";//二级模块，server号。
		//信息初始化语句+ WHERE 语句。 
		c_sql_condition = c_sql_condition + c_sql_where + c_order_by;
		Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);

		fetchRowCount = 0;
		sqlstr = c_sql_condition;
		//信息初始化条件准备。
		cmd_sql.Parameters.Set("sub_system_ename", v_moid.ToUpper());// 一级模块 
		cmd_sql.SetCommandText(c_sql_condition);
		cmd_sql.ExecuteQuery(bcls_ret->Tables[0]);		
		cmd_sql.Close();

		//根据指定列，特殊处理信息， 
		//SRV_SUM= srv对应的程序个数。
		//==================
		int v_row_num = bcls_ret->Tables[0].Rows.get_Count();
		CDecimal v_srv_num = 0;
		CString v_srv_id = ""; //server号。
		for (i = 0; i < v_row_num;i++)
		{
			v_srv_id = " ";
			v_srv_num = 0;

			if (bcls_ret->Tables[0].Columns.Contains("SRV_ID"))
			{
				v_srv_id = bcls_ret->Tables[0].Rows[i]["SRV_ID"];
			}


			//若存在指定列， 则进行加工赋值。
			if (bcls_ret->Tables[0].Columns.Contains("SRV_SUM"))
			{				
				v_srv_num = 0;
				c_sql_condition = " SELECT COUNT(1) FROM  tea01 t "
					" where t.srv_id = @srv_id  "
					;
				sqlstr = c_sql_condition;
				//信息初始化条件准备。
				cmd_sql.Parameters.Set("srv_id", v_srv_id);//SERVER号
				cmd_sql.SetCommandText(c_sql_condition);
				v_srv_num = cmd_sql.ExecuteScalar();
				cmd_sql.Close();
				bcls_ret->Tables[0].Rows[i]["SRV_SUM"] = v_srv_num;
			}


			Log::Trace("", __FUNCTION__, "in ==v_srv_id[{0}]  ", v_srv_id);
			Log::Trace("", __FUNCTION__, "in ==v_srv_num[{0}]  ", v_srv_num);

		}


		fetchRowCount = bcls_ret->Tables[0].Rows.get_Count();
		/*设置系统返回参数*/ 
		sprintf(s.msg, "查询到[%d]条记录。", fetchRowCount);


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


