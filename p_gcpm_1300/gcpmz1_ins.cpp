/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2014-10-22 10:52:48
功能: 功能清单明细_新增
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
/// 功能清单明细_新增
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz1_ins)

int f_gcpmz1_ins(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__); 

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz1_ins";                //定义函数英文名称  
	CString FunctionCname = "功能清单明细_新增";              //定义函数中文名称
	 

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
	CString  v_dllname = "";


	try
	{



		 
	CModel tgcpmz1("TGCPMZ1");
		
		
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //新增条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //新增条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		 

		/*根据计划类型+库区，获取对应的流水号*/
		CString v_seq_name =  ""; //序号名称="GCPMZ1_"+ 二级模块 
		

		
		 
		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{

			tgcpmz1.Reset(); //为了保证数据的准确性，暂定加个 RESET();
			tgcpmz1.MergeFrom(bcls_rec->Tables[0].Rows[i]); //前台传入的数据。
			tgcpmz1["REC_CREATOR"] = userid;     //创建者
			tgcpmz1["REC_CREATE_TIME"] = dateNow;  //创建日期
			tgcpmz1["REC_REVISE_TIME"] = "";
			tgcpmz1["REC_REVISOR"] = "";
			tgcpmz1.TrimOrBlank();


			Log::Trace("", __FUNCTION__, "第[{0}]个，tgcpmz1.SRV_NAME. =[{1}]画面代码[{2}]功能按钮[{3}]  "
				, i + 1, tgcpmz1["SRV_NAME"].ToString(), tgcpmz1["FORM_CODE"].ToString(), tgcpmz1["FUNC_ID"].ToString());

			v_dllname = tgcpmz1["DLLNAME"];
			/*if (v_dllname.GetLength() >= 4)
			{
				v_dllname = v_dllname.Substring(0, 4);
			} */

			//根据DLL ，进行对应责任者的校验，确认有权限，才允许操作。
			//=============================
			/*
			 select t.code,t.code_desc_2_content,t.* from tgcpmsi00 t
			 where t.code_class = 'GCPP';
			 */
			//根据 DLL ,当前用户的权限校验。 
			v_cnt = 0;
			c_sql_condition = "select count(1) from tgcpmsi00 t "
				" where t.code_class          ='GCPP' " 
				" and   t.code_desc_4_content = '1' "//启用的DLL
				" and   t.code                = @code " //指定的DLL
				" and    ( t.code_desc_2_content || ','  LIKE  '%' || @code_desc_2_content || ',%' or t.code_desc_2_content = ' ' )   "  //在[责任者]要求范围内。
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("code", v_dllname);//指定的DLL
			cmd_sql.Parameters.Set("code_desc_2_content", userid);//责任者。 
			cmd_sql.SetCommandText(c_sql_condition);
			v_cnt = cmd_sql.ExecuteScalar();
			cmd_sql.Close();

			if (v_cnt <= 0)
			{//若没有找到记录，则说明当前用户，不能进行当前DLL 的操作。

				sprintf(s.msg, "您的帐号[%s]没有DLL[%s]的操作权限，当前操作失败。"
					, (const char*)userid, (const char*)v_dllname);
				throw CApplicationException(-1, s.msg, log.Location);
			}


			


			////新增校验，若已经存在，则不新增，继续下一个记录。
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

				Log::Trace("", __FUNCTION__, "dllname[{0}]form_code[{2}]func_id[{3}]srv_name[{4}] ,数据已经存在，不新增，继续下个 "
					, tgcpmz1["DLLNAME"].ToString(), tgcpmz1["FORM_CODE"].ToString(), tgcpmz1["FUNC_ID"].ToString(), tgcpmz1["SRV_NAME"].ToString());
				continue;
			}


			/*
			CString SRV_NAME;   //后台服务名称==service名称
			CString SRV_FUNC_DESC;   //后台服务功能描述==service 描述[包含下一级函数]
			CString SRV_ID;   //后台SERVER标识 ==service号。
			 
			*/

			//初始化 后台程序描述，后台service号。
			//==================== 
			//暂定：维护service 的详细信息，放在修改点。
			//SO,以下信息都做初始化处理。
			//=======================================
			tgcpmz1["SRV_FUNC_DESC"] = ""; //功能描述
			tgcpmz1["SRV_ID"] = "";//若是service要获取对应的service 号。

			 


			//CString TRACK_SEQ_NO;   //事件跟踪序列号
			//CDecimal SEQ_NO;   //序号 
			//显示序号的处理，取‘按钮’信息的F后面，
			//比如 F2,序号= 2； F12,序号= 12
			CString v_tmp = "";  
			//若功能号是F 开头的，说明是按钮，则，将按钮的流水，转换成对应的数字，进行排序。
			//============CString FUNC_ID;   //功能标识
			if (tgcpmz1["FUNC_ID"].ToString().Substring(0,1) == "F")
			{
				
				v_tmp = tgcpmz1["FUNC_ID"].ToString().Replace("F", "0"); 

				try
				{
					tgcpmz1["SEQ_NO"] = CDecimal::Parse(v_tmp); //字符变成数字。
				}
				catch (CException& ex)
				{////若不是合理的数字，则强制变成99。
					Log::Trace("", __FUNCTION__, "数字合法性校验失败，v_tmp[{0}]", v_tmp);
					//若不是合理的数字，则强制变成99。
					tgcpmz1["SEQ_NO"] = 99;
				}

			 


				
			}
			else
			{//若不是按钮按钮信息，非按钮信息，排最后。SEQ_NO = 99
				tgcpmz1["SEQ_NO"] = 99;
			}
			 
		
			
			//若有DLL信息，二级模块= DLL 的前4位信息。
			if (tgcpmz1["DLLNAME"].ToString().GetLength() >= 4)
			{
				tgcpmz1["MOID"] = tgcpmz1["DLLNAME"].ToString().Substring(0, 4).ToUpper();
			}
			
			//一般索引。。非唯一主键。
			//一级模块[2]+yyyyMMddHHmmssfff[17]
			tgcpmz1["TRACK_SEQ_NO"] = tgcpmz1["MOID"].ToString().Substring(0,2) + CDateTime::Now().ToString("yyyyMMddHHmmssfff");
			 
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



