/*============================================================================*/
/*== [service名  ]:  gcpmdm02_inq       ||  [对应VC#画面 ]:GCPMDM02          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2017-9-26 13:09:52==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMDM02                                          ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： 表TGCPMDM02_信息查询                               ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h"


 


/*<remark>=========================================================
/// <summary>
/// 表TGCPMDM02_信息查询
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmdm02_inq)

int f_gcpmdm02_inq(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmdm02_inq";                //定义函数英文名称  
	CString FunctionCname = "表TGCPMDM02_信息查询";              //定义函数中文名称 


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
	CModel tgcpmdm02("TGCPMDM02");
		
		
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
		//CString CODE_CLASS;   //代码编号
		//CString CODE_NAME;   //代码名称
		  
		  //从1#BLK 中获取静态表的表名称。
		CString v_code_class = "";
		if (bcls_rec->Tables[0].Columns.Contains("CODE_CLASS"))
			v_code_class = bcls_rec->Tables[0].Rows[0]["CODE_CLASS"].ToString(); 
			
		Log::Trace("", __FUNCTION__, "in ==v_code_class[{0}]  ", v_code_class); 

		 

		//信息初始化条件。
		c_sql_where = " where 1 = 1   "; //信息初始化条件。  

		//拼接前台传入的查询条件。
		if (v_code_class.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.code_class like @code_class  || '%' ";
		}

		 

	 

		//SELECT....
		c_sql_condition = " select t.*  from tgcpmdm02 t ";
		//排序信息。
		c_order_by = " order by t.code_class,t.item_ename "; 



		//信息初始化语句+ WHERE 语句。
		c_sql_condition = c_sql_condition + c_sql_where + c_order_by;
		Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);  

		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("code_class", v_code_class); 
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

	//将来可能要拆service处理，SO ，此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;

}







