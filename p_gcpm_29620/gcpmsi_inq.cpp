/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2017-6-12 15:39:51
功能: 根据ED54的配置查询条件+动态SQL_多表关联查询。
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/
// New Include
#include "stdafx.h"



/*<remark>=========================================================
/// <summary>
///  根据ED54的配置查询条件+动态SQL_多表关联查询。
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/

// service入口
BM2F_ENTERACE(gcpmsi_inq)
//-EP_SYSTEM_HEAD_END                                                  
int f_gcpmsi_inq(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);  // 系统日志，必须在代码段开始处定义

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsi_inq";                //定义函数英文名称  
	CString FunctionCname = "根据ED54的配置查询条件+动态SQL_多表关联查询";              //定义函数中文名称 


	//程序用变量
	int   doFlag = 0;
	int   i = 0;
	int   fetchRowCount = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  function_id = "gcpmsi_inq";  //自定义显示项目功能号    
	CString  sqlstr = "";  //SQL 信息。 

	CString v_table_name = "";//表名称。
	CString v_moid = ""; //模块代码
	CString v_func_id = "";


	try
	{



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

		//功能标识 ， F2,F3....F12等。
		CString v_func_id = "";   //功能标识
		if (bcls_rec->Tables[v_blk_name].Columns.Contains("FUNC_ID"))
			v_func_id = bcls_rec->Tables[v_blk_name].Rows[0]["FUNC_ID"].ToString(); 

		if (v_table_name.Trim() == "")
		{
			sprintf(s.msg, "【表名称】不允许为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		}

		if (v_func_id.Trim() == "")
		{
			sprintf(s.msg, "【功能标识】不允许为空。");//F2
			throw CApplicationException(-1, s.msg, log.Location);
		}


		//根据画面+功能标识，获取对应的指定 SERVICE信息。
		//特殊定义的按钮的可操作权限。
		//================   
		CString v_operate_mode = ""; //操作模式=1/2=调用SERVICE/执行SQL 
		CString v_sql_context = "";  //SQL内容

		c_sql_condition = " select   t.OPERATE_MODE " //操作模式=1/2=调用SERVICE/执行SQL 
			" ,t.sql_context    " //SQL内容
			" from  tgcpmsi01 t "
			" where t.table_name    = @table_name "
			" and   t.func_id       = @func_id    "//F2
			" and   t.validate_flag = '1'         "//生效
			" order by t.table_name,t.func_id "
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句 
		// 设置SQL中的变量
		cmd_sql.Parameters.Set("table_name", v_table_name);
		cmd_sql.Parameters.Set("func_id", v_func_id);//功能标识
		cmd_sql.ExecuteReader();
		if (cmd_sql.Read())
		{ 
			v_operate_mode = cmd_sql.GetString(1);//操作模式=1/2=调用SERVICE/执行SQL 
			v_sql_context = cmd_sql.GetString(2).ToUpper();//SQL内容,转大写。
		}
		cmd_sql.Close();

		Log::Trace("", __FUNCTION__, "v_operate_mode[{0}] v_sql_context[{1}]  "
			, v_operate_mode, v_sql_context);


		//将SQL语句中的变量，替换成前台传入的信息。  
		//===addec by zy on 2018-3-26 13:33:59
		/*
		查询条件ED54功能号， 
		由前台传入。
		*/
		CString v_func_id_where = "";
		if (bcls_rec->Tables[1].Columns.Contains("ED54_WHERE"))
			v_func_id_where = bcls_rec->Tables[1].Rows[0]["ED54_WHERE"].ToString(); 
		Log::Trace("", __FUNCTION__, "in ==v_func_id_where[{0}] "
			, v_func_id_where);

		////查询条件功能号 = 表名称_WHERE===>OMCIZ1_WHERE
		//CString v_func_id_where = v_table_name.ToUpper() + "_WHERE";

		//根据查询功能号 ，拼接WHERE 语句。
		//================================== 
		CString v_item_ename_where = ""; //字段英文
		CString v_item_type_where = "";  //字段类型
		CString v_item_value = ""; //字符变量
		CString v_item_value_n = "0";//数字变量


		//模糊查询控制变量。
		//==========
		int find_start = 0; //查找的开始位置
		int find_id = 0;    //找到的位置
		CString v_spilit_flag = "%"; //查找的内容

		CString v_ed54_where = "";
		c_sql_where = " where 1 = 1 ";
		fetchRowCount = 0;
		c_sql_condition = " select  t.item_ename,t.item_type "
			" from ted54 t "
			" where t.func_id = @func_id "
			" order by t.SEQ_NO "
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句 
		// 设置SQL中的变量
		cmd_sql.Parameters.Set("func_id", v_func_id_where); //查询条件功能号。
		cmd_sql.ExecuteReader();
		while (cmd_sql.Read())
		{
			v_ed54_where = "";//初始化。
			v_item_ename_where = cmd_sql.GetString(1).ToUpper();
			v_item_type_where = cmd_sql.GetString(2).ToUpper();


			/*	Log::Trace("", __FUNCTION__, "查询条件，字段 =[{0}] 字段类型=[{1}]"
			, v_item_ename_where, v_item_type_where);*/


			//根据字段英文名称，获取的该字段的值 
			//==============================
			if (bcls_rec->Tables[0].Columns.Contains(v_item_ename_where))
				v_item_value = bcls_rec->Tables[0].Rows[0][v_item_ename_where];
			v_item_value = v_item_value.Trim(); //去空格。
			

			Log::Trace("", __FUNCTION__, "查询条件，字段 =[{0}] 字段内容=[{1}]字段类型[{2}]"
				, v_item_ename_where, v_item_value, v_item_type_where); 

			if (v_item_type_where.Trim() != "N") 
			{//非数字型。  
				//if (v_item_value.Trim() == "")
				//{//若是NULL ，则默认赋值个空格。
				//	v_item_value = " ";
				//}
				v_item_value = "'" + v_item_value + "'"; //非数字型=字符型，头尾加上单引号。 
			}
			CString v_item_ename_where2 = "@" + v_item_ename_where;
			v_sql_context = v_sql_context.Replace(v_item_ename_where2, v_item_value);

			Log::Trace("", __FUNCTION__, "===v_sql_context[{0}]  ", v_sql_context);
			 
		}
		cmd_sql.Close();

		Log::Trace("", __FUNCTION__, "最终222： v_sql_context[{0}]  ", v_sql_context);





		c_sql_condition = v_sql_context;
		Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition); 
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句 
		bcls_ret->Tables[0].set_TableName(v_table_name);
		cmd_sql.ExecuteQuery(bcls_ret->Tables[0]);
		cmd_sql.Close();



		//返回的记录数。//.Rows.get_Count()
		fetchRowCount = bcls_ret->Tables[0].Rows.get_Count();

		/*设置系统返回参数*/ 
		sprintf(s.msg, "查询到[%d]条记录。", fetchRowCount);

		//此处记录一下当前service名称,用于显示前台。
		//=============c_sql_condition
		strcpy(s.sysmsg, c_sql_condition);


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