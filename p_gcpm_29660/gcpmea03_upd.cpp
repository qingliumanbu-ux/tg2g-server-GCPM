/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2016-7-1 13:41:20
功能: SERVICE帐号批量修改_修改
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h"


//从字符串中根据指定分隔符拆分成数组返回
int f_get_multi_value2(CString v_in_str, CString v_spilit_flag, CString *v_out_str,int *v_out_str_cnt, CDbConnection* conn);

/*<remark>=========================================================
/// <summary>
/// SERVICE帐号批量修改_修改
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmea03_upd)

int f_gcpmea03_upd(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmea03_upd";                //定义函数英文名称  
	CString FunctionCname = "SERVICE帐号批量修改_修改";              //定义函数中文名称
	//LogTrace(1, 1, " **************%s begin*****************", (const char*)FunctionEname);


	//程序用变量
	int   i = 0;
	int   fun_i = 0; //功能个数。
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;  
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0;
	CString  v_remark_desc_tmp = "";
	CString  v_remark_desc = "";
	CString  v_srv_func_desc = "";
	CString  v_srv_id = "";


	CString v_update = "";
	CString v_condi = "";

	//修改信息的字段拼接。
	CString v_update_tmp = "";
	CString v_update_tmp2 = "";
	CString  v_dllname = "";

	//servcie的失效标志
	CString v_code_line = "";
	CString v_false = "";


	try
	{ 
		 
	CModel tgcpmea03("TGCPMEA03");
		
		
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " ORDER BY t.XX ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		 
		CString  v_srv_name = "";//后台服务信息。
		CString  v_srv_name2[20] = {""}; //字符型数组。存放多个service信息。

		CString v_out_str[200];
		int     v_out_str_cnt = 0;
		CString v_out_str_tmp = "";
		int ii  =0;
		CString v_form_name_chk = ""; //用于校验的画面代码。
		CString v_out_form_name[200] = { "" };//字符数组
		int     v_out_form_cnt = 0; //数组中的有值的个数。


		//跟BLK_NAME ,获取信息。
		//GCPMSI_BATCH_SET==批量修改。
		//GCPMSI_SET==设置参数
		//==============
		


		//CString SRV_ID;   //后台SERVER标识
		CString v_srv_id = "";   //后台SERVER标识
		CString v_program_maker = ""; //程序责任者。

		CString v_program_path;   //程序路径

		CString v_blk_name = "GCPMSI_SET"; 
		if (!bcls_rec->Tables.Contains(v_blk_name))
		{ 
			sprintf(s.msg, "表信息获取失败，表名称[%s]", (const char*)v_blk_name);//  
			throw CApplicationException(-1, s.msg, log.Location);	 
		}

		if (bcls_rec->Tables[v_blk_name].Columns.Contains("SRV_ID"))
			v_srv_id = bcls_rec->Tables[v_blk_name].Rows[0]["SRV_ID"].ToString().TrimOrBlank();

		if (bcls_rec->Tables[v_blk_name].Columns.Contains("PROGRAM_MAKER"))
			v_program_maker = bcls_rec->Tables[v_blk_name].Rows[0]["PROGRAM_MAKER"].ToString().TrimOrBlank();

		Log::Trace("", __FUNCTION__, "in ==v_srv_id[{0}]", v_srv_id);
		Log::Trace("", __FUNCTION__, "in ==v_program_maker[{0}]", v_program_maker);

		//if (v_srv_id.Trim() == "")
		//{
		//	sprintf(s.msg,"[新]SERVICE号 不允许为空。");
		//	throw CApplicationException(-1, s.msg, FunctionEname);
		//}


		v_blk_name = "GCPMSI_BATCH_SET";
		if (!bcls_rec->Tables.Contains(v_blk_name))
		{
			sprintf(s.msg, "表信息获取失败，表名称[%s]", (const char*)v_blk_name);//  
			throw CApplicationException(-1, s.msg, log.Location);
		}
		for (i = 0; i < bcls_rec->Tables[v_blk_name].Rows.get_Count(); i++)
		{
			tgcpmea03.Reset(); //为了保证数据的准确性，暂定加个 RESET();
			tgcpmea03.MergeFrom(bcls_rec->Tables[v_blk_name].Rows[i]); //前台传入的数据。 
			//CString SVC_NAME;   //后台SERVICE名称
			Log::Trace("", __FUNCTION__, "in ==SVC_NAME[{0}]", tgcpmea03["SVC_NAME"].ToString());


			//若前台设定了内容，
			//则以前台设定区域的信息为准，进行修改。
			//=========================
			if (v_srv_id.Trim() != "")
			{
				tgcpmea03["SRV_ID"] = v_srv_id;
			}

			//若前台设定了，责任者，则修正责任者。
			if (v_program_maker.Trim() != "")
			{
				tgcpmea03["PROGRAM_MAKER"] = v_program_maker;
			}

			/*Log::Trace("", __FUNCTION__, "修改内容 ==tgcpmea03["SRV_ID"].ToString()[{0}]", tgcpmea03["SRV_ID"].ToString());
			Log::Trace("", __FUNCTION__, "修改内容 ==tgcpmea03["PROGRAM_MAKER"].ToString()[{0}]", tgcpmea03["PROGRAM_MAKER"].ToString());
			 
			*/ 
			c_sql_condition = "update tgcpmea03 t "
				" set   t.srv_id        = @srv_id  " //新 SERVICE号。
				"      ,t.program_maker = @program_maker "
				" where t.svc_name      = @svc_name "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("srv_id", tgcpmea03["SRV_ID"].ToString());
			cmd_sql.Parameters.Set("program_maker", tgcpmea03["PROGRAM_MAKER"].ToString());//程序责任者。
			cmd_sql.Parameters.Set("svc_name", tgcpmea03["SVC_NAME"].ToString());  
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();  
			 
		}  


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

	//此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;


}

