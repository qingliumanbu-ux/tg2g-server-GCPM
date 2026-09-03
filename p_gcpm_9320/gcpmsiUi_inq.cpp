/*============================================================================*/
/*== [service名  ]:  gcpmsiui_inq       ||  [对应VC#画面 ]:GCPMSI00          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2022/8/5 10:33:37==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMSI00                                          ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： 基表信息_查询                                ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h" 
#include "Be2UserModel/SI/CFormDevConfig.h"
//#include "Be2UserModel/SI/si00_common.h"

 


/*<remark>=========================================================
/// <summary>
/// 表TGCPMSI00_信息查询
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmsiUi_inq)

int f_gcpmsiUi_inq(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsiUi_inq";                //定义函数英文名称  
	CString FunctionCname = "表TGCPMSI00_信息查询";              //定义函数中文名称 


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
		CString    c_sql_condition  = "  SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString    c_sql_where      = "  WHERE   1 = 1 "; //修改条件。 
		CString    c_sql_orderBY    = "  order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		  
		//==查询条件信息。 
		//从1#BLK 中获取静态表的表名称。
		CString v_table_name = "";//业务表名称。
		if (bcls_rec->Tables[0].Columns.Contains("TABLE_NAME"))
			v_table_name = bcls_rec->Tables[0].Rows[0]["TABLE_NAME"].ToString(); 
		Log::Trace("", __FUNCTION__, "in ==v_table_name[{0}]  ", v_table_name); 

		c_sql_orderBY = "";//排序信息。
		if (bcls_rec->Tables[0].Columns.Contains("ORDER_BY"))
			c_sql_orderBY = bcls_rec->Tables[0].Rows[0]["ORDER_BY"].ToString();
		Log::Trace("", __FUNCTION__, "in ==c_sql_orderBY[{0}]  ", c_sql_orderBY);
 

		if (v_table_name.Trim() == "")
		{
			sprintf(s.msg, "业务表信息不能为空，当前操作失败。");
			throw CApplicationException(-1, s.msg, log.Location);
		} 
        c_sql_condition = " select * from " + v_table_name ;
		//信息初始化条件。
		c_sql_where = " where 1 = 1   "; //信息初始化条件。  
		
		
		
		
		//查询语句+ WHERE 语句。
		c_sql_condition = c_sql_condition + c_sql_where;
		BE2::CFormDevConfig::SetParameters(cmd_sql, c_sql_condition, bcls_rec); //自动拼接N个where条件信息。
		c_sql_condition = c_sql_condition + c_sql_orderBY; 
		 
		Log::Trace("",__FUNCTION__,"最终=c_sql_condition = [{0}]  ",c_sql_condition ); 
		sqlstr = c_sql_condition; 
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
		cmd_sql.ExecuteQuery(bcls_ret->Tables[0]);
		cmd_sql.Close();
		bcls_ret->Tables[0].set_TableName(v_table_name); 

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