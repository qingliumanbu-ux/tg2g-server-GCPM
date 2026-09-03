 /*============================================================================*/
/*== [service名  ]:  gcpmdm01_do        ||  [对应VC#画面 ]:GCPMDM01           ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2018-3-14 16:13:05 ==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                   ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMDM01                                            ==*/
/*== [调用函数   ]： 无				                                      ==*/
/*== [service功能]： TGCPMDM01_信息维护                                   ==*/
/*========================================================================*/
/******框架头******/  
#include "stdafx.h"
 
 
 

//SERVICE入口
BM2F_ENTERACE(gcpmdm01_do);

int f_gcpmdm01_do(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection * conn)
{
	CTracer log(__FUNCTION__);

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmdm01_do";                //定义函数英文名称  
	CString FunctionCname = "系统代码主信息维护";           //定义函数中文名称

	/* 程序内部变量 */
	int i = 0;
	int doFlag = 0; 
	int v_row_num = 0; //行个数。
	CDecimal v_cnt = 0; 
	CString v_blk_name = "XX";


	/* 实体类定义 */
	CModel tgcpmdm01("TGCPMDM01");
	CModel tgcpmdm02("TGCPMDM02");
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


		/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString  c_sql_where2 = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition2 = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString  c_sql_orderBy2 = "";


		 
		//获得系统时间，当前用户代码
		CString systime = CDateTime::Now().ToString("yyyyMMddHHmmss");
		CString userid = s.userid;



		//根据[表名称]，进行对应责任者的校验，确认有权限，才允许操作。
		//============================= 
		CString v_table_name = "TGCPMDM01";
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
		CString v_col_code_name = "";
		

		
		v_blk_name = "GCPMDM01_INS";//新增的BLK
		if (bcls_rec->Tables.Contains(v_blk_name))
		{
			v_row_num = bcls_rec->Tables[v_blk_name].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "in v_blk_name[{0}]v_row_num[{1}]  ", v_blk_name, v_row_num);
			for (i = 0; i < v_row_num; i++)
			{
				//   循环获取输入参数
				//======================
				tgcpmdm01.CopyFrom(tgcpmdm02);
				tgcpmdm01.MergeFrom(bcls_rec->Tables[v_blk_name].Rows[i]);
				tgcpmdm01.TrimOrBlank();


				//==主键信息。 
				 v_col_code_class = tgcpmdm01["CODE_CLASS"];//代码编号
				 v_col_code_name = tgcpmdm01["CODE_NAME"];//代码名称。

				Log::Trace("", __FUNCTION__, "第[{0}]个，CODE_CLASS =[{1}]CODE_NAME[{2}]"
					, i + 1, v_col_code_class, v_col_code_name);

				//==================================================== 
				//   输入参数校验
				//====================================================
				if (tgcpmdm01["CODE_CLASS"].ToString().Trim() == "")
				{
					sprintf(s.msg, "事件号不允许为空。");
					throw CApplicationException(-1, s.msg, log.Location);
				}
				 

				//主键重复性的校验。
				//=================
				v_cnt = 0;
				c_sql_condition = "select count(1) from tgcpmdm01 t "
					" where t.code_class  = @code_class "
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", v_col_code_class);
				cmd_sql.SetCommandText(c_sql_condition);
				v_cnt = cmd_sql.ExecuteScalar();
				cmd_sql.Close();

				if (v_cnt >= 1)
				{//若找到记录，则说明主键重复了。

					sprintf(s.msg, "代码编号[%s]，对应的信息已经存在，\n不允许新增。"
						, (const char*)v_col_code_class);
					throw CApplicationException(-1, s.msg, log.Location);
				}



				//==============
				//新增。
				//==============
				tgcpmdm01["REC_CREATOR"] = userid;
				tgcpmdm01["REC_CREATE_TIME"] = systime;// 
				tgcpmdm01["REC_REVISE_TIME"] = "";
				tgcpmdm01["REC_REVISOR"] = "";
				tgcpmdm01.TrimOrBlank();
				sqlstr = "insert into tgcpmdm01,代码编号[" + tgcpmdm01["CODE_CLASS"].ToString() + "]";
				tgcpmdm01.Insert();








			}

		}


		



		v_blk_name = "GCPMDM01_UPD";//修改的BLK
		if (bcls_rec->Tables.Contains(v_blk_name))
		{
			v_row_num = bcls_rec->Tables[v_blk_name].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "in v_blk_name[{0}]v_row_num[{1}]  ", v_blk_name, v_row_num);
			for (i = 0; i < v_row_num; i++)
			{
				//   循环获取输入参数
				//======================
				tgcpmdm01.MergeFrom(bcls_rec->Tables[v_blk_name].Rows[i]);
				tgcpmdm01.TrimOrBlank();

				//==主键信息。 
				v_col_code_class = tgcpmdm01["CODE_CLASS"];//代码编号
				v_col_code_name = tgcpmdm01["CODE_NAME"];//代码名称。

				Log::Trace("", __FUNCTION__, "第[{0}]个，CODE_CLASS =[{1}]CODE_NAME[{2}]"
					, i + 1, v_col_code_class, v_col_code_name);

				if (v_col_code_class.Trim() == "")
				{
					sprintf(s.msg, "[代码编号]不能为空。");
					throw CApplicationException(-1, s.msg, log.Location);
				}

				//数据【存在性】的校验。
				//=================
				v_cnt = 0;
				c_sql_condition = "select count(1) from tgcpmdm01 t "
					" where t.code_class  = @code_class "
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", v_col_code_class);
				cmd_sql.SetCommandText(c_sql_condition);
				v_cnt = cmd_sql.ExecuteScalar();
				cmd_sql.Close();

				if (v_cnt <= 0)
				{//若没找到记录，则说明信息不存在了。

					sprintf(s.msg, "代码编号[%s],对应的信息不存在，\n不需要修改。\n请及时刷新画面。"
						, (const char*)v_col_code_class);
					throw CApplicationException(-1, s.msg, log.Location);
				} 
				 

				////==============
				////修改表。。。
				////============== 
				//CString CODE_CLASS;   //代码编号
				//CString CODE_NAME;   //代码名称
				//CString CODE_ENAME;   //代码英文名称
				//CString CODE_DEFINE_DESC;   //代码定义描述
				//CString CHINESE_USNM_ABBR;   //中文简称
				//CString ENGLISH_USNM_ABBR;   //英文简称
				//CString VALID_FLAG;   //生效标记
				//CString CODE_DESC_1_NAME;   //代码描述一名称
				//CString CODE_DESC_2_NAME;   //代码描述二名称
				//CString CODE_DESC_3_NAME;   //代码描述三名称
				//CString CODE_DESC_4_NAME;   //代码描述四名称
				//CString CODE_DESC_5_NAME;   //代码描述五名称

				tgcpmdm01["REC_REVISE_TIME"] = systime;
				tgcpmdm01["REC_REVISOR"] = userid;
				tgcpmdm01.TrimOrBlank();
				sqlstr = "update tgcpmdm01,事件号[" + tgcpmdm01["CODE_CLASS"].ToString() + "]";
				tgcpmdm01.Update("CODE_NAME,CODE_ENAME,CODE_DEFINE_DESC"
					",CHINESE_USNM_ABBR,ENGLISH_USNM_ABBR"
					",VALID_FLAG" //独立定义标志。
					",CODE_DESC_1_NAME,CODE_DESC_2_NAME,CODE_DESC_3_NAME"
					",CODE_DESC_4_NAME,CODE_DESC_5_NAME"
					",REC_REVISE_TIME,REC_REVISOR"
					,"CODE_CLASS" //主键
					);




				//若代码主信息是‘独立定义’，
				//则默认新增 DM02,DM03
				//=======================================
				Log::Trace("", __FUNCTION__, "第[{0}]个独立定义标志[{1}]"
					, i + 1, tgcpmdm01["VALID_FLAG"].ToString());
				if (tgcpmdm01["VALID_FLAG"].ToString().Trim().ToUpper() == "Y")
				{
					//先根据CODE_CLASS ,进行删除操作。
					//==================
					c_sql_condition = " delete from tgcpmdm02 t "
						" where t.code_class  = @code_class "
						;
					sqlstr = c_sql_condition;
					cmd_sql.Parameters.Set("code_class", v_col_code_class);
					cmd_sql.SetCommandText(c_sql_condition);
					cmd_sql.ExecuteNonQuery();
					cmd_sql.Close();

					//先根据CODE_CLASS ,进行删除操作。
					//==================
					c_sql_condition = " delete from tgcpmdm03 t "
						" where t.code_class  = @code_class "
						;
					sqlstr = c_sql_condition;
					cmd_sql.Parameters.Set("code_class", v_col_code_class);
					cmd_sql.SetCommandText(c_sql_condition);
					cmd_sql.ExecuteNonQuery();
					cmd_sql.Close();



					//若代码主信息是‘独立定义’，
					//则默认新增 DM02,DM03
					//=======================================
					tgcpmdm02.CopyFrom(tgcpmdm01);
					tgcpmdm02["ITEM_ENAME"] = "A"; //独立定义的，默认就一个项目A.
					tgcpmdm02["ITEM_CNAME"]  = tgcpmdm01["CODE_NAME"];   //项目中文= 代码中文
					tgcpmdm02["ITEM_TYPE"]  = "C"; //CString ITEM_TYPE;   //字段类型=C=字符
					tgcpmdm02["ITEM_LEN"]  = 1; //CString ITEM_LEN;   //字段长度=1=默认
					tgcpmdm02.TrimOrBlank();
					sqlstr = "复制新增DT02表,系统代码[" + tgcpmdm02["CODE_CLASS"].ToString() + "]";
					tgcpmdm02.Insert();



					//若代码主信息是‘独立定义’，
					//则默认新增 DM02,DM03
					//=======================================
					tgcpmdm03.CopyFrom(tgcpmdm02);
					tgcpmdm03["CODE"] = tgcpmdm03["CODE_DESC_1_NAME"]; //独立定义的，CODE = 值集编号
					tgcpmdm03.TrimOrBlank();
					sqlstr = "自动复制新增DT03表， 系统代码[" + tgcpmdm03["CODE_CLASS"].ToString() + "]";
					tgcpmdm03.Insert();

				}// if。。end //若代码主信息是‘独立定义’，



				//根据‘代码编号’获取对应的N个代码项目，
				//若项目中有独立定义的，则自动新增项目对应明细。
				//CString ITEM_ENAME;   //字段英文名
				//===============
				tgcpmdm02.Reset();
				c_sql_condition = "select t.* from tgcpmdm02 t "
					" where t.code_class  = @code_class "
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", v_col_code_class);
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteReader();
				while (cmd_sql.Read())
				{
					cmd_sql.Fetch(tgcpmdm02);
					if (tgcpmdm02["VALID_FLAG"].ToString().Trim().ToUpper() == "Y")
					{//若发现某个项目是独立定义的。
						Log::Trace("", __FUNCTION__, "代码[{0}]项目[{1}]独立定义[{2}]"
							, v_col_code_class
							, tgcpmdm02["ITEM_ENAME"].ToString()
							, tgcpmdm02["VALID_FLAG"].ToString() );


						//自动新增该项目对应的明细。
						//================
						//先删除，再新增
						//=============

						//先根据CODE_CLASS+项目号 ,进行删除操作。
						//==================
						c_sql_condition2 = " delete from tgcpmdm03 t "
							" where t.code_class  = @code_class "//代码编号
							" and   t.item_ename  = @item_ename "//项目号
							;
						sqlstr = c_sql_condition2;
						cmd_sql2.Parameters.Set("code_class", v_col_code_class);
						cmd_sql2.Parameters.Set("item_ename", tgcpmdm02["ITEM_ENAME"].ToString());
						cmd_sql2.SetCommandText(c_sql_condition2);
						cmd_sql2.ExecuteNonQuery();
						cmd_sql2.Close();


						//新增03表。
						//========= 
						tgcpmdm03.CopyFrom(tgcpmdm02);
						tgcpmdm03["CODE"]  = tgcpmdm03["CODE_DESC_1_NAME"]; //独立定义的，CODE = 值集编号
						tgcpmdm03["CODE_DESC"]  = tgcpmdm02["ITEM_CNAME"]; //CString CODE_DESC;   //明细描述 = 项目名称

						//获取最大 SEQ_NO
						//=================
						v_cnt = 0;
						c_sql_condition2 = "select MAX(t.seq_no) + 1 "
							" from  tgcpmdm03 t "
							" where t.code_class  = @code_class "
							;
						sqlstr = c_sql_condition2;
						cmd_sql2.Parameters.Set("code_class", v_col_code_class);
						cmd_sql2.SetCommandText(c_sql_condition2);
						v_cnt = cmd_sql2.ExecuteScalar();
						cmd_sql2.Close();

						//获取最新的 SEQ_NO
						tgcpmdm03["SEQ_NO"] = v_cnt;

						tgcpmdm03.TrimOrBlank();
						sqlstr = "新增DT03表, 系统代码[ "
							+ tgcpmdm03["CODE_CLASS"].ToString() + "]项目["
							+ tgcpmdm03["ITEM_ENAME"].ToString() + "]";
						tgcpmdm03.Insert();

					}



					//新增逻辑，
					//若项目对应的明细不存在，则默认新增到明细表 03
					//============  
					v_cnt = 0;
					//先根据CODE_CLASS+项目号 ,判断是否在03表。
					//==================
					c_sql_condition2 = "select count(1) from tgcpmdm03 t "
						" where t.code_class  = @code_class "//系统代码
						" and   t.item_ename  = @item_ename "//ITEM_ENAME=项目号
						;
					sqlstr = c_sql_condition2;
					cmd_sql2.Parameters.Set("code_class", v_col_code_class);
					cmd_sql2.Parameters.Set("item_ename", tgcpmdm02["ITEM_ENAME"].ToString());
					cmd_sql2.SetCommandText(c_sql_condition2);
					v_cnt = cmd_sql2.ExecuteScalar();
					cmd_sql2.Close();

					Log::Trace("", __FUNCTION__, "==代码[{0}]项目[{1}]存在个数[{2}]"
						, v_col_code_class, tgcpmdm02["ITEM_ENAME"].ToString(), v_cnt);


					if (v_cnt <= 0)
					{//若没有对应的信息， 则自动新增一笔。

						//新增03表。
						//========= 
						tgcpmdm03.CopyFrom(tgcpmdm02);
						tgcpmdm03["CODE"] = ""; //CODE = 明细
						tgcpmdm03["CODE_DESC"] = tgcpmdm02["ITEM_CNAME"]; //CODE_DESC;   //明细描述=项目说明

						//获取最大 SEQ_NO
						//=================
						v_cnt = 0;
						c_sql_condition2 = "select MAX(t.seq_no) + 1 "
							" from  tgcpmdm03 t "
							" where t.code_class  = @code_class "
							;
						sqlstr = c_sql_condition2;
						cmd_sql2.Parameters.Set("code_class", v_col_code_class);
						cmd_sql2.SetCommandText(c_sql_condition2);
						v_cnt = cmd_sql2.ExecuteScalar();
						cmd_sql2.Close();

						//获取最新的 SEQ_NO
						tgcpmdm03["SEQ_NO"] = v_cnt;

						tgcpmdm03.TrimOrBlank();
						sqlstr = "新增DT03表, 系统代码[ "
							+ tgcpmdm03["CODE_CLASS"].ToString()
							+ "]项目["
							+ tgcpmdm03["ITEM_ENAME"].ToString() + "]";
						tgcpmdm03.Insert();



					}

				}
				cmd_sql.Close();






			}
		}


		//删除的BLK.
		//==========
		v_blk_name = "GCPMDM01_DEL";//删除的BLK
		if (bcls_rec->Tables.Contains(v_blk_name))
		{
			v_row_num = bcls_rec->Tables[v_blk_name].Rows.get_Count();
			Log::Trace("", __FUNCTION__, "in v_blk_name[{0}]v_row_num[{1}]  ", v_blk_name, v_row_num);
			for (i = 0; i < v_row_num; i++)
			{
				//   循环获取输入参数
				//======================
				tgcpmdm01.MergeFrom(bcls_rec->Tables[v_blk_name].Rows[i]);
				tgcpmdm01.TrimOrBlank();

				//==主键信息。 
				v_col_code_class = tgcpmdm01["CODE_CLASS"];//代码编号
				v_col_code_name = tgcpmdm01["CODE_NAME"];//代码名称。

				Log::Trace("", __FUNCTION__, "第[{0}]个，CODE_CLASS =[{1}]CODE_NAME[{2}]"
					, i + 1, v_col_code_class, v_col_code_name);

				if (v_col_code_class.Trim() == "")
				{
					sprintf(s.msg, "[代码编号]不能为空。");
					throw CApplicationException(-1, s.msg, log.Location);
				}

				//数据【存在性】的校验。
				//=================
				v_cnt = 0;
				c_sql_condition = "select count(1) from tgcpmdm01 t "
					" where t.code_class  = @code_class "
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", v_col_code_class);
				cmd_sql.SetCommandText(c_sql_condition);
				v_cnt = cmd_sql.ExecuteScalar();
				cmd_sql.Close();

				if (v_cnt <= 0)
				{//若没找到记录，则说明信息不存在了。

					sprintf(s.msg, "代码编号[%s],对应的信息不存在，\n不需要删除。\n请及时刷新画面。"
						, (const char*)v_col_code_class);
					throw CApplicationException(-1, s.msg, log.Location);
				}


				//校验通过过，进行删除操作。-代码主信息01
				//=========================  
				c_sql_condition = "delete from tgcpmdm01 t "
					" where t.code_class  = @code_class "
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", v_col_code_class);
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close();


				//校验通过过，进行删除操作。--代码项目信息02
				//=========================  
				c_sql_condition = "delete from tgcpmdm02 t "
					" where t.code_class  = @code_class "
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", v_col_code_class);
				cmd_sql.SetCommandText(c_sql_condition);
				cmd_sql.ExecuteNonQuery();
				cmd_sql.Close();


				//校验通过过，进行删除操作。--代码明细信息03
				//=========================  
				c_sql_condition = "delete from tgcpmdm03 t "
					" where t.code_class  = @code_class "
					;
				sqlstr = c_sql_condition;
				cmd_sql.Parameters.Set("code_class", v_col_code_class);
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
