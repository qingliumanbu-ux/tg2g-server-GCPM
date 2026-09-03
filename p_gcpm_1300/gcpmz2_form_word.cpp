/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2015-5-22 15:33:59
功能: 画面清单WORD格式_查询
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h"



/*<remark>=========================================================
/// <summary>
/// 画面清单WORD格式_查询
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz2_form_word)

int f_gcpmz2_form_word(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz2_form_word";                //定义函数英文名称  
	CString FunctionCname = "画面清单WORD格式_查询";              //定义函数中文名称
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
		CString v_srv_name = "";   //服务名称  

		 

		if (bcls_rec->Tables[0].Columns.Contains("FORM_CODE"))
			v_form_code = bcls_rec->Tables[0].Rows[0]["FORM_CODE"].ToString().TrimOrBlank();
	 
		Log::Trace("", __FUNCTION__, "v_form_code[{0}]  ", v_form_code);  
		CString v_form_code_tmp = ""; //画面代码 

		 
		CString v_table_name = "word_content";

		CString v_col_name = "WORD"; //内容
		CString v_col_name2 = "WORD_FONT";//格式
		//bcls_ret->Tables.Add(v_table_name);
		bcls_ret->Tables[0].set_TableName(v_table_name);
		bcls_ret->Tables[v_table_name].Columns.Add(DT_STRING, v_col_name);
		bcls_ret->Tables[v_table_name].Columns.Add(DT_STRING, v_col_name2); //字体格式。
		//bcls_ret->Tables[v_table_name].Rows.Add();
		//bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_total_count;

		int row_i = 0;
		int row_j = 0;
		CString v_word_tmp = ""; //WORD内容信息。
		CString v_word_font_tmp = ""; //WORD字体格式。
		fetchRowCount = 0; 
		 
		//Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);

		//标题信息。

		c_sql_condition = "  select t.*  from tgcpmz1 t  "
			" where t.form_code = @form_code   "
			" order by t.seq_no                "
			;
		sqlstr = c_sql_condition;
		//信息初始化条件准备。 
		cmd_sql.Parameters.Set("form_code", v_form_code);//画面代码。  
		cmd_sql.SetCommandText(c_sql_condition);
		cmd_sql.ExecuteReader();
		if (cmd_sql.Read())
		{
			cmd_sql.Fetch(tgcpmz1);
		}
		cmd_sql.Close();

		/*
		v_tmp = v_in_str.Replace("\t","");
		v_tmp2 = v_tmp.Replace("\r","");
		v_tmp3 = v_tmp2.Replace("\n","");
		*/

		v_word_tmp = tgcpmz1["FORM_NAME"];
		v_word_tmp = v_word_tmp.Replace("\t", "");
		v_word_tmp = v_word_tmp.Replace("\r", "");
		v_word_tmp = v_word_tmp.Replace("\n", "");
		bcls_ret->Tables[v_table_name].Rows.Add();
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_word_tmp;
		v_word_font_tmp = "1"; //一级
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name2] = v_word_font_tmp;
		row_i++;


		v_word_tmp = "功能描述";
		bcls_ret->Tables[v_table_name].Rows.Add();
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_word_tmp;
		v_word_font_tmp = "2"; //二级
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name2] = v_word_font_tmp;
		row_i++;


		v_word_tmp = tgcpmz1["FORM_DESC"];
		v_word_tmp = v_word_tmp.Replace("\t", "");
		v_word_tmp = v_word_tmp.Replace("\r", "");
		v_word_tmp = v_word_tmp.Replace("\n", "");
		v_word_tmp = "    " + v_word_tmp; //加上4个前空格。
		bcls_ret->Tables[v_table_name].Rows.Add();
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_word_tmp;
		v_word_font_tmp = "4"; //4级
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name2] = v_word_font_tmp;
		row_i++;


		v_word_tmp = "操作步骤";
		bcls_ret->Tables[v_table_name].Rows.Add();
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_word_tmp;
		v_word_font_tmp = "2"; //二级
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name2] = v_word_font_tmp;
		row_i++;





		

		c_sql_condition = "  select t.*  from tgcpmz1 t  "
			" where t.form_code = @form_code   "
			" order by t.seq_no                "
			; 
		sqlstr = c_sql_condition;
		//信息初始化条件准备。 
		cmd_sql.Parameters.Set("form_code", v_form_code);//画面代码。  
		cmd_sql.SetCommandText(c_sql_condition);  
		cmd_sql.ExecuteReader();
		while (cmd_sql.Read())
		{
			fetchRowCount++;
			cmd_sql.Fetch(tgcpmz1);

			//FUNC_CNAME
			//FUNC_DESCRIPTION

			v_word_tmp = tgcpmz1["FUNC_CNAME"].ToString() + ": " + tgcpmz1["FUNC_DESCRIPTION"].ToString();

			v_word_tmp = v_word_tmp.Replace("\t", "");
			v_word_tmp = v_word_tmp.Replace("\r", "");
			v_word_tmp = v_word_tmp.Replace("\n", "");
			bcls_ret->Tables[v_table_name].Rows.Add();
			bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_word_tmp;
			v_word_font_tmp = "4"; //
			bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name2] = v_word_font_tmp;
			row_i++;

		} 
		cmd_sql.Close();  


		v_word_tmp = "画面样张";
		bcls_ret->Tables[v_table_name].Rows.Add();
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_word_tmp;
		v_word_font_tmp = "2"; //二级
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name2] = v_word_font_tmp;
		row_i++;

		v_word_tmp = "********************** ";// 
		bcls_ret->Tables[v_table_name].Rows.Add();
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_word_tmp;
		v_word_font_tmp = "3"; //二级
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name2] = v_word_font_tmp;
		row_i++;
		v_word_tmp = "@@@@@@@@@@@@@@@@@@@@@@@ ";// 
		bcls_ret->Tables[v_table_name].Rows.Add();
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_word_tmp;
		v_word_font_tmp = "3"; //二级
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name2] = v_word_font_tmp;
		row_i++;
		v_word_tmp = "********************** ";// 
		bcls_ret->Tables[v_table_name].Rows.Add();
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_word_tmp;
		v_word_font_tmp = "3"; //二级
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name2] = v_word_font_tmp;
		row_i++;

		v_word_tmp = "功能说明";//空格
		bcls_ret->Tables[v_table_name].Rows.Add();
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_word_tmp;
		v_word_font_tmp = "2"; //二级
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name2] = v_word_font_tmp;
		row_i++;


		c_sql_condition = "  select t.*  from tgcpmz1 t  "
			" where t.form_code = @form_code   "
			" order by t.seq_no                "
			;
		sqlstr = c_sql_condition;
		//信息初始化条件准备。 
		cmd_sql.Parameters.Set("form_code", v_form_code);//画面代码。  
		cmd_sql.SetCommandText(c_sql_condition);
		cmd_sql.ExecuteReader();
		while (cmd_sql.Read())
		{
			fetchRowCount++;
			cmd_sql.Fetch(tgcpmz1);

			//FUNC_CNAME
			//REMARK_DESC

			v_word_tmp = tgcpmz1["FUNC_CNAME"]; //功能名称
			v_word_tmp = v_word_tmp.Replace("\t", "");
			v_word_tmp = v_word_tmp.Replace("\r", "");
			v_word_tmp = v_word_tmp.Replace("\n", "");
			bcls_ret->Tables[v_table_name].Rows.Add();
			bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_word_tmp;
			v_word_font_tmp = "3"; //3级
			bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name2] = v_word_font_tmp;
			row_i++;

			v_word_tmp = tgcpmz1["FUNC_REMARK"];//功能说明
			v_word_tmp = v_word_tmp.Replace("\t", "");
			v_word_tmp = v_word_tmp.Replace("\r", "");
			v_word_tmp = v_word_tmp.Replace("\n", "");
			v_word_tmp = "    " + v_word_tmp; //加上4个前空格。
			bcls_ret->Tables[v_table_name].Rows.Add();
			bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_word_tmp;
			v_word_font_tmp = "4"; //4级
			bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name2] = v_word_font_tmp;
			row_i++;

		}
		cmd_sql.Close();

		v_word_tmp = "常见问题说明";
		bcls_ret->Tables[v_table_name].Rows.Add();
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_word_tmp;
		v_word_font_tmp = "2"; //2级
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name2] = v_word_font_tmp;
		row_i++;

		v_word_tmp = "无。";
		bcls_ret->Tables[v_table_name].Rows.Add();
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name] = v_word_tmp;
		v_word_font_tmp = "4"; //4级
		bcls_ret->Tables[v_table_name].Rows[row_i][v_col_name2] = v_word_font_tmp;
		row_i++;



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








