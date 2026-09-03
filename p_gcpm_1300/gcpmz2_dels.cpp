/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2016-4-1 11:05:02
功能: 后台程序[配置+关联编译]信息彻底删除
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h" 


/*<remark>=========================================================
/// <summary>
/// 后台程序[配置+关联编译]信息彻底删除
/// <para>
///    /// TEA01,TEA03,===后台程序基本信息表。
        /// TEA05,TEA07==关联编译信息表。
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz2_dels)

int f_gcpmz2_dels(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz2_dels";                //定义函数英文名称  
	CString FunctionCname = "后台程序[配置+关联编译]信息删除";              //定义函数中文名称
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
	CString  v_svc_name = "";


	try
	{

 
		
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //批量修改条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //批量修改条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		
		 
		 
		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{ 
			if (bcls_rec->Tables[0].Columns.Contains("SVC_NAME"))
				v_svc_name = bcls_rec->Tables[0].Rows[i]["SVC_NAME"];

			Log::Trace("", __FUNCTION__, "v_svc_name =[{0}]  " 	, v_svc_name );

			if (v_svc_name.Trim() == "")
			{//程序名称，不允许为空。
				sprintf(s.msg, "[后台程序名称]不允许为空，当前操作失败。");
				throw CApplicationException(-1, s.msg, log.Location);
			}
 
			if (userid.Trim() != "178029")
			{//若不是指定用户，则不允许修改，功能的覆盖产线。

				sprintf(s.msg, "您的帐号[%s]没有权限，当前操作失败。"
					, (const char*)userid);
				throw CApplicationException(-1, s.msg, log.Location);
			}
			 
			/*
			TEA01, TEA03, == = 后台程序配置信息表。
			TEA05, TEA07 == 关联编译信息表。
			delete from tea05 where svc_name='pmol45_del_m'
			delete from tea07 where svc_name='pmol45_del_m';
			delete from tea07 where svc_name_called='pmol45_del_m'

			*/

		 

			//彻底删除。==TEA01
			//==========  
			c_sql_condition = "delete from  tea01 t " 
				" where t.svc_name       = @svc_name "  //后台服务名称
				;
			sqlstr = c_sql_condition; 
			cmd_sql.Parameters.Set("svc_name", v_svc_name);//后台服务名称。 
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close(); 

			//彻底删除。==TEA03
			//==========  
			c_sql_condition = "delete from  tea03 t "
				" where t.svc_name       = @svc_name "  //后台服务名称
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("svc_name", v_svc_name);//后台服务名称。 
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();

			//彻底删除。==TEA05
			//==========  
			c_sql_condition = "delete from  tea05 t "
				" where t.svc_name       = @svc_name "  //后台服务名称
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("svc_name", v_svc_name);//后台服务名称。 
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();

			//彻底删除。==TEA07
			//==========  
			c_sql_condition = "delete from  tea07 t "
				" where t.svc_name       = @svc_name "  //后台服务名称
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("svc_name", v_svc_name);//后台服务名称。 
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();

			//delete from tea07 where svc_name_called='pmol45_del_m'
			//==========================
			c_sql_condition = "delete from  tea07 t "
				" where t.svc_name_called       = @svc_name_called "  //后台服务名称
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("svc_name_called", v_svc_name);//后台服务名称。 
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


