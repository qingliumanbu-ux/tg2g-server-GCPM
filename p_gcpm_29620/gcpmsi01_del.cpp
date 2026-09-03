 /*============================================================================*/
/*== [service名  ]:  gcpmsi01_del       ||  [对应VC#画面 ]:GCPMSI01          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2016-9-26 16:55:18==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMSI01                                          ==*/
/*== [调用函数   ]： 无				                                          ==*/
/*== [service功能]： 表TGCPMSI01_信息删除                               ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h"


 


/*<remark>=========================================================
/// <summary>
/// 表TGCPMSI01_信息删除
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmsi01_del)

int f_gcpmsi01_del(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsi01_del";                //定义函数英文名称  
	CString FunctionCname = "表TGCPMSI01_信息删除";              //定义函数中文名称 


	//程序用变量
	int   i = 0;
	int   fun_i = 0; //功能个数。
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;  
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0; 


	//写履历表 TGCPMSI99
	//=================
	CString v_column_name = "";  //列名称们...
	CString v_column_value = ""; //列内容们...
	CString v_rec_creator2 = userid;   //记录创建责任者
	CString v_rec_create_time2 = dateNow;   //记录创建时刻  
	CString v_track_seq_no2 = CDateTime::Now().ToString("yyyyMMddHHmmssfff");;   //事件跟踪序列号
	CString v_table_name99 = "";   //数据库表名
	CString v_event_id2 = "DEL";   //事件标识
	CString v_error_remark2 = "";   //备注==表中的每个列信息拼接起来。比如： [工序]A1[工序类型]SM11...




	try
	{
	CModel tgcpmsi01("TGCPMSI01");
		
		
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //删除条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //删除条件。
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

			CString v_curr_userid = tgcpmsi01["REC_CREATOR"];
			Log::Trace("", __FUNCTION__, "v_curr_userid[{0}],userid[{1}]"
				, v_curr_userid, userid);
			 

			
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


			//数据【存在性】的校验。
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

			if (v_cnt <= 0)
			{//若没找到记录，则说明信息已经不存在了。 
				sprintf(s.msg, "数据库表名[%s]功能标识[%s]，对应的信息不存在，"
					"\n不需要删除。\n请及时刷新画面。"
					, (const char*)v_col_table_name
					, (const char*)v_col_func_id);
				throw CApplicationException(-1, s.msg, log.Location);
			}


			 
			//校验通过过，进行删除操作。
			//=========================  
			c_sql_condition = "delete from tgcpmsi01 t "
				" where t.table_name  = @table_name "
				" and   t.func_id     = @func_id "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("table_name", v_col_table_name);
			cmd_sql.Parameters.Set("func_id", v_col_func_id);
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery(); 
			cmd_sql.Close();


			////记录静态表异动履历， 表TGCPMSI99
			////================================
			//v_rec_creator2 = userid;   //记录创建责任者
			//v_rec_create_time2 = dateNow;   //记录创建时刻  
			//v_track_seq_no2 = CDateTime::Now().ToString("yyyyMMddHHmmssfff");;   //事件跟踪序列号
			//v_table_name99 = v_table_name;   //数据库表名
			//v_event_id2 = "DEL";   //事件标识
			////备注= 关键信息的拼接处理一下，即可。TABLE_NAME+func_id
			//v_error_remark2 = "表名称[" + v_col_table_name + "]功能号[" + v_col_func_id + "]";


			//Log::Trace("", __FUNCTION__, "v_table_name99[{0}]v_error_remark2[{0}]  "
			//	, v_table_name99, v_error_remark2);

			//if (v_error_remark2.Trim() == "")
			//{//若没有备注信息，则不用新增。
			//	Log::Trace("", __FUNCTION__, "若没有备注信息[{0}]，则不用新增履历。继续下一个", v_error_remark2);
			//	continue;
			//}

			//v_column_name = "(REC_CREATOR,REC_CREATE_TIME,TRACK_SEQ_NO,TABLE_NAME,EVENT_ID,ERROR_REMARK)";
			//v_column_value = CString::Format(" values('%s','%s','%s','%s','%s','%s')"
			//	, (const char*)v_rec_creator2, (const char*)v_rec_create_time2, (const char*)v_track_seq_no2
			//	, (const char*)v_table_name99, (const char*)v_event_id2, (const char*)v_error_remark2);


			//c_sql_condition = " insert into tgcpmsi99 " + v_column_name + v_column_value;
			//Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);
			//sqlstr = c_sql_condition;
			//cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
			//cmd_sql.ExecuteNonQuery();
			//cmd_sql.Close();




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







