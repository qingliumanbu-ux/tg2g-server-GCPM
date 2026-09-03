/*========================================================================*/
/*== [service名  ]:  f_edsetcustominfo_mm   ||  [对应VC#画面 ]:  ALL          ==*/
/*== [程序编制人 ]:  张颖                   ||  [程序定稿日期]:2012-1-11 13:25:12==*/ 
/*== [程序修改人 ]：                        ||  [程序修改日期]:               ==*/
/*========================================================================*/
/*== [数据库表   ]： tpmof03                                            ==*/
/*== [调用函数   ]： 无				                                          ==*/
/*== [service功能]： PM模块_MM跟踪抛帐接口参数设定                      ==*/
/*========================================================================*/ 
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h"  
 
 

/*<remark>=========================================================
/// <summary>
///  MM模块_MM跟踪抛帐接口参数设定
/// <para>
///    功能叙述段落
//   

/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/

BM2_FUNCTION_EXPORT
int f_edsetcustominfo_mm(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);  // 系统日志，必须在代码段开始处定义 

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_edsetcustominfo_mm";                //定义函数英文名称  
	CString FunctionCname = "MM模块_MM跟踪抛帐接口参数设定";              //定义函数中文名称
	 

	//程序用变量
	int   doFlag = 0;
	int   fetchRowCount = 0;
	int   i = 0;
	int   j = 0;

	CString sqlstr = "";


	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  function_id = "OMOM00B2_INS" ;  //自定义显示项目功能号    

	//v_event_id
	//v_mat_kind
	CString  v_event_id     = ""; 
	CString  v_mat_kind     = ""; 
	CString  v_mat_line_type= "";
	CString  v_table_name     = ""; 


	CString v_item_ename = "";
	CString v_item_cname = "";

	CString v_update = "";  //修改的字段信息
	CString v_condi  = "";  //过滤的字段信息。

	CString v_func_id = ""; 
	CDecimal v_cnt = 0; 

	try
	{  

		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where      = "   "; //查询条件。
		CString  c_sql_condition  =  " SELECT  t.* FROM tmm0099 t  "
			" where t.event_id        = @event_id  "
			" and   t.event_line_type = @event_line_type  "
			" and   t.item_kind       = '0'  "
			" order by t.seq_no " ; 


		if(bcls_rec->Tables[0].Columns.Contains("EVENT_ID"))
		{
			v_event_id	= bcls_rec->Tables[0].Rows[0]["EVENT_ID"].ToString().TrimOrBlank();				 
		}
		if(bcls_rec->Tables[0].Columns.Contains("MAT_KIND"))
		{
			v_mat_kind	= bcls_rec->Tables[0].Rows[0]["MAT_KIND"].ToString().TrimOrBlank();				 
		}
		if(bcls_rec->Tables[0].Columns.Contains("MAT_LINE_TYPE"))
		{
			v_mat_line_type	= bcls_rec->Tables[0].Rows[0]["MAT_LINE_TYPE"].ToString().TrimOrBlank();				 
		}

		if(bcls_rec->Tables[0].Columns.Contains("BLK_NAME"))
		{
			v_table_name	= bcls_rec->Tables[0].Rows[0]["BLK_NAME"].ToString().TrimOrBlank();				 
		}


		if(v_event_id.Trim() == "")
		{
			sprintf(s.msg,"MM抛帐[事件号]不允许为空");
			throw CApplicationException(-1, s.msg, log.Location);
		}
		if(v_mat_kind.Trim() == "")
		{
			sprintf(s.msg,"MM抛帐[物料种类]不允许为空");
			throw CApplicationException(-1, s.msg, log.Location);
		}
		if(v_mat_line_type.Trim() == "")
		{//若没有获取物料的产线类型，那么默认=00=通用
			v_mat_line_type = "00"; //00=通用的产线类型。
		}

		//MM0099==MM总函数 ,在RET中创建指定块信息。
		if( v_table_name.Trim() == "")
		{
			v_table_name = "MM0099";
		}	


		if(!bcls_ret->Tables.Contains(v_table_name))
		{//若没有，则新增TABLE。
			bcls_ret->Tables.Add(v_table_name);          //MM信息接收的TABLE。  
		} 
		//清空指定TABLE的结构信息。 
		bcls_ret->Tables[v_table_name].Clear();


		Log::Trace("",__FUNCTION__,"v_event_id =[{0}] v_mat_kind =[{1}] v_mat_line_type=[{2}]"
			,v_event_id,v_mat_kind,v_mat_line_type);





		


		//item_kind ==存放字段类型=S/N=字符/数字
		 
		//event_line_type =00==说明是通用产线的。
		//event_line_type = SM =说明是给炼钢产线的定义接口。
		//===和LJH讨论后，确定如上：===update by zy on 2014-7-22 17:08:25
		//===和LJH讨论后，启用接口参数字段：[item_para] ==update by zy on 2014-8-6 14:00:50
		c_sql_where = " where t.event_id        = @event_id    "
			"           and   t.mat_kind        = @mat_kind "
			"           and   t.event_line_type = @event_line_type   " 
			"           and   t.item_para       = 'Y' " //==是接口字段。
			; 
		//设置SQL中的变量
		cmd_sql.Parameters.Set("event_id", v_event_id);
		cmd_sql.Parameters.Set("mat_kind", v_mat_kind); //物料种类。
		cmd_sql.Parameters.Set("event_line_type", v_mat_line_type);//物料产线类型。 

		v_cnt = 0;
		//根据[事件号]+事件产线类型+启用标志，创建对应的参数列。
		c_sql_condition = " SELECT COUNT(1) FROM tmm0099 t  ";  
		c_sql_condition =c_sql_condition + c_sql_where;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句 
		v_cnt = cmd_sql.ExecuteScalar(); //执行读取 
		cmd_sql.Close(); //关闭游标
		if(v_cnt <= 0)
		{ 
			//若根据event_line_type的查询条件，那么获取通用产线的事件信息。
			//=============
			v_mat_line_type = "00"; 
			v_cnt = 0; 
			cmd_sql.Parameters.Set("event_line_type", v_mat_line_type);//物料产线类型。 
			cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句
			v_cnt = cmd_sql.ExecuteScalar(); //执行读取 
			cmd_sql.Close(); //关闭游标 

			Log::Trace("", __FUNCTION__, "2222==v_event_id =[{0}] v_mat_kind =[{1}] v_mat_line_type=[{2}]"
				, v_event_id, v_mat_kind, v_mat_line_type);

			if(v_cnt <= 0)
			{  
				sprintf(s.msg,"根据事件号[%s]物料种类[%s]产线类型[%s]没有获取对应的事件参数信息，请及时维护。"
					,(const char*)v_event_id,(const char*)v_mat_kind,(const char*)v_mat_line_type);
				throw CApplicationException(-1, s.msg, log.Location); 
			} 

		} 


		//根据[事件号]+事件产线类型+启用标志，创建对应的参数列。
		//获取[TMM0099]表的最新信息。
		bcls_ret->Tables[v_table_name].Clear(); //清空指定TABLE的信息CLEAR。

		c_sql_condition = " SELECT t.SEQ_NO,t.ITEM_KIND,t.ITEM_ENAME,t.ITEM_CNAME  "
			" FROM  tmm0099 t   " 
			;
		c_sql_condition =c_sql_condition + c_sql_where;

		Log::Trace("",__FUNCTION__,"c_sql_condition[{0}]  ",c_sql_condition);
		sqlstr = c_sql_condition; 

		//设置SQL中的变量
		cmd_sql.Parameters.Set("event_id", v_event_id);
		cmd_sql.Parameters.Set("mat_kind", v_mat_kind); //物料种类。
		cmd_sql.Parameters.Set("event_line_type", v_mat_line_type);//物料产线类型。 
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句

		CDecimal v_mm0099_seq_no = 0;
		CString  v_mm0099_item_kind = "";
		CString  v_mm0099_item_ename = "";
		CString  v_mm0099_item_cname = "";
		//逐行读取
		cmd_sql.ExecuteReader();
		while(cmd_sql.Read())
		{
			v_mm0099_seq_no     = cmd_sql.GetDecimal(1);
			v_mm0099_item_kind  = cmd_sql.GetString(2);//字段类型。
			v_mm0099_item_ename = cmd_sql.GetString(3);
			v_mm0099_item_cname = cmd_sql.GetString(4);

			v_item_ename = v_mm0099_item_ename.Trim().ToLower();
			v_item_cname = v_mm0099_item_cname.Trim();


			Log::Trace("",__FUNCTION__,"SEQ_NO =[{0}] ITEM_KIND =[{1}] ITEM_ENAME=[{2}] ITEM_CNAME[{3}]"
				, v_mm0099_seq_no, v_mm0099_item_kind
				, v_mm0099_item_ename, v_mm0099_item_cname);

			if (v_mm0099_item_kind.ToUpper() == "S")
			{//字符型的
				bcls_ret->Tables[v_table_name].Columns.Add(DT_STRING,v_item_ename,v_item_cname);
			}
			else
			{//非字符型的，
				bcls_ret->Tables[v_table_name].Columns.Add(DT_DECIMAL,v_item_ename,v_item_cname);
			}


		} 
		cmd_sql.Close();

		//必输项目：MM要求如下是必输项目。
		//char   mat_kind[3];                      /* 物料种类 */
		//char   mat_line_type[5];                 /* 物料产线类型 */
		v_item_ename = "MAT_KIND";
		v_item_cname = "物料种类";
		if(!bcls_ret->Tables[v_table_name].Columns.Contains(v_item_ename.ToUpper()))
		{//若不存在该字段，则进行新增操作。
			bcls_ret->Tables[v_table_name].Columns.Add(DT_STRING,v_item_ename,v_item_cname);		 
		}

		v_item_ename = "MAT_LINE_TYPE";
		v_item_cname = "物料产线类型";
		if (!bcls_ret->Tables[v_table_name].Columns.Contains(v_item_ename.ToUpper()))
		{//若不存在该字段，则进行新增操作。
			bcls_ret->Tables[v_table_name].Columns.Add(DT_STRING,v_item_ename,v_item_cname);		 
		}

		v_item_ename = "EVENT_ID";
		v_item_cname = "事件号";
		if (!bcls_ret->Tables[v_table_name].Columns.Contains(v_item_ename.ToUpper()))
		{//若不存在该字段，则进行新增操作。
			bcls_ret->Tables[v_table_name].Columns.Add(DT_STRING,v_item_ename,v_item_cname);		 
		}


		//EVENT_LINE_TYPE
		v_item_ename = "EVENT_LINE_TYPE";
		v_item_cname = "事件产线类型";
		if (!bcls_ret->Tables[v_table_name].Columns.Contains(v_item_ename.ToUpper()))
		{//若不存在该字段，则进行新增操作。
			bcls_ret->Tables[v_table_name].Columns.Add(DT_STRING, v_item_ename, v_item_cname);
		}





		strcpy(s.msg, "恭喜，处理成功。");//处理成功。  

	}




	/*捕获数据库操作异常*/
	catch (CDbException& ex)  //捕获数据库操作异常
	{
		CString str = "DB err:[" + sqlstr + "]\r\n" + ex.GetMsg();
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

	//此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;

}
