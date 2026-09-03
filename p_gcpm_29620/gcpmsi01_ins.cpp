 /*============================================================================*/
/*== [service名  ]:  gcpmsi01_ins       ||  [对应VC#画面 ]:GCPMSI01          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2016-9-26 16:55:18==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMSI01                                          ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： 表TGCPMSI01_信息新增                               ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h"


 

/*<remark>=========================================================
/// <summary>
/// 表TGCPMSI01_信息新增
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmsi01_ins)

int f_gcpmsi01_ins(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsi01_ins";                //定义函数英文名称  
	CString FunctionCname = "表TGCPMSI01_信息新增";              //定义函数中文名称 


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
	CModel tgcpmsi01("TGCPMSI01");
		
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //新增条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //新增条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;




		////根据[表名称]，进行对应责任者的校验，确认有权限，才允许操作。
		////============================= 
		//CString v_table_name = "TGCPMSI01";
		//v_cnt = 0;
		//c_sql_condition = "select count(1) from tgcpmsi00 t "
		//	" where t.code_class   ='GCPE' "
		//	" and   t.code = @code " //指定的表名称
		//	" and ( t.code_desc_5_content || ','  LIKE  '%' || @code_desc_5_content || ',%'  "
		//	"       or t.code_desc_5_content = ' ' )   "  //在[责任者]要求范围内。
		//	;
		//sqlstr = c_sql_condition;
		//cmd_sql.Parameters.Set("code", v_table_name);//表名称
		//cmd_sql.Parameters.Set("code_desc_5_content", userid);//责任者。 
		//cmd_sql.SetCommandText(c_sql_condition);
		//v_cnt = cmd_sql.ExecuteScalar().ToInt32();
		//cmd_sql.Close();

		//if (v_cnt <= 0)
		//{//若没有找到记录，则说明当前用户，不能进行当前DLL 的操作。
		//	sprintf(s.msg, "您的帐号[%s]没有业务表[%s]的操作权限，当前操作失败。"
		//		, (const char*)userid, (const char*)v_table_name);
		//	throw CApplicationException(-1, s.msg, log.Location);
		//}
	 
		  
		
		 
		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{

			tgcpmsi01.Reset(); //为了保证数据的准确性，暂定加个 RESET();
			tgcpmsi01.MergeFrom(bcls_rec->Tables[0].Rows[i]); //前台传入的数据。
			
			
			//==主键信息。
			//CString TABLE_NAME;   //数据库表名 
			//CString FUNC_ID;   //功能标识

			CString v_col_table_name = tgcpmsi01["TABLE_NAME"];
			CString v_col_func_id = tgcpmsi01["FUNC_ID"];

			Log::Trace("", __FUNCTION__, "第[{0}]个，TABLE_NAME =[{1}]FUNC_ID[{2}]"
				, i + 1, v_col_table_name, v_col_func_id);
				
			if (v_col_table_name.Trim() == "")
			{
				sprintf(s.msg, "[数据库表名]不能为空。");
				throw CApplicationException(-1, s.msg, log.Location);
			}

			if (v_col_func_id.Trim() == "")
			{
				sprintf(s.msg, "[功能标识]不能为空。");
				throw CApplicationException(-1, s.msg, log.Location);
			}


			//主键重复性的校验。
			//=================
			v_cnt = 0;
			c_sql_condition = "select count(1) from tgcpmsi01 t "
				" where t.table_name  = @table_name " 
				" and   t.func_id     = @func_id " 
				 ;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("table_name", v_col_table_name);
			cmd_sql.Parameters.Set("func_id", v_col_func_id);
			cmd_sql.SetCommandText(c_sql_condition);
			v_cnt = cmd_sql.ExecuteScalar();
			cmd_sql.Close();

			if (v_cnt  >=1)
			{//若找到记录，则说明主键重复了。

				sprintf(s.msg, "数据库表名[%s]功能标识[%s]，对应的信息已经存在，\n不允许新增。"
					, (const char*)v_col_table_name
					, (const char*)v_col_func_id);
				throw CApplicationException(-1, s.msg, log.Location);
			}
			 

			 
			//校验通过过，进行新增操作。
			//=========================  
			//给指定表的指定列赋值。
			tgcpmsi01["REC_CREATOR"] = userid;
			tgcpmsi01["REC_CREATE_TIME"] = dateNow;
			tgcpmsi01["REC_REVISOR"] = "";
			tgcpmsi01["REC_REVISE_TIME"] = "";  

			tgcpmsi01.TrimOrBlank(); 
			sqlstr = "insert into tgcpmsi01";
			tgcpmsi01.TrimOrBlank();
			tgcpmsi01.Insert();
			 
		} 


		/*设置系统返回参数*/
		strcpy(s.msg, "恭喜，处理成功。");//处理成功。 

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







