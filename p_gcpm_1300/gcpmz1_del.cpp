/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2014-10-22 12:58:03
功能: 功能清单明细_删除
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h"



/*<remark>=========================================================
/// <summary>
/// 功能清单明细_删除
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz1_del)

int f_gcpmz1_del(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz1_del";                //定义函数英文名称  
	CString FunctionCname = "功能清单明细_删除";              //定义函数中文名称
	//LogTrace(1, 1, " **************%s begin*****************", (const char*)FunctionEname);


	//程序用变量
	int   i = 0;
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;  
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0;
	CString  v_dllname = "";


	try
	{



		 
	CModel tgcpmz1("TGCPMZ1");
		
		
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //删除条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " ORDER BY t.XX ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //删除条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		 
		 
		 
		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{

			tgcpmz1.MergeFrom(bcls_rec->Tables[0].Rows[i]); //前台传入的数据。


			v_dllname = tgcpmz1["DLLNAME"];
			/*if (v_dllname.GetLength() >= 4)
			{
				v_dllname = v_dllname.Substring(0, 4);
			}*/

			//根据DLL ，进行对应责任者的校验，确认有权限，才允许操作。
			//=============================
			/*
			select t.code,t.code_desc_2_content,t.* from tgcpmsi00 t
			where t.code_class = 'GCPP';
			*/
			//根据 DLL ,当前用户的权限校验。 
			v_cnt = 0;
			c_sql_condition = "select count(1) from tgcpmsi00 t "
				" where t.code_class   ='GCPP' "
				" and   t.VALID_FLAG = '1' "//1=生效。
				" and   t.code = @code " //指定的DLL
				" and    ( t.code_desc_2_content || ','  LIKE  '%' || @code_desc_2_content || ',%' or t.code_desc_2_content = ' ' )   "  //在[责任者]要求范围内。
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("code", v_dllname);//指定的DLL
			cmd_sql.Parameters.Set("code_desc_2_content", userid);//责任者。 
			cmd_sql.SetCommandText(c_sql_condition);
			v_cnt = cmd_sql.ExecuteScalar();
			cmd_sql.Close();

			if (v_cnt <= 0 && userid != "178029")
			{//若没有找到记录，则说明当前用户，不能进行当前DLL 的操作。

				sprintf(s.msg, "您的帐号[%s]没有DLL[%s]的操作权限，当前操作失败。"
					, (const char*)userid, (const char*)v_dllname);
				throw CApplicationException(-1, s.msg, log.Location);
			}
			 


			//////删除 
			//////==========
			////CString TRACK_SEQ_NO;   //事件跟踪序列号 ==K 

			////删除前，先写入历史表。
			////==================
			//c_sql_condition = "insert into hgcpmz1 select t.*  from tgcpmz1 t "
			//	" where t.track_seq_no      = @track_seq_no "
			//	;
			//sqlstr = c_sql_condition;
			//cmd_sql.Parameters.Set("track_seq_no", tgcpmz1["TRACK_SEQ_NO"].ToString());//跟踪号。 
			//cmd_sql.SetCommandText(c_sql_condition);
			//cmd_sql.ExecuteNonQuery();
			//cmd_sql.Close();


			 
			 //再删除。
			c_sql_condition = "delete  from tgcpmz1 t "
				" where t.track_seq_no      = @track_seq_no " 
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("track_seq_no", tgcpmz1["TRACK_SEQ_NO"].ToString());//跟踪号。 
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

	//此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;


}


