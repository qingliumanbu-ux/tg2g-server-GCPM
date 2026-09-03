/*============================================================================*/
/*== [service名  ]:  gcpmsi01_inq       ||  [对应VC#画面 ]:GCPMSI01          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2016-9-26 16:55:18==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMSI01                                          ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： 表TGCPMSI01_信息查询                               ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h" 

 


/*<remark>=========================================================
/// <summary>
/// 表TGCPMSI01_信息查询
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmsi01_inq)

int f_gcpmsi01_inq(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsi01_inq";                //定义函数英文名称  
	CString FunctionCname = "表TGCPMSI01_信息查询";              //定义函数中文名称 


	//程序用变量
	int   i = 0;
	int   fun_i = 0; //功能个数。
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;  
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0; 


	try
	{ 
		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		  
		//==查询条件信息。
		//CString TABLE_NAME;   //数据库表名 
		//CString FUNC_ID;   //功能标识
		  
		  //从1#BLK 中获取静态表的表名称。
		CString v_table_name = "";
		if (bcls_rec->Tables[0].Columns.Contains("TABLE_NAME"))
			v_table_name = bcls_rec->Tables[0].Rows[0]["TABLE_NAME"].ToString();

		CString v_func_id = "";
		if (bcls_rec->Tables[0].Columns.Contains("FUNC_ID"))
			v_func_id = bcls_rec->Tables[0].Rows[0]["FUNC_ID"].ToString();
			
		Log::Trace("", __FUNCTION__, "in ==v_table_name[{0}]  ", v_table_name);
		Log::Trace("", __FUNCTION__, "in ==v_func_id[{0}]  ", v_func_id);

		//SERVICE_NAME ==新增查询条件 ON 2017-4-10 11:23:47
		CString v_service_name = "";
		if (bcls_rec->Tables[0].Columns.Contains("SERVICE_NAME"))
			v_service_name = bcls_rec->Tables[0].Rows[0]["SERVICE_NAME"].ToString();
		Log::Trace("", __FUNCTION__, "in ==v_service_name[{0}]  ", v_service_name);


		//信息初始化条件。
		c_sql_where = " where 1 = 1   "; //信息初始化条件。  

		//拼接前台传入的查询条件。
		if (v_table_name.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.table_name like @table_name ";
		}

		if (v_func_id.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.func_id = @func_id ";
		}

		if (v_service_name.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.service_name = @service_name ";
		}


		//SELECT....
		c_sql_condition = " select t.*  from tgcpmsi01 t ";
		//排序信息。
		c_order_by = " order by t.table_name,t.func_id "; 



		//信息初始化语句+ WHERE 语句。
		c_sql_condition = c_sql_condition + c_sql_where + c_order_by;
		Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);  

		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("table_name", v_table_name);
		cmd_sql.Parameters.Set("func_id", v_func_id);
		cmd_sql.Parameters.Set("service_name", v_service_name);
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
		cmd_sql.ExecuteQuery(bcls_ret->Tables[0]);
		cmd_sql.Close();



		//返回的记录数。 
		fetchRowCount = bcls_ret->Tables[0].Rows.get_Count();

		/*设置系统返回参数*/ 
		sprintf(s.msg, "查询到[%d]条记录。", fetchRowCount);
		  
	}

	/*捕获数据库操作异常*/
	catch (CDbException& ex)  //捕获数据库操作异常
	{
		CString str = "err:[" + sqlstr + "]";
		sprintf(s.msg, "sqlCode[%d]-[%s],%s"
			, ex.GetCode(), (const char*)ex.GetMsg()
			, (const char*)str);
		doFlag = -1;        //数据库异常时返回-1，事务将被回滚

		Log::Trace("", __FUNCTION__, "sqlstr =[{0}]  ", sqlstr);
		Log::Trace("", __FUNCTION__, "sqlerr =[{0}]-[{1}]  "
			, ex.GetCode(), ex.GetMsg());

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

	//将来可能要拆service处理，SO ，此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;

}







