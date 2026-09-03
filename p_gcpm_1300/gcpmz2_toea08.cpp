/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2015-4-7 13:19:51
功能: 功能清单同步到【TEA08】表。
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/ 
// New Include
#include "stdafx.h" 



//从字符串中根据指定分隔符拆分成数组返回
int f_get_multi_value2(CString v_in_str, CString v_spilit_flag, CString *v_out_str, int *v_out_str_cnt, CDbConnection* conn);


/*<remark>=========================================================
/// <summary>
/// 功能清单同步到【TEA08】表。
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmz2_toea08)

int f_gcpmz2_toea08(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz2_toea08";                //定义函数英文名称  
	CString FunctionCname = "功能清单同步到【TEA08】表。";              //定义函数中文名称
	//LogTrace(1, 1, " **************%s begin*****************", (const char*)FunctionEname);


	//程序用变量
	int   i = 0;
	int   ii_button = 0;
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;  
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0;
	CString  v_dllname = "";


	//信息拆分使用变量。
	CString v_out_str[200];
	int     v_out_str_cnt = 0;
	CString v_out_str_tmp = "";
	int ii = 0;


	//新增信息。
	CString v_column_name = "";  //列名称们...
	CString v_column_value = ""; //列内容们...
	CString v_type = "";
	CDecimal v_seq = 0;
	CString v_svc_desc = "";
	CString v_svc_id = "";

	


	try
	{
	CModel tgcpmz1("TGCPMZ1");
 
		
			/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where      = "  WHERE   1 = 1 "; //批量修改条件。
		CString    c_sql_condition  =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
		CString    c_order_by       =  " order by t.order_no ";
		
		
			/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2     = "  WHERE   1 = 1 "; //批量修改条件。
		CString    c_sql_condition2 =  " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no" ;
	 
		 
		 
		//form_code = 前台画面代码
		CString v_form_code = "";
		 
		 
		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{ 
			if (bcls_rec->Tables[0].Columns.Contains("FORM_CODE"))
				v_form_code = bcls_rec->Tables[0].Rows[i]["FORM_CODE"];

			Log::Trace("", __FUNCTION__, "第[{0}]个画面v_form_code =[{1}] "
				,i+1, v_form_code);

			if (v_form_code.Trim() == "")
			{//画面代码，不允许为空。
				sprintf(s.msg, "[画面代码]不允许为空，当前操作失败。");
				throw CApplicationException(-1, s.msg, log.Location);
			}
 
			if (userid.Trim() != "178029")
			{//若不是指定用户，则不允许修改，功能的覆盖产线。

				sprintf(s.msg, "您的帐号[%s]没有【同步到EA08】的权限，当前操作失败。"
					, (const char*)userid);
				throw CApplicationException(-1, s.msg, log.Location);
			}

			/*
			select t.form_name,t.form_desc,t.seq_no,t.type,t.func_id
			--,t.*
			from tea08 t
			where t.form_name like 'PMOABW01E%'
			and   t.type = 'F' --eped54功能号信息。
			order by t.form_name,t.type,t.seq_no,t.btn_name
			;


			select t.form_name,t.form_desc,t.seq_no,t.type,t.btn_name,t.btn_desc
			,t.svc_name,t.svc_desc,t.svc_id,t.func_id
			--,t.*
			from tea08 t
			where t.form_name like 'PMOABW01E%'
			and   t.type = 'S'--按钮的service信息
			order by t.form_name,t.type,t.seq_no,t.btn_name
			;
			*/

			/*
			select t.form_code,t.form_name,T.SEQ_NO,t.func_id,t.srv_name,t.remark
			--,t.*
			from tgcpmz1 t
			where t.form_code  = 'PMOASM01E'
			order by t.seq_no
			;
			*/
			 
			//先删除老的
			//再新增 type = S [按钮对应的service信息] 
			//再新增 type = F [ed54配置信息]
			//============

			//先删除老的信息。
			//=================
			c_sql_condition = " delete from tea08 t where t.form_name = @form_name ";
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("form_name", v_form_code);//画面代码。 
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close(); 

			//再新增 type = S [按钮对应的service信息] 
			//再新增 type = F [ed54配置信息]
			//=================
			ii_button = 0;
			c_sql_condition = " select t.* "
				" from  tgcpmz1 t  "
			    " where      t.form_code = @form_code "
			    " order by   t.seq_no"
			    ;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("form_code", v_form_code);//画面代码。 
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteReader();
			while (cmd_sql.Read())
			{
				cmd_sql.Fetch(tgcpmz1);
				//t.form_code,t.form_name,T.seq_no,t.func_id,t.srv_name,t.remark
				ii_button++;


				if (ii_button == 1)
				{//若是画面中的第一个功能键信息，则获取对应的ED54配置信息，写表 TEA08

					Log::Trace("", __FUNCTION__, "画面 =[{0}] 按钮 =[{1}] EPED54[{2}]"
						,tgcpmz1["FORM_CODE"].ToString(), tgcpmz1["FUNC_ID"].ToString(), tgcpmz1["REMARK"].ToString());
					//tgcpmz1["REMARK"] 中存放的是多个ED54功能号，并用换行分割。

					//可能存在 tgcpmz1.SRV_NAME 中有多个service 的情况，并用换行符分隔。
					//拆分后的多个service 放到数组变量 v_srv_name2[]中。

					//多个EPED54配置信息的拆分成一个字符数组v_out_str，返回。
					//============================
					doFlag = f_get_multi_value2(tgcpmz1["REMARK"].ToString(), "\n", v_out_str, &v_out_str_cnt, conn);
					if (doFlag != 0)
					{
						throw CApplicationException(-1, s.msg, log.Location);
					}

					//显示返回的数组信息。
					for (ii = 0; ii <= v_out_str_cnt; ii++)
					{
						v_out_str_tmp = v_out_str[ii]; 
						Log::Trace("", __FUNCTION__, "ii [{0}] v_out_str_tmp[{1}] ", ii, v_out_str_tmp);

						if (v_out_str_tmp.Trim() == "")
						{
							//内容为空，继续下一个。
							continue;
						}
						 
						/*
						select t.form_name,t.form_desc,t.seq_no,t.type,t.func_id
						--,t.*
						from tea08 t
						where t.form_name like 'PMOABW01E%'
						and   t.type = 'F' --eped54功能号信息。
						order by t.form_name,t.type,t.seq_no,t.btn_name
						;
						*/
						v_type = "F"; //--eped54功能号信息。
						v_seq = 0;
						v_column_name = "(form_name,form_desc,seq_no,type,func_id)";
						v_column_value = CString::Format(" values('%s','%s','%d','%s','%s')"
							, (const char*)tgcpmz1["FORM_CODE"].ToString(), (const char*)tgcpmz1["FORM_NAME"].ToString(), v_seq.ToInt32()
							, (const char*)v_type, (const char*)v_out_str_tmp);


						c_sql_condition2 = " insert into tea08 " + v_column_name + v_column_value;
						Log::Trace("", __FUNCTION__, "c_sql_condition2[{0}]  ", c_sql_condition2);
						sqlstr = c_sql_condition2;
						cmd_sql2.SetCommandText(c_sql_condition2);// 设置执行的SQL语句  
						cmd_sql2.ExecuteNonQuery();
						cmd_sql2.Close();


					}//多个ED54配置循环处理结束。 







				}



				//按钮对应的service 信息的维护。
				//===================
				Log::Trace("", __FUNCTION__, "画面 =[{0}] 按钮 =[{1}] service[{2}]"
					, tgcpmz1["FORM_CODE"].ToString(), tgcpmz1["FUNC_ID"].ToString(), tgcpmz1["SRV_NAME"].ToString());


				//可能存在 tgcpmz1.SRV_NAME 中有多个service 的情况，并用换行符分隔。
				//拆分后的多个service 放到数组变量 v_srv_name2[]中。

				//多个EPED54配置信息的拆分成一个字符数组v_out_str，返回。
				//============================
				doFlag = f_get_multi_value2(tgcpmz1["SRV_NAME"].ToString(), "\n", v_out_str, &v_out_str_cnt, conn);
				if (doFlag != 0)
				{
					throw CApplicationException(-1, s.msg, log.Location);
				}

				//显示返回的数组信息。
				for (ii = 0; ii <= v_out_str_cnt; ii++)
				{
					v_out_str_tmp = v_out_str[ii]; 
					Log::Trace("", __FUNCTION__, "service==>ii [{0}] v_out_str_tmp[{1}] ", ii, v_out_str_tmp);

					if (v_out_str_tmp.Trim() == "")
					{
						//内容为空，继续下一个。
						continue;
					}
					//根据拆分后的serivce获取对应的描述+service号。
					//找到service号 ，则新增。
					//========================
					//后台服务说明。
					v_svc_desc = ""; //某个后台服务说明。
					c_sql_condition2 = "select  t.program_desc   "
						" from tea03 t  "
						" where t.svc_name  = @svc_name "
						;
					sqlstr = c_sql_condition2;
					cmd_sql2.Parameters.Set("svc_name", v_out_str_tmp);//某个后台程序描述。
					cmd_sql2.SetCommandText(c_sql_condition2);
					cmd_sql2.ExecuteReader();
					if (cmd_sql2.Read())
					{
						//当前service 用粗体的框。
						v_svc_desc = cmd_sql2.GetString(1);
					}
					cmd_sql2.Close();


					//service号。
					v_svc_id = "";
					c_sql_condition2 = "select  t.srv_id   "
						" from tea01 t  "
						" where t.svc_name  = @svc_name "
						;
					sqlstr = c_sql_condition2;
					cmd_sql2.Parameters.Set("svc_name", v_out_str_tmp);//某个后台程序号。
					cmd_sql2.SetCommandText(c_sql_condition2);
					cmd_sql2.ExecuteReader();
					if (cmd_sql2.Read())
					{
						v_svc_id = cmd_sql2.GetString(1);
					}
					cmd_sql2.Close();

					Log::Trace("", __FUNCTION__, "v_out_str_tmp =[{0}] v_svc_desc =[{1}]v_srv_id[{2}]"
						, v_out_str_tmp, v_svc_desc, v_svc_id);



					//若对应的service号不为空，则说明是有效的service,则进行 TEA08的新增。
					if (v_svc_id.Trim() != "")
					{
						/*
						select t.form_name,t.form_desc,t.seq_no,t.type,t.btn_name,t.btn_desc
						,t.svc_name,t.svc_desc,t.svc_id
						--,t.*
						from tea08 t
						where t.form_name like 'PMOABW01E%'
						and   t.type = 'S'--按钮的service信息
						order by t.form_name,t.type,t.seq_no,t.btn_name
						;
						*/
						v_type = "S"; //--service信息。
						v_column_name = "(form_name,form_desc,seq_no "
							",type,btn_name,btn_desc"
							",svc_name,svc_desc,svc_id)";
						v_column_value = CString::Format(" values('%s','%s','%d','%s','%s','%s'"
							",'%s','%s','%s')"
							, (const char*)tgcpmz1["FORM_CODE"].ToString(), (const char*)tgcpmz1["FORM_NAME"].ToString(), tgcpmz1["SEQ_NO"].ToDecimal().ToInt32()
							, (const char*)v_type, (const char*)tgcpmz1["FUNC_ID"].ToString(), (const char*)tgcpmz1["FUNC_CNAME"].ToString()
							, (const char*)v_out_str_tmp, (const char*)v_svc_desc, (const char*)v_svc_id);


						c_sql_condition2 = " insert into tea08 " + v_column_name + v_column_value;
						Log::Trace("", __FUNCTION__, "c_sql_condition2[{0}]  ", c_sql_condition2);
						sqlstr = c_sql_condition2;
						cmd_sql2.SetCommandText(c_sql_condition2);// 设置执行的SQL语句  
						cmd_sql2.ExecuteNonQuery();
						cmd_sql2.Close();

					}
					


				}//多个ED54配置循环处理结束。 






			}
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



