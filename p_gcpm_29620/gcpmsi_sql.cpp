/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2016-7-21 17:08:52
功能: GCPM提供的动态SQL
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/
// New Include
#include "stdafx.h" 


/*<remark>=========================================================
/// <summary>
///  GCPM提供的动态SQL
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/

// service入口
BM2F_ENTERACE(gcpmsi_sql)
//-EP_SYSTEM_HEAD_END                                                  
int f_gcpmsi_sql(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);  // 系统日志，必须在代码段开始处定义

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsi_sql";                //定义函数英文名称  
	CString FunctionCname = "GCPM提供的动态SQL";              //定义函数中文名称
	////LogTrace(1, 1, " **************%s begin*****************", (const char*)FunctionEname);


	//程序用变量
	int   doFlag = 0;
	int   i = 0;
	int   fetchRowCount = 0;
	 

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  v_userid = s.userid;
	CString  function_id = "gcpmsi_sql";  //自定义显示项目功能号    
	CString  sqlstr = "";  //SQL 信息。 

	CString v_table_name = "";//表名称。
	CString v_moid = ""; //模块代码
	CString v_func_id = "";


	//获得系统时间，当前用户代码
	CString systime = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString userid = s.userid;

	 
	try
	{


		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString  c_sql_orderBy = "";
		
		
		//获取0#blk,第一列信息。
		//===================
		CString v_sql = "";
		if (bcls_rec->Tables[0].Columns.Contains("SQL"))
			v_sql = bcls_rec->Tables[0].Rows[0]["SQL"].ToString(); 
		Log::Trace("", __FUNCTION__, "in , v_sql =[{0}]  ", v_sql);


		//获取第2列信息。
		int v_show_num_max = 5000; //最大值=5000行。
		CString v_show_num = "1";//默认= 1=高数据量====》//0/1=低/高数据量
		if (bcls_rec->Tables[0].Columns.Contains("SHOW_NUM"))
			v_show_num = bcls_rec->Tables[0].Rows[0]["SHOW_NUM"].ToString();
		Log::Trace("", __FUNCTION__, "in , v_show_num =[{0}]  ", v_show_num);

		v_show_num_max = 5000; //默认最多显示5K行记录
		if (v_show_num == "0")
		{//若是低数据量查询，
			v_show_num_max = 10;
		}  


		//SELECT 
		CString v_sql_chk = "";
		if (v_sql.Trim().GetLength() >= 6)
		{
			v_sql_chk = v_sql.Trim().Substring(0, 6); 
		}
		else
		{//错误的SQL 语句
			sprintf(s.msg,"错误的sql语句[%s]",(const char*)v_sql);
			throw CApplicationException(-1, s.msg, log.Location);
		}

		if (v_sql_chk.Trim().ToUpper() == "SELECT")
		{//查询执行

			c_sql_condition = v_sql;
			sqlstr = c_sql_condition; 
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteQuery(bcls_ret->Tables[0], 0, v_show_num_max);//每次最大【v_show_num_max】行记录
			cmd_sql.Close();

			int v_row_num = bcls_ret->Tables[0].Rows.get_Count();
			sprintf(s.msg, "查询到[%d]条记录,allowMax[%d]", v_row_num, v_show_num_max);
			Log::Trace("", __FUNCTION__, "out[{0}]  ", s.msg);

		}
		else
		{//非查询执行
			/*if (v_userid.Trim() != "178029")
			{
				sprintf(s.msg,"您的帐号[%s]，不允许【非查询】的SQL操作。",(const char*)v_userid);
				throw CApplicationException(-1, s.msg, log.Location);
			}*/


			CDecimal v_cnt = 0;
			c_sql_condition = "select count(1) from tgcpmsi00 t "
				" where t.code_class   ='GCP0' "
				" and   t.code         = @code " //指定的人员
				" and   t.valid_flag   = '1' " //1=有效
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("code", v_userid);//责任者。 
			cmd_sql.SetCommandText(c_sql_condition);
			v_cnt = cmd_sql.ExecuteScalar();
			cmd_sql.Close();

			if (v_cnt <= 0 
				//&& v_userid.Trim() != "178029"
				)
			{//若没有找到记录，则说明当前用户，不能进行当前操作。 
				sprintf(s.msg, "您的帐号[%s]没有当前操作权限，\n当前操作失败。"
					, (const char*)v_userid);
				throw CApplicationException(-1, s.msg, log.Location);
			}

			c_sql_condition = v_sql;
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();
			sprintf(s.msg, "处理成功。");
		} 

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