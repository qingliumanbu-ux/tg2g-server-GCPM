/*========================================================================*/
/*== [service名  ]:  gcpmsi_chk       ||  [对应VC#画面 ]:  ALL          ==*/
/*== [程序编制人 ]:  张颖             ||  [程序定稿日期]:2015-3-30 15:40:08==*/ 
/*== [程序修改人 ]：                  ||  [程序修改日期]:               ==*/
/*========================================================================*/
/*== [数据库表   ]： tgcpmsi01                                          ==*/
/*== [调用函数   ]： 无				                                          ==*/
/*== [service功能]： 纯ED54配置的静态数据维护_功能键校验                ==*/
/*========================================================================*/ 
/***** C/C++ 的标准头文件部分 *****/ 
#include "stdafx.h"
 
 


/*<remark>=========================================================
/// <summary>
///  纯ED54配置的静态数据维护_功能键校验
/// <para>
/// //用于判断：
    //是否有该权限，
    //是否调用指定service .
/// </para> 
===========================================================</remark>*/

// service入口
BM2F_ENTERACE(gcpmsi_chk)
//-EP_SYSTEM_HEAD_END                                                  
int f_gcpmsi_chk(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);  // 系统日志，必须在代码段开始处定义

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsi_chk";                //定义函数英文名称  
	CString FunctionCname = "纯ED54配置的静态数据维护_功能键校验";              //定义函数中文名称


	//程序用变量
	int   doFlag = 0;
	int   i = 0;
	int   fetchRowCount = 0;


	//获得系统时间，当前用户代码
	CString systime = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString userid = s.userid;
	 
	CString  sqlstr = "";  //SQL 信息。 

	CString v_table_name = "";//表名称。
	CString v_moid = ""; //模块代码
	CString v_func_id = "";


	try
	{
		 
		//CTGCPMSI01 tgcpmsi01(conn);

		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString  c_sql_orderBy = "";



		//从2#BLK 中获取静态表的表名称。
		//===需要根据BLK名称，来获取信息。
		CString v_blk_name = "blk2";
		if (bcls_rec->Tables[v_blk_name].Columns.Contains("MOID"))
			v_moid = bcls_rec->Tables[v_blk_name].Rows[0]["MOID"].ToString();

		if (bcls_rec->Tables[v_blk_name].Columns.Contains("TABLE_NAME"))
			v_table_name = bcls_rec->Tables[v_blk_name].Rows[0]["TABLE_NAME"].ToString();

		Log::Trace("", __FUNCTION__, "二级模块 =[{0}] 表名称 =[{1}] ", v_moid, v_table_name);


		//功能号 ， F2,F3....F12等。
		CString v_func_id = "";   //功能标识
		if (bcls_rec->Tables[v_blk_name].Columns.Contains("FUNC_ID"))
			v_func_id = bcls_rec->Tables[v_blk_name].Rows[0]["FUNC_ID"].ToString();



		/*if (v_moid.Trim() == "")
		{
			sprintf(s.msg, "【二级模块】不允许为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		}*/

		if (v_table_name.Trim() == "")
		{
			sprintf(s.msg, "【表名称】不允许为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		}

		if (v_func_id.Trim() == "")
		{
			sprintf(s.msg, "【功能标识】不允许为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		}


		//===========
		//根据[表名称]，进行对应责任者的校验，确认有权限，才允许操作。
		//暂定: F2---查询不需要校验权限...ON 2017-6-8 21:43:03
		//============================= 
		//根据 业务表名称 ,获取是否已经维护GCPE的信息。
		CString v_flag = "xx";
		c_sql_condition = "select t.code_desc_3_content "
			" from tgcpmsi00 t "
			" where t.code_class   ='GCPE' "
			" and   t.code = @code " //指定的表名称 
			;
		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("code", v_table_name);//表名称 
		cmd_sql.SetCommandText(c_sql_condition);
		cmd_sql.ExecuteReader();
		if (cmd_sql.Read())
		{
			v_flag = cmd_sql.GetString(1);
		}
		cmd_sql.Close();

		if (v_flag.Trim() != "1")
		{// 
			sprintf(s.msg, "没有业务表[%s]的生效信息，\n详见控制代码[GCPE],\n当前操作失败。"
				, (const char*)v_table_name);
			throw CApplicationException(-1, s.msg, log.Location);
		}




		CDecimal v_cnt = 0;
		
		//整表的控制权限,由代码 GCPE 控制。 
		//暂定: F2---查询不需要校验权限...ON 2017-6-8 21:43:03
		//=============================
		//==================
		c_sql_condition = "select count(1) from tgcpmsi00 t "
			" where t.code_class   ='GCPE' "
			" and   t.code = @code " //指定的表名称
			" and ( t.code_desc_5_content || ','  LIKE  '%' || @code_desc_5_content || ',%'  "
			"       or t.code_desc_5_content = ' ' )   "  //在[责任者]要求范围内。
			;
		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("code", v_table_name);//表名称
		cmd_sql.Parameters.Set("code_desc_5_content", userid);//责任者。 
		cmd_sql.SetCommandText(c_sql_condition);
		v_cnt = cmd_sql.ExecuteScalar();
		cmd_sql.Close();

		//暂定: 非F2功能的时候，才需要校验权限...ON 2017-6-8 21:43:03
		//=============================
		if (v_cnt <= 0
			&& v_func_id.Trim().ToUpper() != "F2" //非F2按钮的情况下，才需要有权限。
			)
		{//若没有找到记录，则说明当前用户，不能进行当前操作。

			sprintf(s.msg, "您的帐号[%s]没有业务表[%s]的操作权限，\n当前操作失败。"
				, (const char*)userid, (const char*)v_table_name);
			throw CApplicationException(-1, s.msg, log.Location);
		} 
	 

		//根据画面+功能标识，获取对应的指定 SERVICE信息。
		//特殊定义的按钮的可操作权限。
		//================  
		CString v_maker_remark = "";  //指定操作者。
		CString v_operate_mode = ""; //操作模式=1/2=调用SERVICE/执行SQL
		CString v_service_name = ""; //SERVICEQ名
		CString v_sql_context = "";  //SQL内容

		c_sql_condition = " select t.maker_remark  " //指定操作者。
			" ,t.OPERATE_MODE " //操作模式=1/2=调用SERVICE/执行SQL
			" ,t.service_name " //SERVICEQ名
			" ,t.sql_context  " //SQL内容
			" from  tgcpmsi01 t "
			" where t.table_name    = @table_name "
			" and   t.func_id       = @func_id "  
			" and   t.validate_flag = '1' "//生效
			" order by t.table_name,t.func_id "
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句 
		// 设置SQL中的变量
		cmd_sql.Parameters.Set("table_name", v_table_name);  
		cmd_sql.Parameters.Set("func_id", v_func_id);  
		cmd_sql.ExecuteReader();
		if (cmd_sql.Read())
		{
			v_maker_remark = cmd_sql.GetString(1);
			v_operate_mode = cmd_sql.GetString(2);//操作模式=1/2=调用SERVICE/执行SQL
			v_service_name = cmd_sql.GetString(3);
			v_sql_context = cmd_sql.GetString(4);
		}
		cmd_sql.Close();

		Log::Trace("", __FUNCTION__, "表名[{0}] 功能标识[{1}]  "
			, v_table_name, v_func_id);
		Log::Trace("", __FUNCTION__, "v_operate_mode[{0}]", v_operate_mode);
		Log::Trace("", __FUNCTION__, "service[{0}] v_sql_context[{1}] v_maker_remark[{2}]"
			, v_service_name, v_sql_context, v_maker_remark);
		int   find_id = 0;
		int   find_start = 0;		
		if (v_maker_remark.Trim() != ""  
			)
		{//若【按钮】有指定的【操作者】 ，则进行比较，是否包含当前用户。


			find_start = 0;
			find_id = 0;
			find_id = v_maker_remark.Find(userid, find_start); //在授权用户中，查找[当前用户]
			if (find_id < 0)
			{//从头找也没找到指定的[当前用户],则报错。

				sprintf(s.msg, "您的帐号[%s]没有业务表[%s]功能键[%s]的操作权限，\n详见控制画面[GCPMSI01],\n当前操作失败。"
					, (const char*)userid, (const char*)v_table_name
					, (const char*)v_func_id);
				throw CApplicationException(-1, s.msg, log.Location);
			}

		}


		CString v_sql_txt = ""; //拼接的SQL语句。
		//v_operate_mode = cmd_sql.GetString(2);//操作模式=1/2=调用SERVICE/执行SQL
		if (v_operate_mode.Trim() == "2")
		{//若是执行SQL模式， 则将service名称调整为 动态Sql的service.= gcpmsi_sql

			//根据自定义查询SQL ，进行多表综合查询。
			v_service_name = "gcpmsi_inq"; 
		}


		 
		//设置返回信息。

		/// <summary>
		/// 返回总记录数
		/// </summary>     
		CString v_table_name = "SERVICE";
		bcls_ret->Tables.Add(v_table_name);
		bcls_ret->Tables[v_table_name].Columns.Add(DT_STRING, "SERVICE_NAME");//service名称 
		bcls_ret->Tables[v_table_name].Rows.Add();
		bcls_ret->Tables[v_table_name].Rows[0]["SERVICE_NAME"] = v_service_name; 
 


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