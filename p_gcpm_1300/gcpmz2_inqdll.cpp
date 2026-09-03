/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2015-4-3 15:44:03
功能: 前台DLL清单_查询
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
/// 前台DLL清单_查询
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz2_inqdll)

int f_gcpmz2_inqdll(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz2_inqdll";                //定义函数英文名称  
	CString FunctionCname = "后台程序清单_查询";              //定义函数中文名称
	//LogTrace(1, 1, " **************%s begin*****************", (const char*)FunctionEname);


	//程序用变量
	int   fetchRowCount = 0;
	int   doFlag = 0;

	 



	CString v_code_name      = ""; 
	int     v_total_count    = 0;

	 
	CString sqlstr = "";  //SQL 信息。 


	try
	{



		 
		 
		
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //查询条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //查询条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		
		
 
		CString v_moid = "";   //二级代码 

		if (bcls_rec->Tables[0].Columns.Contains("MOID"))
			v_moid = bcls_rec->Tables[0].Rows[0]["MOID"].ToString().TrimOrBlank(); 

		Log::Trace("", __FUNCTION__, "v_moid[{0}]  ", v_moid); 



		

		

/* DLLNAME,DLL地址
select distinct t.dllname,t.dllpath
,t.description
,t.*
from tesformresinfo t
where t.dllname like 'PM%'
order by t.dllname,t.dllpath

*/

       

		//后台程序。//t.dllname,COUNT(DISTINCT t.form_code) ==画面的个数。
		c_sql_condition = " select t.dllname,count(DISTINCT t.form_code) AS num  "
			" from tgcpmz1 t "
			; 
		 

		//查询条件。
		c_sql_where = " where 1 = 1 ";

		//排序。
		c_order_by = " GROUP by t.dllname   ";

		//二级模块.
		if (v_moid.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.dllname like @dllname || '%'  ";
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
		//		c_sql_where += " AND substr(t.dllname,1,4) in ( " + v_tmp + " ) ";

		//	}

		//}

		 

		//信息初始化语句+ WHERE 语句。
		c_sql_condition  = c_sql_condition + c_sql_where + c_order_by ;  
		Log::Trace("",__FUNCTION__,"c_sql_condition[{0}]  ",c_sql_condition); 

		 
		fetchRowCount = 0; 
		sqlstr = c_sql_condition;
		//信息初始化条件准备。
		cmd_sql.Parameters.Set("dllname", v_moid.ToUpper());//二级模块
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








