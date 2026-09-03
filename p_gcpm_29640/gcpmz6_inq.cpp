/*============================================================================*/
/*== [service名  ]:  gcpmz6_inq         ||  [对应VC#画面 ]:GCPMSI01          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2017-4-27 16:32:33==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TTADT00                                            ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： 表TTADT00_信息查询                                 ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h"
//#include "ttadt00.h"




/*<remark>=========================================================
/// <summary>
/// 表TTADT00_信息查询
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz6_inq)

int f_gcpmz6_inq(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);


	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz6_inq";                //定义函数英文名称  
	CString FunctionCname = "表TTADT00_信息查询";              //定义函数中文名称 


	//程序用变量
	int   i = 0;
	int   fun_i = 0; //功能个数。
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0;


	try
	{

		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString    c_order_by = " order by t.order_no ";


		/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2 = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition2 = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";




		////允许查询系统TTA结构信息的帐号。
		////暂定，和 GCPMZ5E操作画面，同权限控制。
		////======================
		//v_cnt = 0;
		//c_sql_condition = "select count(1) from tgcpmsi00 t "
		//	" where t.code_class     =  'GCPD' " //查询权限。
		//	" and   t.valid_flag     = '1' " //生效的
		//	" and   t.code           =  @code "//对应的帐号。
		//	;
		//sqlstr = c_sql_condition;
		//cmd_sql.Parameters.Set("code", userid);//责任者。 
		//cmd_sql.SetCommandText(c_sql_condition);
		//v_cnt = cmd_sql.ExecuteScalar();
		//cmd_sql.Close();

		////查询不约束人员。
		//if (v_cnt <= 0)
		//{//若没有找到记录，则说明当前用户，不能进行当前操作。

		//	sprintf(s.msg, "您的帐号[%s]没有进行当前操作的权限。"
		//		, (const char*)userid);
		//	throw CApplicationException(-1, s.msg, log.Location);
		//}





		//==查询条件信息。
		//CString TABLE_NAME;   //数据库表名 
		//CString FUNC_ID;   //功能标识
		//SUB_SYSTEM_ENAME  二级模块

		//从1#BLK 中获取静态表的表名称。
		CString v_table_ename = "";
		if (bcls_rec->Tables[0].Columns.Contains("TABLE_ENAME"))
			v_table_ename = bcls_rec->Tables[0].Rows[0]["TABLE_ENAME"].ToString().ToUpper(); //默认转大写。

		CString v_table_cname = "";
		if (bcls_rec->Tables[0].Columns.Contains("TABLE_CNAME"))
			v_table_cname = bcls_rec->Tables[0].Rows[0]["TABLE_CNAME"].ToString();

		Log::Trace("", __FUNCTION__, "in ==v_table_ename[{0}]  ", v_table_ename);
		Log::Trace("", __FUNCTION__, "in ==v_table_cname[{0}]  ", v_table_cname);


	 

		 


		/*--oracle数据库的TTA表名称定义信息。
		SELECT   T.TABLE_NAME,T.COMMENTS --表名称代码， 表名称描述
		,T.*
		FROM ALL_TAB_COMMENTS T
		WHERE T.TABLE_NAME LIKE 'TQX%'
		AND T.OWNER  LIKE 'BGNSS%'
		AND T.TABLE_TYPE = 'TABLE'
		ORDER BY T.TABLE_NAME
		;
		*/

		/*--DB2数据库的表名称定义表信息。
		select t.name,t.remarks  
		,t.* from Sysibm.systables t
		where t.type = 'T'
		and t.name    LIKE   'TSI00%' --'TSI00GRIDVIEW'
		and t.creator = 'BM2MMS'
		--select t.* from Sysibm.syscolumns t
		
		*/


		 



		switch (conn->DatabaseKind)
		{
		case DB_KIND_ORACLE:

			//ORACLE数据库 
			c_sql_condition = "SELECT t.table_name as table_ename" //表名称英文
				" ,t.comments as table_cname "//表名称说明
				"  from ALL_TAB_COMMENTS t "
				"  where T.TABLE_NAME LIKE 'T%'   " //在线表   
				"  and   T.TABLE_TYPE = 'TABLE'     "//数据库表
				"  and   t.table_name like         @table_ename   || '%' "
				"  and   t.comments   like '%' ||  @table_cname   || '%' "
				"  order by t.table_name "
				;
			break;
		case DB_KIND_DB2_ORACLE:
		case DB_KIND_DB2: 
			//DB2数据库。
			/*
			select t.name, t.remarks
				, t.*from Sysibm.systables t
			where t.type = 'T'
				and t.name    LIKE   'TSI00%' --'TSI00GRIDVIEW'
				and t.creator = 'BM2MMS'
				*/

			c_sql_condition = "SELECT  t.name  as table_ename " //表名称英文
				" , t.remarks as table_cname "//表名称说明 
				"FROM  Sysibm.systables t "
				"WHERE  t.type = 'T'"
				"  and   t.name      like         @table_ename || '%' "
				"  and   t.remarks   like '%' ||  @table_cname || '%' "
				"  order by t.name "
				;

			break; 
		default:
			//暂定是 ORACLE模式。  
			c_sql_condition = "SELECT t.table_name as table_ename" //表名称英文
				" ,t.comments as table_cname "//表名称说明
				"  from ALL_TAB_COMMENTS t "
				"  where T.TABLE_NAME LIKE 'T%'   " //在线表   
				"  and   T.TABLE_TYPE = 'TABLE'     "//数据库表
				"  and   t.table_name like         @table_ename   || '%' "
				"  and   t.comments   like '%' ||  @table_cname   || '%' "
				"  order by t.table_name "
				;

		}



		//信息初始化语句+ WHERE 语句。 
		Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);

		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("table_ename", v_table_ename);//表名称代码
		cmd_sql.Parameters.Set("table_cname", v_table_cname); //表名称说明 
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
		cmd_sql.ExecuteQuery(bcls_ret->Tables[0]);
		cmd_sql.Close();






		//返回的记录数。 
		fetchRowCount = bcls_ret->Tables[0].Rows.get_Count();
		/*设置系统返回参数*/
		sprintf(s.msg, "查询到[%02d]条记录。", fetchRowCount);



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


