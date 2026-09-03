/*========================================================================*/
/*== [service名  ]:  gcpmz2epg_inq    ||  [对应VC#画面 ]:  ALL       ==*/
/*== [程序编制人 ]:  张颖             ||  [程序定稿日期]:2017-11-23 9:52:06==*/ 
/*== [程序修改人 ]：                  ||  [程序修改日期]:               ==*/
/*========================================================================*/
/*== [数据库表   ]： tgcpmz1                                            ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： 系统功能清单_分组汇总                              ==*/
/*========================================================================*/ 
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h"
 

//从字符串中根据指定分隔符拆分数据
// 入口字符，分隔字符，函数是返回字符信息。
CString f_get_multi_value(CString v_in_str, CString v_spilit_flag, CDbConnection * conn);

/*<remark>=========================================================
/// <summary>
/// 系统功能清单_分组汇总
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz2epg_inq)

int f_gcpmz2epg_inq(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__); 

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz2epg_inq";                //定义函数英文名称  
	CString FunctionCname = "系统功能清单_分组汇总";          //定义函数中文名称 


	//程序用变量
	int i = 0;
	int   fetchRowCount = 0;
	int   doFlag = 0;  
	CString sqlstr = "";  //SQL 信息。 


	try
	{ 
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //查询条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_group_by = " group by t.order_no ";  //分组信息
		CString    c_order_by =  " order by t.order_no "; //排序信息
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //查询条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		
		CString v_moid = "";   //二级代码
		CString v_dllname = "";   //DLL名称
		CString v_form_code = "";   //画面代码 

		if (bcls_rec->Tables[0].Columns.Contains("MOID"))
			v_moid = bcls_rec->Tables[0].Rows[0]["MOID"].ToString().TrimOrBlank();

		if (bcls_rec->Tables[0].Columns.Contains("DLLNAME"))
			v_dllname = bcls_rec->Tables[0].Rows[0]["DLLNAME"].ToString().TrimOrBlank();

		if (bcls_rec->Tables[0].Columns.Contains("FORM_CODE"))
			v_form_code = bcls_rec->Tables[0].Rows[0]["FORM_CODE"].ToString().TrimOrBlank();
		 

		Log::Trace("", __FUNCTION__, "v_moid[{0}]  ", v_moid);
		Log::Trace("", __FUNCTION__, "v_dllname[{0}]  ", v_dllname);
		Log::Trace("", __FUNCTION__, "v_form_code[{0}]  ", v_form_code);
 
		



		/*--一级模块，二级模块，画面名称，功能状态，画面下的功能个数
		select  t.moid,t.dllname,t.form_name,t.status_name,count(t.func_id)
		--,t.*
		from tgcpmz1 t
		where  t.func_id <> 'F0'  
		GROUP BY t.moid,t.dllname,t.form_code,t.status_name 
		*/

       

		//后台程序。//t.dllname,COUNT(DISTINCT t.form_code) ==画面的个数。
		//FORM_NUM ==画面总数
		//FUN_NUM  ==功能总数
		//SVR_NUM  ==service总数
		//F_NUM    ==函数总数。
		//=============================
		c_sql_condition = " select  substr(t.moid,1,2) as main_moid "//一级模块
			" ,t.moid,t.dllname " //，二级模块， DLL 
			" ,'有效' as status_name           "//功能状态。
			" ,substr(t.moid,1,2) as main_moid "//一级模块。
			" ,count(DISTINCT t.form_code ) as FORM_NUM "//画面总数
			" ,count(t.func_id)             as FUN_NUM " //功能总数。 
			" , 0 as SVR_NUM "// ==service总数
			" , 0 as F_NUM   "// ==函数总数。
			" from tgcpmz1 t "
			;  

	

		//分组
		c_group_by = " GROUP BY substr(t.moid,1,2),t.moid,t.dllname "
			//",t.status_name    "
			;

		//排序。
		c_order_by = " ORDER BY substr(t.moid,1,2),t.moid,t.dllname " 
			//",t.status_name    "
			;



		//查询条件。
		c_sql_where = " where  t.func_id <> 'F0' " //非F0功能号
			" and t.dllname <> ' ' " //DLL 名称非空
			;

		 

		if (v_moid.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.moid like @moid || '%' ";
		}

		if (v_dllname.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.dllname =  @dllname  ";
		}

 
		 

		//信息初始化语句+ WHERE 语句 +分组+排序。。。
		c_sql_condition = c_sql_condition + c_sql_where + c_group_by + c_order_by;
		Log::Trace("",__FUNCTION__,"c_sql_condition[{0}]  ",c_sql_condition); 

		 
		fetchRowCount = 0; 
		sqlstr = c_sql_condition;
		//信息初始化条件准备。 
		cmd_sql.Parameters.Set("moid", v_moid.ToUpper());
		cmd_sql.Parameters.Set("dllname", v_dllname);//  
		cmd_sql.SetCommandText(c_sql_condition); 
		cmd_sql.ExecuteQuery(bcls_ret->Tables[0]);  
		cmd_sql.Close();  

		//FORM_NUM ==画面总数
		//FUN_NUM  ==功能总数
		//SVR_NUM  ==service总数
		//F_NUM    ==函数总数。
		//t.moid,t.dllname
		//=============================
		/*循环处理查询到的返回信息。*/
		v_moid = "";  //二级模块。
		CString v_chk_moid = ""; //
		int    v_chk_ii = 0; 

		CDecimal v_svr_num = 0;
		CDecimal v_f_num = 0;
		int v_row_num = bcls_ret->Tables[0].Rows.get_Count();
		for (i = 0; i < v_row_num; i++)
		{

			/*获取当前行信息*/
			if (bcls_ret->Tables[0].Columns.Contains("MOID"))
				v_moid = bcls_ret->Tables[0].Rows[i]["MOID"].ToString();
			 

			//暂定， 将后台程序个数， 记录在‘二级模块’第一次出现的行信息中。
			//则将后台程序总数，记录在当前行中。
			//=============================

			if (v_moid.Trim() == v_chk_moid.Trim())
			{//若2者一致，则不需要计算‘后台程序总数’，默认= 0；

				v_svr_num = 0;
				v_f_num = 0;
			}
			else
			{//若2者不一致，说明是第一次出现的‘二级模块’ 
				v_chk_moid = v_moid;

				//根据二级模块，获取对应的后台，svr,函数总数。
				//FUNC_TYPE IN('S', 'F') "// --SERVICE, 函数 
				//================ 
				v_svr_num = 0;
				c_sql_condition = " select  COUNT(T.SVC_NAME)  "
					" FROM TEA03 T         "
					" WHERE 1=1  "
					" AND T.sub_system_ename  LIKE  @sub_system_ename || '%'  " //LIKE 二级模块。
					" AND T.FUNC_TYPE = 'S' " // --SERVICE
					;
				Log::Trace("", __FUNCTION__, "22==c_sql_condition[{0}]  ", c_sql_condition);
				sqlstr = c_sql_condition;
				//信息初始化条件准备。 
				cmd_sql.Parameters.Set("sub_system_ename", v_moid.ToUpper());
				cmd_sql.SetCommandText(c_sql_condition);
				v_svr_num = cmd_sql.ExecuteScalar(); //SVR总数。
				cmd_sql.Close();


				v_f_num = 0;
				c_sql_condition = " select  COUNT(T.SVC_NAME)  "
					" FROM TEA03 T         "
					" WHERE 1=1  "
					" AND (  T.sub_system_ename  LIKE @sub_system_ename || '%'  " // LIKE 二级模块。
					"     or T.sub_system_ename  LIKE 'GC' || @sub_system_ename || '%' " //【封装函数】比如： GCPMOF
					"     ) "
					" AND T.FUNC_TYPE = 'F' " //函数
					;
				Log::Trace("", __FUNCTION__, "22==c_sql_condition[{0}]  ", c_sql_condition);
				sqlstr = c_sql_condition;
				//信息初始化条件准备。 
				cmd_sql.Parameters.Set("sub_system_ename", v_moid.ToUpper());
				cmd_sql.SetCommandText(c_sql_condition);
				v_f_num = cmd_sql.ExecuteScalar(); //函数总数。
				cmd_sql.Close(); 

			}


			/*返回XX总数*/
			//SVR_NUM  ==service总数
			//F_NUM    ==函数总数。
			//========================
			if (bcls_ret->Tables[0].Columns.Contains("SVR_NUM"))
				bcls_ret->Tables[0].Rows[i]["SVR_NUM"] = v_svr_num;

			if (bcls_ret->Tables[0].Columns.Contains("F_NUM"))
				bcls_ret->Tables[0].Rows[i]["F_NUM"] = v_f_num;

		}


		fetchRowCount = bcls_ret->Tables[0].Rows.get_Count();
		/*设置系统返回参数*/
		sprintf(s.msg, "查询到[%d]条记录。", fetchRowCount);
		 


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








