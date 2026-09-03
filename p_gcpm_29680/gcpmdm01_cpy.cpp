/*============================================================================*/
/*== [service名  ]:  gcpmdm01_cpy       ||  [对应VC#画面 ]:GCPMDM01          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2017-10-9 10:36:44==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMDM01                                          ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： 表TGCPMDM01_信息复制                               ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h" 

 


/*<remark>=========================================================
/// <summary>
/// 表TGCPMDM01_信息复制
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmdm01_cpy)

int f_gcpmdm01_cpy(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmdm01_cpy";                //定义函数英文名称  
	CString FunctionCname = "表TGCPMDM01_信息复制";              //定义函数中文名称 


	//程序用变量
	int   i = 0;
	int   fun_i = 0; //功能个数。
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  v_systime = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  v_userid = s.userid;
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0; 


	try
	{ 
		//CTGCPMDM01 tgcpmdm01(conn);
		CString v_model_tname = "TGCPMDM01"; //业务表名称。
		CModel tgcpmdm01 = CModel("TGCPMDM01"); //创建动态头文件 
		CModel tgcpmdm02 = CModel("TGCPMDM02"); //创建动态头文件 
		CModel tgcpmdm03 = CModel("TGCPMDM03"); //创建动态头文件 
		
		
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

		CString v_new_code_class = ""; //新代码编号
		if (bcls_rec->Tables[0].Columns.Contains("NEW_CODE_CLASS"))
			v_new_code_class = bcls_rec->Tables[0].Rows[0]["NEW_CODE_CLASS"].ToString();
			
		Log::Trace("", __FUNCTION__, "in ==v_code_class[{0}]  ", v_code_class);
		Log::Trace("", __FUNCTION__, "in ==v_new_code_class[{0}]  ", v_new_code_class);

		  

		 //入口信息校验。
		if (v_code_class.Trim() == "")
		{
			sprintf(s.msg, "[代码编号]不能为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		}

		if (v_new_code_class.Trim() == "")
		{
			sprintf(s.msg, "[新代码编号]不能为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		}


		//检查[代码编号]的规则信息是否存在，存在报错。
		//======================
		v_cnt = 0;
		c_sql_condition = " select COUNT(1) from tgcpmdm01 "
			" where code_class = @code_class " 
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句
		// 设置SQL中的变量
		cmd_sql.Parameters.Set("code_class", v_code_class); 
		v_cnt = cmd_sql.ExecuteScalar();
		cmd_sql.Close();
		if (v_cnt <= 0)
		{
			sprintf(s.msg, "代码编号[%s]信息【不存在】，无法复制。"
				, (const char*)v_code_class);
			throw CApplicationException(-1, s.msg, log.Location);
		}

		//检查[新代码编号]的规则信息是否存在，存在报错。
		//================
		v_cnt = 0;
		c_sql_condition = " select COUNT(1) from tgcpmdm01 "
			" where code_class = @code_class "
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句
		// 设置SQL中的变量
		cmd_sql.Parameters.Set("code_class", v_new_code_class);
		v_cnt = cmd_sql.ExecuteScalar();
		cmd_sql.Close();
		if (v_cnt >= 1)
		{
			sprintf(s.msg, "【新】代码编号[%s]信息【已存在】，无法复制。"
				, (const char*)v_new_code_class );
			throw CApplicationException(-1, s.msg, log.Location);
		}

		//根据[v_code_class]的信息，复制一套变成新的代码[v_new_code_class]
		//============================tgcpmdm01,02,03 
		 
		tgcpmdm01.Reset();
		c_sql_condition = " select * from tgcpmdm01 "
			" where code_class = @code_class "
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句
		// 设置SQL中的变量
		cmd_sql.Parameters.Set("code_class", v_code_class); 
		cmd_sql.ExecuteReader(); //执行读取
		while (cmd_sql.Read()) // 
		{
			cmd_sql.Fetch(tgcpmdm01);		  
			//赋予新的事件号。
			tgcpmdm01["CODE_CLASS"] = v_new_code_class;//新代码编号。 
			tgcpmdm01["REC_CREATE_TIME"] = v_systime;//创建时刻
			tgcpmdm01["REC_CREATOR"] = v_userid;//创建者
			tgcpmdm01["REC_REVISE_TIME"] = "";
			tgcpmdm01["REC_REVISOR"] = "";

			tgcpmdm01.TrimOrBlank();
			tgcpmdm01.Insert(); 
		}
		cmd_sql.Close(); //关闭游标


		tgcpmdm02.Reset();
		c_sql_condition = " select * from tgcpmdm02 "
			" where code_class = @code_class "
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句
		// 设置SQL中的变量
		cmd_sql.Parameters.Set("code_class", v_code_class);
		cmd_sql.ExecuteReader(); //执行读取
		while (cmd_sql.Read()) // 
		{
			cmd_sql.Fetch(tgcpmdm02);
			//赋予新的事件号。
			tgcpmdm02["CODE_CLASS"] = v_new_code_class;//新代码编号。 
			tgcpmdm02["REC_CREATE_TIME"] = v_systime;//创建时刻
			tgcpmdm02["REC_CREATOR"] = v_userid;//创建者
			tgcpmdm02["REC_REVISE_TIME"] = "";
			tgcpmdm02["REC_REVISOR"] = "";

			tgcpmdm02.TrimOrBlank();
			tgcpmdm02.Insert();
		}
		cmd_sql.Close(); //关闭游标



		tgcpmdm03.Reset();
		c_sql_condition = " select * from tgcpmdm03 "
			" where code_class = @code_class "
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句
		// 设置SQL中的变量
		cmd_sql.Parameters.Set("code_class", v_code_class);
		cmd_sql.ExecuteReader(); //执行读取
		while (cmd_sql.Read()) // 
		{
			cmd_sql.Fetch(tgcpmdm03);
			//赋予新的事件号。
			tgcpmdm03["CODE_CLASS"] = v_new_code_class;//新代码编号。 
			tgcpmdm03["REC_CREATE_TIME"] = v_systime;//创建时刻
			tgcpmdm03["REC_CREATOR"] = v_userid;//创建者
			tgcpmdm03["REC_REVISE_TIME"] = "";
			tgcpmdm03["REC_REVISOR"] = "";

			tgcpmdm03.TrimOrBlank();
			tgcpmdm03.Insert();
		}
		cmd_sql.Close(); //关闭游标

	 
 
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

	//将来可能要拆service处理，SO ，此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;

}







