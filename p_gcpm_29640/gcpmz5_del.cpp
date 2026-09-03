/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2015-6-27 18:05:02
功能: ED54配置信息_删除
删除历史：
日期:________；删除人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h"



/*<remark>=========================================================
/// <summary>
/// ED54配置信息_删除
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz5_del)

int f_gcpmz5_del(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz5_del";                //定义函数英文名称  
	CString FunctionCname = "ED54配置信息_删除";              //定义函数中文名称
	//LogTrace(1, 1, " **************%s begin*****************", (const char*)FunctionEname);


	//程序用变量
	int   i = 0;
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  systime = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;  
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0;
	CString  v_dllname = "";
	CString  v_table_name = "";


	//写履历表 TGCPMSI99
	//=================
	CString v_column_name = "";  //列名称们...
	CString v_column_value = ""; //列内容们...
	CString v_rec_creator2 = userid;   //记录创建责任者
	CString v_rec_create_time2 = systime;   //记录创建时刻  
	CString v_track_seq_no2 = CDateTime::Now().ToString("yyyyMMddHHmmssfff");;   //事件跟踪序列号
	CString v_table_name99 = "";   //数据库表名
	CString v_event_id2 = "DEL";   //事件标识
	CString v_error_remark2 = "";   //备注==表中的每个列信息拼接起来。比如： [工序]A1[工序类型]SM11...


	try
	{



		 
	CModel tgcpmz1("TGCPMZ1");
		
		
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //批量删除条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //批量删除条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		 
		CString v_func_id = ""; //功能号，
		CString v_func_desc = "";//功能描述


		//允许批量修改ED54信息的帐号。
		//======================
		v_cnt = 0;
		c_sql_condition = "select count(1) from tgcpmsi00 t "
			" where t.code_class          =  'GCPD' " //ED54批量修正的权限。
			" and   t.code_desc_2_content = '1' " //生效的
			" and   t.code                =  @code "//对应的帐号。
			;
		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("code", userid);//责任者。 
		cmd_sql.SetCommandText(c_sql_condition);
		v_cnt = cmd_sql.ExecuteScalar();
		cmd_sql.Close();

		if (v_cnt <= 0)
		{//若没有找到记录，则说明当前用户，不能进行当前操作。

			sprintf(s.msg, "您的帐号[%s]没有进行当前操作的权限。"
				, (const char*)userid);
			throw CApplicationException(-1, s.msg, log.Location);
		}
		 
		 
		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{ 
			 
			if (bcls_rec->Tables[0].Columns.Contains("FUNC_ID"))
				v_func_id = bcls_rec->Tables[0].Rows[i]["FUNC_ID"].ToString().TrimOrBlank();  
			Log::Trace("", __FUNCTION__, "v_func_id =[{0}]  " 	, v_func_id );




			


			//删除 :==ted53
			//==========  
			c_sql_condition = "delete from  ted53 t " 
				" where t.func_id        = @func_id " 
				;
			sqlstr = c_sql_condition; 
			cmd_sql.Parameters.Set("func_id", v_func_id); 
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();

			Log::Trace("", __FUNCTION__, "delete from ted53");

			//删除 :==ted53_res
			//==========  
			c_sql_condition = "delete from  ted53_res t "
				" where t.func_id        = @func_id "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("func_id", v_func_id);
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();
			Log::Trace("", __FUNCTION__, "delete from ted53_res");


			//删除 :==ted54
			//==========  
			c_sql_condition = "delete from  ted54 t "
				" where t.func_id        = @func_id "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("func_id", v_func_id);
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();

			Log::Trace("", __FUNCTION__, "delete from ted54");

			//删除 :==ted54_res
			//==========  
			c_sql_condition = "delete from  ted54_res t "
				" where t.func_id        = @func_id "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("func_id", v_func_id);
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();
			Log::Trace("", __FUNCTION__, "delete from ted54_res");
			
			
			
			
			//记录静态表移动履历， 表TGCPMSI99
			//================================
			v_table_name = "ted53,ted54";
			v_rec_creator2 = userid;   //记录创建责任者
			v_rec_create_time2 = systime;   //记录创建时刻  
			v_track_seq_no2 = CDateTime::Now().ToString("yyyyMMddHHmmssfff");;   //事件跟踪序列号
			v_table_name99 = v_table_name;   //数据库表名
			v_event_id2 = "DEL";   //事件标识
			v_error_remark2 = "ED54功能号删除,功能号["+ v_func_id +"]";   //备注


			Log::Trace("", __FUNCTION__, "v_table_name99[{0}]v_error_remark2[{0}]  ", v_table_name99, v_error_remark2);

			if (v_error_remark2.Trim() == "")
			{//若没有备注信息，则不用新增。
				Log::Trace("", __FUNCTION__, "若没有备注信息[{0}]，则不用新增履历。继续下一个", v_error_remark2);
				continue;
			}

			v_column_name = "(REC_CREATOR,REC_CREATE_TIME,TRACK_SEQ_NO,TABLE_NAME,EVENT_ID,ERROR_REMARK)";
			v_column_value = CString::Format(" values('%s','%s','%s','%s','%s','%s')"
				, (const char*)v_rec_creator2, (const char*)v_rec_create_time2, (const char*)v_track_seq_no2
				, (const char*)v_table_name99, (const char*)v_event_id2, (const char*)v_error_remark2);


			c_sql_condition = " insert into tgcpmsi99 " + v_column_name + v_column_value;
			Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
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


