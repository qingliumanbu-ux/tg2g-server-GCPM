/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2015-10-11 15:33:22
功能: 【TEA08】表,同步到功能清单[TGCPMZ1]。
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
BM2F_ENTERACE(gcpmz2_from_ea08)

int f_gcpmz2_from_ea08(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmz2_from_ea08";                //定义函数英文名称  
	CString FunctionCname = "【TEA08】表,同步到功能清单[TGCPMZ1]";              //定义函数中文名称
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
		CString v_func_id = ""; //功能标识= 功能按钮。
		 
		 
		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{ 
			//画面代码
			if (bcls_rec->Tables[0].Columns.Contains("FORM_CODE"))
				v_form_code = bcls_rec->Tables[0].Rows[i]["FORM_CODE"];

			//功能标识 
			if (bcls_rec->Tables[0].Columns.Contains("FUNC_ID"))
				v_func_id = bcls_rec->Tables[0].Rows[i]["FUNC_ID"];

			Log::Trace("", __FUNCTION__, "第[{0}]个画面v_form_code =[{1}]v_func_id[{2}] "
				, i + 1, v_form_code, v_func_id);

			if (v_form_code.Trim() == "")
			{//画面代码，不允许为空。
				sprintf(s.msg, "[画面代码]不允许为空，当前操作失败。");
				throw CApplicationException(-1, s.msg, log.Location);
			}

			if (v_func_id.Trim() == "")
			{//功能标识，不允许为空。
				sprintf(s.msg, "[功能标识]不允许为空，当前操作失败。");
				throw CApplicationException(-1, s.msg, log.Location);
			}
 
			if (userid.Trim() != "178029")
			{//若不是指定用户，则不允许修改，功能的覆盖产线。

				sprintf(s.msg, "您的帐号[%s]没有【从EA08同步到功能清单】的权限，当前操作失败。"
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
			 
			//将 TGCPMZ1中已经存在的 FORM_CODE ,
			//对应的信息从TEA08表中获取过来，进行内容修改。
			//将 TEA08中 seq_no = 0的内容，新增到 TGPMZ1中的0 序号对应的信息中。
			//有就修改，没有就新增。
			//===================
			//再新增 type = S [按钮对应的service信息] 
			//再新增 type = F [ed54配置信息]
			//============

			/*
			select t.form_name,t.btn_name,t.seq_no,t.type,t.svc_name,t.func_id,t.* from tea08 t
where t.form_name like 'PSBW13'
			*/

			////先删除老的信息。
			////=================
			//c_sql_condition = " delete from tea08 t where t.form_name = @form_name ";
			//sqlstr = c_sql_condition;
			//cmd_sql.Parameters.Set("form_name", v_form_code);//画面代码。 
			//cmd_sql.SetCommandText(c_sql_condition);
			//cmd_sql.ExecuteNonQuery();
			//cmd_sql.Close(); 

			//再新增 type = S [按钮对应的service信息] 
			//再新增 type = F [ed54配置信息]
			//=================
			CString v_form_name_08 = "";
			CString v_btn_name_08 = "";
			CDecimal v_seq_no_08 = 0;
			CString v_type_08 = "";

			//service信息，ED54功能号信息。
			CString v_svc_name_08_tmp = "";
			CString v_func_id_08_tmp = "";

			CString v_svc_name_08 = "";
			CString v_func_id_08 = "";


			//在TEA08中， SEQ_NO = 0的时候， btn_name = 空格，
			//SO ,此处需要特殊处理。
			//=============
			v_btn_name_08 = v_func_id;

			if (v_func_id.Trim() == "F0")
			{
				v_btn_name_08 = " "; //空格。
			}


			ii_button = 0;
			c_sql_condition = " select t.svc_name,t.func_id "
				" from       tea08 t  "
			    " where      t.form_name = @form_name " 
				" and        t.btn_name  = @btn_name "
				//" and        t.type = 'S' " //service信息。
			    " order by   t.type   " // 根据信息类型排序。
			    ;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("form_name", v_form_code);//画面代码。 
			cmd_sql.Parameters.Set("btn_name", v_btn_name_08);//按钮名称。
			cmd_sql.SetCommandText(c_sql_condition);
			cmd_sql.ExecuteReader();
			while (cmd_sql.Read())
			{
				v_svc_name_08_tmp = cmd_sql.GetString(1); //service
				v_func_id_08_tmp = cmd_sql.GetString(2); //ED54功能号。

				ii_button++;


				if (ii_button == 1)
				{//若是 第一个信息， 有内容才赋值。

					if (v_svc_name_08_tmp.Trim() != "")
					{
						v_svc_name_08 = v_svc_name_08_tmp;
					}
					if (v_func_id_08_tmp.Trim() != "")
					{
						v_func_id_08 = v_func_id_08_tmp;
					}
				}
				else
				{//拼接， 有内容才赋值。
					if (v_svc_name_08_tmp.Trim() != "")
					{
						v_svc_name_08 = v_svc_name_08 + "\n" + v_svc_name_08_tmp;
					}
					
					if (v_func_id_08_tmp.Trim() != "")
					{
						v_func_id_08 = v_func_id_08 + "\n" + v_func_id_08_tmp;
					}
				} 

			}
			cmd_sql.Close();

			v_svc_name_08 = v_svc_name_08.TrimOrBlank();
			v_func_id_08 = v_func_id_08.TrimOrBlank();

			//将内容修改到 TGCPMZ1表中。
			//================
			c_sql_condition = " update tgcpmz1 t  "
			" set t.remark      = @remark " // 存放ED54功能号 , tgcpmz1.REMARK
			"    ,t.srv_name    = @srv_name " //后台服务名称，tgcpmz1.SRV_NAME
			" where t.form_code = @form_code "
			" and   t.func_id   = @func_id "
			;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("remark", v_func_id_08);
			cmd_sql.Parameters.Set("srv_name", v_svc_name_08);

			cmd_sql.Parameters.Set("form_code", v_form_code);//画面代码。 
			cmd_sql.Parameters.Set("func_id", v_func_id);//功能号=按钮代码=F2/F3。。。。 
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


