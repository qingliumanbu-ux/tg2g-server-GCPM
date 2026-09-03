/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2014-10-25 13:59:43
功能: 功能清单明细_查询
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h"



/*<remark>=========================================================
/// <summary>
/// 功能清单明细_查询
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz2_inq)

int f_gcpmz2_inq(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz2_inq";                //定义函数英文名称  
	CString FunctionCname = "功能清单明细_查询";              //定义函数中文名称
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
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //查询条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		 
		
 
		CString v_moid = "";   //一级代码
		CString v_dllname = "";   //DLL名称
		CString v_form_code = "";   //画面代码
		CString v_form_name = "";   //画面名称
		CString v_srv_name = "";   //服务名称  
		CString v_status = ""; //功能状态。

		if (bcls_rec->Tables[0].Columns.Contains("MOID"))
			v_moid = bcls_rec->Tables[0].Rows[0]["MOID"].ToString().TrimOrBlank();

		if (bcls_rec->Tables[0].Columns.Contains("DLLNAME"))
			v_dllname = bcls_rec->Tables[0].Rows[0]["DLLNAME"].ToString().TrimOrBlank();

		if (bcls_rec->Tables[0].Columns.Contains("FORM_CODE"))
			v_form_code = bcls_rec->Tables[0].Rows[0]["FORM_CODE"].ToString().TrimOrBlank();

		if (bcls_rec->Tables[0].Columns.Contains("FORM_NAME"))
			v_form_name = bcls_rec->Tables[0].Rows[0]["FORM_NAME"].ToString().TrimOrBlank();


		if (bcls_rec->Tables[0].Columns.Contains("SRV_NAME"))
			v_srv_name = bcls_rec->Tables[0].Rows[0]["SRV_NAME"].ToString().TrimOrBlank();

		if (bcls_rec->Tables[0].Columns.Contains("STATUS"))
			v_status = bcls_rec->Tables[0].Rows[0]["STATUS"].ToString().TrimOrBlank();

		Log::Trace("", __FUNCTION__, "v_moid[{0}]  ", v_moid);
		Log::Trace("", __FUNCTION__, "v_form_code[{0}]  ", v_form_code);
		Log::Trace("", __FUNCTION__, "v_form_name[{0}]  ", v_form_name);

		//信息初始化条件。
		c_sql_where = " where 1 = 1   "; //信息初始化条件。 

		
		
		c_sql_condition = "  select  t.* "               
			 " from tgcpmz1 t  " 
			; 

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

		if (v_form_name.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.form_name like  '%' || @form_name || '%'  ";
		}

		if (v_srv_name.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.srv_name like @srv_name || '%'  ";
		}

		//v_status
		if (v_status.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.status =  @status  ";
		}


		c_order_by = " order by t.dllname,t.form_code,t.seq_no,t.func_id  "; //按[动态库][画面代码][功能序号]排序

		//信息初始化语句+ WHERE 语句。
		c_sql_condition  = c_sql_condition + c_sql_where + c_order_by ;  
		Log::Trace("",__FUNCTION__,"c_sql_condition[{0}]  ",c_sql_condition);
		 


		CString v_form_code_old = "";
		CString v_form_name_old = "";      //画面名称
		CString v_form_desc_old = ""; //画面功能描述
		CString v_dllname_old = ""; //DLL

		CDecimal v_seq_no_old = 0;

		int row_i = 0;
		int row_j = 0;
		fetchRowCount = 0;

		sqlstr = c_sql_condition;
		//信息初始化条件准备。
		cmd_sql.Parameters.Set("moid", v_moid.ToUpper());
		cmd_sql.Parameters.Set("dllname", v_dllname);// 
		cmd_sql.Parameters.Set("form_code", v_form_code.ToUpper()); //画面代码。
		cmd_sql.Parameters.Set("form_name", v_form_name); //画面名称
		cmd_sql.Parameters.Set("srv_name", v_srv_name);// 。   
		cmd_sql.Parameters.Set("status", v_status);// 

		cmd_sql.SetCommandText(c_sql_condition); 
		//cmd_sql.ExecuteQuery(bcls_ret->Tables[0]);
		cmd_sql.ExecuteReader();
		while (cmd_sql.Read())
		{
			
			cmd_sql.Fetch(tgcpmz1);
			
			///将一个画面代码下，只返回第一行记录的，画面名称+画面功能描述
			//CString FORM_NAME;   //画面名称
			//CString FORM_DESC;   //画面功能描述
			fetchRowCount++;
			if (fetchRowCount == 0)
			{
				v_form_code_old = tgcpmz1["FORM_CODE"];
				v_form_name_old = tgcpmz1["FORM_NAME"];
				v_form_desc_old = tgcpmz1["FORM_DESC"];
				v_dllname_old = tgcpmz1["DLLNAME"];

			}
			else
			{//非第一行，进行比较。
				if (v_form_code_old == tgcpmz1["FORM_CODE"].ToString())
				{//若还是相同的名字，则清空返回的 【画面名称+画面功能描述】
					v_form_name_old = "";
					v_form_desc_old = "";
					v_dllname_old = "";
				}
				else
				{//若不一样，说明是新的画面出现了，则返回的 【画面名称+画面功能描述】
					v_form_code_old = tgcpmz1["FORM_CODE"];
					v_form_name_old = tgcpmz1["FORM_NAME"];
					v_form_desc_old = tgcpmz1["FORM_DESC"];
					v_dllname_old = tgcpmz1["DLLNAME"];
				}
			}

			//特殊处理：返回的 【画面名称+画面功能描述】
			tgcpmz1["FORM_NAME"] = v_form_name_old ;
			tgcpmz1["FORM_DESC"] = v_form_desc_old ;
			tgcpmz1["DLLNAME"] = v_dllname_old; 


			//servic后台对应的地址显示，
			//比如：/Dev/PM/p_pmof_10520
			//加工到字段： KEYVALUE_GCPM2 中。
			//MOID
			CString v_moid_id_main = ""; //一级模块
			CString v_moid_id = "";//二级模块
			tgcpmz1["KEYVALUE_GCPM2"] = "";
			if (tgcpmz1["MOID"].ToString().Trim() != "")
			{
				v_moid_id_main = tgcpmz1["MOID"].ToString().Trim().Substring(0, 2).ToUpper();
				v_moid_id = tgcpmz1["MOID"].ToString().Trim().ToLower();

				if (tgcpmz1["SRV_ID"].ToString().Trim() != "")
				{//若有serve号， 则 拼接一下对应的后台程序地址。
					////比如：/Dev/PM/p_pmof_10520
					tgcpmz1["KEYVALUE_GCPM2"] = "/Dev/" + v_moid_id_main + "/p_" + v_moid_id + "_" + tgcpmz1["SRV_ID"].ToString();
				}
			}
			

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


