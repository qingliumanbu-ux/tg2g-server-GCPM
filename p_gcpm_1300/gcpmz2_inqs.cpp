/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2014-11-15 16:15:42
功能: 后台程序清单_查询
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h"
 
//从字符串中根据指定分隔符拆分数据
// 入口字符，分隔字符，函数是返回字符信息。
CString f_get_multi_value(CString v_in_str, CString v_spilit_flag, CDbConnection * conn);

/*<remark>=========================================================
/// <summary>
/// 后台程序清单_查询
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz2_inqs)

int f_gcpmz2_inqs(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz2_inqs";                //定义函数英文名称  
	CString FunctionCname = "后台程序清单_查询";              //定义函数中文名称
	//LogTrace(1, 1, " **************%s begin*****************", (const char*)FunctionEname);


	//程序用变量
	int   fetchRowCount = 0;
	int   doFlag = 0; 

	CString v_code_name      = ""; 
	int     v_total_count    = 0; 
	CString sqlstr = "";  //SQL 信息。 


	try
	{



		 
		 
		
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //查询条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //查询条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		 
		
 
		CString v_moid = "";   //一级代码
		CString v_srv_name = "";   //服务名称  

		if (bcls_rec->Tables[0].Columns.Contains("MOID"))
			v_moid = bcls_rec->Tables[0].Rows[0]["MOID"].ToString().Trim();

		

		if (bcls_rec->Tables[0].Columns.Contains("SRV_NAME"))
			v_srv_name = bcls_rec->Tables[0].Rows[0]["SRV_NAME"].ToString().Trim();

		 

		Log::Trace("", __FUNCTION__, "v_moid[{0}]  ", v_moid);
		Log::Trace("", __FUNCTION__, "v_srv_name[{0}]  ", v_srv_name);

		//WHERE条件的拼接处理
		//=========
		c_sql_where = " and 1 = 1 ";
		if (v_moid.Trim() != "")
		{
			c_sql_where += " and   t.sub_system_ename like @sub_system_ename || '%' "; //二级模块.	
		}

		////二级模块范围==v_moid 
		////=================== 
		//CString v_tmp = "";
		//if (v_moid.Trim() != "")
		//{
		//	//根据指定分隔符，拆分字符信息。
		//	v_tmp = f_get_multi_value(v_moid, ",", conn);

		//	if (v_tmp.Trim() != "")
		//	{//返回的信息，不为空。

		//		//进行字符拆分处理。 ===f_get_multi_value() 
		//		c_sql_where += " AND t.sub_system_ename in ( " + v_tmp + " ) ";

		//	}

		//}

		

/* 程序名称，程序描述，程序类型，二级模块，service号。
select t.svc_name,t.program_desc,t.func_type,t.sub_system_ename,a.srv_id
,t.* 
from tea03 t,tea01 a
where t.sub_system_ename in ( select aa.code_desc_3_content from tgcpmsi00 aa where aa.CODE_CLASS = 'GCPP' and aa.code_desc_1_content = '1')
and   t.func_type in ('S','F','H') --service,函数，头文件
and   t.svc_name = a.svc_name(+) --以TEA03中的数据为准。
order by t.sub_system_ename,t.func_type desc,a.srv_id,t.svc_name
;

*/
		//select * from tmp left join tmp1  on tmp.a=tmp1.b
		//=========以上是通用的左 JOIN 
       

		//后台程序。
		//c_sql_condition = " select t.svc_name,t.program_desc,t.func_type,t.sub_system_ename,t.code_line,a.srv_id "
		//	"  from tea03 t, tea01 a " 
		//	"  where t.sub_system_ename in "
		//	"  ( select aa.code_desc_3_content from tgcpmsi00 aa  "
		//	"    where  aa.CODE_CLASS = 'GCPP'                  " //GCPP小代码控制的二级模块。
		//	"    and    aa.code_desc_4_content = '1' )          "
		//	" and   t.func_type in('S', 'F', 'H') " //--service, 函数，头文件
		//	" and   t.svc_name = a.svc_name(+)    " //--以TEA03中的数据为准。 "
		//	" and   t.sub_system_ename like @sub_system_ename || '%' " //一级模块.
		//	" and   t.svc_name like @svc_name || '%'  " //==程序名称
		//	" order by t.sub_system_ename,t.func_type desc,a.srv_id,t.svc_name ";
		//	;

			//分数据库的逻辑处理
			//==========
			switch (conn->DatabaseKind)
			{
			case DB_KIND_MSSQL:
				//c_sql_condition = " SELECT  t.* FROM ted21 t  where t.seq_name = @seq_name    ";
				break;
			case DB_KIND_ORACLE:
				//" and   t.svc_name = a.svc_name(+)    " //--以TEA03中的数据为准。 "
				c_sql_condition = " select t.* " // t.svc_name,t.program_desc,t.func_type,t.sub_system_ename,t.code_line
					",a.srv_id "
					"  from tea03 t, tea01 a "
					"  where 1=1 "
					//"  and t.sub_system_ename in "
					//"  ( select aa.code_desc_3_content from tgcpmsi00 aa  "
					//"    where  aa.CODE_CLASS = 'GCPP'                  " //GCPP小代码控制的二级模块。
					//"    and    aa.code_desc_4_content = '1' )          "
					" and   t.func_type in('S', 'F', 'H','B') " //--service, 函数，头文件,批处理
					" and   t.svc_name = a.svc_name(+)    " //--以TEA03中的数据为准。 "
					" and   t.svc_name like @svc_name || '%'  " //==程序名称
					//" and   t.sub_system_ename like @sub_system_ename || '%' " //一级模块.					
					//" order by t.sub_system_ename,t.func_type desc,a.srv_id,t.svc_name ";
				    ; 

				break;
			case DB_KIND_DB2://db2 的左链接
				//select * from tmp left join tmp1  on tmp.a=tmp1.b---以 tmp表数据为准。
				c_sql_condition = " select  t.* " //t.svc_name,t.program_desc,t.func_type,t.sub_system_ename,t.code_line
					" ,a.srv_id "
					"  from  tea03 t left join tea01 a   on t.svc_name = a.svc_name " //--以TEA03中的数据为准。 
					"  where 1=1 "
					//"  and   t.sub_system_ename in "
					//"       ( select aa.code_desc_3_content from tgcpmsi00 aa  "
					//"         where  aa.CODE_CLASS = 'GCPP'                  " //GCPP小代码控制的二级模块。
					//"         and    aa.code_desc_4_content = '1' )          "
					" and   t.func_type in('S', 'F', 'H','B') " //--service, 函数，头文件,批处理
					////" and   t.svc_name = a.svc_name(+)    " //--以TEA03中的数据为准。 "
					"  and   t.svc_name like @svc_name || '%'  " //==程序名称
					//"  and   t.sub_system_ename like @sub_system_ename || '%' " //一级模块.				
					//"  order by t.sub_system_ename,t.func_type desc,a.srv_id,t.svc_name ";
				   ; 
				
				break;
			default:
				c_sql_condition = "";
				break;
			}
			c_order_by = "  order by t.sub_system_ename,t.func_type desc,a.srv_id,t.svc_name ";
			c_sql_condition = c_sql_condition + c_sql_where + c_order_by;
		 

		 
		//信息初始化语句+ WHERE 语句。 
		Log::Trace("",__FUNCTION__,"c_sql_condition[{0}]  ",c_sql_condition);
		 

 

		 

		 
		fetchRowCount = 0;
		sqlstr = c_sql_condition;
		//信息初始化条件准备。
		cmd_sql.Parameters.Set("sub_system_ename", v_moid.ToUpper());// 
		cmd_sql.Parameters.Set("svc_name", v_srv_name);//后台服务 
		cmd_sql.SetCommandText(c_sql_condition); 
		cmd_sql.ExecuteQuery(bcls_ret->Tables[0]); 
		cmd_sql.Close();  


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
