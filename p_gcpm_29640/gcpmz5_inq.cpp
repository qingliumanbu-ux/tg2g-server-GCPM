/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2015-6-27 18:05:02
功能: ED54配置信息_查询
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/
// New Include
#include "stdafx.h" 


/*<remark>=========================================================
/// <summary>
/// ED54配置信息_查询
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz5_inq)

int f_gcpmz5_inq(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);


	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz5_inq";                //定义函数英文名称  
	CString FunctionCname = "ED54配置信息_查询 ";              //定义函数中文名称
	//LogTrace(1, 1, " **************%s begin*****************", (const char*)FunctionEname);


	//程序用变量
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
		CString    c_order_by = " order by t.order_no ";


		/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2 = "  WHERE   1 = 1 "; //查询条件。
		CString    c_sql_condition2 = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";




		CString v_moid = "";   //二级模块
		CString v_func_id = "";   //ED54功能号
		CString v_form_code = "";   //画面代码
		CString v_srv_name = "";   //服务名称  



		if (bcls_rec->Tables[0].Columns.Contains("FUNC_ID"))
			v_func_id = bcls_rec->Tables[0].Rows[0]["FUNC_ID"].ToString().TrimOrBlank();

		Log::Trace("", __FUNCTION__, "v_func_id[{0}]  ", v_func_id);






		//SQL语句。
		c_sql_condition = "  select  t.*  from ted53  t"
			;
		//排序信息。
		c_order_by = " order by    t.func_id ,t.SEQ_NO  ";

		//where条件。 
		c_sql_where = " where 1 = 1";

		if (v_func_id.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.func_id like  @func_id || '%' ";
		}


		

		//ED54信息语句+ WHERE 语句。
		c_sql_condition = c_sql_condition + c_sql_where + c_order_by;
		Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);



		sqlstr = c_sql_condition;
		//未编辑功能条件准备。 
		cmd_sql.Parameters.Set("func_id", v_func_id);//  
		cmd_sql.SetCommandText(c_sql_condition);
		cmd_sql.ExecuteQuery(bcls_ret->Tables[0]);
		cmd_sql.Close();


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


