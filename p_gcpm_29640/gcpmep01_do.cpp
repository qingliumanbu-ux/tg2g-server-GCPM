/*============================================================================*/
/*== [service名  ]:  gcpmep01_do        ||  [对应VC#画面 ]:GCPMEP01           ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2019/3/20 16:09:57==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMEP01                                            ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： TGCPMEP01_信息维护                                   ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h"
#include "tgcpmep01.h"

//SERVICE入口
BM2F_ENTERACE(gcpmep01_do);

int f_gcpmep01_do(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection * conn)
{
	CTracer log(__FUNCTION__);

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmep01_do";                //定义函数英文名称  
	CString FunctionCname = "TGCPMEP01_信息维护";              //定义函数中文名称

	/* 程序内部变量 */
	int i = 0;
	int doFlag = 0;
	int v_row_num = 0; //行个数。
	CDecimal v_cnt = 0;
	CString v_blk_name = "XX";

	/* 数据库SQL操作字符串*/
	CString  sqlstr = "";
	try
	{
		CTGCPMEP01 tgcpmep01(conn);

		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString  c_sql_orderBy = "";



		//获得系统时间，当前用户代码
		CString systime = CDateTime::Now().ToString("yyyyMMddHHmmss");
		CString userid = s.userid;





		v_blk_name = "GCPMEP01_INS";//新增的BLK
		if (bcls_rec->Tables.Contains(v_blk_name))
		{
			v_row_num = bcls_rec->Tables[v_blk_name].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "in v_blk_name[{0}]v_row_num[{1}]  ", v_blk_name, v_row_num);
			for (i = 0; i < v_row_num; i++)
			{
				//   循环获取输入参数
				//======================
				tgcpmep01.MergeFrom(bcls_rec->Tables[v_blk_name].Rows[i]);
				tgcpmep01.TrimOrBlank();
				Log::Trace("", __FUNCTION__, "in 第[{0}]个代码编号[{1}]"
					, i+1, tgcpmep01.CODE_CLASS);
				//==================================================== 
				//   输入参数校验
				//====================================================
				if (tgcpmep01.CODE_CLASS.Trim() == "")
				{
					sprintf(s.msg, "[代码编号]不允许为空。");
					throw CApplicationException(-1, s.msg, log.Location);
				}
				 

				v_cnt = 0;
				c_sql_condition = "select count(1) from tep0001 t "
					" where t.code_class   = @code_class " //代码编号
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", tgcpmep01.CODE_CLASS);//代码编号。 
				cmd_sql.SetCommandText(c_sql_condition);
				v_cnt = cmd_sql.ExecuteScalar();
				cmd_sql.Close();

				if (v_cnt >= 1)
				{//若已经存在，则不允许报错。
					sprintf(s.msg, "代码编号[%s]已经存在，不需要新增。\n当前操作失败。"
						, (const char*)tgcpmep01.CODE_CLASS);
					throw CApplicationException(-1, s.msg, log.Location);
				}

				//==============
				//新增表， TEP0001,TEP0001_RES
				//============== 
				/*
				t.rec_creator,t.rec_create_time
				CString CODE_CLASS;   //代码编号
	CString CODE_NAME;   //代码名称
	CString CODE_DESC_1_NAME;   //代码描述一名称
	CString CODE_DESC_2_NAME;   //代码描述二名称
	CString CODE_DESC_3_NAME;   //代码描述三名称
	CString CODE_DESC_4_NAME;   //代码描述四名称
	CString CODE_DESC_5_NAME;   //代码描述五名称
	CString REMARK;   //备注
				*/

				/*
				INSERT INTO TEP0001(CODE_CLASS,CODE_NAME
,CODE_DESC_1_NAME)
VALUES('aa','bb','cc')
				*/
				tgcpmep01.TrimOrBlank(); 
				c_sql_condition = "insert into tep0001 ( "
					" rec_creator,rec_create_time " //创建者，创建时刻。
					" ,CODE_CLASS,CODE_NAME"
					" ,CODE_DESC_1_NAME,CODE_DESC_2_NAME"
					" ,CODE_DESC_3_NAME,CODE_DESC_4_NAME"
					" ,CODE_DESC_5_NAME" 	
					")VALUES ("
					" @rec_creator,@rec_create_time"
					",@code_class,@code_name"
					",@code_desc_1_name,@code_desc_2_name"
					",@code_desc_3_name,@code_desc_4_name"
					",@code_desc_5_name" 
					")";

				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("rec_creator", userid);//创建者
				cmd_sql.Parameters.Set("rec_create_time", systime);//创建时刻。
				
				cmd_sql.Parameters.Set("code_class", tgcpmep01.CODE_CLASS);//代码编号。
				cmd_sql.Parameters.Set("code_name", tgcpmep01.CODE_NAME);// 
				cmd_sql.Parameters.Set("code_desc_1_name", tgcpmep01.CODE_DESC_1_NAME); 
				cmd_sql.Parameters.Set("code_desc_2_name", tgcpmep01.CODE_DESC_2_NAME); 
				cmd_sql.Parameters.Set("code_desc_3_name", tgcpmep01.CODE_DESC_3_NAME); 
				cmd_sql.Parameters.Set("code_desc_4_name", tgcpmep01.CODE_DESC_4_NAME); 
				cmd_sql.Parameters.Set("code_desc_5_name", tgcpmep01.CODE_DESC_5_NAME); 
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close();

				Log::Trace("", __FUNCTION__, "insert into tep0001,代码编号[{0}]", tgcpmep01.CODE_CLASS);


				//TEP0001_RES.
				//culture = zh_Hans= 中文简体
				c_sql_condition = "insert into tep0001_res ( "
					"  CULTURE " //语言。
					" ,CODE_CLASS,CODE_NAME"
					" ,CODE_DESC_1_NAME,CODE_DESC_2_NAME"
					" ,CODE_DESC_3_NAME,CODE_DESC_4_NAME"
					" ,CODE_DESC_5_NAME"
					")VALUES ("
					" 'zh_Hans'"//中文简体
					",@code_class,@code_name"
					",@code_desc_1_name,@code_desc_2_name"
					",@code_desc_3_name,@code_desc_4_name"
					",@code_desc_5_name"
					")";

				sqlstr = c_sql_condition; 
				cmd_sql.Parameters.Set("code_class", tgcpmep01.CODE_CLASS);//代码编号。
				cmd_sql.Parameters.Set("code_name", tgcpmep01.CODE_NAME);// 
				cmd_sql.Parameters.Set("code_desc_1_name", tgcpmep01.CODE_DESC_1_NAME);
				cmd_sql.Parameters.Set("code_desc_2_name", tgcpmep01.CODE_DESC_2_NAME);
				cmd_sql.Parameters.Set("code_desc_3_name", tgcpmep01.CODE_DESC_3_NAME);
				cmd_sql.Parameters.Set("code_desc_4_name", tgcpmep01.CODE_DESC_4_NAME);
				cmd_sql.Parameters.Set("code_desc_5_name", tgcpmep01.CODE_DESC_5_NAME); 
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close();
				Log::Trace("", __FUNCTION__, "insert into tep0001_res,代码编号[{0}]", tgcpmep01.CODE_CLASS);



			}
		}






		v_blk_name = "GCPMEP01_UPD";//修改的BLK
		if (bcls_rec->Tables.Contains(v_blk_name))
		{
			v_row_num = bcls_rec->Tables[v_blk_name].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "in v_blk_name[{0}]v_row_num[{1}]  ", v_blk_name, v_row_num);
			for (i = 0; i < v_row_num; i++)
			{
				//   循环获取输入参数
				//======================
				tgcpmep01.MergeFrom(bcls_rec->Tables[v_blk_name].Rows[i]);
				tgcpmep01.TrimOrBlank();
				//==================================================== 
				//   输入参数校验
				//====================================================
				if (tgcpmep01.CODE_CLASS.Trim() == "")
				{
					sprintf(s.msg, "[代码编号]不允许为空。");
					throw CApplicationException(-1, s.msg, log.Location);
				}

				//v_cnt = 0;
				//c_sql_condition = "select count(1) from tep0001 t "
				//	" where t.code_class   = @code_class " //代码编号
				//	;
				//sqlstr = c_sql_condition;
				//cmd_sql.Parameters.Set("code_class", tgcpmep01.CODE_CLASS);//代码编号。 
				//cmd_sql.SetCommandText(c_sql_condition);
				//v_cnt = cmd_sql.ExecuteScalar();
				//cmd_sql.Close();

				//if (v_cnt < 1)
				//{//若已经存在，则不允许报错。
				//	sprintf(s.msg, "代码编号[%s]不存在，不需要修改。\n当前操作失败。"
				//		, (const char*)tgcpmep01.CODE_CLASS);
				//	throw CApplicationException(-1, s.msg, log.Location);
				//}


				 

				//==============
				//修改表。。。tep0001,tep0001_res
				//==============  
				c_sql_condition = "update tep0001 t "
					" set t.code_name = @code_name "
					"     ,t.code_desc_1_name = @code_desc_1_name "
					"     ,t.code_desc_2_name = @code_desc_2_name "
					"     ,t.code_desc_3_name = @code_desc_3_name "
					"     ,t.code_desc_4_name = @code_desc_4_name "
					"     ,t.code_desc_5_name = @code_desc_5_name "
					"     ,t.remark = @remark "
					" where t.code_class = @code_class "	//代码编号。				
					; 
				sqlstr = c_sql_condition;  
				cmd_sql.Parameters.Set("code_class", tgcpmep01.CODE_CLASS);//代码编号。
				cmd_sql.Parameters.Set("code_name", tgcpmep01.CODE_NAME);// 
				cmd_sql.Parameters.Set("code_desc_1_name", tgcpmep01.CODE_DESC_1_NAME);
				cmd_sql.Parameters.Set("code_desc_2_name", tgcpmep01.CODE_DESC_2_NAME);
				cmd_sql.Parameters.Set("code_desc_3_name", tgcpmep01.CODE_DESC_3_NAME);
				cmd_sql.Parameters.Set("code_desc_4_name", tgcpmep01.CODE_DESC_4_NAME);
				cmd_sql.Parameters.Set("code_desc_5_name", tgcpmep01.CODE_DESC_5_NAME);
				cmd_sql.Parameters.Set("remark", tgcpmep01.REMARK);
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close(); 

				Log::Trace("", __FUNCTION__, "update tep0001,代码编号[{0}]", tgcpmep01.CODE_CLASS);


				//==============
				//修改表。。。tep0001,tep0001_res
				//==============  
				//TEP0001_RES.
				//culture = zh_Hans= 中文简体
				c_sql_condition = "update tep0001_res t "
					" set t.code_name = @code_name "
					"     ,t.code_desc_1_name = @code_desc_1_name "
					"     ,t.code_desc_2_name = @code_desc_2_name "
					"     ,t.code_desc_3_name = @code_desc_3_name "
					"     ,t.code_desc_4_name = @code_desc_4_name "
					"     ,t.code_desc_5_name = @code_desc_5_name " 
					" where t.code_class = @code_class "	//代码编号。
					" and   t.culture    = 'zh_Hans' " //中文简体
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", tgcpmep01.CODE_CLASS);//代码编号。
				cmd_sql.Parameters.Set("code_name", tgcpmep01.CODE_NAME);// 
				cmd_sql.Parameters.Set("code_desc_1_name", tgcpmep01.CODE_DESC_1_NAME);
				cmd_sql.Parameters.Set("code_desc_2_name", tgcpmep01.CODE_DESC_2_NAME);
				cmd_sql.Parameters.Set("code_desc_3_name", tgcpmep01.CODE_DESC_3_NAME);
				cmd_sql.Parameters.Set("code_desc_4_name", tgcpmep01.CODE_DESC_4_NAME);
				cmd_sql.Parameters.Set("code_desc_5_name", tgcpmep01.CODE_DESC_5_NAME); 
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close();

				Log::Trace("", __FUNCTION__, "update tep0001_res,代码编号[{0}]", tgcpmep01.CODE_CLASS);

 

			}
		}


		//删除的BLK.
		//==========
		v_blk_name = "GCPMEP01_DEL";//删除的BLK
		if (bcls_rec->Tables.Contains(v_blk_name))
		{
			v_row_num = bcls_rec->Tables[v_blk_name].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "in v_blk_name[{0}]v_row_num[{1}]  ", v_blk_name, v_row_num);
			for (i = 0; i < v_row_num; i++)
			{
				//   循环获取输入参数
				//======================
				tgcpmep01.MergeFrom(bcls_rec->Tables[v_blk_name].Rows[i]);
				tgcpmep01.TrimOrBlank();
				//==================================================== 
				//   输入参数校验
				//====================================================
				if (tgcpmep01.CODE_CLASS.Trim() == "")
				{
					sprintf(s.msg, "[代码编号]不允许为空。");
					throw CApplicationException(-1, s.msg, log.Location);
				}

				//v_cnt = 0;
				//c_sql_condition = "select count(1) from tep0001 t "
				//	" where t.code_class   = @code_class " //代码编号
				//	;
				//sqlstr = c_sql_condition;
				//cmd_sql.Parameters.Set("code_class", tgcpmep01.CODE_CLASS);//代码编号。 
				//cmd_sql.SetCommandText(c_sql_condition);
				//v_cnt = cmd_sql.ExecuteScalar();
				//cmd_sql.Close();

				//if (v_cnt < 1)
				//{//若已经存在，则不允许报错。
				//	sprintf(s.msg, "代码编号[%s]不存在，不需要删除。\n当前操作失败。"
				//		, (const char*)tgcpmep01.CODE_CLASS);
				//	throw CApplicationException(-1, s.msg, log.Location);
				//}

				//==============
				//删除表。。。tep0001,tep0001_res,tep0002,tep0002_res
				//==============  
				c_sql_condition = "delete from tep0001 t " 
					" where t.code_class = @code_class "	//代码编号。				
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", tgcpmep01.CODE_CLASS);//代码编号。 
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close();


				c_sql_condition = "delete from tep0001_res t "
					" where t.code_class = @code_class "	//代码编号。				
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", tgcpmep01.CODE_CLASS);//代码编号。 
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close();

				c_sql_condition = "delete from tep0002 t "
					" where t.code_class = @code_class "	//代码编号。				
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", tgcpmep01.CODE_CLASS);//代码编号。 
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close();

				c_sql_condition = "delete from tep0002_res t "
					" where t.code_class = @code_class "	//代码编号。				
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", tgcpmep01.CODE_CLASS);//代码编号。 
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
