/*============================================================================*/
/*== [service名  ]:  gcpmsi00_inq2      ||  [对应VC#画面 ]:GCPMSI00          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2022/10/13 21:28:58==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMSI00                                          ==*/
/*== [调用函数   ]： 无				                                        ==*/
/*== [service功能]： TGCPMSI00_信息查询                               ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h" 

 


/*<remark>=========================================================
/// <summary>
/// 表TGCPMSI00_信息查询,N个code_class,就返回N个 BLK. 
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmsi00_inq2)

int f_gcpmsi00_inq2(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsi00_inq2";                //定义函数英文名称  
	CString FunctionCname = "表TGCPMSI00_信息查询,N个code_class,就返回N个 BLK. ";              //定义函数中文名称 


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
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		CString v_code_class = "";
		CString v_blk_name = "";
		int     v_blk_cnt = 0; //返回的BLK个数。
		int     v_row_cnt = 0;
		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{
			 
			if (bcls_rec->Tables[0].Columns.Contains("CODE_CLASS"))
				v_code_class = bcls_rec->Tables[0].Rows[i]["CODE_CLASS"].ToString();
			Log::Trace("", __FUNCTION__, "in ==v_code_class[{0}]  ", v_code_class);



			//新增返回的N个blk 
			v_blk_name = v_code_class; //返回的BLK名称= CODE_CLASS
			bcls_ret->Tables.Add(v_blk_name);

			c_sql_condition = " SELECT t.code,t.code_desc_1_content "
				" from  tgcpmsi00 t "
				" where t.code_class    = @code_class "
				" and   t.VALID_FLAG    = '1' "//1=生效。
				" order by t.show_seq  " //根据显示顺
				;
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句
			// 设置SQL中的变量
			cmd_sql.Parameters.Set("code_class", v_code_class);
			cmd_sql.ExecuteQuery(bcls_ret->Tables[v_blk_name]);
			cmd_sql.Close();

			v_row_cnt = bcls_ret->Tables[v_blk_name].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "code_class[{0}]返回[{1}]行信息。", v_code_class, v_row_cnt);

			//返回的BLK个数。
			v_blk_cnt++;


		} 
		 

		/*设置系统返回参数*/ 
		sprintf(s.msg, "查询到[%d]个值集信息。", v_blk_cnt);
		  
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