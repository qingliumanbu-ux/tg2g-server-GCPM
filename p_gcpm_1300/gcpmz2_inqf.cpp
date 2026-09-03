/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2014-11-13 12:43:03
功能: 画面清单_查询
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h"



/*<remark>=========================================================
/// <summary>
/// 画面清单_查询
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz2_inqf)

int f_gcpmz2_inqf(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz2_inqf";                //定义函数英文名称  
	CString FunctionCname = "画面清单_查询";              //定义函数中文名称
	//LogTrace(1, 1, " **************%s begin*****************", (const char*)FunctionEname);


	//程序用变量
	int   fetchRowCount = 0;
	int   doFlag = 0;

	 



	CString v_code_name      = ""; 
	int     v_total_count    = 0;

	 
	CString sqlstr = "";  //SQL 信息。 


	try
	{ 
		 
	CModel tgcpmz1("TGCPMZ1");
		
		
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //查询条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  " ;
		CString    c_order_by       =  " ORDER BY t.ORDER_NO ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //查询条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		 
		
 
		CString v_moid = "";   //一级代码
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
		Log::Trace("", __FUNCTION__, "v_form_code[{0}]  ", v_form_code);

		//信息初始化条件。
		c_sql_where = " where  t.form_code <> '后台'  "; //画面清单中，不显示后台程序信息。 

		/*
		CString v_moid = "";   //一级代码
		CString v_dllname = "";   //DLL名称
		CString v_form_code = "";   //画面代码
		CString v_srv_name = "";   //服务名称
		*/


		if (v_moid.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.moid like @moid || '%' ";
		}
		if (v_dllname.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.dllname =  @dllname  ";
		}
		if (v_form_code.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.form_code like @form_code || '%'  ";
		}

//		if (v_srv_name.Trim() != "")
//		{
//			c_sql_where = c_sql_where + " AND t.srv_name like @srv_name || '%'  ";
//		}


		c_order_by = " order by t.dllname,t.form_code "; //按[动态库][画面代码] 排序

		//DLL,画面代码。
		c_sql_condition = "  select  distinct t.DLLNAME,t.FORM_CODE "
			" from tgcpmz1 t  "
			;

		//信息初始化语句+ WHERE 语句。
		c_sql_condition  = c_sql_condition + c_sql_where + c_order_by ;  
		Log::Trace("",__FUNCTION__,"c_sql_condition[{0}]  ",c_sql_condition);
		 


		CString v_form_code_tmp = ""; //画面代码
		CString v_dllname_tmp = ""; //DLL

		 

		int row_i = 0;
		int row_j = 0;
		fetchRowCount = 0;

		sqlstr = c_sql_condition;
		//信息初始化条件准备。
		cmd_sql.Parameters.Set("moid", v_moid.ToUpper()); //
		cmd_sql.Parameters.Set("dllname", v_dllname);// 
		cmd_sql.Parameters.Set("form_code", v_form_code.ToUpper()); ///画面代码。  
		cmd_sql.SetCommandText(c_sql_condition); 
		//cmd_sql.ExecuteQuery(bcls_ret->Tables[0]);
		cmd_sql.ExecuteReader();
		while (cmd_sql.Read())
		{
			fetchRowCount++;
			v_dllname_tmp = cmd_sql.GetString(1);
			v_form_code_tmp = cmd_sql.GetString(2);

			//cmd_sql.Fetch(tgcpmz1);
			
			//因为是画面清单，一个画面，只返回第一个功能点的行信息，即可。
			//=================
			tgcpmz1.Reset();
			c_sql_condition2 = "select t.* from tgcpmz1 t "
				" where t.dllname   = @dllname "
				" and   t.form_code = @form_code "
				" order by t.dllname,t.form_code,t.seq_no" //按序号排列，获取第一行，即可。
				;
			sqlstr = c_sql_condition2;
			cmd_sql2.Parameters.Set("dllname", v_dllname_tmp);// 
			cmd_sql2.Parameters.Set("form_code", v_form_code_tmp);//画面代码。 
			cmd_sql2.SetCommandText(c_sql_condition2);// 设置执行的SQL语句
			cmd_sql2.ExecuteReader();
			if(cmd_sql2.Read()) //只读取第一行信息。
			{// 
				cmd_sql2.Fetch(tgcpmz1);
			} 
			cmd_sql2.Close();

			//特殊字段处理=
			//============
			//CString CODE_LINE;   //产线代码 
			c_sql_condition2 = "select t.code_desc_2_content from tgcpmsi00 t "
				" where t.code_class = 'GCPM' "
				" and   t.code_desc_1_content = @code_desc_1_content " //覆盖产线代码
				" order by t.code " 
				;
			sqlstr = c_sql_condition2;
			cmd_sql2.Parameters.Set("code_desc_1_content", tgcpmz1["CODE_LINE"].ToString());//  
			cmd_sql2.SetCommandText(c_sql_condition2);// 设置执行的SQL语句
			cmd_sql2.ExecuteReader();
			if (cmd_sql2.Read()) //只读取第一行信息。
			{// 
				tgcpmz1["CODE_LINE"] = cmd_sql2.GetString(1); //覆盖产线说明
			}
			cmd_sql2.Close();

			//画面名称= 名称+换行+代码
			//FORM_NAME
			//==============
			tgcpmz1["FORM_NAME"] = tgcpmz1["FORM_NAME"]; // +"\n\r" + tgcpmz1["FORM_CODE"].ToString();
				 


			tgcpmz1.TrimOrBlank();
			tgcpmz1.MergeTo(bcls_ret->Tables[0], false);

		}

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




