/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2016-7-25 15:29:42
功能: 生产合同业务流水号表[TGCPM21]_新增
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h"
#include "tgcpm21.h"


/*<remark>=========================================================
/// <summary>
///  生产合同业务流水号表[TGCPM21]_新增
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/

// service入口
BM2F_ENTERACE(gcpm21_ins)
//-EP_SYSTEM_HEAD_END                                                  
int f_gcpm21_ins(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);  // 系统日志，必须在代码段开始处定义
	
	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpm21_ins";                     //定义函数英文名称  
	CString FunctionCname = "生产合同业务流水号表[TGCPM21]_新增"; //定义函数中文名称 


	//程序用变量
	int   doFlag = 0;
	int   i = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  v_userid = s.userid;
	CString  function_id = "pmop10_ins" ;  //自定义显示项目功能号   
	CDecimal v_cnt = 0;
	CString  sqlstr = "";  //SQL 信息。
 
	CString v_code_class = "";
	CString v_code_name  = ""; 
	CString v_condi      = "";
	CString v_aa = "";
	double   v_daydiff = 0;	                 //日期差。 
	int v_rule_seq_no = 0;

	

	

	try
	{
		CTGCPM21 tgcpm21(conn);
		CTGCPM21 tgcpm21_chk(conn);


		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where     = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;



		/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString  c_sql_where2 = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition2 = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";





		for(i = 0; i < bcls_rec->Tables[0].Rows.get_Count();i++ )
		{
			tgcpm21.MergeFrom(bcls_rec->Tables[0].Rows[i]);
			tgcpm21.TrimOrBlank();

			//CString SEQ_NAME;   //序号名称
			//CString SEQ_DESC;   //序号描述
			//CDecimal SEQ_BEGIN;   //起始序号
			//CDecimal SEQ_NOW;   //当前序号
			//CDecimal SEQ_END;   //终了序号
			//CString SEQ_PRE;   //序号前缀
			//CDecimal SEQ_LEN;   //序号长度
			//CString SEQ_RECYCLE_FLAG;   //流水号复位标记

			Log::Trace("", __FUNCTION__, "in SEQ_NAME= {0}", tgcpm21.SEQ_NAME);
			Log::Trace("", __FUNCTION__, "in SEQ_END= {0}", tgcpm21.SEQ_END);

			if (tgcpm21.SEQ_NAME.Trim() == "")
			{//流水号名称，不允许为空
				sprintf(s.msg, "序号名称，不允许为空");
				throw CApplicationException(-1, s.msg, log.Location);
			}

			if (tgcpm21.SEQ_END <= 0)
			{//最大序号，不允许小于等于0
				sprintf(s.msg, "终了序号，不允许小于等于0");
				throw CApplicationException(-1, s.msg, log.Location);
			}

			if (tgcpm21.SEQ_BEGIN <= 0)
			{
				tgcpm21.SEQ_BEGIN = 0;
			}

			if (tgcpm21.SEQ_NOW <= 0)
			{
				tgcpm21.SEQ_NOW = 0;
			}
			//SEQ_RECYCLE_FLAG
			if (tgcpm21.SEQ_RECYCLE_FLAG.Trim() == "")
			{
				sprintf(s.msg, "复位标志，不允许为空");
				throw CApplicationException(-1, s.msg, log.Location);
			}

			

			//以上1个是业务主键，不能重复。
			//===============
			v_cnt = 0;
			c_sql_condition = " SELECT COUNT(1) "
				" FROM   tgcpm21   t         "
				" WHERE  t.seq_name  = @seq_name  " 
				;
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.Parameters.Set("seq_name", tgcpm21.SEQ_NAME); 
			v_cnt = cmd_sql.ExecuteScalar();
			cmd_sql.Close();
			if (v_cnt >= 1)
			{//若业务主键已经存在，则报错提醒。
				sprintf(s.msg,"流水号[%s]对应的信息已经存在，不用新增。"
					, (const char*)tgcpm21.SEQ_NAME);
				throw CApplicationException(-1, s.msg, log.Location); 
			}


			//若校验通过了，则计算表的系统主键 SEQ_NO，进行新增
			//=============== 
			//一定要有修改时刻。
			tgcpm21.REC_CREATE_TIME = dateNow;
			tgcpm21.REC_CREATOR = v_userid;
			tgcpm21.REC_REVISE_TIME = dateNow; 

			tgcpm21.TrimOrBlank();
			sqlstr = "insert into tgcpm21 ,seq_name is[" + tgcpm21.SEQ_NAME + "]";
			tgcpm21.Insert(); 

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