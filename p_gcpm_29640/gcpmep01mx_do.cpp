/*============================================================================*/
/*== [service名  ]:  gcpmep01mx_do      ||  [对应VC#画面 ]:GCPMEP01MX           ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2019/3/21 15:44:09==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMEP01MX                                            ==*/
/*== [调用函数   ]： 无				                                        ==*/
/*== [service功能]： TGCPMEP01MX_信息维护                                   ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h"
#include "tgcpmep01mx.h"

//SERVICE入口
BM2F_ENTERACE(gcpmep01mx_do);

int f_gcpmep01mx_do(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection * conn)
{
	CTracer log(__FUNCTION__);

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmep01mx_do";                //定义函数英文名称  
	CString FunctionCname = "TGCPMEP01MX_信息维护";              //定义函数中文名称

	/* 程序内部变量 */
	int i = 0;
	int doFlag = 0;
	int v_row_num = 0; //行个数。
	CDecimal v_cnt = 0;
	CString v_blk_content = "XX";

	/* 数据库SQL操作字符串*/
	CString  sqlstr = "";
	try
	{
		CTGCPMEP01MX tgcpmep01mx(conn);

		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString  c_sql_orderBy = "";



		//获得系统时间，当前用户代码
		CString systime = CDateTime::Now().ToString("yyyyMMddHHmmss");
		CString userid = s.userid;

		//获取 CODE_CLASS 
		//===========
		CString v_code_class = "";
		if (bcls_rec->Tables[0].Columns.Contains("CODE_CLASS"))
			v_code_class = bcls_rec->Tables[0].Rows[0]["CODE_CLASS"];
		Log::Trace("", __FUNCTION__, " in ==v_code_class[{0}]", v_code_class);





		v_blk_content = "GCPMEP01MX_INS";//新增的BLK
		if (bcls_rec->Tables.Contains(v_blk_content))
		{
			v_row_num = bcls_rec->Tables[v_blk_content].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "in v_blk_content[{0}]v_row_num[{1}]  ", v_blk_content, v_row_num);
			for (i = 0; i < v_row_num; i++)
			{
				//   循环获取输入参数
				//======================
				tgcpmep01mx.MergeFrom(bcls_rec->Tables[v_blk_content].Rows[i]);
				tgcpmep01mx.TrimOrBlank();
				Log::Trace("", __FUNCTION__, "ins= 第[{0}]个代码编号[{1}]代码[{2}]"
					, i+1, tgcpmep01mx.CODE_CLASS
					,tgcpmep01mx.CODE);

				tgcpmep01mx.CODE_CLASS = v_code_class;
				//==================================================== 
				//   输入参数校验
				//====================================================
				if (tgcpmep01mx.CODE_CLASS.Trim() == "")
				{
					sprintf(s.msg, "[代码编号]不允许为空。");
					throw CApplicationException(-1, s.msg, log.Location);
				}

			/*	if (tgcpmep01mx.CODE.Trim() == "")
				{
					sprintf(s.msg, "[代码]不允许为空。");
					throw CApplicationException(-1, s.msg, log.Location);
				}*/
				 

				v_cnt = 0;
				c_sql_condition = "select count(1) from tep0002 t "
					" where t.code_class   = @code_class " //代码编号
					" and   t.code         = @code "
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", tgcpmep01mx.CODE_CLASS);//代码编号。
				cmd_sql.Parameters.Set("code", tgcpmep01mx.CODE);
				cmd_sql.SetCommandText(c_sql_condition);
				v_cnt = cmd_sql.ExecuteScalar();
				cmd_sql.Close();

				if (v_cnt >= 1)
				{//若已经存在，则不允许报错。
					sprintf(s.msg, "代码编号[%s]代码[%s]已经存在，不需要新增。\n当前操作失败。"
						, (const char*)tgcpmep01mx.CODE_CLASS
						, (const char*)tgcpmep01mx.CODE);
					throw CApplicationException(-1, s.msg, log.Location);
				}

				//==============
				//新增表， tep0002,tep0002_RES
				//============== 
				/*
				CString CODE_CLASS;   //代码编号
	CString CODE;   //代码
	CString CODE_DESC_1_CONTENT;   //代码描述一内容
	CString CODE_DESC_2_CONTENT;   //代码描述二内容
	CString CODE_DESC_3_CONTENT;   //代码描述三内容
	CString CODE_DESC_4_CONTENT;   //代码描述四内容
	CString CODE_DESC_5_CONTENT;   //代码描述五内容
				*/

				/*
				INSERT INTO tep0002(CODE_CLASS,CODE
,CODE_DESC_1_CONTENT)
VALUES('aa','bb','cc')
				*/
				tgcpmep01mx.TrimOrBlank(); 
				c_sql_condition = "insert into tep0002 ( "
					" rec_creator,rec_create_time " //创建者，创建时刻。
					" ,code_class,code"
					" ,code_desc_1_content,code_desc_2_content"
					" ,code_desc_3_content,code_desc_4_content"
					" ,code_desc_5_content" 	
					")VALUES ("
					" @rec_creator,@rec_create_time"
					",@code_class,@code"
					",@code_desc_1_content,@code_desc_2_content"
					",@code_desc_3_content,@code_desc_4_content"
					",@code_desc_5_content" 
					")";

				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("rec_creator", userid);//创建者
				cmd_sql.Parameters.Set("rec_create_time", systime);//创建时刻。
				
				cmd_sql.Parameters.Set("code_class", tgcpmep01mx.CODE_CLASS);//代码编号。
				cmd_sql.Parameters.Set("code", tgcpmep01mx.CODE);// 
				cmd_sql.Parameters.Set("code_desc_1_content", tgcpmep01mx.CODE_DESC_1_CONTENT); 
				cmd_sql.Parameters.Set("code_desc_2_content", tgcpmep01mx.CODE_DESC_2_CONTENT);
				cmd_sql.Parameters.Set("code_desc_3_content", tgcpmep01mx.CODE_DESC_3_CONTENT);
				cmd_sql.Parameters.Set("code_desc_4_content", tgcpmep01mx.CODE_DESC_4_CONTENT);
				cmd_sql.Parameters.Set("code_desc_5_content", tgcpmep01mx.CODE_DESC_5_CONTENT);
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close();

				Log::Trace("", __FUNCTION__, "insert into tep0002,代码编号[{0}]代码[{1}]"
					,tgcpmep01mx.CODE_CLASS
					,tgcpmep01mx.CODE);


				//tep0002_RES.
				//culture = zh_Hans= 中文简体
				c_sql_condition = "insert into tep0002_res ( "
					"  CULTURE " //语言。
					" ,CODE_CLASS,CODE"
					" ,CODE_DESC_1_CONTENT,CODE_DESC_2_CONTENT"
					" ,CODE_DESC_3_CONTENT,CODE_DESC_4_CONTENT"
					" ,CODE_DESC_5_CONTENT"
					")VALUES ("
					" 'zh_Hans'"//中文简体
					",@code_class,@code"
					",@code_desc_1_content,@code_desc_2_content"
					",@code_desc_3_content,@code_desc_4_content"
					",@code_desc_5_content"
					")";

				sqlstr = c_sql_condition; 
				cmd_sql.Parameters.Set("code_class", tgcpmep01mx.CODE_CLASS);//代码编号。
				cmd_sql.Parameters.Set("code", tgcpmep01mx.CODE);// 
				cmd_sql.Parameters.Set("code_desc_1_content", tgcpmep01mx.CODE_DESC_1_CONTENT);
				cmd_sql.Parameters.Set("code_desc_2_content", tgcpmep01mx.CODE_DESC_2_CONTENT);
				cmd_sql.Parameters.Set("code_desc_3_content", tgcpmep01mx.CODE_DESC_3_CONTENT);
				cmd_sql.Parameters.Set("code_desc_4_content", tgcpmep01mx.CODE_DESC_4_CONTENT);
				cmd_sql.Parameters.Set("code_desc_5_content", tgcpmep01mx.CODE_DESC_5_CONTENT); 
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close();
				Log::Trace("", __FUNCTION__, "insert into tep0002_res,代码编号[{0}]"
					, tgcpmep01mx.CODE_CLASS);



			}
		}






		v_blk_content = "GCPMEP01MX_UPD";//修改的BLK
		if (bcls_rec->Tables.Contains(v_blk_content))
		{
			v_row_num = bcls_rec->Tables[v_blk_content].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "upd= v_blk_content[{0}]v_row_num[{1}]  ", v_blk_content, v_row_num);
			for (i = 0; i < v_row_num; i++)
			{
				//   循环获取输入参数
				//======================
				tgcpmep01mx.MergeFrom(bcls_rec->Tables[v_blk_content].Rows[i]);
				tgcpmep01mx.TrimOrBlank();

				Log::Trace("", __FUNCTION__, "in 第[{0}]个代码编号[{1}]代码[{2}]"
					, i + 1, tgcpmep01mx.CODE_CLASS
					, tgcpmep01mx.CODE);
				//==================================================== 
				//   输入参数校验
				//====================================================
				if (tgcpmep01mx.CODE_CLASS.Trim() == "")
				{
					sprintf(s.msg, "[代码编号]不允许为空。");
					throw CApplicationException(-1, s.msg, log.Location);
				}

				//v_cnt = 0;
				//c_sql_condition = "select count(1) from tep0002 t "
				//	" where t.code_class   = @code_class " //代码编号
				//	;
				//sqlstr = c_sql_condition;
				//cmd_sql.Parameters.Set("code_class", tgcpmep01mx.CODE_CLASS);//代码编号。 
				//cmd_sql.SetCommandText(c_sql_condition);
				//v_cnt = cmd_sql.ExecuteScalar();
				//cmd_sql.Close();

				//if (v_cnt < 1)
				//{//若已经存在，则不允许报错。
				//	sprintf(s.msg, "代码编号[%s]不存在，不需要修改。\n当前操作失败。"
				//		, (const char*)tgcpmep01mx.CODE_CLASS);
				//	throw CApplicationException(-1, s.msg, log.Location);
				//}


				 

				//==============
				//修改表。。。tep0002,tep0002_res
				//==============  
				c_sql_condition = "update tep0002 t "
					" set t.code = t.code "
					"     ,t.code_desc_1_content = @code_desc_1_content "
					"     ,t.code_desc_2_content = @code_desc_2_content "
					"     ,t.code_desc_3_content = @code_desc_3_content "
					"     ,t.code_desc_4_content = @code_desc_4_content "
					"     ,t.code_desc_5_content = @code_desc_5_content " 
					" where t.code_class = @code_class "	//代码编号。	
					" and   t.code       = @code "
					; 
				sqlstr = c_sql_condition;  
				cmd_sql.Parameters.Set("code_class", tgcpmep01mx.CODE_CLASS);//代码编号。
				cmd_sql.Parameters.Set("code", tgcpmep01mx.CODE);// 
				cmd_sql.Parameters.Set("code_desc_1_content", tgcpmep01mx.CODE_DESC_1_CONTENT);
				cmd_sql.Parameters.Set("code_desc_2_content", tgcpmep01mx.CODE_DESC_2_CONTENT);
				cmd_sql.Parameters.Set("code_desc_3_content", tgcpmep01mx.CODE_DESC_3_CONTENT);
				cmd_sql.Parameters.Set("code_desc_4_content", tgcpmep01mx.CODE_DESC_4_CONTENT);
				cmd_sql.Parameters.Set("code_desc_5_content", tgcpmep01mx.CODE_DESC_5_CONTENT); 
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close(); 

				Log::Trace("", __FUNCTION__, "update tep0002,代码编号[{0}]", tgcpmep01mx.CODE_CLASS);


				//==============
				//修改表。。。tep0002,tep0002_res
				//==============  
				//tep0002_RES.
				//culture = zh_Hans= 中文简体
				c_sql_condition = "update tep0002_res t "
					" set t.code = t.code "
					"     ,t.code_desc_1_content = @code_desc_1_content "
					"     ,t.code_desc_2_content = @code_desc_2_content "
					"     ,t.code_desc_3_content = @code_desc_3_content "
					"     ,t.code_desc_4_content = @code_desc_4_content "
					"     ,t.code_desc_5_content = @code_desc_5_content " 
					" where t.code_class = @code_class "	//代码编号。
					" and   t.code       = @code "
					" and   t.culture    = 'zh_Hans' " //中文简体
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", tgcpmep01mx.CODE_CLASS);//代码编号。
				cmd_sql.Parameters.Set("code", tgcpmep01mx.CODE);// 
				cmd_sql.Parameters.Set("code_desc_1_content", tgcpmep01mx.CODE_DESC_1_CONTENT);
				cmd_sql.Parameters.Set("code_desc_2_content", tgcpmep01mx.CODE_DESC_2_CONTENT);
				cmd_sql.Parameters.Set("code_desc_3_content", tgcpmep01mx.CODE_DESC_3_CONTENT);
				cmd_sql.Parameters.Set("code_desc_4_content", tgcpmep01mx.CODE_DESC_4_CONTENT);
				cmd_sql.Parameters.Set("code_desc_5_content", tgcpmep01mx.CODE_DESC_5_CONTENT); 
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close();

				Log::Trace("", __FUNCTION__, "update tep0002_res,代码编号[{0}]", tgcpmep01mx.CODE_CLASS);

 

			}
		}


		//删除的BLK.
		//==========
		v_blk_content = "GCPMEP01MX_DEL";//删除的BLK
		if (bcls_rec->Tables.Contains(v_blk_content))
		{
			v_row_num = bcls_rec->Tables[v_blk_content].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "del= v_blk_content[{0}]v_row_num[{1}]  ", v_blk_content, v_row_num);
			for (i = 0; i < v_row_num; i++)
			{
				//   循环获取输入参数
				//======================
				tgcpmep01mx.MergeFrom(bcls_rec->Tables[v_blk_content].Rows[i]);
				tgcpmep01mx.TrimOrBlank();
				Log::Trace("", __FUNCTION__, "in 第[{0}]个代码编号[{1}]代码[{2}]"
					, i + 1, tgcpmep01mx.CODE_CLASS
					, tgcpmep01mx.CODE);
				//==================================================== 
				//   输入参数校验
				//====================================================
				if (tgcpmep01mx.CODE_CLASS.Trim() == "")
				{
					sprintf(s.msg, "[代码编号]不允许为空。");
					throw CApplicationException(-1, s.msg, log.Location);
				}

				//v_cnt = 0;
				//c_sql_condition = "select count(1) from tep0002 t "
				//	" where t.code_class   = @code_class " //代码编号
				//	;
				//sqlstr = c_sql_condition;
				//cmd_sql.Parameters.Set("code_class", tgcpmep01mx.CODE_CLASS);//代码编号。 
				//cmd_sql.SetCommandText(c_sql_condition);
				//v_cnt = cmd_sql.ExecuteScalar();
				//cmd_sql.Close();

				//if (v_cnt < 1)
				//{//若已经存在，则不允许报错。
				//	sprintf(s.msg, "代码编号[%s]不存在，不需要删除。\n当前操作失败。"
				//		, (const char*)tgcpmep01mx.CODE_CLASS);
				//	throw CApplicationException(-1, s.msg, log.Location);
				//}

				//==============
				//删除表。。。tep0002,tep0002_res,tep0002,tep0002_res
				//==============  
				c_sql_condition = "delete from tep0002 t " 
					" where t.code_class = @code_class "	//代码编号。
					" and   t.code       = @code "
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", tgcpmep01mx.CODE_CLASS);//代码编号。 
				cmd_sql.Parameters.Set("code", tgcpmep01mx.CODE);// 
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close();


				c_sql_condition = "delete from tep0002_res t "
					" where t.code_class = @code_class "	//代码编号。
					" and   t.code       = @code "

					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", tgcpmep01mx.CODE_CLASS);//代码编号。
				cmd_sql.Parameters.Set("code", tgcpmep01mx.CODE);// 
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close();

				 


			}
		}

		//处理成功。 
		strcpy(s.msg, "恭喜，处理成功。");


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

	//此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;

}
