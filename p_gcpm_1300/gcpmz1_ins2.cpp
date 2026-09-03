/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2014-11-26 13:01:15
功能: 功能清单明细_后台程序新增
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/
// New Include
#include "stdafx.h"



/******定义调用的函数******/


/*<remark>=========================================================
/// <summary>
/// 功能清单明细_后台程序新增
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz1_ins2)

int f_gcpmz1_ins2(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);


	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz1_ins2";                //定义函数英文名称  
	CString FunctionCname = "功能清单明细_后台程序新增";              //定义函数中文名称
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


	try
	{




		CModel tgcpmz1("TGCPMZ1");


		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where = "  WHERE   1 = 1 "; //后台程序新增条件。
		CString    c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString    c_order_by = " order by t.order_no ";


		/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2 = "  WHERE   1 = 1 "; //后台程序新增条件。
		CString    c_sql_condition2 = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";



		/*根据计划类型+库区，获取对应的流水号*/
		CString v_seq_name = ""; //序号名称="GCPMZ1_"+ 二级模块 




		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{

			tgcpmz1.Reset(); //为了保证数据的准确性，暂定加个 RESET();
			tgcpmz1.MergeFrom(bcls_rec->Tables[0].Rows[i]); //前台传入的数据。
			tgcpmz1["REC_CREATOR"] = userid;     //创建者
			tgcpmz1["REC_CREATE_TIME"] = dateNow;  //创建日期
			tgcpmz1["REC_REVISE_TIME"] = "";
			tgcpmz1["REC_REVISOR"] = "";


			Log::Trace("", __FUNCTION__, "第[{0}]个，tgcpmz1.FUNC_CNAME =[{1}]画面代码[{2}]  "
				, i + 1, tgcpmz1["FUNC_CNAME"].ToString(), tgcpmz1["FORM_CODE"].ToString());

			//CString FUNC_CNAME;   //功能名称

			if (tgcpmz1["FUNC_CNAME"].ToString().Trim() == "")
			{//后台程序新增的时候， [功能名称]不能为空。
				sprintf(s.msg, "[功能名称]不允许为空。");
				throw CApplicationException(-1, s.msg, FunctionEname);
			}

			tgcpmz1["SRV_NAME"] = tgcpmz1["FUNC_CNAME"];

			////后台程序新增校验，若已经存在，则不后台程序新增，继续下一个记录。?
			////==========  

			//CString DLLNAME;   //调用名==K          ==动态库文件
			//CString FORM_CODE;   //画面编号 ==k     ==画面代码
			//CString FUNC_ID;     //功能标识 ==k     ==按钮代码
			//CString SRV_NAME;    //后台服务名称 ==K ==后台程序名称

			//先校验，若已经存在，则继续下一个。
			//===================
			v_cnt = 0;
			c_sql_condition = "select count(1) from tgcpmz1 t "
				" where t.dllname   = @dllname "
				" and   t.form_code = @form_code "
				" and   t.func_id   = @func_id "
				" and   t.srv_name  = @srv_name "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("dllname", tgcpmz1["DLLNAME"].ToString());//DLL
			cmd_sql.Parameters.Set("form_code", tgcpmz1["FORM_CODE"].ToString());//画面代码。
			cmd_sql.Parameters.Set("func_id", tgcpmz1["FUNC_ID"].ToString());//功能标识。
			cmd_sql.Parameters.Set("srv_name", tgcpmz1["SRV_NAME"].ToString());//后台。
			cmd_sql.SetCommandText(c_sql_condition);
			v_cnt = cmd_sql.ExecuteScalar();
			cmd_sql.Close();

			if (v_cnt >= 1)
			{//若根据4大信息，数据已经存在，则继续下一个。

				Log::Trace("", __FUNCTION__, "dllname[{0}]form_code[{2}]func_id[{3}]srv_name[{4}] ,数据已经存在，不后台程序新增，继续下个 "
					, tgcpmz1["DLLNAME"].ToString(), tgcpmz1["FORM_CODE"].ToString(), tgcpmz1["FUNC_ID"].ToString(), tgcpmz1["SRV_NAME"].ToString());
				continue;
			}


			/*
			CString SRV_NAME;   //后台服务名称==service名称
			CString SRV_FUNC_DESC;   //后台服务功能描述==service 描述
			CString SRV_ID;   //后台SERVER标识 ==service号。
			CString REMARK_DESC;   //备注说明1==调用的[一级]函数信息.
			*/

			//初始化 后台程序描述，后台service号。
			//==================== 
			tgcpmz1["SRV_FUNC_DESC"] = ""; //功能描述
			tgcpmz1["SRV_ID"] = "";//若是service要获取对应的service 号。


			//如有后台service信息。
			if (tgcpmz1["SRV_NAME"].ToString().Trim() != "")
			{//若有service信息，则获取对应其他信息。

				/*
				service 描述，service号。
				select  t.program_desc, a.srv_id from tea03 t,tea01 a
				where t.svc_name = 'pmog00_inq'
				and   t.svc_name = a.svc_name
				;
				*/

				//因为函数没有对应的TEA01表信息，SO ,此处要分开处理。 

				/*c_sql_condition = "select  t.program_desc, a.srv_id  "
					" from tea03 t,tea01 a "
					" where t.svc_name  = @svc_name "
					" and   t.svc_name = a.svc_name "
					;*/



				//功能描述
				//select t.svc_name,t.sub_system_ename,t.* from tea03 t
				c_sql_condition = "select  t.program_desc,t.sub_system_ename   "
					" from tea03 t  "
					" where t.svc_name  = @svc_name "
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("svc_name", tgcpmz1["SRV_NAME"].ToString());//后台。
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteReader();
				if (cmd_sql.Read())
				{
					tgcpmz1["SRV_FUNC_DESC"] = cmd_sql.GetString(1);
					tgcpmz1["DLLNAME"] = cmd_sql.GetString(2) + ".DLL";
				}
				cmd_sql.Close();



				//service号。
				c_sql_condition = "select  t.srv_id   "
					" from tea01 t  "
					" where t.svc_name  = @svc_name "
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("svc_name", tgcpmz1["SRV_NAME"].ToString());//后台。
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteReader();
				if (cmd_sql.Read())
				{
					tgcpmz1["SRV_ID"] = cmd_sql.GetString(1);
				}
				cmd_sql.Close();

				Log::Trace("", __FUNCTION__, "tgcpmz1.SRV_NAME. =[{0}] SRV_FUNC_DESC =[{1}]servide号"
					, tgcpmz1["SRV_NAME"].ToString(), tgcpmz1["SRV_FUNC_DESC"].ToString(), tgcpmz1["SRV_ID"].ToString());



				//获取service 对应的一级函数+函数名称等。
				//============
				/*
				select t.svc_name,t.func_name,a.program_desc
				--,t.*
				from tea05 t,tea03 a
				where t.svc_name = 'f_pmof99_v3'
				and   t.call_type = 'C'
				and   t.func_name = a.svc_name
				;

				CString REMARK_DESC;   //备注说明1==调用的[一级]函数信息.
				*/
				fun_i = 0;
				v_remark_desc_tmp = "";
				v_remark_desc = "";
				c_sql_condition = "select  t.func_name,a.program_desc "
					" from tea05 t,tea03 a "
					" where t.svc_name  = @svc_name "//后台程序。
					" and   t.call_type = 'C' " //下一级的调用信息。
					" and   t.FUNC_NAME LIKE 'f%' "//函数
					" and   t.FUNC_NAME NOT LIKE 'f_bm2%' " //非框架函数
					" and   t.func_name = a.svc_name "
					" order by t.func_name " //根据名称排序。
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("svc_name", tgcpmz1["SRV_NAME"].ToString());//后台。
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteReader();
				while (cmd_sql.Read()) //多个信息的循环。
				{
					fun_i++;
					v_remark_desc_tmp = "[" + cmd_sql.GetString(1) + "]" + cmd_sql.GetString(2);
					Log::Trace("", __FUNCTION__, "v_remark_desc_tmp =[{0}]", v_remark_desc_tmp);

					if (fun_i == 1)
					{
						v_remark_desc = v_remark_desc_tmp;
					}
					else
					{//从第2个开始，拼接换行符。
						v_remark_desc = v_remark_desc + "\r\n" + v_remark_desc_tmp;
						//是不是在显示之前需要进行替换，将\r\n转换成 <br/>？
						//v_remark_desc = v_remark_desc + "<br/>" + v_remark_desc_tmp;
					}


				}
				cmd_sql.Close();

				Log::Trace("", __FUNCTION__, "tgcpmz1.SRV_NAME. =[{0}] v_remark_desc =[{1}]"
					, tgcpmz1["SRV_NAME"].ToString(), v_remark_desc);
				//tgcpmz1["REMARK"]_DESC = v_remark_desc;

				//service + 下级子函数的信息，存放在以下字段中。
				//CString SRV_FUNC_DESC;   //后台服务功能描述
				if (v_remark_desc.Trim() != "")
				{
					tgcpmz1["SRV_FUNC_DESC"] = tgcpmz1["SRV_FUNC_DESC"].ToString() + "\r\n[下一级函数：]\r\n" + v_remark_desc;
				}

				//以下字段，根据前台输入的信息为准。
				//tgcpmz1["REMARK"]_DESC = "";

			}



			//CString TRACK_SEQ_NO;   //事件跟踪序列号
			//CDecimal SEQ_NO;   //序号 

			//显示序号的处理 
			tgcpmz1["SEQ_NO"] = 99;

			tgcpmz1["FORM_CODE"] = "后台";
			tgcpmz1["FUNC_ID"] = "service";
			//若是 f_开头的程序，则是‘函数’
			//===========
			if (tgcpmz1["SRV_NAME"].ToString().Substring(0, 2) == "f_")
			{
				tgcpmz1["FUNC_ID"] = "函数";
			}


			tgcpmz1["MOID"] = tgcpmz1["DLLNAME"].ToString().Substring(0, 4); //后台程序的二级模块= 动态库信息。 
			//一般索引。。非唯一主键。
			tgcpmz1["TRACK_SEQ_NO"] = tgcpmz1["MOID"].ToString().Substring(0, 2) + CDateTime::Now().ToString("yyyyMMddHHmmssfff");

			Log::Trace("", __FUNCTION__, "dllname[{0}]跟踪序列号[{1}] "
				, tgcpmz1["DLLNAME"].ToString(), tgcpmz1["TRACK_SEQ_NO"].ToString());



			////新增点，默认是‘01_构思中’。
			//CString STATUS;   //状态
			//CString STATUS_NAME;   //状态类型名称
			tgcpmz1["STATUS"] = "01";
			tgcpmz1["STATUS_NAME"] = "01_构思中";


			tgcpmz1.TrimOrBlank();
			sqlstr = "insert into tgcpmz1";
			tgcpmz1.Insert();
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


