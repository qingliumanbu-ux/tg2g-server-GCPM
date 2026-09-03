/*============================================================================*/
/*== [service名  ]:  gcpmsiUi_inq2       ||  [对应VC#画面 ]:GCPMSI00          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2022/9/7 16:00:40==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMSI00                                          ==*/
/*== [调用函数   ]： 无				                                          ==*/
/*== [service功能]： 基表信息_查询[分页模式]                            ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h" 
#include "Be2UserModel/SI/CFormDevConfig.h"
//#include "Be2UserModel/SI/si00_common.h"




/*<remark>=========================================================
/// <summary>
/// 基表信息_查询[分页模式]
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmsiUi_inq2)

int f_gcpmsiUi_inq2(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);


	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsiUi_inq2";                //定义函数英文名称  
	CString FunctionCname = "基表信息_查询[分页模式]";              //定义函数中文名称 


	//程序用变量
	int   i = 0;
	int   fun_i = 0; //功能个数。
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0;
	int v_total_count = 0; //总数。

	//系统的分页类信息。
	CPageInfo pageInfo;


	try
	{
		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_condition = "  SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString    c_sql_where = "  WHERE   1 = 1 "; //修改条件。 
		CString    c_sql_orderBY = "  order by t.order_no ";


		/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2 = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition2 = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";



		//获取入口参数信息：  
		try{//获取前台DEV控件传入的分页信息
			pageInfo.MergeFrom(bcls_rec->Tables["PageInfo"].Rows[0]);
		}
		catch (CException& ce)
		{//2个变量信息是由前台的分页控件信息传入的，获取失败时,人工赋值一下。 
			pageInfo.RecordFrom = 0;
			pageInfo.PageSize = 500;

		}


		//==查询条件信息。 
		//从指定BLK中获取静态表的表名称。
		CString v_blk_name = "blk2";//业务表名称。
		CString v_table_name = "";
		if (bcls_rec->Tables[v_blk_name].Columns.Contains("TABLE_NAME"))
			v_table_name = bcls_rec->Tables[v_blk_name].Rows[0]["TABLE_NAME"].ToString();
		Log::Trace("", __FUNCTION__, "in ==v_table_name[{0}]  ", v_table_name);

		c_sql_orderBY = "";//排序信息。
		if (bcls_rec->Tables[v_blk_name].Columns.Contains("ORDER_BY"))
			c_sql_orderBY = bcls_rec->Tables[v_blk_name].Rows[0]["ORDER_BY"].ToString();
		Log::Trace("", __FUNCTION__, "in ==c_sql_orderBY[{0}]  ", c_sql_orderBY);
		if (c_sql_orderBY.Trim() != "")
		{
			c_sql_orderBY = " order by " + c_sql_orderBY;
		}


		if (v_table_name.Trim() == "")
		{
			sprintf(s.msg, "业务表信息不能为空，当前操作失败。");
			throw CApplicationException(-1, s.msg, log.Location);
		}

		c_sql_condition = " select * from " + v_table_name;
		c_sql_condition2 = " select COUNT(1)  from " + v_table_name;
		//信息初始化条件。
		c_sql_where = " where 1 = 1   "; //信息初始化条件。  



		//内容查询结果。。。
		c_sql_condition = c_sql_condition + c_sql_where;
		BE2::CFormDevConfig::SetParameters(cmd_sql, c_sql_condition, bcls_rec);
		c_sql_condition = c_sql_condition + c_sql_orderBY;
		Log::Trace("", __FUNCTION__, "最终=c_sql_condition = [{0}]  ", c_sql_condition);
		//内容查询。
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
		cmd_sql.ExecuteQuery(bcls_ret->Tables[0], pageInfo.RecordFrom, pageInfo.PageSize);
		cmd_sql.Close();



		//查询总个数结果。。。
		c_sql_condition2 = c_sql_condition2 + c_sql_where;
		BE2::CFormDevConfig::SetParameters(cmd_sql, c_sql_condition2, bcls_rec);
		Log::Trace("", __FUNCTION__, "最终=c_sql_condition2 = [{0}]  ", c_sql_condition2);
		//汇总行的计算。==v_row_total
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

	/// <summary>
	/// 返回总记录数
	/// </summary>     
	bcls_ret->Tables.Add("PageInfo");
	bcls_ret->Tables["PageInfo"].Columns.Add(DT_DECIMAL, "TotalRecordCount");
	bcls_ret->Tables["PageInfo"].Rows.Add();
	bcls_ret->Tables["PageInfo"].Rows[0]["TotalRecordCount"] = v_total_count;

	//将来可能要拆service处理，SO ，此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;

}