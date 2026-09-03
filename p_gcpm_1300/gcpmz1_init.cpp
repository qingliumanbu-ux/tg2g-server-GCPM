/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2014-10-22 9:53:45
功能: 功能清单明细_未编辑功能 
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h" 


/*<remark>=========================================================
/// <summary>
/// 功能清单明细_未编辑功能 
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz1_init)

int f_gcpmz1_init(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);

	Log::Trace("", __FUNCTION__, "IN== new address =TEST ");
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz1_init";                //定义函数英文名称  
	CString FunctionCname = "功能清单明细_未编辑功能 ";              //定义函数中文名称
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
		CString v_dllname = "";   //DLL名称
		CString v_form_code = "";   //画面代码
		CString v_srv_name = "";   //服务名称  

		if (bcls_rec->Tables[0].Columns.Contains("MOID"))
			v_moid = bcls_rec->Tables[0].Rows[0]["MOID"].ToString().TrimOrBlank();

		if (bcls_rec->Tables[0].Columns.Contains("DLLNAME"))
			v_dllname = bcls_rec->Tables[0].Rows[0]["DLLNAME"].ToString().TrimOrBlank();

		if (bcls_rec->Tables[0].Columns.Contains("FORM_CODE"))
			v_form_code = bcls_rec->Tables[0].Rows[0]["FORM_CODE"].ToString().TrimOrBlank();

		if (bcls_rec->Tables[0].Columns.Contains("SRV_NAME"))
			v_srv_name = bcls_rec->Tables[0].Rows[0]["SRV_NAME"].ToString().TrimOrBlank();

		 

		Log::Trace("", __FUNCTION__, "v_moid[{0}]  ", v_moid);
		Log::Trace("", __FUNCTION__, "v_dllname[{0}]  ", v_dllname);
		Log::Trace("", __FUNCTION__, "v_form_code[{0}]  ", v_form_code);

		//未编辑功能条件。
		c_sql_where = "    "; //未编辑功能条件。  


		//区分数据库的，长SQL语句的DEMO.
		//==============================

		CString v_sql_oralce = "  select  substr(t.dllname,1,4) as MOID ";//ORACLE 是从1#位置开始
		CString v_sql_db2 = "  select  substr(t.dllname,1,4) as MOID "; //DB2是从1#位置开始



		c_sql_condition = //"  select  substr(t.dllname,1,4) as MOID " //二级模块
			" ,t.dllname as DLLNAME  "                              //DLL
			" ,t.name as FORM_CODE,t.description as FORM_NAME  "    //画面代码，画面名称
			" ,a.name as FUNC_ID,a.name || '[' || a.description || ']' as FUNC_CNAME, a.description as FUNC_DESCRIPTION   "       //按钮代码，按钮代码+描述，描述
			" from  tesformresinfo t, tesbuttonresinfo a  "
			" where t.name = a.fname    "
			//" and  t.description not like '%[暂停]%' " //暂停使用的画面，不显示出来。
			" and  t.dllname IN( select aa.code from tgcpmsi00 aa  "
			"                    where aa.code_class = 'GCPP'    "
			"                    and aa.VALID_FLAG = '1')  " //1=生效
			" and  NOT EXISTS (select 1 from tgcpmz1 bb  "
			"             where bb.dllname = t.dllname and bb.form_code = t.name and bb.func_id = a.name) " //DLL+画面+按钮，不存在于 TGCPMZ1表中。
			// "order by t.dllname,t.name,a.name            "
			;


		switch (conn->DatabaseKind)
		{
		case DB_KIND_MSSQL:
			//c_sql_condition = " select power(10,@seq_len -1 ) from XX ";
			break;

		case DB_KIND_ORACLE:
			c_sql_condition = v_sql_oralce + c_sql_condition;
			break;

		case DB_KIND_DB2:
			c_sql_condition = v_sql_db2 + c_sql_condition;
			break;

		default: //默认暂定= ORALCE.
			c_sql_condition = v_sql_oralce + c_sql_condition;
			break;
		} 
		 


		if (v_moid.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.dllname like @moid || '%' ";
		}
		if (v_dllname.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.dllname =  @dllname  ";
		}
		if (v_form_code.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.name like @name || '%'  ";
		}

		 

		c_order_by = " order by t.dllname,t.name,a.name  ";

		//未编辑功能语句+ WHERE 语句。
		c_sql_condition  = c_sql_condition + c_sql_where + c_order_by ; 

		Log::Trace("",__FUNCTION__,"c_sql_condition[{0}]  ",c_sql_condition);
		 


		sqlstr = c_sql_condition;
		//未编辑功能条件准备。
		cmd_sql.Parameters.Set("moid", v_moid.ToUpper());//二级模块 
		cmd_sql.Parameters.Set("dllname", v_dllname);// 
		cmd_sql.Parameters.Set("name", v_form_code.ToUpper());//画面代码。   
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
