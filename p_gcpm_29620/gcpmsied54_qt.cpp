/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2015-2-4 11:10:43
功能: 根据ED54的配置信息_其他业务。
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/
// New Include
#include "stdafx.h" 


/*<remark>=========================================================
/// <summary>
///  根据ED54的配置信息_其他业务。
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/

// service入口
BM2F_ENTERACE(gcpmsied54_qt)
//-EP_SYSTEM_HEAD_END                                                  
int f_gcpmsied54_qt(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);  // 系统日志，必须在代码段开始处定义

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsied54_qt";                //定义函数英文名称  
	CString FunctionCname = "根据ED54的配置信息_其他业务。";              //定义函数中文名称
	////LogTrace(1, 1, " **************%s begin*****************", (const char*)FunctionEname);


	//程序用变量
	int   doFlag = 0;
	int   i = 0;
	int   fetchRowCount = 0;
	int   v_cnt = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  function_id = "gcpmsied54_qt";  //自定义显示项目功能号    
	CString  sqlstr = "";  //SQL 信息。 

	CString v_table_name = "";//表名称。
	CString v_moid = ""; //模块代码
	CString v_func_id = "";

	//获得系统时间，当前用户代码
	CString systime = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString userid = s.userid;

	//写履历表 TGCPMSI99
	//=================
	CString v_column_name = "";  //列名称们...
	CString v_column_value = ""; //列内容们...
	CString v_rec_creator2 = userid;   //记录创建责任者
	CString v_rec_create_time2 = systime;   //记录创建时刻  
	CString v_track_seq_no2 = CDateTime::Now().ToString("yyyyMMddHHmmssfff");;   //事件跟踪序列号
	CString v_table_name99 = "";   //数据库表名
	CString v_event_id2 = "DEL";   //事件标识
	CString v_error_remark2 = "";   //备注==表中的每个列信息拼接起来。比如： [工序]A1[工序类型]SM11...



	try
	{
		
		//暂定，其他业务都需要定制service支持。
		//本Service只做‘提示处理’。
		//=========================
		doFlag = -1;
		s.flag = doFlag;
		sprintf(s.msg,"本按钮是‘其他业务’不支持通用service，\n请到画面【GCPMSI01】中及时维护对应的service。");
		return doFlag;


		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString  c_sql_orderBy   = "";

		 
		 
   

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

	//将来可能要拆service处理，SO ，此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;


}