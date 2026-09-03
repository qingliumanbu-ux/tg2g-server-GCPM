/*=============================================================================*/
/*== [service名  ]:  gcpma4_ins       ||  [对应VC#画面 ]:  ALL               ==*/
/*== [程序编制人 ]:  张颖             ||  [程序定稿日期]:2015-12-16 13:02:00 ==*/
/*== [程序修改人 ]：                  ||  [程序修改日期]:                    ==*/
/*=============================================================================*/
/*== [数据库表   ]： TGCPMA4                                              ==*/
/*== [调用函数   ]： 无				                                            ==*/
/*== [service功能]： 本地表结构导入到分析表                               ==*/
/*==========================================================================*/

/******框架头******/
#include "stdafx.h"

/******业务头******/ 
  

 


/******定义调用的函数******/
 
 

 

/******service入口******/
BM2F_ENTERACE(gcpma4_ins)

int f_gcpma4_ins(EIClass * bcls_rec, EIClass * bcls_ret,CDbConnection * conn)
{    
	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpma4_ins";                //定义函数英文名称  
	CString FunctionCname = "本地表结构导入到分析表";              //定义函数中文名称


	////EDLog(1, 1, " **************%s begin*****************", (const char*)FunctionEname);
	CTracer log(__FUNCTION__);  // 系统日志，必须在代码段开始处定义

	/*定义程序用变量*/
	int doFlag = 0;
	int i = 0;
	int num  = 0;
	int v_blk_num = 0;

	CString  systime = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;

	CString  function_id = "XX" ;  //自定义显示项目功能号    
	CString  sqlstr = "";

	CString v_confm_plan_no = "";
	CString v_mat_kind = "";

	CString v_bill_no  = ""; 
	CString v_rec_revisor = "";
	CString v_rec_revise_time  = ""; 

	CString v_bill_revoke_reason = "xx";
	CString v_reje_reas_code = "";
	CString v_mat_no = "";


	CString v_table_name     = "xx";
	CString v_update = "";  //修改的字段信息
	CString v_condi  = "";  //过滤的字段信息。
	CDecimal v_total_num = 0;
	CDecimal v_cnt = 0;

	///*定义函数调用块*/
	//EIClass MMTEMP;/*物料跟踪自定义抛账规则*/






	/****** 业务处理开始 ******/
	try
	{
		 
		/* 实体类定义 */ 
	CModel tgcpma4("TGCPMA4");


		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where     = "  WHERE   1 = 1 "; //查询条件。
		CString c_sql_condition  =  " SELECT  t.* FROM tpmof03 t  where t.order_no = @order_no" ;



		/* 数据库操作类定义 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString  c_sql_where2     = "  WHERE   1 = 1 "; //查询条件。
		CString c_sql_condition2  =  " SELECT  t.* FROM tpmof03 t  where t.order_no = @order_no" ;
		 
		CDecimal v_seq_no = 0; //计划中的序号。


		/*
		--MTA00
		--MOM00
		--TOM00
		SELECT A.* FROM TTADT01 A
		WHERE A.TABLE_ENAME = 'MTA00'
		ORDER BY  A.TABLE_ITEM_SEQ;

		SELECT A.* FROM TTADT01 A
		WHERE A.TABLE_ENAME = 'MOM00'
		ORDER BY  A.TABLE_ITEM_SEQ;

		SELECT A.* FROM TTADT01 A
		WHERE A.TABLE_ENAME = 'TOM00'
		ORDER BY  A.TABLE_ITEM_SEQ;




		--新增的 表+字段的 --采用=模块表
		--====================
		SELECT A.* FROM TTADT01 A
		WHERE A.TABLE_ENAME IN (select t.CODE  from tgcpmsi00 t
		where t.code_class = 'GCP5'
		AND   T.CODE_DESC_2_CONTENT = '1')
		and   A.TABLE_ITEM_TYPE = '1' --使用模版表。
		ORDER BY A.TABLE_ENAME,A.TABLE_ITEM_SEQ


		SELECT aa.table_ename,aa.model_table_ename
		FROM TTADT01 aa
		WHERE aa.TABLE_ENAME IN (select bb.CODE  from tgcpmsi00 bb
		where bb.code_class = 'GCP5'
		AND   bb.CODE_DESC_2_CONTENT = '1')
		and   aa.TABLE_ITEM_TYPE = '1' --使用模版表。
		ORDER BY aa.table_ename,aa.model_table_ename
		;



		--新增 表+字段的关系. --采用=非模版表
		--=====================
		SELECT A.TABLE_ENAME,A.TABLE_ITEM_SEQ
		,B.ITEM_ENAME,B.ITEM_CNAME,B.ITEM_TYPE,B.ITEM_LEN
		--,B.*
		FROM TTADT01 A,TTADI00 B
		WHERE A.TABLE_ENAME = 'TOM00'
		AND   A.ITEM_SEQ = B.ITEM_SEQ
		ORDER BY A.TABLE_ITEM_SEQ
		;

		*/ 
		CString v_table_ename_tta = "";
		CString v_model_table_ename = "";
		CDecimal v_table_item_seq = 0;


		//新增校验，若业务表数据已经存在，则提示先删除，再导入。
		//=====================
		c_sql_condition = "		SELECT aa.table_ename  " 
			"  FROM  TTADT00 aa                                             "
			"  WHERE aa.TABLE_ENAME IN(select bb.code  from tgcpmsi00 bb     "
			"                          where bb.code_class = 'GCP5'        "
			"                          and   bb.code_desc_2_content = '1') " //1=需要导入。 
			"  ORDER BY aa.table_ename " //ORDER BY 表名称 
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);
		//cmd_sql.Parameters.Set("code", tgcpma4["TABLE_ENAME"].ToString());
		cmd_sql.ExecuteReader();
		while (cmd_sql.Read())
		{
			v_table_ename_tta = cmd_sql.GetString(1); 

			Log::Trace("", __FUNCTION__, "v_table_ename_tta[{0}]模版表名[{1}]"
				, v_table_ename_tta, v_model_table_ename);

			//新增校验，若业务表数据已经存在，则提示先删除，再导入。
			//=====================
			v_cnt = 0;
			c_sql_condition2 = "select count(1) from tgcpma4 t "
				" where t.table_ename = @table_ename "
				" and   t.view_pos    = 1 "//必须是本系统的表结构，
				;
			sqlstr = c_sql_condition2;
			cmd_sql2.SetCommandText(c_sql_condition2);
			cmd_sql2.Parameters.Set("table_ename", v_table_ename_tta);//业务表名称
			v_cnt = cmd_sql2.ExecuteScalar();
			cmd_sql2.Close();
			if (v_cnt >= 1)
			{
				sprintf(s.msg, "1#项目，业务表[%s]结构信息已经存在，请先删除，再进行当前操作。"
					, (const char*)v_table_ename_tta);
				throw CApplicationException(-1, s.msg, log.Location);
			}
		}
		cmd_sql.Close();

	
		//先导入模版表部分。
		//=================
		c_sql_condition = "		SELECT aa.table_ename,aa.model_table_ename "
			"  ,aa.table_item_seq  "
			" FROM TTADT01 aa                                             "
			"  WHERE aa.TABLE_ENAME IN(select bb.code  from tgcpmsi00 bb     "
		    "                          where bb.code_class = 'GCP5'        "
			"                          and   bb.code_desc_2_content = '1') " //1=需要导入。
			"  and   aa.TABLE_ITEM_TYPE = '1'             " //--使用模版表。
			"  ORDER BY aa.table_ename, aa.table_item_seq " //ORDER BY 表名称+表内序号。
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);
		//cmd_sql.Parameters.Set("code", tgcpma4["TABLE_ENAME"].ToString());
		cmd_sql.ExecuteReader();
		while (cmd_sql.Read())
		{
			v_table_ename_tta = cmd_sql.GetString(1);
			v_model_table_ename = cmd_sql.GetString(2);
			v_table_item_seq = cmd_sql.GetDecimal(3);

			Log::Trace("", __FUNCTION__, "v_table_ename_tta[{0}]模版表名[{1}]"
				, v_table_ename_tta, v_model_table_ename); 

			/*
			CDecimal GROUP_NO;   //组号
			CDecimal GROUP_SEQ_NO;   //组内序号
			*/
			c_sql_condition2 = " insert into tgcpma4 (VIEW_POS,TABLE_ENAME"
				",GROUP_SEQ_NO,COMPANY_NAME" //表内字段顺序，模版表名称[借用字段公司名称]
				",ITEM_ENAME,ITEM_CNAME,ITEM_TYPE,ITEM_LEN "
				",TABLE_INDEX_TYPE,REMARK,GROUP_NO)" //借用字段存放表内组号：GROUP_NO ，用于模版表的顺序控制。
				" select 1,@table_ename"
				"         ,aa.table_item_seq,@table_ename2 " //本表就是模版表
				"         ,bb.item_ename,bb.item_cname,bb.item_type,bb.item_len "
				"         ,' ',' ',@view_ename "
				" FROM TTADT01 aa,TTADI00 bb "
				" WHERE aa.table_ename = @table_ename2 "
				" AND   aa.item_seq    = bb.item_seq  "
				;
			sqlstr = c_sql_condition2;
			cmd_sql2.SetCommandText(c_sql_condition2);
			cmd_sql2.Parameters.Set("table_ename", v_table_ename_tta);//业务表名称
			cmd_sql2.Parameters.Set("table_ename2", v_model_table_ename);//模版表名称
			cmd_sql2.Parameters.Set("view_ename", v_table_item_seq);//借用字段view_ename，存放模版表组号。
			cmd_sql2.ExecuteNonQuery();
			cmd_sql2.Close();

		}
		cmd_sql.Close();


		//再导入【非】模版表部分。
		//=================
		c_sql_condition2 = " insert into tgcpma4 (VIEW_POS,TABLE_ENAME"
			",GROUP_SEQ_NO,COMPANY_NAME" //表内字段顺序，模版表名称[借用字段公司名称]
			",ITEM_ENAME,ITEM_CNAME,ITEM_TYPE,ITEM_LEN "
			",TABLE_INDEX_TYPE,REMARK,GROUP_NO)"
			" select 1,aa.table_ename "
			"         ,aa.table_item_seq,aa.model_table_ename "
			"         ,bb.item_ename,bb.item_cname,bb.item_type,bb.item_len "
			"         ,' ',' ',9999 " //最大的4位数字。
			" FROM TTADT01 aa,TTADI00 bb "
			" WHERE aa.table_ename IN(select aaa.code  from tgcpmsi00 aaa     "
		    "                          where aaa.code_class = 'GCP5'        "
			"                          and   aaa.code_desc_2_content = '1') " //1=需要导入。
			" AND   aa.TABLE_ITEM_TYPE  <> '1' "//非引用模块表模式。
			" AND   aa.item_seq    = bb.item_seq  "
			;
		sqlstr = c_sql_condition2;
		cmd_sql2.SetCommandText(c_sql_condition2);
		//cmd_sql2.Parameters.Set("table_ename", v_table_ename_tta);//业务表名称 
		cmd_sql2.ExecuteNonQuery();
		cmd_sql2.Close();
		 

	 
		  
 
		 

		/*处理成功。*/
		strcpy(s.msg,"恭喜，处理成功。");	

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
