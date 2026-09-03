 /*============================================================================*/
/*== [service名  ]:  gcpmsi02_del       ||  [对应VC#画面 ]:GCPMSI02          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2018-3-15 15:24:47==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMSI02                                          ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： 表TGCPMSI02_信息删除                               ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h"


 


/*<remark>=========================================================
/// <summary>
/// 表TGCPMSI02_信息删除
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmsi02_del)

int f_gcpmsi02_del(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsi02_del";                //定义函数英文名称  
	CString FunctionCname = "表TGCPMSI02_信息删除";              //定义函数中文名称 


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
	CModel tgcpmsi02("TGCPMSI02");
		 
		
		
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //删除条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //删除条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 

		 
		CString v_table_name = "TGCPMSI02";
		
		 
		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{

			tgcpmsi02.Reset(); //为了保证数据的准确性，暂定加个 RESET();
			tgcpmsi02.MergeFrom(bcls_rec->Tables[0].Rows[i]); //前台传入的数据。
			
			 

			//==主键信息。
			//CString TABLE_NAME; //数据库表名 
			//CString SEQ_NO;    //序号
			Log::Trace("", __FUNCTION__, "第[{0}]个，TABLE_NAME =[{1}]SEQ_NO[{2}]"
				, i + 1, tgcpmsi02["TABLE_NAME"].ToString(), tgcpmsi02["SEQ_NO"].ToDecimal());

			if (tgcpmsi02["TABLE_NAME"].ToString().Trim() == "")
			{
				sprintf(s.msg, "[数据库表名]不能为空。");
				throw CApplicationException(-1, s.msg, log.Location);
			}

			if (tgcpmsi02["SEQ_NO"].ToDecimal() < 0)
			{
				sprintf(s.msg, "[序号]不能小于零。");
				throw CApplicationException(-1, s.msg, log.Location);
			}


			//主键重复性的校验。
			//=================
			v_cnt = 0;
			c_sql_condition = "select count(1) from tgcpmsi02 t "
				" where t.table_name  = @table_name "
				" and   t.seq_no      = @seq_no "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("table_name", tgcpmsi02["TABLE_NAME"].ToString());
			cmd_sql.Parameters.Set("seq_no", tgcpmsi02["SEQ_NO"].ToDecimal());
			cmd_sql.SetCommandText(c_sql_condition);
			v_cnt = cmd_sql.ExecuteScalar();
			cmd_sql.Close();
			if (v_cnt <= 0)
			{//若找到记录，则说明主键重复了。

				sprintf(s.msg, "业务表名[%s]序号[%d]，对应的信息不存在，"
					"\n不需要删除。\n请及时刷新画面。"
					, (const char*)tgcpmsi02["TABLE_NAME"].ToString()
					, tgcpmsi02["SEQ_NO"].ToDecimal().ToInt32());
				throw CApplicationException(-1, s.msg, log.Location);
			} 

			 
			//校验通过过，进行删除操作。
			//=========================  
			c_sql_condition = "delete from tgcpmsi02 t "
				" where t.table_name  = @table_name "
				" and   t.seq_no     = @seq_no "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("table_name", tgcpmsi02["TABLE_NAME"].ToString());
			cmd_sql.Parameters.Set("seq_no", tgcpmsi02["SEQ_NO"].ToDecimal());
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
			//v_error_remark2 = "表名称[" + tgcpmsi02["TABLE_NAME"].ToString() + "]序号[" + tgcpmsi02["TABLE_NAME"].ToString() + "]";


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







