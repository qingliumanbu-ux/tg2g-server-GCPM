/*============================================================================*/
/*== [service名  ]:  gcpm_tele          ||  [对应VC#画面 ]:GCPMSI00          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2022/12/9 16:56:42=*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： xx                                                 ==*/
/*== [调用函数   ]： 无				                                          ==*/
/*== [service功能]： GCPM_电文发送测试                                  ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h" 

 
////广钢电文如下：==ON 2022/12/9 17:03:57
////=================
//5152L1	准发资源发送
//5152L2	合同变更封锁信息
//5152L3	合同生产结案信息
//5152L4	现货资源信息
//5152L5	产成品库位变更信息

int f_cm_5152L1_snd(EIClass *bcls_rec, EIClass *bcls_ret, CDbConnection * conn);
int f_cm_5152L2_snd(EIClass *bcls_rec, EIClass *bcls_ret, CDbConnection * conn);
int f_cm_5152L3_snd(EIClass *bcls_rec, EIClass *bcls_ret, CDbConnection * conn);
int f_cm_5152L4_snd(EIClass *bcls_rec, EIClass *bcls_ret, CDbConnection * conn);
int f_cm_5152L5_snd(EIClass *bcls_rec, EIClass *bcls_ret, CDbConnection * conn);


/*<remark>=========================================================
/// <summary>
///  GCPM_电文发送测试
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpm_tele)

int f_gcpm_tele(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpm_tele";                //定义函数英文名称  
	CString FunctionCname = "GCPM_电文发送测试";              //定义函数中文名称 


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
		CString    c_sql_where      = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		  
		//==查询条件信息。
		//工程项目的各种电文发送测试。
		//=========================
		//CString TC_NO;   //电文号。
	  //CString CONFM_PLAN_NO;   //准发计划号。
	  
		  
		  //从1#BLK 中获取 电文号， 关键列等。
		CString v_tc_no = "";
		if (bcls_rec->Tables[0].Columns.Contains("TC_NO"))
			v_tc_no = bcls_rec->Tables[0].Rows[0]["TC_NO"].ToString();

		CString v_confm_plan_no = "";
		if (bcls_rec->Tables[0].Columns.Contains("CONFM_PLAN_NO"))
			v_confm_plan_no = bcls_rec->Tables[0].Rows[0]["CONFM_PLAN_NO"].ToString(); 
			
		Log::Trace("", __FUNCTION__, "in ==v_tc_no[{0}]  ", v_tc_no);
		Log::Trace("", __FUNCTION__, "in ==v_confm_plan_no[{0}]  ", v_confm_plan_no);
 
		////广钢电文如下：==ON 2022/12/9 17:03:57
		////=================
		//5152L1	准发资源发送
		//5152L2	合同变更封锁信息
		//5152L3	合同生产结案信息
		//5152L4	现货资源信息
		//5152L5	产成品库位变更信息

		if (v_tc_no.Trim().ToUpper() == "5152L1")
		{
			doFlag = f_cm_5152L1_snd(bcls_rec, bcls_ret, conn);
			if (doFlag != 0)
			{
				throw CApplicationException(-1, s.msg, s.svc_name);
			}
		}
		 


		 

		 

		/*设置系统返回参数*/ 
		sprintf(s.msg, "恭喜 ， 处理成功。");
		  
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

	//将来可能要拆service处理，SO ，此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;

}