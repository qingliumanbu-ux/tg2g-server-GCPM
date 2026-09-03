/*==========================================================================*/
/*== [service名  ]:  gcpmz1_get       ||  [对应VC#画面 ]:  ALL            ==*/
/*== [程序编制人 ]:  张颖             ||  [程序定稿日期]:2016-10-21 23:02:09==*/ 
/*== [程序修改人 ]：                  ||  [程序修改日期]:                 ==*/
/*==========================================================================*/
/*== [数据库表   ]： tgcpmz1                                              ==*/
/*== [调用函数   ]： 无				                                      ==*/
/*== [service功能]： GCPM_从TGCPMZ1表中获取信息                           ==*/
/*==========================================================================*/ 
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h" 

 
 
 

/*<remark>=========================================================
/// <summary>
///  GCPM_,
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/

// service入口
BM2F_ENTERACE(gcpmz1_get)
//-EP_SYSTEM_HEAD_END                                                  
int f_gcpmz1_get(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);  // 系统日志，必须在代码段开始处定义 

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz1_get";                //定义函数英文名称  
	CString FunctionCname = "GCPM_从TGCPMZ1表中获取信息";              //定义函数中文名称

 


	//程序用变量
	int   doFlag = 0;
	int   fetchRowCount = 0;
	int   i = 0;
	CString sqlstr = "";

	CString  v_userid = s.userid;
	CString  systime = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  function_id = "gcpmz1_get" ;  //自定义显示项目功能号    
	

	 
	CDecimal v_cnt    = 0;

	CString  v_condi = "";
	CString  v_update= ""; 
	CString  v_table_name     = "xx";

 
	try
	{
		CPageInfo pageInfo;	

		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where      = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition  =  " SELECT  1 FROM XX " ; //

		/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString  c_sql_where2 = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition2 = " SELECT  1 FROM XX "; //

		/* 实体类定义 */  
		//根据 TGCPMZ1 表中的信息，修正 表 中已有的数据。 
		//========== 

		CString v_moid_main = "PM"; //本画面导入的信息，暂定一级模块都是PM.

		CString v_moid = "";
		CString v_form_code = "";
		CString v_form_name = "";
		CString v_from_desc = "";
		CDecimal v_seq_no = 0; //序号。


		//获取最大序号+1===前台画面。。。
		c_sql_condition2 = "select max(t.seq_no) + 1 from tgcpm01 t ";
		sqlstr = c_sql_condition2;
		cmd_sql2.SetCommandText(c_sql_condition2); //设置执行的SQL语句
		v_seq_no = cmd_sql2.ExecuteScalar();
		cmd_sql2.Close();
		Log::Trace("", __FUNCTION__, "当前最大值。。画面==v_seq_no[{0}]", v_seq_no);

		c_sql_condition =  "  select DISTINCT "
			"  t.moid        " // 二级模块
			"  ,t.form_code   " //画面代码
			"  ,t.form_name   " //画面名称
			" from tgcpmz1 t "
			" where t.form_code not in "
			"      (select aa.program_code from tgcpm01 aa )" //确保只新增不存在于TGCPM01 表中的信息。
			" and  ( t.form_code LIKE 'PM%' OR t.form_code LIKE 'OM%' ) " //om/pm模块的相关画面。
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句
		// 设置SQL中的变量 
		cmd_sql.ExecuteReader();
		while (cmd_sql.Read())
		{
			v_moid = cmd_sql.GetString(1);
			v_form_code = cmd_sql.GetString(2);
			v_form_name = cmd_sql.GetString(3); 

			//画面描述= ‘按钮说明的集合’
			//===============
			v_from_desc = "";
			c_sql_condition2 = "select t.FUNC_CNAME from tgcpmz1 t "
				"               where t.form_code  = @form_code    "
				"               and   t.FUNC_CNAME <> '画面加载点' "
				;
			sqlstr = c_sql_condition2;
			cmd_sql2.Parameters.Set("form_code", v_form_code);
			cmd_sql2.SetCommandText(c_sql_condition2); //设置执行的SQL语句
			cmd_sql2.ExecuteReader();
			while (cmd_sql2.Read())
			{
				v_from_desc = v_from_desc + "/" + cmd_sql2.GetString(1).Trim();
			}			 
			cmd_sql2.Close();



			
			v_seq_no = v_seq_no + 1;
			Log::Trace("", __FUNCTION__, "v_seq_no[{0}]",v_seq_no); 
			Log::Trace("", __FUNCTION__, "v_from_desc[{0}]",v_from_desc);






			c_sql_condition2 = " insert into tgcpm01(PROJECT_NO,SEQ_NO "
				", SUB_SYSTEM_ENAME  "
				", MODULE_NAME_1  , MODULE_NAME_2  "
				", PROGRAM_TYPE   , PROGRAM_CODE  "
				", PROGRAM_NAME   , PROGRAM_DESC  "
				", PROGRAM_MAKER  , PROGRAM_DVLPER  "
				", PROGRAM_SYSTEM_TEST_FLAG  " //开发状态
				" ,REC_CREATOR,REC_CREATE_TIME ) " //创建者, 创建时刻
				"  select 'zy_pro',@seq_no "
				"  ,'XWTGC0' " //子系统= 西王特钢。
				"  ,@moid_main        ,@moid  "//一级模块， 二级模块
				"  ,'Form'            ,@form_code " //画面，画面代码
				"  ,@form_name        ,@from_desc "//画面名称, 画面描述
				"  ,'张颖'            , '张颖' " //责任者，开发者
				"  ,'2' " //开发状态 = 2 = 已开发未测试
				"  ,@rec_creator ,@rec_create_time " //创建者，创建时刻。
				" from tgcpmz1 t "
				" where t.form_code = @form_code " //指定的画面代码。
				" and   t.FUNC_ID = 'F0' "//FORM_LOAD 点。
				 ;
			sqlstr = c_sql_condition2;
			cmd_sql2.Parameters.Set("seq_no", v_seq_no);
			cmd_sql2.Parameters.Set("moid_main", v_moid_main);//v_moid_main
			cmd_sql2.Parameters.Set("moid", v_moid);
			cmd_sql2.Parameters.Set("form_code", v_form_code);
			cmd_sql2.Parameters.Set("form_name", v_form_name);
			cmd_sql2.Parameters.Set("from_desc", v_from_desc.TrimOrBlank());

			cmd_sql2.Parameters.Set("rec_creator", v_userid);//设置SQL中的变量
			cmd_sql2.Parameters.Set("rec_create_time", systime);
			cmd_sql2.SetCommandText(c_sql_condition2); //设置执行的SQL语句 
			cmd_sql2.ExecuteNonQuery();
			cmd_sql2.Close();

		}
		cmd_sql.Close(); 



		//获取最大序号+1===接口函数。。。
		c_sql_condition2 = "select max(t.seq_no) + 1 from tgcpm01 t ";
		sqlstr = c_sql_condition2;
		cmd_sql2.SetCommandText(c_sql_condition2); //设置执行的SQL语句
		v_seq_no = cmd_sql2.ExecuteScalar();
		cmd_sql2.Close();
		Log::Trace("", __FUNCTION__, "当前最大值。。接口函数==v_seq_no[{0}]", v_seq_no);


		/*
		select  t.sub_system_ename,t.svc_name,t.program_desc,t.archive_flag, t.* from tea03 t
		where t.sub_system_ename like 'OM%'
		and     t.func_type = 'F'
		*/
		v_from_desc = ""; 
		c_sql_condition = "  select   "
			"  t.sub_system_ename        " // 二级模块
			"  ,t.svc_name       " //函数代码
			"  ,t.program_desc   " //函数名称
			" from tea03 t "
			" where t.svc_name not in "
			"      (select aa.program_code from tgcpm01 aa )" //确保只新增不存在于TGCPM01 表中的信息。
			//" and  ( t.sub_system_ename LIKE 'PM%' OR t.sub_system_ename LIKE 'OM%' ) " //om/pm模块的相关内容。
			" and   t.func_type = 'F' " //函数。
			" and   t.program_desc like '[关键函数]%' "//函数说明是[关键函数] ，开头的。
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句
		// 设置SQL中的变量 
		cmd_sql.ExecuteReader();
		while (cmd_sql.Read())
		{
			v_moid = cmd_sql.GetString(1);
			v_form_code = cmd_sql.GetString(2);
			v_form_name = cmd_sql.GetString(3); 


			v_seq_no = v_seq_no + 1;
			Log::Trace("", __FUNCTION__, "v_seq_no[{0}]", v_seq_no);
			Log::Trace("", __FUNCTION__, "v_from_desc[{0}]", v_from_desc); 

			//暂定函数： 说明=名称。
			v_from_desc = v_form_name;
			c_sql_condition2 = " insert into tgcpm01(PROJECT_NO,SEQ_NO "
				", SUB_SYSTEM_ENAME  "
				", MODULE_NAME_1  , MODULE_NAME_2  "
				", PROGRAM_TYPE   , PROGRAM_CODE  "
				", PROGRAM_NAME   , PROGRAM_DESC  "
				", PROGRAM_MAKER  , PROGRAM_DVLPER  "
				", PROGRAM_SYSTEM_TEST_FLAG  " //开发状态
				" ,REC_CREATOR,REC_CREATE_TIME ) " //创建者, 创建时刻
				"  values('zy_pro',@seq_no "
				"  ,'XWTGC0' " //子系统= 西王特钢。
				"  ,@moid_main        ,@moid      "//一级模块， 二级模块
				"  ,'Function'        ,@form_code "//函数，画面代码
				"  ,@form_name        ,@from_desc "//画面名称, 画面描述
				"  ,'张颖'            , '张颖'    "//责任者，开发者
				"  ,'2' " //开发状态 = 2 = 已开发未测试
				"  ,@rec_creator ,@rec_create_time " //创建者，创建时刻。
				" ) "
				;
			sqlstr = c_sql_condition2;
			cmd_sql2.Parameters.Set("seq_no", v_seq_no);
			cmd_sql2.Parameters.Set("moid_main", v_moid_main);//v_moid_main
			cmd_sql2.Parameters.Set("moid", v_moid);
			cmd_sql2.Parameters.Set("form_code", v_form_code);
			cmd_sql2.Parameters.Set("form_name", v_form_name);
			cmd_sql2.Parameters.Set("from_desc", v_from_desc.TrimOrBlank());

			cmd_sql2.Parameters.Set("rec_creator", v_userid);//设置SQL中的变量
			cmd_sql2.Parameters.Set("rec_create_time", systime);
			cmd_sql2.SetCommandText(c_sql_condition2); //设置执行的SQL语句 
			cmd_sql2.ExecuteNonQuery();
			cmd_sql2.Close();

		}
		cmd_sql.Close();

 

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

