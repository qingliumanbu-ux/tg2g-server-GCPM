/*============================================================================*/
/*== [service名  ]:  gcpmsi00_upd       ||  [对应VC#画面 ]:GCPMSI00          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2022/7/28 20:30:40==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMSI00                                          ==*/
/*== [调用函数   ]： 无				                                          ==*/
/*== [service功能]： 表TGCPMSI00_信息修改                               ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h"





/*<remark>=========================================================
/// <summary>
/// 表TGCPMSI00_信息修改
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmsi00_upd)

int f_gcpmsi00_upd(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);


	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsi00_upd";                //定义函数英文名称  
	CString FunctionCname = "表TGCPMSI00_信息修改";              //定义函数中文名称 


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
	CString v_code_class99 = "";   //数据库表名
	CString v_event_id2 = "DEL";   //事件标识
	CString v_error_remark2 = "";   //备注==表中的每个列信息拼接起来。比如： [工序]A1[工序类型]SM11...



	try
	{
		CModel tgcpmsi00("TGCPMSI00");
		CModel tgcpmsi00_chk("TGCPMSI00");

		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString    c_order_by = " order by t.order_no ";


		/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2 = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition2 = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";

		v_cnt = 0;
		c_sql_condition = "select count(1) from tgcpmsi00 t "
			" where t.code_class   ='PMF0' "
			" and   t.code         = @code " //指定的人员
			" and   t.valid_flag   = '1' " //1=有效
			;
		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("code", userid);//责任者。 
		cmd_sql.SetCommandText(c_sql_condition);
		v_cnt = cmd_sql.ExecuteScalar();
		cmd_sql.Close();

		if (v_cnt <= 0
			//&& userid.Trim() != "178029"
			)
		{//若没有找到记录，则说明当前用户，不能进行当前操作。 
			sprintf(s.msg, "您的帐号[%s]没有当前操作权限，\n当前操作失败。"
				, (const char*)userid);
			throw CApplicationException(-1, s.msg, log.Location);
		}


		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{
			tgcpmsi00.MergeFrom(bcls_rec->Tables[0].Rows[i]); //前台传入的数据。

			//==主键信息。
			//CString CODE_CLASS;   //数据库表名 
			//CString CODE;   //功能标识

			CString v_code_class = tgcpmsi00["CODE_CLASS"];
			CString v_code = tgcpmsi00["CODE"];

			Log::Trace("", __FUNCTION__, "第[{0}]个，CODE_CLASS =[{1}]CODE[{2}]"
				, i + 1, v_code_class, v_code);

			if (v_code_class.Trim() == "")
			{
				sprintf(s.msg, "[代码编号]不能为空。");
				throw CApplicationException(-1, s.msg, log.Location);
			}

			if (v_code.Trim() == "")
			{
				sprintf(s.msg, "[代码]不能为空。");
				throw CApplicationException(-1, s.msg, log.Location);
			}


			//数据【存在性】的校验。tgcpmsi00_chk.Reset();
			//=================
			tgcpmsi00_chk.Reset();
			c_sql_condition = "select t.* from tgcpmsi00 t "
				" where t.code_class  = @code_class "
				" and   t.code        = @code "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("code_class", v_code_class);
			cmd_sql.Parameters.Set("code", v_code);
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteReader();
			if (cmd_sql.Read())
			{
				cmd_sql.Fetch(tgcpmsi00_chk);
			}
			cmd_sql.Close();


			//修改= 先删除， 后新增
			//===========
			c_sql_condition = "delete from  tgcpmsi00 t "
				" where t.code_class  = @code_class "
				" and   t.code        = @code "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("code_class", v_code_class);
			cmd_sql.Parameters.Set("code", v_code);
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();




			//=========================    
			//保留原信息。
			tgcpmsi00["REC_CREATOR"] = tgcpmsi00_chk["REC_CREATOR"];
			tgcpmsi00["REC_CREATE_TIME"] = tgcpmsi00_chk["REC_CREATE_TIME"];

			//修改时刻。
			tgcpmsi00["REC_REVISOR"] = userid;
			tgcpmsi00["REC_REVISE_TIME"] = dateNow;
			tgcpmsi00.TrimOrBlank();

			sqlstr = "insert into tgcpmsi00";
			tgcpmsi00.TrimOrBlank();
			tgcpmsi00.Insert();


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


