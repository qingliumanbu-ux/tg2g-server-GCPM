/*============================================================================*/
/*== [service名  ]:  gcpma4_chk       ||  [对应VC#画面 ]:  ALL              ==*/
/*== [程序编制人 ]:  张颖             ||  [程序定稿日期]:2015-12-15 13:26:31==*/
/*== [程序修改人 ]：                  ||  [程序修改日期]:               ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMA4                                            ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： 同名表信息差异化分析                               ==*/
/*========================================================================*/

/******框架头******/
#include "stdafx.h"

/******业务头******/ 
#include "tgcpma4.h"  

#define zero 0.00001


/******定义调用的函数******/
 
 

 

/******service入口******/
BM2F_ENTERACE(gcpma4_chk)

int f_gcpma4_chk(EIClass * bcls_rec, EIClass * bcls_ret,CDbConnection * conn)
{    
	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpma4_chk";                //定义函数英文名称  
	CString FunctionCname = "同名表信息差异化分析";              //定义函数中文名称


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
		CTGCPMA4 tgcpma4(conn); 


		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where     = "  WHERE   1 = 1 "; //查询条件。
		CString c_sql_condition  =  " SELECT  t.* FROM tpmof03 t  where t.order_no = @order_no" ;



		/* 数据库操作类定义 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString  c_sql_where2     = "  WHERE   1 = 1 "; //查询条件。
		CString c_sql_condition2  =  " SELECT  t.* FROM tpmof03 t  where t.order_no = @order_no" ;
		 
		CDecimal v_seq_no = 0; //计划中的序号。
		/****** 获得输入参数 ******/ 
		int v_total_num = bcls_rec->Tables[0].Rows.get_Count();
		Log::Trace("", __FUNCTION__, "v_total_num =[{0}]  ", v_total_num);

		if (v_total_num <= 0)
		{
			sprintf(s.msg, "没有可操作的记录。");
			throw CApplicationException(-1, s.msg, log.Location);
		}

		//只取选中行的第一行的表名称，作为比较的要求。
		//========================== 
		for(i = 0;i<v_total_num;i++)
		{
			/****** 获取前台传入的材料信息 ******/
			if (bcls_rec->Tables[0].Columns.Contains("TABLE_ENAME"))
				tgcpma4.TABLE_ENAME = bcls_rec->Tables[0].Rows[i]["TABLE_ENAME"];

			Log::Trace("", __FUNCTION__, "TABLE_ENAME =[{0}]  ", tgcpma4.TABLE_ENAME);
			 
			if (tgcpma4.TABLE_ENAME.Trim() == "")
			{
				sprintf(s.msg,"[表名称]不允许为空。");
				throw CApplicationException(-1, s.msg, log.Location);
			}

			//根据表名称，清空 REMARK .
			c_sql_condition = " update tgcpma4 t "
				" set   t.REMARK = ' ' "
				" where t.table_ename = @table_ename "
				;
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.Parameters.Set("table_ename", tgcpma4.TABLE_ENAME);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();




			//1.=找0#项目 [9672]  有， 1#项目 [MMS011]没有。
			//=========================
			//2.=找1#项目 [MMS011]有， 0#项目 [9672]  没有。
			//=========================

			/*
			CDecimal VIEW_POS;   //项目序= 0/1=左/右=9672/MMS011
	CString VIEW_ENAME;   //视图英文名称
	CString TABLE_ENAME;   //系统数据结构表英文名
	CDecimal TABLE_ITEM_SEQ;   //表内数据项序号
	CString ITEM_ENAME;   //字段英文名
	CString ITEM_CNAME;   //字段中文名
	CString ITEM_TYPE;   //字段类型
	CString ITEM_LEN;   //字段长度
	CString TABLE_INDEX_TYPE;   //数据库表索引类型
			
			*/
			 


			//1.=找0#项目 [9672]  有， 1#项目 [MMS011]没有。
			//=========================
			c_sql_condition =  " update tgcpma4 t "
				" set   t.remark = '[0]' " //只有0#项目有。
				" where t.view_pos      = 0 "  //0#项目= 左项目序号
				" and   t.table_ename   = @table_ename " //表英文
				" and   t.item_ename not in ( select aa.item_ename from tgcpma4 aa "
				"                             where  aa.view_pos = 1                "  //1#项目= 右项目序号
				"                             and    aa.table_ename = t.table_ename )" 
				;
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.Parameters.Set("table_ename", tgcpma4.TABLE_ENAME);
			cmd_sql.ExecuteNonQuery(); 
			cmd_sql.Close(); 

			//2.=找1#项目 [MMS011]有， 0#项目 [9672]  没有。
			//=========================
			c_sql_condition = " update tgcpma4 t "
				" set   t.remark        = '[1]' " //只有1#项目有。 
				" where t.view_pos      = 1 "     //1#项目= 右项目序号
				" and   t.table_ename   = @table_ename " //表英文
				" and   t.item_ename not in ( select aa.item_ename from tgcpma4 aa "
				"                             where  aa.view_pos = 0                "  //0#项目= 左项目序号
				"                             and    aa.table_ename = t.table_ename )"
			    ;
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.Parameters.Set("code", tgcpma4.TABLE_ENAME);
			cmd_sql.ExecuteNonQuery(); 
			cmd_sql.Close(); 

		}



		  
 
		 

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
