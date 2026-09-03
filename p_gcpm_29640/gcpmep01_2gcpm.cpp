/*============================================================================*/
/*== [service名  ]:  gcpmep01_2gcpm        ||  [对应VC#画面 ]:GCPMEP01           ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2021/10/3 15:39:38==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMEP01                                            ==*/
/*== [调用函数   ]： 无				                                            ==*/
/*== [service功能]： 框架值集同步到GCPM值集                               ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h"


//SERVICE入口
BM2F_ENTERACE(gcpmep01_2gcpm);

int f_gcpmep01_2gcpm(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection * conn)
{
	CTracer log(__FUNCTION__);

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmep01_2gcpm";                //定义函数英文名称  
	CString FunctionCname = "框架值集同步到GCPM值集";              //定义函数中文名称

	/* 程序内部变量 */
	int i = 0;
	int doFlag = 0;
	int v_row_num = 0; //行个数。
	CDecimal v_cnt = 0;
	CString v_blk_name = "XX";

	/* 数据库SQL操作字符串*/
	CString  sqlstr = "";
	try
	{
	CModel tgcpmep01("TGCPMEP01");

		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString  c_sql_orderBy = "";



		//获得系统时间，当前用户代码
		CString systime = CDateTime::Now().ToString("yyyyMMddHHmmss");
		CString userid = s.userid;




 
			v_row_num = bcls_rec->Tables[0].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "in v_blk_name[{0}]v_row_num[{1}]  ", v_blk_name, v_row_num);
			for (i = 0; i < v_row_num; i++)
			{
				//   循环获取输入参数
				//======================
				tgcpmep01.MergeFrom(bcls_rec->Tables[0].Rows[i]);
				tgcpmep01.TrimOrBlank();
				Log::Trace("", __FUNCTION__, "in 第[{0}]个代码编号[{1}]"
					, i+1, tgcpmep01["CODE_CLASS"].ToString());
				//==================================================== 
				//   输入参数校验
				//====================================================
				if (tgcpmep01["CODE_CLASS"].ToString().Trim() == "")
				{
					sprintf(s.msg, "[代码编号]不允许为空。");
					throw CApplicationException(-1, s.msg, log.Location);
				}
				 

				v_cnt = 0;
				c_sql_condition = "select count(1) from tgcpmsi00 t "
					" where t.code_class   = @code_class " //代码编号
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", tgcpmep01["CODE_CLASS"].ToString());//代码编号。 
				cmd_sql.SetCommandText(c_sql_condition);
				v_cnt = cmd_sql.ExecuteScalar();
				cmd_sql.Close();

				if (v_cnt >= 1)
				{//若已经存在，则不允许同步操作。
					sprintf(s.msg, "代码编号[%s]已经存在，不需要同步。\n当前操作失败。"
						, (const char*)tgcpmep01["CODE_CLASS"].ToString());
					throw CApplicationException(-1, s.msg, log.Location);
				}

				//==============
				//新增表， tgcpmsi00.
				//============== 
				/*
				--将EP代码查询出结果。
select t.code_class,t.code_name
,'1' as VALID_FLAG 
,aa.code
,aa.code_desc_1_content,aa.code_desc_2_content
,aa.code_desc_3_content,aa.code_desc_4_content
,aa.code_desc_5_content 
,' ' as remark 
,0 as show_seq 
--,aa.*   
from tep0001 t, tep0002 aa 
where t.code_class = aa.code_class 

and (t.code_class like 'GC%' or t.code_class like 'OM%' or t.code_class like 'PM%')
				*/  
				tgcpmep01.TrimOrBlank(); 
				c_sql_condition = "insert into tgcpmsi00 ( "
					"  REC_CREATOR,REC_CREATE_TIME " //创建者，创建时刻。
					" ,CODE_CLASS,CODE_NAME" //代码编号,代码名称
					" ,VALID_FLAG " //有效标志
					" ,CODE "//有效标志,CODE 
					" ,code_desc_1_content,code_desc_2_content"
					" ,code_desc_3_content,code_desc_4_content"
					" ,code_desc_5_content" 	
					") "
					" select @rec_creator,@rec_create_time"
					",t.code_class,t.code_name"
					",'1' "
					",aa.code"
					",aa.code_desc_1_content,aa.code_desc_2_content"
					",aa.code_desc_3_content,aa.code_desc_4_content"
					",aa.code_desc_5_content" 
					" from tep0001 t, tep0002 aa  "
					" where t.code_class = aa.code_class "
					" and   t.code_class = @code_class "
					;

				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("rec_creator", userid);//创建者
				cmd_sql.Parameters.Set("rec_create_time", systime);//创建时刻。
				//====where
				cmd_sql.Parameters.Set("code_class", tgcpmep01["CODE_CLASS"].ToString());//代码编号。 
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close();

				Log::Trace("", __FUNCTION__, "insert into tgcpmsi00,代码编号[{0}]"
					, tgcpmep01["CODE_CLASS"].ToString());

				 


			} 

		 

 
		

		//处理成功。 
		strcpy(s.msg, "恭喜，处理成功。");


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

	//此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;

}
