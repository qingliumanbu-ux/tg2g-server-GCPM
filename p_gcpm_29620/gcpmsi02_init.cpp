 /*============================================================================*/
/*== [service名  ]:  gcpmsi02_init      ||  [对应VC#画面 ]:GCPMSI02          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2018-3-16 10:52:58==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMSI02                                          ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： 表TGCPMSI02_信息初始化                             ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h"


 


/*<remark>=========================================================
/// <summary>
/// 表TGCPMSI02_信息初始化
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmsi02_init)

int f_gcpmsi02_init(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsi02_init";                //定义函数英文名称  
	CString FunctionCname = "表TGCPMSI02_信息初始化";              //定义函数中文名称 


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
		
		CString v_chk_table_name = "";
		CDecimal v_chk_seq_no = 0;
		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{  
			
			
			tgcpmsi02.MergeFrom(bcls_rec->Tables[0].Rows[i]); //前台传入的数据。
			
			 

			//==主键信息。
			//CString TABLE_NAME; //数据库表名 
			//CString SEQ_NO;    //序号
			v_chk_table_name = tgcpmsi02["TABLE_NAME"];
			v_chk_seq_no = tgcpmsi02["SEQ_NO"];

			//根据 MODE_NO  ，初始化相关信息。比如 二级模块， ED54功能号名称等。
			Log::Trace("", __FUNCTION__, "第[{0}]个，TABLE_NAME =[{1}]SEQ_NO[{2}]"
				, i + 1, v_chk_table_name, v_chk_seq_no
				);

			if (v_chk_table_name.Trim() == "")
			{
				sprintf(s.msg, "[数据库表名]不能为空。");
				throw CApplicationException(-1, s.msg, log.Location);
			}

			if (v_chk_seq_no < 0)
			{
				sprintf(s.msg, "[序号]不能小于零。");
				throw CApplicationException(-1, s.msg, log.Location);
			}

			//基本信息校验。
			//================= 
			tgcpmsi02.Reset(); //为了保证数据的准确性，暂定加个 RESET();
			c_sql_condition = "select t.* "
				" from tgcpmsi02 t "
				" where t.table_name  = @table_name "
				" and   t.seq_no      = @seq_no "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("table_name", v_chk_table_name);
			cmd_sql.Parameters.Set("seq_no", v_chk_seq_no);
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteReader();
			if (cmd_sql.Read())
			{
				cmd_sql.Fetch(tgcpmsi02);
			}
			cmd_sql.Close(); 

			if (tgcpmsi02["MODE_NO"].ToString().Trim() == "")
			{
				sprintf(s.msg, "业务画面[%s]序号[%d],对应的【画面模式】为空,\n不能进行【配置信息初始化】。"
					, (const char*)v_chk_table_name
					, v_chk_seq_no.ToInt32());
				throw CApplicationException(-1, s.msg, log.Location);
			}


			 

			 
			//校验通过过，进行信息初始化操作。
			//=========================  
			//CString TABLE_NAME;   //数据库表名 ===PK
			//CDecimal SEQ_NO;   //序号===PK
			//CString MODE_NO;   //模式号===画面模式。
			////暂定： 初始化以下信息。
			//CString MOID;   //模块号 = 二级模块 = 业务表名称的第2~5位
			//CString SEQ_CODE;   //显示序号代码 = 二级模块_ZZ
			//CString FORM_CODE;   //画面编号 = 若画面模式=01=多表单记录，则， 画面编号 = 二级模块SIC=静态表集合画面
			//CString KEYVALUE_1;   //关键字串1 =允许操作者权限
			//CString KEYVALUE_2;   //关键字串2 =ED54功能号_查询条件
			//CString KEYVALUE_3;   //关键字串3 =ED54功能号_多记录显示
			//CString KEYVALUE_4;   //关键字串4 =ED54功能号_单记录显示
			//CString KEYVALUE_5;   //关键字串5
			//CString KEYVALUE_6;   //关键字串6
			//CString VALID_FLAG;   //生效标记 =1=生效
			//CString REMARK;   //备注500

			/*画面模式如下：//CString MODE_NO;   //模式号===画面模式。
			01	  多表单画面         ==一个ED54号
			01G	  多表单画面分组     ==一个ED54号

			02	  单表单画面[非分页] ==2个ED54号
			02M	  单表单画面[分页]   ==2个ED54号
			02PG  pGrid统计画面      ==2个ED54号
			03	  单表单记录画面     ==3个ED54号
			 

			//CString FORM_CODE;   //画面编号 = 
			若画面模式=01=多表单记录，则画面编号 = 二级模块SIC=静态表集合画面,比如 PMOLSIC
			其他：
			若画面模式=02/02M/02PG
			          则画面编号 = 表名称+XX+画面模式，比如 TPMOLSI00XX02,TPMOLSI00XX02M,TPMOLSI00XX02PG			
			*/
			//若二级模块为空， 则默认= = 业务表名称的第2~5位
			if (tgcpmsi02["MOID"].ToString().Trim() == "")
			{
				tgcpmsi02["MOID"] = tgcpmsi02["TABLE_NAME"].ToString().Substring(1, 4); //二级模块
			}

			//v_form_code_tmp = 业务表名称的第2位到最后。
			//比如：  TPMOFSI00 ---> PMOFSI00
			//============
			CString v_form_code_tmp = "";
			int v_form_len = v_chk_table_name.GetLength(); //业务表名称的长度。
			v_form_code_tmp = v_chk_table_name.Trim().Substring(1, v_form_len - 1);

			
			tgcpmsi02["SEQ_CODE"] = tgcpmsi02["MOID"].ToString() + "ZZ"; //多表单画面时，显示顺序。
			tgcpmsi02["KEYVALUE_1"] = " ";   //关键字串1 =允许操作者权限,默认不约束。 
			
			//根据画面模式，初始化对应列信息。
			if (tgcpmsi02["MODE_NO"].ToString().Trim() == "01")
			{//多表单画面
				tgcpmsi02["FORM_CODE"] = tgcpmsi02["MOID"].ToString() + "SIC"; //比如 PMOFSIC 
			}
			else if (tgcpmsi02["MODE_NO"].ToString().Trim() == "05G")
			{//多表单画面分组,多用于综合查询的分组。
				tgcpmsi02["FORM_CODE"] = tgcpmsi02["MOID"].ToString() + "SIG"; //比如 PMOFSIG 
			}
			else
			{//画面编号 = 表名称+F+画面模式，
			 //比如:表  TPMOFSI00 , 画面代码：PMOFSI00F02, PMOFSI00F02M,PMOFSI00F03	 
				tgcpmsi02["FORM_CODE"] = v_form_code_tmp + "F" + tgcpmsi02["MODE_NO"].ToString();
				tgcpmsi02["SEQ_CODE"] = "X"; //非多表的情况下， 显示序号= 空格。
			}
			Log::Trace("", __FUNCTION__, "最终画面编号[{0}]", tgcpmsi02["FORM_CODE"].ToString());

			//CString KEYVALUE_2;   //关键字串3 =ED54功能号_多记录显示 = 二级模块_表名称 = PMOL_TPMOLSI00
			//CString KEYVALUE_3;   //关键字串2 =ED54功能号_查询条件 = 二级模块_表名称_WHERE = PMOL_TPMOLSI00_WHERE
			//CString KEYVALUE_4;   //关键字串4 =ED54功能号_单记录显示 = 二级模块_表名称_ONE = PMOL_TPMOLSI00_ONE 
			//====================================================
			tgcpmsi02["KEYVALUE_2"] = tgcpmsi02["MOID"].ToString() + "_" + tgcpmsi02["TABLE_NAME"].ToString();
			tgcpmsi02["KEYVALUE_3"] = tgcpmsi02["MOID"].ToString() + "_" + tgcpmsi02["TABLE_NAME"].ToString() + "_WHERE";
			tgcpmsi02["KEYVALUE_4"] = tgcpmsi02["MOID"].ToString() + "_" + tgcpmsi02["TABLE_NAME"].ToString() + "_ONE";
			if (tgcpmsi02["MODE_NO"].ToString().Trim().Substring(0,2) == "01")
			{//多表单画面模式。
				tgcpmsi02["KEYVALUE_3"] = "";
				tgcpmsi02["KEYVALUE_4"] = "";
			}
			else if (tgcpmsi02["MODE_NO"].ToString().Trim() == "05G")
			{//多表单画面分组,多用于综合查询的分组。
				tgcpmsi02["KEYVALUE_3"] = "";
				tgcpmsi02["KEYVALUE_4"] = "";
			}
			else if (tgcpmsi02["MODE_NO"].ToString().Trim().Substring(0, 2) == "02")
			{//2个 GRID 模式。
				tgcpmsi02["KEYVALUE_4"] = "";
			}
			else
			{//其他模式下， 暂不处理。
				Log::Trace("", __FUNCTION__, "其他画面模式[{0}]，不处理 ED54功能号", tgcpmsi02["MODE_NO"].ToString());
			}


			//PAGE_NUM==每页行数
			//===========
			tgcpmsi02["PAGE_NUM"] = 0;
			if (tgcpmsi02["MODE_NO"].ToString().Trim() == "02M")
			{//若当前是‘分页模式’，则给出单页记录数。
				tgcpmsi02["PAGE_NUM"] = 200;
			}


			//CString MOID;   //模块号 = 二级模块 = 业务表名称的第2~5位
			//CString SEQ_CODE;   //显示序号代码 = 二级模块_ZZ
			//CString FORM_CODE;   //画面编号 = 若画面模式=01=多表单记录，则， 画面编号 = 二级模块SIC=静态表集合画面
			//CString KEYVALUE_1;   //关键字串1 =允许操作者权限
			//CString KEYVALUE_2;   //关键字串2 =ED54功能号_查询条件
			//CString KEYVALUE_3;   //关键字串3 =ED54功能号_多记录显示
			//CString KEYVALUE_4;   //关键字串4 =ED54功能号_单记录显示

			tgcpmsi02.TrimOrBlank();
			c_sql_condition = "update tgcpmsi02 t "
				" set   t.moid       = @moid "
				"      ,t.seq_code   = @seq_code "
				"      ,t.form_code  = @form_code "
				"      ,t.keyvalue_1 = @keyvalue_1 "
				"      ,t.keyvalue_2 = @keyvalue_2 "
				"      ,t.keyvalue_3 = @keyvalue_3 "
				"      ,t.keyvalue_4 = @keyvalue_4 "
				"      ,t.page_num   = @page_num "
				" where t.table_name  = @table_name "
				" and   t.seq_no     = @seq_no "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("moid", tgcpmsi02["MOID"].ToString());
			cmd_sql.Parameters.Set("seq_code", tgcpmsi02["SEQ_CODE"].ToString());
			cmd_sql.Parameters.Set("form_code", tgcpmsi02["FORM_CODE"].ToString());
			cmd_sql.Parameters.Set("keyvalue_1", tgcpmsi02["KEYVALUE_1"].ToString());
			cmd_sql.Parameters.Set("keyvalue_2", tgcpmsi02["KEYVALUE_2"].ToString());
			cmd_sql.Parameters.Set("keyvalue_3", tgcpmsi02["KEYVALUE_3"].ToString());
			cmd_sql.Parameters.Set("keyvalue_4", tgcpmsi02["KEYVALUE_4"].ToString());
			cmd_sql.Parameters.Set("page_num", tgcpmsi02["PAGE_NUM"].ToDecimal());//单页记录数
			//-------------
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
			//v_error_remark2 = "表名称[" + tgcpmsi02["TABLE_NAME"].ToString() + "]序号[" + tgcpmsi02["SEQ_NO"].ToDecimal().ToString() + "]";


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







