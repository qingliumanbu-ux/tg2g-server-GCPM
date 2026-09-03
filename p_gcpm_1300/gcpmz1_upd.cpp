/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2014-10-22 16:12:40
功能: 功能清单明细_修改
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
/// 功能清单明细_修改
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz1_upd)

int f_gcpmz1_upd(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz1_upd";                //定义函数英文名称  
	CString FunctionCname = "功能清单明细_修改";              //定义函数中文名称
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
		 
	CModel tgcpmz1("TGCPMZ1");
		
		
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
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

		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{
			tgcpmz1.Reset(); //为了保证数据的准确性，暂定加个 RESET();
			tgcpmz1.MergeFrom(bcls_rec->Tables[0].Rows[i]); //前台传入的数据。 

			
			if (i == 0)
			{//若是第一行信息，则记录下来，写入数组。
				v_form_name_chk = tgcpmz1["FORM_CODE"];
				v_out_form_name[v_out_form_cnt] = v_form_name_chk.Trim();
			}
			else
			{//非第一行，进行比较，若不同则记录到数组中。
				if (v_form_name_chk.Trim() != tgcpmz1["FORM_CODE"].ToString().Trim())
				{
					v_out_form_cnt++;
					v_form_name_chk = tgcpmz1["FORM_CODE"];
					v_out_form_name[v_out_form_cnt] = v_form_name_chk.Trim();
				}

			}

			v_dllname = tgcpmz1["DLLNAME"];
			/*if (v_dllname.GetLength() >= 4)
			{
				v_dllname = v_dllname.Substring(0, 4);
			}*/

			//根据DLL ，进行对应责任者的校验，确认有权限，才允许操作。
			//=============================
			/*
			select t.code,t.code_desc_2_content,t.* from tgcpmsi00 t
			where t.code_class = 'GCPP';
			*/
			//根据 DLL ,当前用户的权限校验。 
			v_cnt = 0;
			c_sql_condition = "select count(1) from tgcpmsi00 t "
				" where t.code_class   ='GCPP' "
				" and   t.code_desc_4_content = '1' "//启用的DLL
				" and   t.code = @code " //指定的DLL
				" and    ( t.code_desc_2_content || ','  LIKE  '%' || @code_desc_2_content || ',%' or t.code_desc_2_content = ' ' )   "  //在[责任者]要求范围内。
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("code", v_dllname);//DLL。
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
		

			//修正==画面名称。
			//=============
			CString v_form_call_mode = "0";
			CString v_form_desc = "";
			//tgcpmz1["FORM_NAME"] = ""; 

			//画面名称 
			//"select   t.description || '\r\n' || t.name   " //画面名称+画面代码
			c_sql_condition = "select   t.description " //画面名称   //应PSSM反馈，这里只显示中文描述。
				" ,t.dllname "
				//",t.form_call_mode "//dll,调用方式 = 1=子画面
				" from  tesformresinfo t  "
				" where t.name  = @name "
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("name", tgcpmz1["FORM_CODE"].ToString());//画面代码
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteReader();
			if (cmd_sql.Read())
			{
				v_form_desc = cmd_sql.GetString(1);//画面中文描述
				tgcpmz1["DLLNAME"] = cmd_sql.GetString(2);
				 
			}
			cmd_sql.Close(); 
			tgcpmz1["FORM_NAME"] = v_form_desc;

			//修正==按钮名称。
			//==============
			/*
			CString FUNC_ID;   //功能标识
			CString FUNC_CNAME;   //功能名称
			*/
			//若没有找到，则保持不变。
			c_sql_condition = "select   t.name || '[' || t.description || ']'   " //按钮代码+按钮名称
				" from  tesbuttonresinfo t  "
				" where t.fname  = @fname " //画面代码，
				" and   t.name   = @name "  //按钮代码
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("fname", tgcpmz1["FORM_CODE"].ToString());//画面代码
			cmd_sql.Parameters.Set("name", tgcpmz1["FUNC_ID"].ToString());//按钮代码。
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteReader();
			if (cmd_sql.Read())
			{
				tgcpmz1["FUNC_CNAME"] = cmd_sql.GetString(1);
			}
			cmd_sql.Close(); 
			

			//初始化 后台程序描述，后台service号。
			//====================
			tgcpmz1["SRV_FUNC_DESC"] = ""; //功能描述
			tgcpmz1["SRV_ID"] = "";//若是service要获取对应的service 号。


			//若当前是子画面，并且对应的母画面是 GCPMSI开头的， 则自动获取对应的service名称
			//F2 = gcpmsied54_inq
			//F2 =分页查询= gcpmsied54_inq2
			//F3 = gcpmsied54_ins
			//F4 = gcpmsied54_upd
			//F5 = gcpmsied54_del
			//===================
			CString v_form_call_mode22 = ""; //画面调用模式= 1=子画面模式 
			CString v_form_base_name22 = ""; //母画面代码。
			CString v_table_name22 = ""; //业务表
			CString v_mode_no22 = ""; //画面模式
			CString v_keyvalue_522 = ""; //统计service
			CString v_service_name22 = ""; //自定义service程序。
			CString v_operate_mode22 = ""; //按钮对应模式= 1/2=service/SQL

			////ED54功能号等信息。
			//CString v_ed54_func_id22 = ""; //画面对应的ED54功能号。
			//CString v_keyvalue_222 = ""; //多记录
			//CString v_keyvalue_322 = ""; //查询条件
			//CString v_keyvalue_422 = ""; //单记录

			c_sql_condition = "select T.FORM_CALL_MODE "  //画面调用方式
				" ,t.description "//画面描述。
				" from  tesformresinfo t "
				" where t.name   = @name "
				;
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.Parameters.Set("name", tgcpmz1["FORM_CODE"].ToString());//画面代码
			cmd_sql.ExecuteReader();
			if (cmd_sql.Read())
			{
				v_form_call_mode22 = cmd_sql.GetString(1); //画面调用方式=1=子画面 
			}
			cmd_sql.Close();

			//若当前画面是子画面， 再判断一下是否是【纯配置画面】，并查找对应的母画面+业务表
			if (v_form_call_mode22.Trim() == "1")
			{
				/*
				select T.FORM_CODE,T.KEYVALUE_6,T.TABLE_NAME
				,t.* from tgcpmsi02 t
				*/
				v_form_base_name22 = "";
				v_table_name22 = "";
				v_mode_no22 = "";
				c_sql_condition = "select  T.KEYVALUE_6,T.TABLE_NAME " //母画面，业务表名称。
					" ,t.MODE_NO ,t.KEYVALUE_5 "//画面模式,pGrid统计service
					" ,t.KEYVALUE_2 ,t.KEYVALUE_3,t.KEYVALUE_4 " //KEYVALUE_2,KEYVALUE_3,KEYVALUE_4=多记录/查询条件/单记录//===3个ED54功能号。
					" from     tgcpmsi02 t " //画面配置主表。
					" where    t.form_code   = @form_code " //画面代码。
					" order by t.form_code "
					;
				sqlstr = c_sql_condition;
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.Parameters.Set("form_code", tgcpmz1["FORM_CODE"].ToString());//画面代码
				cmd_sql.ExecuteReader();
				if (cmd_sql.Read())
				{
					v_form_base_name22 = cmd_sql.GetString(1); //母画面 
					v_table_name22 = cmd_sql.GetString(2); //业务表。
					v_mode_no22 = cmd_sql.GetString(3); //画面模式
					v_keyvalue_522 = cmd_sql.GetString(4);//pGrid统计service

					////ED54功能号信息。
					//v_keyvalue_222 = cmd_sql.GetString(5);
					//v_keyvalue_322 = cmd_sql.GetString(6);
					//v_keyvalue_422 = cmd_sql.GetString(7); 
				}
				cmd_sql.Close();

				

				if (v_mode_no22.Trim() != "")
				{//若画面配置模式不为空，则获取以下默认[service]，配置的ED54功能号。


					////ED54功能号信息。
					//v_ed54_func_id22 = ""; //\r\n
					//if (v_keyvalue_222.Trim() != "")
					//{
					//	v_ed54_func_id22 = v_keyvalue_222.Trim();
					//}
					//if (v_keyvalue_322.Trim() != "")
					//{
					//	v_ed54_func_id22 = v_ed54_func_id22 + "\r\n" + v_keyvalue_322.Trim();
					//}
					//if (v_keyvalue_422.Trim() != "")
					//{
					//	v_ed54_func_id22 = v_ed54_func_id22 + "\r\n" + v_keyvalue_422.Trim();
					//}

					////配置画面， 主动计算对应的ED54功能号。
					//tgcpmz1["FORM_ED54"] = v_ed54_func_id22;


					//F2 = gcpmsied54_inq
					//F2 =分页查询= gcpmsied54_inq2
					//F3 = gcpmsied54_ins
					//F4 = gcpmsied54_upd
					//F5 = gcpmsied54_del
					//===================
					//tgcpmz1["FUNC_ID"]);//按钮代码。
					//if (tgcpmz1["FUNC_ID"].ToString().Trim().ToUpper() == "F2")
					//{
					//	tgcpmz1["SRV_NAME"] = "gcpmsied54_inq";
					//	if (v_mode_no22.Trim() == "02M")
					//	{//若是分页查询模式。
					//		tgcpmz1["SRV_NAME"] = "gcpmsied54_inq2";
					//	}
					//	if (v_mode_no22.Trim() == "02PG")
					//	{//若是pGrid统计模式。
					//		tgcpmz1["SRV_NAME"] = v_keyvalue_522;
					//	}

					//}
					//if (tgcpmz1["FUNC_ID"].ToString().Trim().ToUpper() == "F3")
					//{
					//	tgcpmz1["SRV_NAME"] = "gcpmsied54_ins";
					//}
					//if (tgcpmz1["FUNC_ID"].ToString().Trim().ToUpper() == "F4")
					//{
					//	tgcpmz1["SRV_NAME"] = "gcpmsied54_upd";
					//}
					//if (tgcpmz1["FUNC_ID"].ToString().Trim().ToUpper() == "F5")
					//{
					//	tgcpmz1["SRV_NAME"] = "gcpmsied54_del";
					//}


					//判断一下，是否有按钮自定义逻辑。
					//=============== 
					v_service_name22 = "";
					v_operate_mode22 = ""; //按钮执行模式。1/2=service/SQL
					c_sql_condition = " SELECT  T.SERVICE_NAME "
						" ,t.operate_mode "
						" FROM TGCPMSI01 T "
						" WHERE T.table_name = @table_name "
						" AND   T.func_id    = @func_id "
						;
					sqlstr = c_sql_condition;
					cmd_sql.SetCommandText(c_sql_condition);
					cmd_sql.Parameters.Set("table_name", v_table_name22);//画面代码
					cmd_sql.Parameters.Set("func_id", tgcpmz1["FUNC_ID"].ToString());//画面按钮。
					cmd_sql.ExecuteReader();
					if (cmd_sql.Read())
					{
						v_service_name22 = cmd_sql.GetString(1); //自定义service名称。  
						v_operate_mode22 = cmd_sql.GetString(2); //按钮执行模式
					}
					cmd_sql.Close();

					if (v_operate_mode22.Trim() == "1")
					{//自定义service
						tgcpmz1["SRV_NAME"] = v_service_name22;
					}
					else if (v_operate_mode22.Trim() == "2")
					{//自定义 SQL
						tgcpmz1["SRV_NAME"] = "直接SQL";
					}
					else
					{//其他， 则不若任何处理。

					}



				}
			}
			





			//初始化ED54功能说明：
			//====================
			//tgcpmz1["FORM_ED54"] ===>ED54功能号。
			tgcpmz1["FORM_ED54_DESC"] = ""; //ED54的功能说明。 
			if (tgcpmz1["FORM_ED54"].ToString().Trim() != "")
			{//若ED54功能号 != 空，则获取对应的说明。



				//可能存在 tgcpmz1.SRV_NAME 中有多个service 的情况，并用换行符分隔。
				//拆分后的多个service 放到数组变量 v_srv_name2[]中。

				//多个service信息的拆分成一个字符数组v_out_str，返回。
				//============================
				doFlag = f_get_multi_value2(tgcpmz1["FORM_ED54"].ToString(), "\n", v_out_str, &v_out_str_cnt, conn);
				if (doFlag != 0)
				{
					throw CApplicationException(-1, s.msg, log.Location);
				}

				//显示返回的数组信息。
				for (ii = 0; ii <= v_out_str_cnt; ii++)
				{
					v_out_str_tmp = v_out_str[ii];
					Log::Trace("", __FUNCTION__, "ED54功能号，ii [{0}] v_out_str_tmp[{1}] ", ii, v_out_str_tmp);


					//去掉其中的怪字符。。比如换行，回车，等等。。。
					//======================  
					v_out_str_tmp = v_out_str_tmp.Replace("\t", "");
					v_out_str_tmp = v_out_str_tmp.Replace("\r", "");
					v_out_str_tmp = v_out_str_tmp.Replace("\n", "");



					//说明。
					v_srv_func_desc = ""; //某个说明。
					/*
					select T.FUNC_ID,T.FUNC_DESC
					,t.* from ted53 t
					where t.func_id like 'PMOP02%'
					*/
					c_sql_condition = "select  t.FUNC_DESC   "
						" from  ted53 t  "
						" where t.func_id  = @func_id "
						;
					sqlstr = c_sql_condition;
					cmd_sql.Parameters.Set("func_id", v_out_str_tmp);//某个ED54代码。
					cmd_sql.SetCommandText(c_sql_condition);
					cmd_sql.ExecuteReader();
					if (cmd_sql.Read())
					{
						//【ED54功能号】==ED54功能描述。
						v_srv_func_desc = "【" + v_out_str_tmp + "】==" + cmd_sql.GetString(1);
					}
					cmd_sql.Close();


					//多个ED54代码 的内容的拼接。
					//=============
					if (ii >= 1)
					{//第2个  开始，需要拼接。

						//拼接[空2行]
						tgcpmz1["FORM_ED54_DESC"] = tgcpmz1["FORM_ED54_DESC"].ToString() + "\r\n" + v_srv_func_desc;
					}
					else
					{
						//获取第一个 
						tgcpmz1["FORM_ED54_DESC"] = v_srv_func_desc;

					}

				}//多个ED54功能号==循环处理结束。

			}//ED54功能号处理结束。

			Log::Trace("", __FUNCTION__, "最终获得的，ED54功能号=[{0}] 功能说明{1}] "
				, tgcpmz1["FORM_ED54"].ToString(), tgcpmz1["FORM_ED54_DESC"].ToString());





			//若新的后台服务 != 空，都重新获取对应的下级函数信息。
			//==================
			if (tgcpmz1["SRV_NAME"].ToString().Trim() != "") 
			{//若新的后台服务 != 空，都重新获取对应的下级函数信息。

				/*
				service 描述，service号。
				select  t.program_desc, a.srv_id from tea03 t,tea01 a
				where t.svc_name = 'pmog00_inq'
				and   t.svc_name = a.svc_name
				;
				*/

				//可能存在 tgcpmz1.SRV_NAME 中有多个service 的情况，并用换行符分隔。
				//拆分后的多个service 放到数组变量 v_srv_name2[]中。

				//多个service信息的拆分成一个字符数组v_out_str，返回。
				//============================
				doFlag = f_get_multi_value2(tgcpmz1["SRV_NAME"].ToString(),"\n", v_out_str,&v_out_str_cnt, conn);
				if (doFlag != 0)
				{
					throw CApplicationException(-1, s.msg, log.Location);
				}

				//显示返回的数组信息。
				for (ii = 0; ii <= v_out_str_cnt; ii++)
				{
					v_out_str_tmp = v_out_str[ii];
					Log::Trace("", __FUNCTION__, "ii [{0}] v_out_str_tmp[{1}] ", ii, v_out_str_tmp);


					//去掉其中的怪字符。。比如换行，回车，等等。。。
					//======================  
					v_out_str_tmp = v_out_str_tmp.Replace("\t", "");
					v_out_str_tmp = v_out_str_tmp.Replace("\r", "");
					v_out_str_tmp = v_out_str_tmp.Replace("\n", "");



					//后台服务说明。
					v_srv_func_desc = ""; //某个后台服务说明。
					c_sql_condition = "select  t.program_desc   "
						" ,t.code_line "
						" from tea03 t  "
						" where t.svc_name  = @svc_name "
						;
					sqlstr = c_sql_condition;
					cmd_sql.Parameters.Set("svc_name", v_out_str_tmp);//某个后台程序描述。
					cmd_sql.SetCommandText(c_sql_condition);
					cmd_sql.ExecuteReader();
					if (cmd_sql.Read())
					{
						v_false = "0";//默认不失效。
						v_code_line = " ";//默认是空格。

						//code_line =20位 ，最后一位= 1 ，说明失效。
						v_code_line = cmd_sql.GetString(2); //失效标志。 
						if (v_code_line.GetLength() == 20)
						{
							v_false = v_code_line.Substring(19, 1);
						}
						
						if (v_false.Trim() == "1")
						{//若已经失效。
							v_srv_func_desc = "[" + v_out_str_tmp + "]==【失效】";
						}
						else
						{
							//当前service 用粗体的框。
							v_srv_func_desc = "【" + v_out_str_tmp + "】==" + cmd_sql.GetString(1);
						} 

					}
					cmd_sql.Close();


					//service号。
					v_srv_id = ""; 
					c_sql_condition = "select  t.srv_id   "
						" from tea01 t  "
						" where t.svc_name  = @svc_name "
						;
					sqlstr = c_sql_condition;
					cmd_sql.Parameters.Set("svc_name", v_out_str_tmp);//某个后台程序号。
					cmd_sql.SetCommandText(c_sql_condition);
					cmd_sql.ExecuteReader();
					if (cmd_sql.Read())
					{
						v_srv_id = cmd_sql.GetString(1);
					}
					cmd_sql.Close();

					Log::Trace("", __FUNCTION__, "v_out_str_tmp =[{0}] v_srv_func_desc =[{1}]v_srv_id[{2}]"
						, v_out_str_tmp, v_srv_func_desc, v_srv_id);

					if (v_srv_id.Trim() != "")
					{
						//v_srv_func_desc尾部，拼接上对应的service号。
						v_srv_func_desc = v_srv_func_desc + "【" + v_srv_id + "】";
					}
					


					//获取service 对应的下一级函数+函数名称等。
					//============
					/*
					select t.svc_name,t.func_name,a.program_desc
					--,t.*
					from tea05 t,tea03 a
					where t.svc_name = 'f_pmof99_v3'
					and   t.call_type = 'C'
					and   t.FUNC_NAME LIKE 'f%'
					and   t.FUNC_NAME NOT LIKE 'f_bm2%'
					and   t.func_name = a.svc_name
					;


					*/

					//拼接下一级函数清单。
					//===================
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
						" order by a.program_desc,t.func_name " //根据名称中文，英文排序。
						;
					sqlstr = c_sql_condition;
					cmd_sql.Parameters.Set("svc_name", v_out_str_tmp);//某个合同程序。
					cmd_sql.SetCommandText(c_sql_condition);
					cmd_sql.ExecuteReader();
					while (cmd_sql.Read()) //多个信息的循环。
					{
						fun_i++;
						//[f_pmof_xx]合同跟踪处理
						v_remark_desc_tmp = "[" + cmd_sql.GetString(1) + "]" + cmd_sql.GetString(2);
						Log::Trace("", __FUNCTION__, "v_remark_desc_tmp =[{0}]", v_remark_desc_tmp);

						if (fun_i == 1)
						{
							v_remark_desc = v_remark_desc_tmp;
						}
						else
						{//从第2个开始，拼接换行符。
							v_remark_desc = v_remark_desc + "\r\n" + v_remark_desc_tmp;
						}


					}
					cmd_sql.Close();

					Log::Trace("", __FUNCTION__, "tgcpmz1.SRV_NAME=[{0}] v_remark_desc =[{1}]"
						, tgcpmz1["SRV_NAME"].ToString(), v_remark_desc);


					//CString SRV_FUNC_DESC;   //后台服务功能描述
					//若存在下一级程序信息，则拼接处理子程序信息。
					if (v_remark_desc.Trim() != "")
					{
						//tgcpmz1["SRV_FUNC_DESC"] = tgcpmz1["SRV_FUNC_DESC"].ToString() + "\n[下一级函数：]\n" + v_remark_desc;

						v_srv_func_desc = v_srv_func_desc + "\r\n[下一级函数：]\r\n" + v_remark_desc;
					}

					
					//多个service 的内容的拼接。
					//=============
					if (ii >= 1)
					{//第2个service 开始，需要拼接。

						//拼接[空2行]
						tgcpmz1["SRV_FUNC_DESC"] = tgcpmz1["SRV_FUNC_DESC"].ToString() + "\r\n" + v_srv_func_desc;
					}
					else
					{
						//获取第一个代表service号 ，即可。
						tgcpmz1["SRV_ID"] = v_srv_id;
						tgcpmz1["SRV_FUNC_DESC"] = v_srv_func_desc; 
					}  

				}//多个后台程序循环处理结束。

			}//后台程序处理结束。
			 
			Log::Trace("", __FUNCTION__, "最终获得的，程序名称=[{0}] 程序描述[{1}]程序号[{2}] "
				, tgcpmz1["SRV_NAME"].ToString(), tgcpmz1["SRV_FUNC_DESC"].ToString(), tgcpmz1["SRV_ID"].ToString());

			//修改信息。
			//========== 
			//CString TRACK_SEQ_NO;   //事件跟踪序列号

			tgcpmz1["REC_REVISOR"] = userid; //修改者
			tgcpmz1["REC_REVISE_TIME"] = dateNow;//修改日期 


			 

			//可修改列，根据功能号[GCPMZ1_UPD],来拼接可编辑字段，进行修改操作。

			//以下是，程序逻辑控制的修改信息。
			//===============================
			//除了: [修改者，修改日期] 
			//除了： CString SRV_FUNC_DESC;   //后台服务功能描述
			//除了： SRV_ID //后台服务service号。
			//除了： FORM_NAME //画面名称
			//MOID==模块号。
			//除了： KEYVALUE_GCPM //母子画面配置信息.[EPESPARA]
			//除了： FORM_ED54_DESC//ed54的描述。ADD ON 2015-11-6 10:39:49

			v_update_tmp = "";
			v_update_tmp2 = "";

			c_sql_condition = "select t.ITEM_ENAME from ted54 t "
				" where t.func_id        = 'GCPMZ1_UPD' "
				" and   t.FORM_EDIT_FLAG =  '1' " //可编辑字段。
				" order by t.SEQ_NO "
				;
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);
			//cmd_sql.Parameters.Set("track_seq_no", tgcpmz1["TRACK_SEQ_NO"].ToString());//跟踪序列号   
			cmd_sql.ExecuteReader();
			while(cmd_sql.Read())
			{
				v_update_tmp = cmd_sql.GetString(1);
				v_update_tmp2 = v_update_tmp2 + "," + v_update_tmp;
			}
			cmd_sql.Close();

			Log::Trace("", __FUNCTION__, "序号[{0}]修改的字段列[{1}] "
				, tgcpmz1["TRACK_SEQ_NO"].ToString(), v_update_tmp2);


			//若有DLL信息，二级模块= DLL 的前4位信息。
			if (tgcpmz1["DLLNAME"].ToString().GetLength() >= 4)
			{
				tgcpmz1["MOID"] = tgcpmz1["DLLNAME"].ToString().Substring(0, 4);
			}

			//若功能说明为空 AND 画面代码 = '后台', 
			//则令 func_cname= srv_name
			//=========================			 
			if (tgcpmz1["FUNC_CNAME"].ToString().Trim() == "" && tgcpmz1["FORM_CODE"].ToString() == "后台")
			{
				tgcpmz1["FUNC_CNAME"] = tgcpmz1["SRV_NAME"];
			}

			



			tgcpmz1.TrimOrBlank(); 
			
			//以下是，程序逻辑控制的修改信息。
			//===============================
			//除了: [修改者，修改日期] 
			//除了： CString SRV_FUNC_DESC;   //后台服务功能描述
			//除了： SRV_ID //后台服务service号。
			//除了： FORM_NAME ///画面名称+对应母画面代码[XXXX] //ADD ON 2016-11-14 15:03
			//MOID==模块号。
			//DLLNAME ==动态链接库文件。
			//除了： KEYVALUE_GCPM //母子画面配置信息.[EPESPARA]
			//除了： FORM_ED54_DESC//ed54的描述。 
			//ADD ON 2015-11-6 10:39:49



			v_update = "REC_REVISOR,REC_REVISE_TIME"
				",SRV_FUNC_DESC,SRV_ID,FORM_NAME,MOID,DLLNAME"
				",KEYVALUE_GCPM,FORM_ED54_DESC" + v_update_tmp2; //修改的字段。
			v_condi = "TRACK_SEQ_NO";//WHERE条件。


			Log::Trace("", __FUNCTION__, "序号[{0}]v_update[{1}] "
				, tgcpmz1["TRACK_SEQ_NO"].ToString(), v_update);

			//FORM_NAME
			Log::Trace("", __FUNCTION__, "FORM_CODE[{0}]FORM_NAME[{1}] "
				, tgcpmz1["FORM_CODE"].ToString(), tgcpmz1["FORM_NAME"].ToString());


			sqlstr = "update  tgcpmz1 ,TRACK_SEQ_NO[" + tgcpmz1["TRACK_SEQ_NO"].ToString() + "]";
			tgcpmz1.Update(v_update, v_condi);

			 
		} 



		//对每个选中的功能项对应的画面进行判断，
		//若是母子画面中的【母】画面,
		//则: 维护对应的母子配置信息。
		//若是母子画面中的【子】画面,
		//则: 维护对应的母画面代码。
		//KEYVALUE_GCPM //母子画面配置信息.[EPESPARA]
		//===============
		/*
		select  T.FORM_NAME,T.PK1_NAME,T.PK1
		,T.PK2_NAME,T.PK2
		,T.PK3_NAME,T.PK3
		--t.*
		from  tesformpara t
		where t.form_base_name = 'PMOAXX01E'
		*/
		CString v_out_form_name_tmp = "";
		CString v_form_base_name = "";
		CString v_form_name = "";
		CString v_pk1_name = "";
		CString v_pk1 = "";
		CString v_pk2_name = "";
		CString v_pk2 = "";
		CString v_pk3_name = "";
		CString v_pk3 = "";
		CString v_pk4_name = "";
		CString v_pk4 = "";
		CString v_pk5_name = "";
		CString v_pk5 = "";

		CString v_pk6_name = "";
		CString v_pk6 = "";
		CString v_pk7_name = "";
		CString v_pk7 = "";
		CString v_pk8_name = "";
		CString v_pk8 = "";
		CString v_pk9_name = "";
		CString v_pk9 = "";
		CString v_pk10_name = "";
		CString v_pk10 = "";


		CString v_form_call_mode = ""; //画面调用模式= 1=子画面模式
		CString v_form_desc = ""; //画面描述。

		//组织拼接的数据。
		CString v_str_title_tmp = ""; // "｛母画面/子画面｝【参数1/值1】/【参数2/值2】/【参数3/值3】/【参数4/值4】/【参数5/值5】/【参数6/值6】/【参数7/值7】/【参数8/值8】/【参数9/值9】/【参数10/值10】";
		CString v_str_mode_tmp = "";
		CString v_str_tmp = "";
		CString v_str = "";


		//v_out_form_name
		//v_out_form_cnt
		//显示返回的数组信息。
		Log::Trace("", __FUNCTION__, "不重复的画面个数，v_out_form_cnt [{0}]  ", v_out_form_cnt);

		for (ii = 0; ii <= v_out_form_cnt; ii++)
		{
			v_out_form_name_tmp = v_out_form_name[ii];
			Log::Trace("", __FUNCTION__, "ii [{0}] v_out_form_name_tmp[{1}] ", ii, v_out_form_name_tmp);

		    //先判断，
			//若是子画面，直接获取对应的母画面信息，即可。
			//若不是子画面，再找对应的母子配置信息。
			/*
			select T.FORM_CALL_MODE,t.description,t.* from tesformresinfo t
			where  t.name = 'PMOASM01E'
			*/
			v_form_call_mode = ""; //画面调用模式= 1=子画面模式
			v_form_desc = "";
			c_sql_condition = "select T.FORM_CALL_MODE "  //画面调用方式
				" ,t.description "//画面描述。
				" from  tesformresinfo t "
				" where t.name   = @name " 
				;
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.Parameters.Set("name", v_out_form_name_tmp);//画面代码
			cmd_sql.ExecuteReader();
			if (cmd_sql.Read())
			{
				v_form_call_mode = cmd_sql.GetString(1); //=1=子画面
				v_form_desc = cmd_sql.GetString(2);
			}
			cmd_sql.Close();


			if (v_form_call_mode.Trim() == "1")
			{//若是子画面模式，则直接获取对应的母画面信息。

				c_sql_condition = "select  t.FORM_BASE_NAME,T.FORM_NAME "
					" ,T.PK1_NAME,T.PK1 "
					" ,T.PK2_NAME,T.PK2 "
					" ,T.PK3_NAME,T.PK3 "
					" ,T.PK4_NAME,T.PK4 "
					" ,T.PK5_NAME,T.PK5 "
					" ,T.PK6_NAME,T.PK6 "
					" ,T.PK7_NAME,T.PK7 "
					" ,T.PK8_NAME,T.PK8 "
					" ,T.PK9_NAME,T.PK9 "
					" ,T.PK10_NAME,T.PK10 "
					"from tesformpara t "
					" where t.form_name   = @form_name " //根据子画面查询。
					" order by t.FORM_NAME "
					;

				v_str_mode_tmp = "当前是【子画面】";


			}
			else
			{//非子画面的情况下，根据母画面查询。

				c_sql_condition = "select  t.FORM_BASE_NAME,T.FORM_NAME "//BASE画面， 画面。
					",' ',' '" //" ,T.PK1_NAME,T.PK1 "
					",' ',' '" //" ,T.PK2_NAME,T.PK2 "
					",' ',' '" //" ,T.PK3_NAME,T.PK3 "
					",' ',' '" //" ,T.PK4_NAME,T.PK4 "
					",' ',' '" //" ,T.PK5_NAME,T.PK5 "
					",' ',' '" //" ,T.PK6_NAME,T.PK6 "
					",' ',' '" //" ,T.PK7_NAME,T.PK7 "
					",' ',' '" //" ,T.PK8_NAME,T.PK8 "
					",' ',' '" //" ,T.PK9_NAME,T.PK9 "
					",' ',' '" //" ,T.PK10_NAME,T.PK10 "
					" from  tesformpara t "
					" where t.form_base_name   = @form_base_name " //根据母画面查询。
					" order by t.FORM_NAME "
					;
				v_str_mode_tmp = "当前是【母画面】";
			}

			Log::Trace("", __FUNCTION__, "画面 [{0}] 调用模式[{1}]c_sql_condition[{2}] "
				, v_out_form_name_tmp, v_form_call_mode, c_sql_condition);


			int v_para_num = 0; //启用的参数个数。
			fun_i = 0;
			v_str = "";
			
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.Parameters.Set("form_name", v_out_form_name_tmp);//子画面代码
			cmd_sql.Parameters.Set("form_base_name", v_out_form_name_tmp);//母画面代码
			cmd_sql.ExecuteReader();
			while (cmd_sql.Read())
			{
				fun_i++; 
				v_form_base_name = cmd_sql.GetString(1);
				v_form_name = cmd_sql.GetString(2);

				/*v_pk1_name = cmd_sql.GetString(3);
				v_pk1 = cmd_sql.GetString(4);

				v_pk2_name = cmd_sql.GetString(5);
				v_pk2 = cmd_sql.GetString(6);

				v_pk3_name = cmd_sql.GetString(7);
				v_pk3 = cmd_sql.GetString(8);

				v_pk4_name = cmd_sql.GetString(9);
				v_pk4 = cmd_sql.GetString(10);

				v_pk5_name = cmd_sql.GetString(11);
				v_pk5 = cmd_sql.GetString(12);


				v_pk6_name = cmd_sql.GetString(13);
				v_pk6 = cmd_sql.GetString(14);

				v_pk7_name = cmd_sql.GetString(15);
				v_pk7 = cmd_sql.GetString(16);

				v_pk8_name = cmd_sql.GetString(17);
				v_pk8 = cmd_sql.GetString(18);

				v_pk9_name = cmd_sql.GetString(19);
				v_pk9 = cmd_sql.GetString(20);

				v_pk10_name = cmd_sql.GetString(21);
				v_pk10 = cmd_sql.GetString(22);*/

				/*v_str_tmp = CString::Format("{%s/%s}【%s/%s】/【%s/%s】/【%s/%s】/【%s/%s】/【%s/%s】/【%s/%s】/【%s/%s】/【%s/%s】/【%s/%s】/【%s/%s】"
					, (const char*)v_form_base_name, (const char*)v_form_name
					, (const char*)v_pk1_name, (const char*)v_pk1
					, (const char*)v_pk2_name, (const char*)v_pk2
					, (const char*)v_pk3_name, (const char*)v_pk3
					, (const char*)v_pk4_name, (const char*)v_pk4
					, (const char*)v_pk5_name, (const char*)v_pk5
					, (const char*)v_pk6_name, (const char*)v_pk6
					, (const char*)v_pk7_name, (const char*)v_pk7
					, (const char*)v_pk8_name, (const char*)v_pk8
					, (const char*)v_pk9_name, (const char*)v_pk9
					, (const char*)v_pk10_name, (const char*)v_pk10
					);*/


				//母画面/子画面
				v_str_tmp = CString::Format("{%s/%s}", (const char*)v_form_base_name, (const char*)v_form_name);

				//用FOR 循环10次 。
				//==============
				int v_pk_ii = 0;
				CString v_pk_name_tmp = ""; //参数名
				CString v_pk_tmp = ""; //参数值
				CString v_pk_desc_tmp = "";
				
				for (v_pk_ii = 1; v_pk_ii <= 10; v_pk_ii++)
				{
					v_pk_name_tmp = cmd_sql.GetString(v_pk_ii*2 + 1);//3/5/7/9。。。。
					v_pk_tmp = cmd_sql.GetString(v_pk_ii*2 + 2);//4/6/8/10。。。。
					if(v_pk_name_tmp.Trim() != "")
					{//若参数名不为空，则拼接。
						v_pk_desc_tmp = CString::Format("/[%s/%s]", (const char*)v_pk_name_tmp, (const char*)v_pk_tmp);
						v_str_tmp = v_str_tmp + v_pk_desc_tmp;
					}
					else
					{//若没有参数了，直接离开
						break;
					} 
				}

				Log::Trace("", __FUNCTION__, "2222==v_str_tmp [{0}] ", v_str_tmp); 
			

				if (fun_i == 1)
				{  
					v_str = v_str_tmp;
				}
				else
				{//从第2个开始，拼接换行符。
					v_str = v_str + "\r\n" + v_str_tmp;
				} 

			}
			cmd_sql.Close();

			Log::Trace("", __FUNCTION__, "ii [{0}] v_out_form_name_tmp[{1}]母子配置信息[{2}] "
				, ii, v_out_form_name_tmp, v_str);



			//根据画面代码，先清空字段‘母子画面’信息。
			//删除历史 信息。
			//============================
			c_sql_condition = " update tgcpmz1 t "
				" set   t.keyvalue_gcpm  = ' ' "
				" where t.form_code   = @form_code " 
				;
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition); 
			cmd_sql.Parameters.Set("form_code", v_out_form_name_tmp);//画面代码。
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();


			if (v_str.Trim() != "") 
			{//若有母子画面配置信息，则修改到画面的【最小】序号那一行记录。

				CDecimal v_min_seq = 0;
				c_sql_condition = " select min(t.seq_no) from tgcpmz1 t " 
					" where t.form_code      = @form_code " 
					;
				sqlstr = c_sql_condition;
				cmd_sql.SetCommandText(c_sql_condition); 
				cmd_sql.Parameters.Set("form_code", v_out_form_name_tmp);//画面代码。
				v_min_seq = cmd_sql.ExecuteScalar();
				cmd_sql.Close();


				//v_str_mode_tmp = "";
				v_str_title_tmp = "{母画面/子画面}/[参数/值]";
				v_str = v_str_mode_tmp + "\r\n" + v_str_title_tmp + "\r\n" + v_str; //模式行+标题行+说明。
				Log::Trace("", __FUNCTION__, "最终 v_str[{0}]", v_str);
				////测试阶段， 
				//v_str = v_str_mode_tmp;

				//若有子/母配置信息，则继续修改 FORM_NAME= FROM_NAME +换行+子/母=[aaa]/[bbbb]
				//================
				CString v_str2 = "";
				if (v_form_base_name.Trim() == v_out_form_name_tmp.Trim())
				{//若母画面=当前画面， 则说明当前是母画面
					v_str2 = v_form_desc + "\r\n=当前[母]画面"; //当前是【母】画面 
				}
				else
				{
					v_str2 = v_form_desc + "\r\n=当前[子]画面\r\n=母画面[" + v_form_base_name + "]"; //当前是【子】画面，母画面=[xxxx]
				}

				c_sql_condition = " update tgcpmz1 t "
					" set   t.keyvalue_gcpm  = @keyvalue_gcpm " 
					"      ,t.form_name      = @form_name "
					" where t.form_code      = @form_code "
					" and   t.seq_no         = @seq_no "
					;
				sqlstr = c_sql_condition;
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.Parameters.Set("keyvalue_gcpm", v_str);//子/母配置信息。
				cmd_sql.Parameters.Set("form_name", v_str2);//画面描述
				cmd_sql.Parameters.Set("form_code", v_out_form_name_tmp);//画面代码。
				cmd_sql.Parameters.Set("seq_no", v_min_seq);//v_min_seq
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close(); 

			} 


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


