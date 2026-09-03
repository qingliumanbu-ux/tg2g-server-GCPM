/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2017-10-10 12:51:40
功能: 设置表数据删除标志
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h" 


/*<remark>=========================================================
/// <summary>
/// 设置表数据删除标志
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz6_del)

int f_gcpmz6_del(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz6_del";                //定义函数英文名称  
	CString FunctionCname = "设置表数据删除标志";              //定义函数中文名称 


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
		CString    c_sql_where      = "  WHERE   1 = 1 "; //批量修改条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //批量修改条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		 
		CString v_table_ename = ""; //表名称  
		 
		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{ 
			 //T.TABLE_ENAME 
			if (bcls_rec->Tables[0].Columns.Contains("TABLE_ENAME"))
				v_table_ename = bcls_rec->Tables[0].Rows[i]["TABLE_ENAME"].ToString().TrimOrBlank(); 
		 

			Log::Trace("", __FUNCTION__, "in 第[{0}]个，表[{1}]"
				, i + 1, v_table_ename);

			//修改‘清表标志’=ARCHIVE_FLAG = 0/1=否/是
			//==========  
			c_sql_condition = "update ttadt00 t "
				" set   t.archive_flag      = '1' " //1=是=要清表。
				" where t.table_ename       = @table_ename " 
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("table_ename", v_table_ename); 
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close(); 
		  
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


