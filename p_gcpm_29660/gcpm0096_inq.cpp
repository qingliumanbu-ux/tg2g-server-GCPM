/*============================================================================*/
/*== [service名  ]:  gcpm0096_inq       ||  [对应VC#画面 ]:GCPMSI01          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2018-4-2 10:18:25 ==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TMM0096                                            ==*/
/*== [调用函数   ]： 无				                                          ==*/
/*== [service功能]： 物料异动履历_查询                                  ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h" 

 


/*<remark>=========================================================
/// <summary>
/// 物料异动履历_查询
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpm0096_inq)

int f_gcpm0096_inq(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpm0096_inq";                //定义函数英文名称  
	CString FunctionCname = "物料异动履历_查询";              //定义函数中文名称 


	//程序用变量
	int   i = 0;
	int   fun_i = 0; //功能个数。
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;  
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0; 
	CString v_mat_kind = "";



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
	 
		  
		//==查询条件信息。
		//CString MAT_NO;   //材料号 
		  
		  //从1#BLK 中获取静态表的表名称。
		CString v_mat_no = "";
		if (bcls_rec->Tables[0].Columns.Contains("MAT_NO"))
			v_mat_no = bcls_rec->Tables[0].Rows[0]["MAT_NO"].ToString(); 
		Log::Trace("", __FUNCTION__, "in ==v_mat_no[{0}]  ", v_mat_no); 

		if (bcls_rec->Tables[0].Columns.Contains("MAT_KIND"))
			v_mat_kind = bcls_rec->Tables[0].Rows[0]["MAT_KIND"].ToString();
		Log::Trace("", __FUNCTION__, "in ==v_mat_kind[{0}]  ", v_mat_kind);

		if (v_mat_kind.Trim() == "")
		{
			sprintf(s.msg,"【物料种类】不允许为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		}

		//在线表
		CString v_table_name = "";//tmmsm96
		v_table_name = "tmm"+ v_mat_kind.Trim().ToLower() + "96";
		Log::Trace("", __FUNCTION__, "in ==v_table_name[{0}]  ", v_table_name);

		//历史表信息
		CString v_table_name_h = "";//hmmsm96
		v_table_name_h = "hmm" + v_mat_kind.Trim().ToLower() + "96";
		Log::Trace("", __FUNCTION__, "in ==v_table_name_h[{0}]  ", v_table_name_h);
		//根据材料号获取对应的MAT_KIND
		//==================
		fetchRowCount = 0;
		c_sql_condition = " select t.* from " + v_table_name + " t "
			" where t.mat_no = @mat_no "
			//" order by t.RESUME_SEQ_NO "//根据履历序号正排序。
			" union all "
			" select a.* from " + v_table_name_h + " a "
			" where a.mat_no = @mat_no "
			; 
		Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);   
		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("mat_no", v_mat_no); 
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
		cmd_sql.ExecuteQuery(bcls_ret->Tables[0]);
		cmd_sql.Close();

		//返回的记录数。 
		fetchRowCount = bcls_ret->Tables[0].Rows.get_Count(); 
		

		/*设置系统返回参数*/
		sprintf(s.msg, "查询到[%d]条记录,业务表[%s]-[%s]"
			, fetchRowCount, (const char*)v_table_name, (const char*)v_table_name_h);
		 
		  
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







