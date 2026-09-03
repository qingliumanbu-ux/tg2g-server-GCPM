 /*============================================================================*/
/*== [service名  ]:  gcpmdm03_do        ||  [对应VC#画面 ]:GCPMDM03           ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2018-3-14 16:13:05 ==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                   ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMDM03                                            ==*/
/*== [调用函数   ]： 无				                                      ==*/
/*== [service功能]： TGCPMDM03_信息维护                                   ==*/
/*========================================================================*/
/******框架头******/  
#include "stdafx.h"
 

//SERVICE入口
BM2F_ENTERACE(gcpmdm03_do);

int f_gcpmdm03_do(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection * conn)
{
	CTracer log(__FUNCTION__);

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmdm03_do";                //定义函数英文名称  
	CString FunctionCname = "系统代码明细维护";           //定义函数中文名称

	/* 程序内部变量 */
	int i = 0;
	int doFlag = 0; 
	int v_row_num = 0; //行个数。
	CDecimal v_cnt = 0; 
	CString v_blk_name = "XX";


	/* 实体类定义 */
	CModel tgcpmdm03("TGCPMDM03");

	/* 数据库SQL操作字符串*/
	CString  sqlstr = "";
	try
	{


		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString  c_sql_orderBy = "";


		 
		//获得系统时间，当前用户代码
		CString systime = CDateTime::Now().ToString("yyyyMMddHHmmss");
		CString userid = s.userid;



		//根据[表名称]，进行对应责任者的校验，确认有权限，才允许操作。
		//============================= 
		CString v_table_name = "TGCPMDM03";
		v_cnt = 0;
		c_sql_condition = "select count(1) from tgcpmsi00 t "
			" where t.code_class   ='GCPE' "
			" and   t.code = @code " //指定的表名称
			" and ( t.code_desc_5_content || ','  LIKE  '%' || @code_desc_5_content || ',%'  "
			"       or t.code_desc_5_content = ' ' )   "  //在[责任者]要求范围内。
			;
		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("code", v_table_name);//表名称
		cmd_sql.Parameters.Set("code_desc_5_content", userid);//责任者。 
		cmd_sql.SetCommandText(c_sql_condition);
		v_cnt = cmd_sql.ExecuteScalar().ToInt32();
		cmd_sql.Close();

		if (v_cnt <= 0)
		{//若没有找到记录，则说明当前用户，不能进行当前DLL 的操作。

			sprintf(s.msg, "您的帐号[%s]没有业务表[%s]的操作权限，当前操作失败。"
				, (const char*)userid, (const char*)v_table_name);
			throw CApplicationException(-1, s.msg, log.Location);
		}



		//==主键信息。 
		CString v_col_code_class = "";
		CString v_col_item_ename = ""; 
		CDecimal v_col_seq_no = 0;


		

		
		v_blk_name = "GCPMDM03_INS";//新增的BLK
		if (bcls_rec->Tables.Contains(v_blk_name))
		{
			v_row_num = bcls_rec->Tables[v_blk_name].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "in v_blk_name[{0}]v_row_num[{1}]  ", v_blk_name, v_row_num);
			for (i = 0; i < v_row_num; i++)
			{
				//   循环获取输入参数
				//======================
				tgcpmdm03.MergeFrom(bcls_rec->Tables[v_blk_name].Rows[i]);
				tgcpmdm03.TrimOrBlank();


				//==主键信息。 
				 v_col_code_class = tgcpmdm03["CODE_CLASS"];//代码编号
				 v_col_seq_no = tgcpmdm03["SEQ_NO"];//SEQ

				Log::Trace("", __FUNCTION__, "第[{0}]个，CODE_CLASS =[{1}]v_col_seq_no[{2}]"
					, i + 1, v_col_code_class, v_col_seq_no);

				//==================================================== 
				//   输入参数校验
				//====================================================
				if (tgcpmdm03["CODE_CLASS"].ToString().Trim() == "")
				{
					sprintf(s.msg, "事件号不允许为空。");
					throw CApplicationException(-1, s.msg, log.Location);
				}
				 

				//获取最大流水号+1。
				//=================
				v_col_seq_no = 0;
				c_sql_condition = "select MAX(seq_no)+1 from tgcpmdm03 t "
					" where t.code_class  = @code_class " 
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", v_col_code_class); 
				cmd_sql.SetCommandText(c_sql_condition);
				tgcpmdm03["SEQ_NO"] = cmd_sql.ExecuteScalar();
				cmd_sql.Close();

				 



				//==============
				//新增。
				//==============
				tgcpmdm03["REC_CREATOR"] = userid;
				tgcpmdm03["REC_CREATE_TIME"] = systime; 
				tgcpmdm03["REC_REVISE_TIME"] = "";
				tgcpmdm03["REC_REVISOR"] = "";
				tgcpmdm03.TrimOrBlank();
				sqlstr = "insert into tgcpmdm03,代码编号[" + tgcpmdm03["CODE_CLASS"].ToString() + "]";
				tgcpmdm03.Insert();

			}

		}


		



		v_blk_name = "GCPMDM03_UPD";//修改的BLK
		if (bcls_rec->Tables.Contains(v_blk_name))
		{
			v_row_num = bcls_rec->Tables[v_blk_name].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "in v_blk_name[{0}]v_row_num[{1}]  ", v_blk_name, v_row_num);
			for (i = 0; i < v_row_num; i++)
			{
				//   循环获取输入参数
				//======================
				tgcpmdm03.MergeFrom(bcls_rec->Tables[v_blk_name].Rows[i]);
				tgcpmdm03.TrimOrBlank();

				//==主键信息。 
				v_col_code_class = tgcpmdm03["CODE_CLASS"];//代码编号
				v_col_seq_no = tgcpmdm03["SEQ_NO"];//序号

				Log::Trace("", __FUNCTION__, "第[{0}]个，CODE_CLASS =[{1}]v_col_seq_no[{2}]"
					, i + 1, v_col_code_class, v_col_seq_no);

				


				//==================================================== 
				//   输入参数校验
				//====================================================
				if (tgcpmdm03["CODE_CLASS"].ToString().Trim() == "")
				{
					sprintf(s.msg, "事件号不允许为空。");
					throw CApplicationException(-1, s.msg, log.Location);
				}


				//主键重复性的校验。
				//=================
				v_cnt = 0;
				c_sql_condition = "select count(1) from tgcpmdm03 t "
					" where t.code_class  = @code_class "
					" and   t.seq_no  = @seq_no "
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", v_col_code_class);
				cmd_sql.Parameters.Set("seq_no", v_col_seq_no);
				cmd_sql.SetCommandText(c_sql_condition);
				v_cnt = cmd_sql.ExecuteScalar();
				cmd_sql.Close();

				if (v_cnt <= 0)
				{//若没找到记录，则说明信息不存在了。

					sprintf(s.msg, "代码编号[%s]序号[%d],对应的信息不存在，\n不需要修改。\n请及时刷新画面。"
						, (const char*)v_col_code_class
						, v_col_seq_no.ToInt32());
					throw CApplicationException(-1, s.msg, log.Location);
				} 
				 

				//////==============
				//////修改表。。。
				//////============== 
				//CString CODE_CLASS;   //代码编号
				//CDecimal SEQ_NO;   //序号
				//==================
				//CString ITEM_ENAME;   //字段英文名 
				//CString CONDITION_01;   //条件01
				//CString CODE;   //代码
				//CString CODE_DESC;   //代码描述
				//CString REMARK;   //备注
				//CString CODE_DESC_1_NAME;   //代码描述一名称
				//CString CODE_DESC_2_NAME;   //代码描述二名称
				//CString CODE_DESC_3_NAME;   //代码描述三名称
				//CString CODE_DESC_4_NAME;   //代码描述四名称
				//CString CODE_DESC_5_NAME;   //代码描述五名称
				//CString VALID_FLAG;   //生效标记

				tgcpmdm03["REC_REVISE_TIME"] = systime;
				tgcpmdm03["REC_REVISOR"] = userid;
				tgcpmdm03.TrimOrBlank();
				sqlstr = "update tgcpmdm03,事件号[" + tgcpmdm03["CODE_CLASS"].ToString() + "]";
				tgcpmdm03.Update("ITEM_ENAME,CONDITION_01,CODE,CODE_DESC"
					",REMARK"   
					",VALID_FLAG"
					",CODE_DESC_1_NAME,CODE_DESC_2_NAME,CODE_DESC_3_NAME"
					",CODE_DESC_4_NAME,CODE_DESC_5_NAME"
					",REC_REVISE_TIME,REC_REVISOR" //修改时刻，修改者
					,"CODE_CLASS,SEQ_NO" //主键
					);

			}
		}


		//删除的BLK.
		//==========
		v_blk_name = "GCPMDM03_DEL";//删除的BLK
		if (bcls_rec->Tables.Contains(v_blk_name))
		{
			v_row_num = bcls_rec->Tables[v_blk_name].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "in v_blk_name[{0}]v_row_num[{1}]  ", v_blk_name, v_row_num);
			for (i = 0; i < v_row_num; i++)
			{
				//   循环获取输入参数
				//======================
				tgcpmdm03.MergeFrom(bcls_rec->Tables[v_blk_name].Rows[i]);
				tgcpmdm03.TrimOrBlank();
 
				//==主键信息。 
				v_col_code_class = tgcpmdm03["CODE_CLASS"];//代码编号
				v_col_seq_no = tgcpmdm03["SEQ_NO"];//序号

				Log::Trace("", __FUNCTION__, "第[{0}]个，CODE_CLASS =[{1}]v_col_seq_no[{2}]"
					, i + 1, v_col_code_class, v_col_seq_no);




				//==================================================== 
				//   输入参数校验
				//====================================================
				if (tgcpmdm03["CODE_CLASS"].ToString().Trim() == "")
				{
					sprintf(s.msg, "事件号不允许为空。");
					throw CApplicationException(-1, s.msg, log.Location);
				}


				//主键重复性的校验。
				//=================
				v_cnt = 0;
				c_sql_condition = "select count(1) from tgcpmdm03 t "
					" where t.code_class  = @code_class "
					" and   t.seq_no  = @seq_no "
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", v_col_code_class);
				cmd_sql.Parameters.Set("seq_no", v_col_seq_no);
				cmd_sql.SetCommandText(c_sql_condition);
				v_cnt = cmd_sql.ExecuteScalar();
				cmd_sql.Close();

				if (v_cnt <= 0)
				{//若没找到记录，则说明信息不存在了。

					sprintf(s.msg, "代码编号[%s]序号[%d],对应的信息不存在，\n不需要删除。\n请及时刷新画面。"
						, (const char*)v_col_code_class
						, v_col_seq_no.ToInt32());
					throw CApplicationException(-1, s.msg, log.Location);
				}




				//校验通过过，进行删除操作。
				//=========================  
				c_sql_condition = "delete from tgcpmdm03 t "
					" where t.code_class  = @code_class "
					" and   t.seq_no  = @seq_no "
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", v_col_code_class);
				cmd_sql.Parameters.Set("seq_no", v_col_seq_no);
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery(); 

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
