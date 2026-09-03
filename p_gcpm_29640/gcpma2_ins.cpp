/*========================================================================*/
/*== [service名  ]:  gcpma2_ins       ||  [对应VC#画面 ]:  ALL          ==*/
/*== [程序编制人 ]:  张颖             ||  [程序定稿日期]:2015-7-13 16:58:01==*/
/*== [程序修改人 ]：                  ||  [程序修改日期]:               ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMA2                                            ==*/
/*== [调用函数   ]： 无				                                          ==*/
/*== [service功能]： 工程项目需求信息_新增                                    ==*/
/*========================================================================*/

/******框架头******/
#include "stdafx.h"

/******业务头******/ 
#include "tgcpma2.h"  

#define zero 0.00001


/******定义调用的函数******/
 

 

/******service入口******/
BM2F_ENTERACE(gcpma2_ins)

int f_gcpma2_ins(EIClass * bcls_rec, EIClass * bcls_ret,CDbConnection * conn)
{    
	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpma2_ins";                //定义函数英文名称  
	CString FunctionCname = "工程项目需求信息_新增";              //定义函数中文名称


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
		CTGCPMA2 tgcpma2(conn); 


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

		for(i = 0;i<v_total_num;i++)
		{
			/****** 获取前台传入的材料信息 ******/
			//v_mat_no = bcls_rec->Tables[0].Rows[num]["MAT_NO"].ToString().Trim();
			tgcpma2.MergeFrom(bcls_rec->Tables[0].Rows[i]);

			
			if (tgcpma2.PROJECT_NO.Trim() == "")
			{
				sprintf(s.msg,"[项目号]不允许为空。");
				throw CApplicationException(-1, s.msg, log.Location);
			}

			//EDIT_TIME
			if (tgcpma2.EDIT_TIME.Trim() == "")
			{//若需求提出日期为空，则为当前日期。
				tgcpma2.EDIT_TIME = systime.Substring(0, 8); //yyyyMMdd
			}

			//当前表中，获取对应最大流水号。
			//===================
			v_seq_no = 0;
			c_sql_condition = " select NVL(max(t.SEQ_NO),0) + 1 from tgcpma2 t "
				//" where t.project_no = @project_no "
				;
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.Parameters.Set("project_no", tgcpma2.PROJECT_NO);
			v_seq_no = cmd_sql.ExecuteScalar();
			cmd_sql.Close(); 


			/*
			CString CONFM_PLAN_NO;   //准发计划号[10]=执行日期[8]+流水[2]=
			CString CONFM_STATUS;    //准发计划状态=1=创建
			*/
			tgcpma2.SEQ_NO = v_seq_no;
			if (tgcpma2.REQU_STATUS.Trim() == "")
			{//若前台没给，就初始化。
				tgcpma2.REQU_STATUS = "00"; //需求提出。 
			}

			//根据需求状态代码，修正对应的‘状态描述’==[GCPV]
			//===================
			/*
			select T.CODE,t.code_desc_1_content,t.* from tep0002 t
			where t.code_class = 'GCPV'
			*/
			v_seq_no = 0;
			c_sql_condition = " select t.code_desc_1_content from tep0002 t "
				" where t.code_class = 'GCPV' "
				" and   t.code       = @code "
				;
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.Parameters.Set("code", tgcpma2.REQU_STATUS);
			cmd_sql.ExecuteReader();
			if (cmd_sql.Read())
			{
				tgcpma2.STATUS_DESC = cmd_sql.GetString(1);
			}
			cmd_sql.Close();

			


			//记录材料 
			//=================  
			tgcpma2.REC_CREATOR = userid;      //记录创建责任者
			tgcpma2.REC_CREATE_TIME = systime; //记录创建时刻 
			

			 
			tgcpma2.TrimOrBlank();
			sqlstr = "insert into tgcpma2，项目需求[" + tgcpma2.REMARK_DESC + "]";
			tgcpma2.Insert(); 


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
