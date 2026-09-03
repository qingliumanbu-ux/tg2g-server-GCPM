 /*============================================================================*/
/*== [service名  ]:  gcpmsi02_cmt       ||  [对应VC#画面 ]:GCPMSI02          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2018-3-26 14:03:40==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMSI02                                          ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： 表TGCPMSI02_配置生效                               ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h"


 


/*<remark>=========================================================
/// <summary>
/// 表TGCPMSI02_配置生效
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmsi02_cmt)

int f_gcpmsi02_cmt(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsi02_cmt";                //定义函数英文名称  
	CString FunctionCname = "表TGCPMSI02_配置生效";              //定义函数中文名称 


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

			/*
			画面代码， 画面模式，
			根据画面模式创建 EPESPARA表信息。

			CString FORM_CODE;   //画面编号
			CString MODE_NO;   //模式号=画面模式
			以下存放母画面代码。
			CString KEYVALUE_6;   //关键字串6=记录母画面代码。
			*/

			//数据的校验。
			//================= 
			c_sql_condition = "select t.FORM_CODE "
				" ,t.MODE_NO "
				"from tgcpmsi02 t "
				" where t.table_name  = @table_name "
				" and   t.seq_no      = @seq_no "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("table_name", tgcpmsi02["TABLE_NAME"].ToString());
			cmd_sql.Parameters.Set("seq_no", tgcpmsi02["SEQ_NO"].ToDecimal());
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteReader();
			if (cmd_sql.Read())
			{
				tgcpmsi02["FORM_CODE"] = cmd_sql.GetString(1);
				tgcpmsi02["MODE_NO"] = cmd_sql.GetString(2);
			}			 
			cmd_sql.Close();
			if (tgcpmsi02["FORM_CODE"].ToString().Trim() == "")
			{//若找到记录，则说明主键重复了。

				sprintf(s.msg, "业务表名[%s]序号[%d]，对应的[画面代码]不允许为空。"
					, (const char*)tgcpmsi02["TABLE_NAME"].ToString()
					, tgcpmsi02["SEQ_NO"].ToDecimal().ToInt32());
				throw CApplicationException(-1, s.msg, log.Location);
			} 

			if (tgcpmsi02["MODE_NO"].ToString().Trim() == "")
			{//若找到记录，则说明主键重复了。

				sprintf(s.msg, "业务表名[%s]序号[%d]，对应的[画面模式]不允许为空。"
					, (const char*)tgcpmsi02["TABLE_NAME"].ToString()
					, tgcpmsi02["SEQ_NO"].ToDecimal().ToInt32());
				throw CApplicationException(-1, s.msg, log.Location);
			}

			//根据画面模式获取对应的【母画面代码】
			//=================			
			CString v_base_form_code = ""; //母画面代码。
			c_sql_condition = "select t.CODE_DESC_2_CONTENT " 
				" from  tgcpmsi00 t "
				" where t.code_class  = 'GCPM00' " //画面模式。
				" and   t.code        = @code "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("code", tgcpmsi02["MODE_NO"].ToString()); //画面模式。 
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteReader();
			if (cmd_sql.Read())
			{
				v_base_form_code = cmd_sql.GetString(1); 
			}
			cmd_sql.Close();

			

			 
			//校验通过过，进行【配置生效】
			/*
			新增EPESPARA,子母配置信息。
			SELECT  T.FORM_NAME,T.FORM_BASE_NAME
			,T.* FROM  tesformpara  T
			WHERE T.FORM_BASE_NAME = 'GCPMSIF2M'
			 
			*/
			//=========================  

			//先删除老数据，再新增。
			//======================
			c_sql_condition = "delete from tesformpara t "
				" where t.form_name  = @form_name " 
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("form_name", tgcpmsi02["FORM_CODE"].ToString()); //子画面代码。 
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery(); 
			cmd_sql.Close();

			//insert into tgcpmsi99(aa,bb) values('aa','bb);
			//=====================
			CString v_form_name = tgcpmsi02["FORM_CODE"].ToString();
			CString v_pk1_name = "UiFormName";
			CString v_pk1_value = v_form_name; //默认： PK1值 = 子画面名称
			c_sql_condition = "insert into tesformpara(form_name,form_base_name "
				",PK1_NAME,PK1 )" //K1名称，K1内容
				" values(@form_name,@form_base_name,@pk1_name,@pk1_value)" 
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("form_name", v_form_name); //子画面代码。 
			cmd_sql.Parameters.Set("form_base_name", v_base_form_code); //母画面代码。 
			cmd_sql.Parameters.Set("pk1_name", v_pk1_name); //PK1名称 
			cmd_sql.Parameters.Set("pk1_value", v_pk1_value); //PK1值 
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();



			//修改表 TGCPMSI02,记录当前对应的母画面代码。
			//CString KEYVALUE_6;   //关键字串6=母画面代码。
			//===================
			c_sql_condition = " update tgcpmsi02 t "
				" set   t.keyvalue_6 = @keyvalue_6 "//母画面代码。
				" where t.table_name  = @table_name "
				" and   t.seq_no      = @seq_no "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("keyvalue_6", v_base_form_code);//母画面代码。
			cmd_sql.Parameters.Set("table_name", tgcpmsi02["TABLE_NAME"].ToString());
			cmd_sql.Parameters.Set("seq_no", tgcpmsi02["SEQ_NO"].ToDecimal());
			cmd_sql.SetCommandText(c_sql_condition);
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

	//将来可能要拆service处理，SO ，此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;

}







