/*=============================================================================*/
/*== [service名  ]:  gcpmz2_fun_upd   ||  [对应VC#画面 ]:  ALL               ==*/
/*== [程序编制人 ]:  张颖             ||  [程序定稿日期]:2018-5-18 15:50:19 ==*/
/*== [程序修改人 ]：                  ||  [程序修改日期]:                    ==*/
/*=============================================================================*/
/*== [数据库表   ]： TEA01,TEA03                                          ==*/
/*== [调用函数   ]： 无				                                            ==*/
/*== [service功能]： 后台函数对应的二级模块_修改                          ==*/
/*==========================================================================*/
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h" 


/*<remark>=========================================================
/// <summary>
/// 后台函数对应的二级模块_修改
/// <para>
///    /// TEA00,TEA03,===后台函数对应的二级模块的修改。
SELECT T.SUB_SYSTEM_ENAME,T.* FROM TEA00 T
WHERE T.SRV_ID = '1300'
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz2_fun_upd)

int f_gcpmz2_fun_upd(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz2_fun_upd";                //定义函数英文名称  
	CString FunctionCname = "后台函数对应的二级模块_修改";              //定义函数中文名称 


	//程序用变量
	int   i = 0;
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;  
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0; 
	CString v_srv_id = ""; //SERVER号。


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
	 
		
		//获取2#BLK中的，目标‘二级模块’
		//===================
		CString v_new_sub_system_ename = "";
		if (bcls_rec->Tables[1].Columns.Contains("SUB_SYSTEM_ENAME"))
			v_new_sub_system_ename = bcls_rec->Tables[1].Rows[0]["SUB_SYSTEM_ENAME"];

		Log::Trace("", __FUNCTION__, "v_new_sub_system_ename =[{0}]  ", v_new_sub_system_ename);


		if (v_new_sub_system_ename.Trim() == "")
		{//新‘v_new_sub_system_ename’
			sprintf(s.msg, "新[二级模块]不允许为空，当前操作失败。");
			throw CApplicationException(-1, s.msg, log.Location);
		}
		
		//SVC_NAME ==程序名称
		//FUNC_TYPE==程序类型
		CString v_svc_name = ""; //程序名称
		CString v_func_type = "";//程序类型
		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{ 
			
			if (bcls_rec->Tables[0].Columns.Contains("SVC_NAME"))
				v_svc_name = bcls_rec->Tables[0].Rows[i]["SVC_NAME"];

			if (bcls_rec->Tables[0].Columns.Contains("FUNC_TYPE"))
				v_func_type = bcls_rec->Tables[0].Rows[i]["FUNC_TYPE"];

			Log::Trace("", __FUNCTION__, "in==v_svc_name =[{0}]  ", v_svc_name);
			Log::Trace("", __FUNCTION__, "in==v_func_type =[{0}]  ", v_func_type);

			if (v_svc_name.Trim() == "")
			{//程序名称，不允许为空。
				sprintf(s.msg, "[函数名称]不允许为空，当前操作失败。");
				throw CApplicationException(-1, s.msg, log.Location);
			}

			if (v_func_type.Trim() != "F")
			{//程序类型，不是‘函数’，则报错。
				sprintf(s.msg, "程序类型[%s]不是函数，当前操作失败。"
					, (const char*)v_func_type);
				throw CApplicationException(-1, s.msg, log.Location);
			}

			
			if (userid.Trim() != "178029")
			{//若不是指定用户，则不允许修改，功能的覆盖产线。

				sprintf(s.msg, "您的帐号[%s]没有权限，当前操作失败。"
					, (const char*)userid);
				throw CApplicationException(-1, s.msg, log.Location);
			}
			 
			/*
			select t.program_path,t.sub_system_ename,t.func_type
			,t.* from tea03 t
			where t.svc_name like 'f_pmol4%'
			*/


		 
 

			//修改表。==TEA03
			//==========  
			c_sql_condition = "update  tea03 t "
				" set   t.sub_system_ename = @sub_system_ename " //指定的二级模块。 
				" where t.svc_name         = @svc_name " 
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("sub_system_ename", v_new_sub_system_ename);//新二级模块。
			cmd_sql.Parameters.Set("svc_name", v_svc_name);//函数名称
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


