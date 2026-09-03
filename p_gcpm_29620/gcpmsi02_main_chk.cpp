 /*============================================================================*/
/*== [service名  ]:  gcpmsi02_main_chk  ||  [对应VC#画面 ]:GCPMSI02          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2018-3-27 14:14:30==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMSI02                                          ==*/
/*== [调用函数   ]： 无				                                          ==*/
/*== [service功能]： 表TGCPMSI02_配置主信息校验                         ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h"


 


/*<remark>=========================================================
/// <summary>
/// 表TGCPMSI02_配置主信息校验
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmsi02_main_chk)

int f_gcpmsi02_main_chk(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsi02_main_chk";                //定义函数英文名称  
	CString FunctionCname = "表TGCPMSI02_配置主信息校验";              //定义函数中文名称 


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
	CModel tgcpmsi02_chk("TGCPMSI02");
		
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //修改条件。
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

			if (tgcpmsi02["SEQ_NO"].ToDecimal() <0)
			{
				sprintf(s.msg, "[序号]不能小于零。");
				throw CApplicationException(-1, s.msg, log.Location);
			}

			

			

			//配置主信息的校验。
			//================= 
			c_sql_condition = "select t.* from tgcpmsi02 t "
				" where t.table_name  = @table_name " //业务表名称
				" and   t.seq_no      = @seq_no "//序号
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("table_name", tgcpmsi02["TABLE_NAME"].ToString());
			cmd_sql.Parameters.Set("seq_no", tgcpmsi02["SEQ_NO"].ToDecimal());
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteReader();
			if (cmd_sql.Read())
			{
				cmd_sql.Fetch(tgcpmsi02_chk);
			}
			cmd_sql.Close(); 

			//报错信息的显示。
			if (tgcpmsi02_chk["MODE_NO"].ToString().Trim() == "")
			{
				sprintf(s.msg, "业务表名[%s]序号[%d]，\n对应的【画面模式】不允许为空。"
					, (const char*)tgcpmsi02["TABLE_NAME"].ToString()
					, tgcpmsi02["SEQ_NO"].ToDecimal().ToInt32());
				throw CApplicationException(-1, s.msg, log.Location);
			}

			if (tgcpmsi02_chk["MOID"].ToString().Trim() == "")
			{
				sprintf(s.msg, "业务表名[%s]序号[%d]，\n对应的【二级模块】不允许为空。"
					, (const char*)tgcpmsi02["TABLE_NAME"].ToString()
					, tgcpmsi02["SEQ_NO"].ToDecimal().ToInt32());
				throw CApplicationException(-1, s.msg, log.Location);
			}


			if (tgcpmsi02_chk["FORM_CODE"].ToString().Trim() == "")
			{
				sprintf(s.msg, "业务表名[%s]序号[%d]，\n对应的【画面编号】不允许为空。"
					, (const char*)tgcpmsi02["TABLE_NAME"].ToString()
					, tgcpmsi02["SEQ_NO"].ToDecimal().ToInt32());
				throw CApplicationException(-1, s.msg, log.Location);
			}


			if (tgcpmsi02_chk["KEYVALUE_2"].ToString().Trim() == "")
			{
				sprintf(s.msg, "业务表名[%s]序号[%d]，\n对应的【多记录显示_ED54功能号】不允许为空。"
					, (const char*)tgcpmsi02["TABLE_NAME"].ToString()
					, tgcpmsi02["SEQ_NO"].ToDecimal().ToInt32());
				throw CApplicationException(-1, s.msg, log.Location);
			}




			/*
			TABLE_NAME	业务表名*
			SEQ_NO	序号*
			TABLE_CNAME	业务表描述
			MODE_NO	画面模式====xx必有。
			MOID	二级模块=====xx必有。
			SEQ_CODE	多表显示序号 ===
			FORM_CODE	画面编号 ====xx必有。

			KEYVALUE_1	允许操作者<br>[空为不约束]<br>K1
			KEYVALUE_2	多记录显示<br>功能号<br>K2 ===xx必有。
			KEYVALUE_3	查询条件<br>功能号<br>K3 ==
			KEYVALUE_4	单记录显示<br>功能号<br>K4
			PAGE_NUM	每页行数
			KEYVALUE_5	统计service<br>K5
			KEYVALUE_6	对应母画面<br>K6
			VALID_FLAG	生效标记 ===1=生效。
			*/

			/*
			01	多表单画面
			01G	多表单画面分组
			//以下有查询条件
			02	单表单画面[非分页]
			02M	单表单画面[分页]---》有每页的记录数
			02PG	pGrid统计画面--->有统计service
			//以下有单记录ED54功能号
			03	[单+多]记录显示 
			*/

			//KEYVALUE_3	查询条件<br>功能号<br>K3 ==
			if (tgcpmsi02_chk["KEYVALUE_3"].ToString().Trim() == "")
			{
				if (tgcpmsi02_chk["MODE_NO"].ToString().Trim().Substring(0, 2) == "02"
					|| tgcpmsi02_chk["MODE_NO"].ToString().Trim().Substring(0, 2) == "03"
					)
				{
					sprintf(s.msg, "业务表名[%s]序号[%d]，\n对应的【查询条件_ED54功能号】不允许为空。"
						, (const char*)tgcpmsi02["TABLE_NAME"].ToString()
						, tgcpmsi02["SEQ_NO"].ToDecimal().ToInt32());
					throw CApplicationException(-1, s.msg, log.Location);
				}
			}

			//KEYVALUE_4	单记录显示<br>功能号<br>K4
			if (tgcpmsi02_chk["KEYVALUE_4"].ToString().Trim() == "")
			{
				if (tgcpmsi02_chk["MODE_NO"].ToString().Trim().Substring(0, 2) == "03"
					)
				{
					sprintf(s.msg, "业务表名[%s]序号[%d]，\n对应的【单记录显示_ED54功能号】不允许为空。"
						, (const char*)tgcpmsi02["TABLE_NAME"].ToString()
						, tgcpmsi02["SEQ_NO"].ToDecimal().ToInt32());
					throw CApplicationException(-1, s.msg, log.Location);
				}
			}

			//PAGE_NUM	每页行数
			if (tgcpmsi02_chk["PAGE_NUM"].ToDecimal() <= 0)
			{
				if (tgcpmsi02_chk["MODE_NO"].ToString().Trim() == "02M"
					)
				{
					sprintf(s.msg, "业务表名[%s]序号[%d]，\n分页模式,对应的【每页行数】不允许小于等于0。"
						, (const char*)tgcpmsi02["TABLE_NAME"].ToString()
						, tgcpmsi02["SEQ_NO"].ToDecimal().ToInt32());
					throw CApplicationException(-1, s.msg, log.Location);
				}
			}

			//KEYVALUE_5	统计service<br>K5
			//02PG	pGrid统计画面--->有统计service
			if (tgcpmsi02_chk["KEYVALUE_5"].ToString().Trim() == "")
			{
				if (tgcpmsi02_chk["MODE_NO"].ToString().Trim() == "02PG"
					)
				{
					sprintf(s.msg, "业务表名[%s]序号[%d]，\npGrid统计模式,对应的【统计service】不允许为空。"
						, (const char*)tgcpmsi02["TABLE_NAME"].ToString()
						, tgcpmsi02["SEQ_NO"].ToDecimal().ToInt32());
					throw CApplicationException(-1, s.msg, log.Location);
				}
			}

			//KEYVALUE_6	对应母画面<br>K6
			if (tgcpmsi02_chk["KEYVALUE_6"].ToString().Trim() == "")
			{ 
					sprintf(s.msg, "业务表名[%s]序号[%d]，\n对应的【母画面】不允许为空\n请点击【F7】进行关系创建。"
						, (const char*)tgcpmsi02["TABLE_NAME"].ToString()
						, tgcpmsi02["SEQ_NO"].ToDecimal().ToInt32());
					throw CApplicationException(-1, s.msg, log.Location);
				 
			}

			//VALID_FLAG	生效标记 ===1=生效。
			if (tgcpmsi02_chk["VALID_FLAG"].ToString().Trim() != "1")
			{
				sprintf(s.msg, "业务表名[%s]序号[%d]，对应的配置信息是【失效】状态。"
					, (const char*)tgcpmsi02["TABLE_NAME"].ToString()
					, tgcpmsi02["SEQ_NO"].ToDecimal().ToInt32());
				throw CApplicationException(-1, s.msg, log.Location);

			} 

		} 


		/*设置系统返回参数*/
		strcpy(s.msg, "画面配置【主信息】校验通过。");//处理成功。 

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







