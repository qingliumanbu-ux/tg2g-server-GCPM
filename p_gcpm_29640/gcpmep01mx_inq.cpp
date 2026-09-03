/*============================================================================*/
/*== [service名  ]:  gcpmep01mx_inq     ||  [对应VC#画面 ]:GCPMEP01          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2019-1-15 12:33:28==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： tep0002                                            ==*/
/*== [调用函数   ]： 无				                                          ==*/
/*== [service功能]： 表tep0002_信息查询                                 ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h" 

  


/*<remark>=========================================================
/// <summary>
/// 表tep0002_信息查询
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmep01mx_inq)

int f_gcpmep01mx_inq(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmep01mx_inq";                //定义函数英文名称  
	CString FunctionCname = "表tep0002_信息查询";              //定义函数中文名称 


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
			
		Log::Trace("", __FUNCTION__, "in ==v_code_class[{0}]  ", v_code_class); 

		//信息初始化条件。
		c_sql_where = " where 1 = 1   "; //信息初始化条件。  

		//拼接前台传入的查询条件。
		if (v_code_class.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.code_class = @code_class ";
		}
		 

		 


		//SELECT....
		/*c_sql_condition = " select 1 "
			" ,t.CODE as 代码 "
			" ,t.CODE_DESC_1_CONTENT  as 描述1   ,t.CODE_DESC_2_CONTENT  as 描述2    "
			" ,t.CODE_DESC_3_CONTENT  as 描述3   ,t.CODE_DESC_4_CONTENT  as 描述4   "
			" ,t.CODE_DESC_5_CONTENT  as 描述5    "
			" ,t.CODE_CLASS as 代码编号33  "
			" from tep0002 t "; */
		c_sql_condition = " select   "
			"  t.CODE   "
			" ,t.CODE_DESC_1_CONTENT   ,t.CODE_DESC_2_CONTENT     "
			" ,t.CODE_DESC_3_CONTENT   ,t.CODE_DESC_4_CONTENT     "
			" ,t.CODE_DESC_5_CONTENT     "
			" ,t.CODE_CLASS   "
			" from tep0002 t ";
		c_order_by = " order by   t.code_class,t.code  ";  

		//信息初始化语句+ WHERE 语句。
		c_sql_condition = c_sql_condition + c_sql_where + c_order_by;
		Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);  

		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("code_class", v_code_class); 
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
		cmd_sql.ExecuteQuery(bcls_ret->Tables[0]); 
		cmd_sql.Close();

		 




		//返回列信息的加工,根据 TEP0001表中的[CODE_DESC_1_NAME]
		//==============
		CString v_col_name = "";
		CString v_col_caption = "";//新标题。
		 

		CString v_11 = "";
		CString v_22 = "";
		CString v_33 = "";
		CString v_44 = "";
		CString v_55 = "";
		c_sql_condition = " select t.CODE_DESC_1_NAME,t.CODE_DESC_2_NAME "
			" ,t.CODE_DESC_3_NAME,t.CODE_DESC_4_NAME,t.CODE_DESC_5_NAME "
			" from tep0001 t "
			" where t.code_class = @code_class ";
		Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);
		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("code_class", v_code_class);
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
		//cmd_sql.ExecuteQuery(bcls_ret->Tables[0]);
		cmd_sql.ExecuteReader();
		if (cmd_sql.Read())
		{
			v_11 = cmd_sql.GetString(1);
			v_22 = cmd_sql.GetString(2);
			v_33 = cmd_sql.GetString(3);
			v_44 = cmd_sql.GetString(4);
			v_55 = cmd_sql.GetString(5);
		}
		cmd_sql.Close();
		Log::Trace("", __FUNCTION__, "v_11[{0}]v_22[{1}]  ", v_11, v_22);
		Log::Trace("", __FUNCTION__, "v_33[{0}]v_44[{1}]  ", v_33, v_44);
		Log::Trace("", __FUNCTION__, "v_55[{0}]v_22[{1}]  ", v_55, v_22);

		if (v_11.Trim() == "")
			v_11 = "描述1";

		if (v_22.Trim() == "")
			v_22 = "描述2";

		if (v_33.Trim() == "")
			v_33 = "描述3";

		if (v_44.Trim() == "")
			v_44 = "描述4";

		if (v_55.Trim() == "")
			v_55 = "描述5";
		 

		CDecimal v_ii = 0;
		for (i = 1; i <= 5;i++)
		{
			//若与指定列信息。11
			//==============
			v_ii = v_ii + 1;
			if (v_ii == 1)
			{  
				v_col_caption = v_11; 
			}
			if (v_ii == 2)
			{
				v_col_caption = v_22;
			}
			if (v_ii == 3)
			{
				v_col_caption = v_33;
			}
			if (v_ii == 4)
			{
				v_col_caption = v_44;
			}
			if (v_ii == 5)
			{
				v_col_caption = v_55;
			}
			v_col_name = "CODE_DESC_" + v_ii.ToString() + "_CONTENT"; //CODE_DESC_1_CONTENT 
			Log::Trace("", __FUNCTION__, "v_col_name[{0}]v_ii[{1}]  ", v_col_name, v_ii);
			Log::Trace("", __FUNCTION__, "v_col_标题[{0}]", v_col_caption);

			if (bcls_ret->Tables[0].Columns.Contains(v_col_name)
				&& v_col_caption.Trim() != "")
			{//若有指定列， 则到 TEP0001中获取信息。 
				bcls_ret->Tables[0].Columns[v_col_name].set_Caption(v_col_caption);
				Log::Trace("", __FUNCTION__, "SET--v_col_name[{0}]v_col_caption[{1}]  ", v_col_name, v_col_caption);
			} 

		}

		//设置 CODE 的标题= 代码
		v_col_name = "CODE";
		v_col_caption = "代码值";
		bcls_ret->Tables[0].Columns[v_col_name].set_Caption(v_col_caption);

		 
 



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






