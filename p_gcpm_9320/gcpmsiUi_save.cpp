/*============================================================================*/
/*== [service名  ]:  gcpmsiUi_save      ||  [对应VC#画面 ]:GCPMSI00          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2022/8/5 10:33:37==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMSI00                                          ==*/
/*== [调用函数   ]： 无				                                          ==*/
/*== [service功能]： 基表信息_维护                                      ==*/
/*========================================================================*/

#include "stdafx.h"

// Service 入口
BM2F_ENTERACE(gcpmsiUi_save)

int f_gcpmsiUi_save(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection * conn)
{
	CTracer log(__FUNCTION__);


	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsiUi_save";                //定义函数英文名称  
	CString FunctionCname = "基表信息_维护";              //定义函数中文名称 


	//程序用变量
	int   i = 0;
	int   fun_i = 0; //功能个数。
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0; 

	CString upd_talble = "";
	CString del_table = "";
	CString add_table = ""; 

	/* 数据库操作类定义 */
	CDbCommand cmd_sql(conn); //与DB 建立连接。
	CString    c_sql_condition = "  SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
	CString    c_sql_where = "  WHERE   1 = 1 "; //修改条件。 
	CString    c_sql_orderBY = "  order by t.order_no ";

	//业务操作履历表。
	CModel tgcpmsi99("TGCPMSI99");

	try
	{
		

		CString v_form_name = "";
		if (bcls_rec->Tables[0].Columns.Contains("FORM_NAME"))
			v_form_name = bcls_rec->Tables[0].Rows[0]["FORM_NAME"].ToString().Trim();
		Log::Trace("", __FUNCTION__, "v_form_name = {0}", v_form_name);

		CString v_function_id = "";
		if (bcls_rec->Tables[0].Columns.Contains("FUNCTION_ID"))
			v_function_id = bcls_rec->Tables[0].Rows[0]["FUNCTION_ID"].ToString().Trim();
		Log::Trace("", __FUNCTION__, "v_function_id = {0}", v_function_id);



		/*
		select   T.FORM_NAME,T.FUNCTION_ID, t.key_fields ,T.TABLE_NAME
,T.* 
 from TSI00GRIDVIEW  t
where t.form_name like 'GCPMSIF'
AND   T.FUNCTION_ID = 'GCPM_TGCPM21'
		*/


		CString v_table_name = "";
		CString v_key_fields = "";
		c_sql_condition = "select t.table_name,t.key_fields  "
			" from  TSI00GRIDVIEW  t "//低代码的【画面布局】配置信息。
		    " where t.form_name   = @form_name   "
			" and   t.function_id = @function_id "
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
		cmd_sql.Parameters.Set("form_name", v_form_name);
		cmd_sql.Parameters.Set("function_id", v_function_id);
		cmd_sql.ExecuteReader();
		if (cmd_sql.Read())
		{
			v_table_name = cmd_sql.GetString(1);//数据库表名称
			v_key_fields = cmd_sql.GetString(2);//表主键。
		}
		cmd_sql.Close();
		Log::Trace("", __FUNCTION__, "v_table_name = [{0}]v_key_fields[{1}]", v_table_name, v_key_fields);

		if (v_table_name.Trim() == "" 
			|| v_key_fields.Trim() == "")
		{
			sprintf(s.msg, "根据画面ID[%s],gridVeiwID[%s]没有找到对应的表+主键，当前操作失败。"
				, (const char*)v_form_name, (const char*)v_function_id);
			throw CApplicationException(-1, s.msg, log.Location);
		} 

		//特殊表名称的转换。
		//PMOA10_MAIN_INQ--->TPMOA10
		//==============
		//默认。
		CString v_table_name2 = v_table_name; //CMODEL的表定义= UI定义的数据集表名称
		if (v_table_name.Trim().ToUpper() == "PMOA10_MAIN_INQ")
		{
			v_table_name2 = "TPMOA10";
		}
		
		Log::Trace("", __FUNCTION__, "最终CModle定义表==v_table_name2 = [{0}]v_key_fields[{1}]"
			, v_table_name2, v_key_fields);




		//万能表定义。tgcpmsixx = v_table_name2 【配置指定的任意表】
		CModel tgcpmsixx(v_table_name2); //CModle定义的最终的表名称。


		CString add_table = v_table_name + "_ADD";
		CString upd_talble = v_table_name + "_MODIFY";
		CString del_table = v_table_name + "_DELETE";

		Log::Trace("", __FUNCTION__, "del_table = {0}", del_table);
		Log::Trace("", __FUNCTION__, "upd_talble = {0}", upd_talble);
		Log::Trace("", __FUNCTION__, "add_table = {0}", add_table);
		
		//新增/修改/删除。。。。个数。
		//======================
		int v_add_num = 0;
		int v_upd_num = 0;
		int v_del_num = 0;
		//Log::Trace("", __FUNCTION__, "新增{0}修改[{1}]删除[{2}]", v_add_num, v_upd_num, v_del_num);


		//记录新增//若存在指定BLK.
		if (bcls_rec->Tables.Contains(add_table))
		{
			Log::Trace("", __FUNCTION__, "开始新增 == ");
			v_add_num = bcls_rec->Tables[add_table].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "新增[{0}]条记录 ", v_add_num);
			for (int i = 0; i < v_add_num; i++)
			{
				//先删除，再新增。
				tgcpmsixx.Reset();
				tgcpmsixx.MergeFrom(bcls_rec->Tables[add_table].Rows[i]);
				tgcpmsixx["REC_CREATOR"] = s.userid;
				tgcpmsixx["REC_CREATE_TIME"] = dateNow;
				tgcpmsixx["REC_REVISOR"] = "";
				tgcpmsixx["REC_REVISE_TIME"] = "";
				if (tgcpmsixx.QueryCount(v_key_fields) > 0)
				{//若数据已经存在，报错提醒。
					//tgcpmsixx.Delete(v_key_fields);
					sprintf(s.msg, "根据主键[%s]已存在该纪录，请先删除，再新增！", (const char*)v_key_fields);
					throw CApplicationException(-1, s.msg, log.Location);
				}
				tgcpmsixx.TrimOrBlank();
				sqlstr = "insert into table["+ v_table_name +"]";
				tgcpmsixx.Insert();
			}
		}

		if (bcls_rec->Tables.Contains(upd_talble))
		{
			Log::Trace("", __FUNCTION__, "开始修改==");
			v_upd_num = bcls_rec->Tables[upd_talble].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "修改[{0}]条记录", v_upd_num);
			for (int i = 0; i < v_upd_num; i++)
			{
				// 校验当前会计期是否已抛帐
				tgcpmsixx.Reset();
				tgcpmsixx.MergeFrom(bcls_rec->Tables[upd_talble].Rows[i]);
				 
				if (tgcpmsixx.QueryCount(v_key_fields) <= 0)
				{//若数据已经不存在， 则不用修改
					sprintf(s.msg, "数据表中不存在该纪录，可能已被其他人删除，请重新查询后再修改！");
					throw CApplicationException(-1, s.msg, log.Location);
				}

				Log::Trace("", __FUNCTION__, "修改[{0}]条记录", v_upd_num);
				sqlstr = "update table[" + v_table_name + "]"; 
				tgcpmsixx["REC_REVISE_TIME"] = dateNow;
				tgcpmsixx["REC_REVISOR"] = s.userid;
				tgcpmsixx.TrimOrBlank();
				tgcpmsixx.Update("*", v_key_fields);
			}
		}


		if (bcls_rec->Tables.Contains(del_table))
		{
			Log::Trace("", __FUNCTION__, "开始删除==");
			v_del_num = bcls_rec->Tables[del_table].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "删除[{0}]条记录", v_del_num);
			for (int i = 0; i < v_del_num; i++)
			{
				// 校验当前会计期是否已抛帐
				tgcpmsixx.Reset();
				tgcpmsixx.MergeFrom(bcls_rec->Tables[del_table].Rows[i]);
				 
				tgcpmsixx.TrimOrBlank();
				sqlstr = "delete from table[" + v_table_name + "]";
				tgcpmsixx.Delete();
			}

		} 
		
		/*设置系统返回参数*/
		sprintf(s.msg, "成功新增[%d]修改[%d]删除[%d]笔记录。业务表[%s]"
			,v_add_num,v_upd_num,v_del_num
			,(const char*)v_table_name);//处理成功。 

		//新增基表维护履历表。
		//====
		tgcpmsi99.Reset();
		tgcpmsi99["REC_CREATOR"] = userid;
		tgcpmsi99["REC_CREATE_TIME"] = dateNow;
		tgcpmsi99["TABLE_NAME"] = v_table_name;
		tgcpmsi99["ERROR_REMARK"] = s.msg;
		tgcpmsi99["EVENT_ID"] = "edit";
		tgcpmsi99.TrimOrBlank();
		sqlstr = "insert into tgcpmsi99 ";
		tgcpmsi99.Insert();
 
		 


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





