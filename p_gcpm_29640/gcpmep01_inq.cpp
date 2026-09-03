/*============================================================================*/
/*== [service名  ]:  gcpmep01_inq       ||  [对应VC#画面 ]:GCPMEP01          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2019-1-15 12:33:28==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TEP0001                                            ==*/
/*== [调用函数   ]： 无				                                          ==*/
/*== [service功能]： 表TEP0001_信息查询                                 ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h"  


/*<remark>=========================================================
/// <summary>
/// 表TEP0001_信息查询
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmep01_inq)

int f_gcpmep01_inq(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmep01_inq";                //定义函数英文名称  
	CString FunctionCname = "表TEP0001_信息查询";              //定义函数中文名称 


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
		CString    c_order_by       =  " ORDER BY t.XX ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		  
		//==查询条件信息。
		//CString CODE_CLASS;   //数据库表名 
		//CString CODE_NAME;   //功能标识
		//CString COMPANY_CODE;   //公司代码
		  
		  //从1#BLK 中获取静态表的表名称。
		CString v_code_class = "";
		if (bcls_rec->Tables[0].Columns.Contains("CODE_CLASS"))
			v_code_class = bcls_rec->Tables[0].Rows[0]["CODE_CLASS"].ToString();

		CString v_code_name = "";
		if (bcls_rec->Tables[0].Columns.Contains("CODE_NAME"))
			v_code_name = bcls_rec->Tables[0].Rows[0]["CODE_NAME"].ToString(); 
			
		Log::Trace("", __FUNCTION__, "in ==v_code_class[{0}]  ", v_code_class);
		Log::Trace("", __FUNCTION__, "in ==v_code_name[{0}]  ", v_code_name);  

		//信息初始化条件。
		c_sql_where = " where 1 = 1   "; //信息初始化条件。  

		//拼接前台传入的查询条件。
		if (v_code_class.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.code_class LIKE @code_class || '%' ";
		}

		if (v_code_name.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.code_name LIKE @code_name || '%' ";
		}

		 


		//SELECT....
		c_sql_condition = " select t.*  from tep0001 t ";
		//排序信息。
		c_order_by = " order by   t.code_class  "; 



		//信息初始化语句+ WHERE 语句。
		c_sql_condition = c_sql_condition + c_sql_where + c_order_by;
		Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);  

		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("code_class", v_code_class);
		cmd_sql.Parameters.Set("code_name", v_code_name); 
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






