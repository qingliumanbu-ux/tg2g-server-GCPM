/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2015-4-7 13:19:51
功能: [前台CS]功能覆盖的产线信息_批量修改
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h" 


/*<remark>=========================================================
/// <summary>
/// [前台CS]功能覆盖的产线信息_批量修改
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz2_updf)

int f_gcpmz2_updf(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz2_updf";                //定义函数英文名称  
	CString FunctionCname = "[前台CS]功能覆盖的产线信息_批量修改";              //定义函数中文名称
	//LogTrace(1, 1, " **************%s begin*****************", (const char*)FunctionEname);


	//程序用变量
	int   i = 0;
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;  
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0;
	CString  v_dllname = "";


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
	 
		 
		//获取批量修改的信息，
		//code_line=功能覆盖的产线代码 
		//form_code = 前台画面代码
		CString v_form_code = "";
		CString v_code_line = "";
		if (bcls_rec->Tables[1].Columns.Contains("CODE_LINE"))
			v_code_line = bcls_rec->Tables[1].Rows[0]["CODE_LINE"];

		if (v_code_line.Trim() == "")
		{//覆盖产线，不允许为空。
			sprintf(s.msg, "[覆盖产线]不允许为空，当前操作失败。");
			throw CApplicationException(-1, s.msg, log.Location);
		}
		 
		 
		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{ 
			if (bcls_rec->Tables[0].Columns.Contains("FORM_CODE"))
				v_form_code = bcls_rec->Tables[0].Rows[i]["FORM_CODE"];

			Log::Trace("", __FUNCTION__, "v_form_code =[{0}] v_code_line =[{1}]"
				, v_form_code, v_code_line);

			if (v_form_code.Trim() == "")
			{//画面代码，不允许为空。
				sprintf(s.msg, "[画面代码]不允许为空，当前操作失败。");
				throw CApplicationException(-1, s.msg, log.Location);
			}
 
			if (userid.Trim() != "178029")
			{//若不是指定用户，则不允许修改，功能的覆盖产线。

				sprintf(s.msg, "您的帐号[%s]没有【修改覆盖产线】的权限，当前操作失败。"
					, (const char*)userid);
				throw CApplicationException(-1, s.msg, log.Location);
			}
			 
		 

			////批量修改 
			////==========  
			c_sql_condition = "update tgcpmz1 t "
				" set   t.code_line      = @code_line " 
				" where t.form_code       = @form_code "  //画面代码。
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("code_line", v_code_line);//新产线代码 
			cmd_sql.Parameters.Set("form_code", v_form_code);//后台服务名称。 
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


