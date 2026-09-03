/*============================================================================*/
/*== [service名  ]:  gcpmz6_inqmx       ||  [对应VC#画面 ]:GCPMSI01          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2017-5-11 16:04:34==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TTADT01                                            ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： 表TTADT01_信息查询                                 ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h"



 

/*<remark>=========================================================
/// <summary>
/// 表TTADT01_信息查询[表+字段]
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz6_inqmx)

int f_gcpmz6_inqmx(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);


	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz6_inqmx";                //定义函数英文名称  
	CString FunctionCname = "表TTADT01_信息查询";              //定义函数中文名称 


	//程序用变量
	int   i = 0;
	int   fun_i = 0; //功能个数。
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0;

	//EIClass GCPM_OUT; // 


	try
	{

	CModel tgcpmdt01("TGCPMDT01");

		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString    c_order_by = " order by t.order_no ";


		/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2 = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition2 = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";

		/* 数据库操作类定义3 */
		CDbCommand cmd_sql3(conn); //与DB 建立连接。
		CString    c_sql_where3 = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition3 = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";


		//==查询条件信息。
		//CString TABLE_NAME;   //数据库表名  

		//从1#BLK 中获取静态表的表名称。
		CString v_table_ename = "";
		if (bcls_rec->Tables[0].Columns.Contains("TABLE_ENAME"))
			v_table_ename = bcls_rec->Tables[0].Rows[0]["TABLE_ENAME"].ToString();  
		Log::Trace("", __FUNCTION__, "in ==v_table_ename[{0}]  ", v_table_ename); 


		/*===直接读取数据库的TTA定义表结构。*/ 
		/*
		ITEM_ENAME ==列代码
		ITEM_CNAME==列标题
		ITEM_TYPE==列类型
		ITEM_LEN==列长度
		TABLE_ITEM_SEQ==表内序号
		ITEM_SEQ==列序号。
		*/

		switch (conn->DatabaseKind)
		{
		case DB_KIND_ORACLE:

			//ORACLE数据库 
			c_sql_condition = "SELECT  DISTINCT t1.column_name AS item_ename"
				",t2.comments    AS item_cname"
				",t1.data_type   AS item_type "
				",t1.data_length AS item_len " //字符型长度
				",t1.DATA_PRECISION || ',' || t1.DATA_SCALE  AS item_scale  "//数字型长度。
				",t1.column_id   AS TABLE_ITEM_SEQ  "
				"FROM  ALL_TAB_COLUMNS T1 ,ALL_COL_COMMENTS T2 "
				"WHERE T1.OWNER       = T2.OWNER "
				"AND   T1.TABLE_NAME  = T2.TABLE_NAME  "
				"AND   T1.COLUMN_NAME = T2.COLUMN_NAME "
				"AND   T1.TABLE_NAME  = @tablename     "
				"ORDER BY  t1.column_id "
				;
			break;
		case DB_KIND_DB2_ORACLE:
		case DB_KIND_DB2:

			//DB2数据库。
			c_sql_condition = "SELECT t.name AS item_ename"//列代码。
				",t.remarks AS item_cname "//列标题
				",t.coltype AS item_type  "//列类型
				",t.length  AS item_len   "//列长
				",t.scale   as item_scale "//小数点位数。
				",t.colNo   AS TABLE_ITEM_SEQ   "//表内序号。
				"FROM  Sysibm.syscolumns t "
				"WHERE  1=1 "
				"AND    t.tbname = @tablename "
				"ORDER BY t.colNo ";

			break;
		case DB_KIND_MSSQL:
			c_sql_condition = "select SC.name as item_ename"
				",SC.name     as item_cname "
				",ST.name     as item_type "
				",SC.length   as item_len  "
				",SC.colorder as TABLE_ITEM_SEQ  "
				"from sysobjects SO, syscolumns SC, systypes ST "
				"where SO.id=SC.id and (SO.xtype='U' or SO.xtype='V') "
				"and SO.status >= 0 and SC.xtype=ST.xusertype "
				"and SO.name    = @tablename "
				" ORDER BY SO.name, SC.colorder ";

			break;
		default:
			//暂定是 ORACLE模式。 
			c_sql_condition = "SELECT  DISTINCT t1.column_name AS item_ename"
				",t2.comments    AS item_cname"
				",t1.data_type   AS item_type "
				",t1.data_length AS item_len " //字符型长度
				",t1.DATA_PRECISION || ',' || t1.DATA_SCALE  AS item_scale  "//数字型长度。
				",t1.column_id   AS TABLE_ITEM_SEQ  "
				"FROM  ALL_TAB_COLUMNS T1 ,ALL_COL_COMMENTS T2 "
				"WHERE T1.OWNER       = T2.OWNER "
				"AND   T1.TABLE_NAME  = T2.TABLE_NAME  "
				"AND   T1.COLUMN_NAME = T2.COLUMN_NAME "
				"AND   T1.TABLE_NAME  = @tablename     "
				"ORDER BY  t1.column_id "
				;

		}

		Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);
		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("tablename", v_table_ename);
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
		cmd_sql.ExecuteQuery(bcls_ret->Tables[0]);
		cmd_sql.Close();


		///*循环处理查询到的信息*/
		//CString v_item_scale = ""; //字段小数点。
		//int rows = bcls_ret->Tables[0].Rows.get_Count();
		//for (i = 0; i < rows; i++)
		//{
		//	//特殊列信息的处理。
		//	//===============
		//	cmd_sql.Fetch(tgcpmdt01);
		//	//压入返回BLK
		//	//===========
		//	tgcpmdt01.MergeFrom(bcls_rec->Tables[0].Rows[i]);

		//	/*获取当前处理的材料信息*/
		//	if (bcls_ret->Tables[0].Columns.Contains("item_scale"))
		//		v_item_scale = bcls_ret->Tables[0].Rows[i]["item_scale"].ToString();

		//	if (v_item_scale.Trim() != "" && v_item_scale.Trim() != "0")
		//	{//若非空，非0 ， 则


		//	}
		//}

		



		//返回的记录数。 
		fetchRowCount = bcls_ret->Tables[0].Rows.get_Count();
		/*设置系统返回参数*/ 
		sprintf(s.msg, "查询到[%d]条记录。", fetchRowCount);



		//返回表索引信息。
		//=============
		/*
		SELECT  T.INDEX_ENAME,T.TABLE_INDEX_TYPE,T.INDEX_SEQ,T.ITEM_SEQ
		--,T.*
		FROM TTADT02 T
		WHERE T.TABLE_ENAME = 'TPMOF03'
		ORDER BY T.INDEX_ENAME,T.INDEX_SEQ


		SELECT T.ITEM_ENAME,T.ITEM_CNAME
		,T.*
		FROM TTADI00 T
		WHERE T.ITEM_SEQ = '71b88521-ea00-4b8f-8802-22a65d9b350e'
		*/

		//新增一个返回的table.
		CString v_table_name = "IDX";
		bcls_ret->Tables.Add(v_table_name);


		/*
		DISTINCT aa.ITEM_ENAME,aa.ITEM_CNAME --,aa.TTA_CATALOG
,t.index_ename,t.table_index_type,t.index_seq
		*/
		c_sql_condition = " SELECT  DISTINCT aa.ITEM_ENAME,aa.ITEM_CNAME "//字段英文， 字段中文
			" ,t.index_ename,t.table_index_type,t.index_seq "//索引名称，索引类型，索引内序号
			" FROM    ttadt02 t, ttadi00 aa  "
			" WHERE   t.item_seq      = aa.item_seq    " //字段序号
			" AND     t.table_ename   =  @table_ename  " //表名称
			//" and     t.project_ename = 'BM2MES'       " //暂定是这个分区= BM2MES
			//" and    aa.TTA_CATALOG  = ' ' " //框架： 方奇说， 空格是默认主分区。
			" ORDER BY T.index_ename,T.index_seq "
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句
		// 设置SQL中的变量
		cmd_sql.Parameters.Set("table_ename", v_table_ename);//业务表英文名。
		cmd_sql.ExecuteQuery(bcls_ret->Tables[v_table_name]);
		cmd_sql.Close();



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


