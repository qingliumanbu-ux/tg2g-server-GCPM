/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2017-5-27 12:17:42
功能: 根据ED54的配置信息_信息查询[分页模式]
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/
// New Include
#include "stdafx.h"
 


/*<remark>=========================================================
/// <summary>
///  根据ED54的配置信息_信息查询[分页模式]
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/

// service入口
BM2F_ENTERACE(gcpmsied54_inq2)
//-EP_SYSTEM_HEAD_END                                                  
int f_gcpmsied54_inq2(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);  // 系统日志，必须在代码段开始处定义

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsied54_inq2";                //定义函数英文名称  
	CString FunctionCname = "根据ED54的配置信息_信息查询[分页模式]";              //定义函数中文名称
	////LogTrace(1, 1, " **************%s begin*****************", (const char*)FunctionEname);


	//程序用变量
	int   doFlag = 0;
	int   i = 0;
	int   fetchRowCount = 0;
	int v_total_count = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  function_id = "gcpmsied54_inq2";  //自定义显示项目功能号    
	CString  sqlstr = "";  //SQL 信息。 

	CString v_table_name = "";//表名称。
	CString v_moid = ""; //模块代码
	CString v_func_id = "";


	try
	{
		//系统的分页类信息。
		CPageInfo pageInfo;


		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString  c_sql_orderBy = "";

		//
		CString c_sql_condition2 = " SELECT COUNT(1) FROM tpmof01 t ";

		//获取入口参数信息：  
		try{//获取前台DEV控件传入的分页信息
			pageInfo.MergeFrom(bcls_rec->Tables["PageInfo"].Rows[0]);
		}
		catch (CException& ce)
		{//2个变量信息是由前台的分页控件信息传入的，获取失败时,人工赋值一下。 
			pageInfo.RecordFrom = 0;
			pageInfo.PageSize = 1000;

		}



		 
		//从2#BLK 中获取静态表的表名称。
		CString v_blk_where = "blk2";
		if (bcls_rec->Tables[v_blk_where].Columns.Contains("MOID"))
			v_moid = bcls_rec->Tables[v_blk_where].Rows[0]["MOID"].ToString();

		if (bcls_rec->Tables[v_blk_where].Columns.Contains("TABLE_NAME"))
			v_table_name = bcls_rec->Tables[v_blk_where].Rows[0]["TABLE_NAME"].ToString();

		Log::Trace("", __FUNCTION__, "二级模块 =[{0}] 表名称 =[{1}] ", v_moid, v_table_name);

		if (v_moid.Trim() == "")
		{
			sprintf(s.msg, "【二级模块】不允许为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		}

		if (v_table_name.Trim() == "")
		{
			sprintf(s.msg, "【表名称】不允许为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		} 

		//显示功能号 =二级模块_表名称===> PMOF_TPMOFSI00
		v_func_id = v_moid.ToUpper() + "_" + v_table_name.ToUpper();

		//查询条件功能号 = 二级模块_表名称_WHERE===>PMOF_TPMOFSI00_WHERE
		CString v_func_id_where = v_func_id + "_WHERE";

		//新增功能：
		//===addec by zy on 2017-6-29 16:05:05
		/*
		查询条件ED54功能号，
		显示列  ED54功能号，
		由前台传入。
		*/
		if (bcls_rec->Tables[v_blk_where].Columns.Contains("ED54_WHERE"))
			v_func_id_where = bcls_rec->Tables[v_blk_where].Rows[0]["ED54_WHERE"].ToString();

		if (bcls_rec->Tables[v_blk_where].Columns.Contains("ED54_INQ"))
			v_func_id = bcls_rec->Tables[v_blk_where].Rows[0]["ED54_INQ"].ToString();

		Log::Trace("", __FUNCTION__, "in ==v_func_id_where[{0}] v_func_id[{1}] "
			, v_func_id_where, v_func_id);






		//根据查询功能号 ，拼接WHERE 语句。
		//================================== 
		CString v_item_ename_where = ""; //字段英文
		CString v_item_type_where = "";  //字段类型
		CString v_item_value = "";
		CDecimal v_item_value_n = 0;


		//模糊查询控制变量。
		//==========
		int find_start = 0; //查找的开始位置
		int find_id = 0;    //找到的位置
		CString v_spilit_flag = "%"; //查找的内容

		CString v_ed54_where = "";
		CString v_validate_mode = ""; //检查模式

		c_sql_where = " where 1 = 1 ";
		fetchRowCount = 0;
		c_sql_condition = " select  t.item_ename,t.item_type "
			" ,t.VALIDATE_MODE "//借用字段=检查模式= ** ，则说明不用做任何处理。
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
			v_item_ename_where = cmd_sql.GetString(1);
			v_item_type_where = cmd_sql.GetString(2); 
			v_validate_mode = cmd_sql.GetString(3);

		/*	Log::Trace("", __FUNCTION__, "查询条件，字段 =[{0}] 字段类型=[{1}]"
				, v_item_ename_where, v_item_type_where);*/


			 
		 


			if (v_item_type_where.Trim() == "N")
			{//数字型
				//根据字段英文名称，获取的该字段的值 
				//==============================
				if (bcls_rec->Tables[0].Columns.Contains(v_item_ename_where))
					v_item_value_n = bcls_rec->Tables[0].Rows[0][v_item_ename_where];

				Log::Trace("", __FUNCTION__, "数字型-查询条件，字段 =[{0}] 字段内容=[{1}]字段类型[{2}]"
					, v_item_ename_where, v_item_value_n, v_item_type_where);
				if (v_item_value_n >0)
				{
					v_ed54_where = " and " + v_item_ename_where + " = " + v_item_value_n.ToString(); // and aa = 123
				}
				
			}
			else if (v_item_type_where.Trim() == "D")
			{//日期类型，一般进行日期范围的查询。。。。 date, date2..... 

				if (bcls_rec->Tables[0].Columns.Contains(v_item_ename_where))
					v_item_value = bcls_rec->Tables[0].Rows[0][v_item_ename_where].ToString();

				Log::Trace("", __FUNCTION__, "日期类型，字段 =[{0}] 字段内容=[{1}]字段类型[{2}]"
					, v_item_ename_where, v_item_value, v_item_type_where);


				if (v_item_value.Trim() != "")
				{//若查询条件，不为空。 
					if (v_validate_mode.Trim() == "**")
					{//v_validate_mode ,若对应的内容= **,则说明，不需要做任何处理。
						v_ed54_where = " and " + v_item_ename_where + " = '" + v_item_value + "'"; // and aa = '20180325' 
					}
					else
					{

						//若内容中是2结尾 ，则说明是日期范围中的【结束日期】
						//========================
						find_start = v_item_ename_where.Trim().GetLength() - 1; //查找的开始位置= 字段名称长度-1
						find_id = 0;    //找到的位置
						v_spilit_flag = "2"; //查找的内容
						find_id = v_item_ename_where.Find(v_spilit_flag, find_start); //查找分隔符是[,]
						if (find_id >= 0)
						{//若若是2结尾，则说明是【结束日期】 
							v_item_ename_where = v_item_ename_where.Substring(0, find_start);//调整列名称,从 aaa2，变成aaa
							v_ed54_where = " and " + v_item_ename_where + " <= '" + v_item_value + "999999' "; // and aa <= '20180325' 
						}
						else
						{//【开始日期】
							v_ed54_where = " and " + v_item_ename_where + " >= '" + v_item_value + "000000' "; // and aa >= '20180325' 
						}
					}
				} 

			}
			else
			{//非数字型。
				//根据字段英文名称，获取的该字段的值 
				//==============================
				if (bcls_rec->Tables[0].Columns.Contains(v_item_ename_where))
					v_item_value = bcls_rec->Tables[0].Rows[0][v_item_ename_where].ToString();

				Log::Trace("", __FUNCTION__, "非数字型-查询条件，字段 =[{0}] 字段内容=[{1}]字段类型[{2}]"
					, v_item_ename_where, v_item_value, v_item_type_where);


				if (v_item_value.Trim() != "")
				{
					v_ed54_where = " and " + v_item_ename_where + " = '" + v_item_value + "' "; // and aa = '123'
				} 

				//若内容中包括% ，则支持模糊查询。
				//========================
				find_start = 0; //查找的开始位置
				find_id = 0;    //找到的位置
				v_spilit_flag = "%"; //查找的内容
				find_id = v_item_value.Find(v_spilit_flag, find_start); //查找分隔符是[,]
				if (find_id >= 0)
				{//若找到了百分号，则说明需要模糊查询。
					// and aa like '123%' 
					v_ed54_where = " and " + v_item_ename_where + " like '" + v_item_value + "%' "; // and aa like '123%' 
				}


			} 

			c_sql_where = c_sql_where + v_ed54_where; 
		}
		cmd_sql.Close();

		Log::Trace("", __FUNCTION__, "最终222： c_sql_where[{0}]  ", c_sql_where);





		//根据功能号，获取对应的主键字段，作为排序的依据。
		//================
		CString v_item_key_flag = "";
		CString v_item_ename = "";
		CString v_item_cname = "";
		CString v_orderBy = "";
		fetchRowCount = 0;
		c_sql_condition = " select t.item_key_flag,t.item_ename,t.item_cname "
			" from ted54 t "
			" where t.func_id = @func_id "
			" and   t.item_key_flag = '1' " //主键字段
			" order by t.class_code,t.seq_no "
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句 
		// 设置SQL中的变量
		cmd_sql.Parameters.Set("func_id", v_func_id); //ED54功能号。
		cmd_sql.ExecuteReader();
		while (cmd_sql.Read())
		{
			
			v_item_key_flag = cmd_sql.GetString(1);
			v_item_ename = cmd_sql.GetString(2);
			v_item_cname = cmd_sql.GetString(3);
			Log::Trace("", __FUNCTION__, "主键 =[{0}] 字段 =[{1}] 中文=[{2}]序号[{3}]"
				, v_item_key_flag, v_item_ename, v_item_cname, fetchRowCount);

			if (fetchRowCount == 0)
			{//第一行的时候，
				v_orderBy = v_item_ename;
			}
			else
			{//非第一行的时候，
				v_orderBy = v_orderBy + " ," + v_item_ename;
			}
			fetchRowCount++;
			
		}
		cmd_sql.Close();
		
		if (v_orderBy.Trim() != "")
		{//若有排序字段，则组织排序语句。
			c_sql_orderBy = " ORDER BY " + v_orderBy;
		}
		Log::Trace("", __FUNCTION__, "最终222： _sql_orderBy[{0}]  ", c_sql_orderBy);



		c_sql_condition = " select *  from " + v_table_name + c_sql_where + c_sql_orderBy;
		Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);

		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句 
		bcls_ret->Tables[0].set_TableName(v_table_name);
		//cmd_sql.ExecuteQuery(bcls_ret->Tables[0]);
		cmd_sql.ExecuteQuery(bcls_ret->Tables[0], pageInfo.RecordFrom, pageInfo.PageSize);
		cmd_sql.Close();


		//汇总行的计算。==v_row_total
		c_sql_condition2 = " select COUNT(1)  from " + v_table_name + c_sql_where;
		Log::Trace("", __FUNCTION__, "c_sql_condition2[{0}]  ", c_sql_condition2);
		v_total_count = 0;
		sqlstr = c_sql_condition2;
		cmd_sql.SetCommandText(c_sql_condition2);
		v_total_count = cmd_sql.ExecuteScalar().ToInt32();
		cmd_sql.Close(); 
		 

		/*设置系统返回参数*/ 
		sprintf(s.msg, "查询到[%d]条记录。", v_total_count);


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


	/// <summary>
	/// 返回总记录数
	/// </summary>     
	bcls_ret->Tables.Add("PageInfo");
	bcls_ret->Tables["PageInfo"].Columns.Add(DT_DECIMAL, "TotalRecordCount");
	bcls_ret->Tables["PageInfo"].Rows.Add();
	bcls_ret->Tables["PageInfo"].Rows[0]["TotalRecordCount"] = v_total_count;

	//此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;
}