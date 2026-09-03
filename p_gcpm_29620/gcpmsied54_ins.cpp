/*************************************************
版权: Baosight Software LTD.co Copyright (c) 2012
作者: zy
日期: 2015-2-4 11:11:29
功能: 根据ED54的配置信息_信息新增。
修改历史：
日期:________；修改人：________; 需求提出人________
变更内容:
**************************************************/
/***** C/C++ 的标准头文件部分 *****/
// New Include
#include "stdafx.h" 


/*<remark>=========================================================
/// <summary>
///  根据ED54的配置信息_信息新增。
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/

// service入口
BM2F_ENTERACE(gcpmsied54_ins)
//-EP_SYSTEM_HEAD_END                                                  
int f_gcpmsied54_ins(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);  // 系统日志，必须在代码段开始处定义

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsied54_ins";                //定义函数英文名称  
	CString FunctionCname = "根据ED54的配置信息_信息新增。";              //定义函数中文名称
	////LogTrace(1, 1, " **************%s begin*****************", (const char*)FunctionEname);


	//程序用变量
	int   doFlag = 0;
	int   i = 0;
	int   fetchRowCount = 0;
	int   v_cnt = 0;
	int   v_cnt2 = 0;
	int   v_cnt3 = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  function_id = "gcpmsied54_ins";  //自定义显示项目功能号    
	CString  sqlstr = "";  //SQL 信息。 

	CString v_table_name = "";//表名称。
	CString v_moid = ""; //模块代码
	CString v_func_id = "";


	//获得系统时间，当前用户代码
	CString systime = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString userid = s.userid;

	//写履历表 TGCPMSI99
	//=================
	CString v_rec_creator2 = userid;   //记录创建责任者
	CString v_rec_create_time2 = systime;   //记录创建时刻  
	CString v_track_seq_no2 = CDateTime::Now().ToString("yyyyMMddHHmmssfff");;   //事件跟踪序列号
	CString v_table_name99 = "";   //数据库表名
	CString v_event_id2 = "INS";   //事件标识
	CString v_error_remark2 = "";   //备注==表中的每个列信息拼接起来。比如： [工序]A1[工序类型]SM11...


	try
	{
		 

		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where = "  WHERE   1 = 1 "; //查询条件。
		CString  c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString  c_sql_orderBy = "";



		CString v_table_name2 = "WHERE_COLUMN";//作为WHERE的列信息。
		if (bcls_rec->Tables.Contains(v_table_name2))
		{//若已经存在，则先CLEAR.
			bcls_rec->Tables[v_table_name2].Clear();
		}
		bcls_rec->Tables.Add(v_table_name2);          // 
		bcls_rec->Tables[v_table_name2].Columns.Add(DT_STRING, "ITEM_ENAME");//列英文
		bcls_rec->Tables[v_table_name2].Columns.Add(DT_STRING, "ITEM_TYPE");//列类型
		bcls_rec->Tables[v_table_name2].Columns.Add(DT_STRING, "ITEM_CNAME");//列中文
		bcls_rec->Tables[v_table_name2].Columns.Add(DT_STRING, "ITEM_KEY_FLAG");//ITEM_KEY_FLAG=主键标志。



		CString v_table_name3 = "INSERT_COLUMN";//作为INSERT 的列信息。
		if (bcls_rec->Tables.Contains(v_table_name3))
		{//若已经存在，则先CLEAR.
			bcls_rec->Tables[v_table_name3].Clear();
		}
		bcls_rec->Tables.Add(v_table_name3);          // 
		bcls_rec->Tables[v_table_name3].Columns.Add(DT_STRING, "ITEM_ENAME");//列英文
		bcls_rec->Tables[v_table_name3].Columns.Add(DT_STRING, "ITEM_TYPE");//列类型
		bcls_rec->Tables[v_table_name3].Columns.Add(DT_STRING, "ITEM_CNAME");//列中文
		bcls_rec->Tables[v_table_name3].Columns.Add(DT_STRING, "ITEM_KEY_FLAG");//ITEM_KEY_FLAG=主键标志。



		//从2#BLK 中获取静态表的表名称。
		if (bcls_rec->Tables[1].Columns.Contains("MOID"))
			v_moid = bcls_rec->Tables[1].Rows[0]["MOID"].ToString();

		if (bcls_rec->Tables[1].Columns.Contains("TABLE_NAME"))
			v_table_name = bcls_rec->Tables[1].Rows[0]["TABLE_NAME"].ToString();

		Log::Trace("", __FUNCTION__, "二级模块 =[{0}] 表名称 =[{1}] ", v_moid, v_table_name);

		if (v_moid.Trim() == "")
		{
			sprintf(s.msg, "【二级模块】不允许为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		}

		if (v_table_name.Trim() == "")
		{
			sprintf(s.msg, "【表名称】不允许为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		} 



		//功能号 =二级模块_表名称===> GCPM_TGCPMSI01
		v_func_id = v_moid.ToUpper() + "_" + v_table_name.ToUpper();
		Log::Trace("", __FUNCTION__, "v_func_id =[{0}]", v_func_id);

		//查询条件功能号 = 二级模块_表名称_WHERE===>PMOF_TPMOFSI00_WHERE
		CString v_func_id_where = v_func_id + "_WHERE";

		//新增功能：
		//===addec by zy on 2017-6-29 16:05:05
		/*
		查询条件ED54功能号，
		显示列  ED54功能号，
		由前台传入。
		*/
		if (bcls_rec->Tables[1].Columns.Contains("ED54_WHERE"))
			v_func_id_where = bcls_rec->Tables[1].Rows[0]["ED54_WHERE"].ToString();

		if (bcls_rec->Tables[1].Columns.Contains("ED54_INQ"))
			v_func_id = bcls_rec->Tables[1].Rows[0]["ED54_INQ"].ToString();

		Log::Trace("", __FUNCTION__, "in ==v_func_id_where[{0}] v_func_id[{1}] "
			, v_func_id_where, v_func_id);





		//根据功能号，获取对应的主键字段，作为[新增]的依据。
		//================
		CString v_item_key_flag = "";
		CString v_item_ename = "";
		CString v_item_cname = "";
		CString v_item_type = "";
		CString v_item_value = "";

		CString v_inert_item = "";  





		//获取需要【WHERE条件】的列信息。
		//======================
		v_item_key_flag = "";
		v_item_ename = "";
		v_item_cname = "";
		v_item_type = "";		 

		fetchRowCount = 0;
		c_sql_condition = " select t.item_key_flag,t.item_ename,t.item_cname,t.item_type "
			" from ted54 t "
			" where t.func_id = @func_id "
			" and   t.item_key_flag  = '1' " //主键字段。
			" and   t.item_hide_flag = '0' " //修改的字段，是画面上显示的字段。不隐藏的字段。
			" order by t.class_code,t.seq_no " //必须顺序一致。
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句 
		// 设置SQL中的变量
		cmd_sql.Parameters.Set("func_id", v_func_id); //ED54功能号。
		cmd_sql.ExecuteReader();
		while (cmd_sql.Read())
		{

			v_item_key_flag = cmd_sql.GetString(1);
			v_item_ename = cmd_sql.GetString(2);
			v_item_cname = cmd_sql.GetString(3);
			v_item_type = cmd_sql.GetString(4); //字段类型
			Log::Trace("", __FUNCTION__, "主键字段--主键标志 =[{0}] 字段 =[{1}] 中文=[{2}]类型[{3}]"
				, v_item_key_flag, v_item_ename, v_item_cname, v_item_type);



			//列信息，压入一个BLK. 主键字段。
			//============ 
			bcls_rec->Tables[v_table_name2].Rows.Add();
			bcls_rec->Tables[v_table_name2].Rows[fetchRowCount]["ITEM_ENAME"] = v_item_ename;
			bcls_rec->Tables[v_table_name2].Rows[fetchRowCount]["ITEM_CNAME"] = v_item_cname;
			bcls_rec->Tables[v_table_name2].Rows[fetchRowCount]["ITEM_TYPE"] = v_item_type;
			fetchRowCount++;

		}
		cmd_sql.Close();

		 
		/*
		--新增表
		insert into hom00
		(REC_CREATOR,REC_CREATE_TIME）
		values(@REC_CREATOR,@REC_CREATE_TIME)
		;
		*/

		v_inert_item = ""; // "REC_CREATOR,REC_CREATE_TIME";//初始化：记录创建者,记录创建时刻
		fetchRowCount = 0;
		c_sql_condition = " select t.item_key_flag,t.item_ename,t.item_cname,t.item_type "
			" from ted54 t "
			" where t.func_id = @func_id "
			" and   t.item_hide_flag = '0' " //新增的字段，是画面上显示的字段,不隐藏的字段。
			" order by t.class_code,t.seq_no " //必须顺序一致。
			;
		sqlstr = c_sql_condition;
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句 
		// 设置SQL中的变量
		cmd_sql.Parameters.Set("func_id", v_func_id); //ED54功能号。
		cmd_sql.ExecuteReader();
		while (cmd_sql.Read())
		{
			
			v_item_key_flag = cmd_sql.GetString(1); //主键
			v_item_ename    = cmd_sql.GetString(2);
			v_item_cname    = cmd_sql.GetString(3);
			v_item_type     = cmd_sql.GetString(4); //字段类型
			Log::Trace("", __FUNCTION__, "主键标志 =[{0}] 字段 =[{1}] 中文=[{2}]类型[{3}]"
				, v_item_key_flag, v_item_ename, v_item_cname, v_item_type);

			if (fetchRowCount == 0)
			{//第一行的时候，
				v_inert_item = v_item_ename;
			}
			else
			{//非第一行的时候，
				v_inert_item = v_inert_item + " ," + v_item_ename;
			}


			//列信息，压入一个BLK.
			//============ 
			bcls_rec->Tables[v_table_name3].Rows.Add();
			bcls_rec->Tables[v_table_name3].Rows[fetchRowCount]["ITEM_ENAME"] = v_item_ename;
			bcls_rec->Tables[v_table_name3].Rows[fetchRowCount]["ITEM_TYPE"] = v_item_type;
			bcls_rec->Tables[v_table_name3].Rows[fetchRowCount]["ITEM_CNAME"] = v_item_cname;
			bcls_rec->Tables[v_table_name3].Rows[fetchRowCount]["ITEM_KEY_FLAG"] = v_item_key_flag; //主键标志。
			fetchRowCount++;
			
		}
		cmd_sql.Close();


		 
		v_cnt2 = bcls_rec->Tables[v_table_name2].Rows.get_Count();
		v_cnt3 = bcls_rec->Tables[v_table_name3].Rows.get_Count();

		Log::Trace("", __FUNCTION__, "新增字段个数： [{0}] 主键字段个数[{1}]", v_cnt3, v_cnt2);

		if (v_cnt2 <= 0)
		{//若没有主键，则报错。
			sprintf(s.msg, "【%s】表没有定义【主键列】，无法新增。\n请到EPED54画面及时维护。"
				, (const char*)v_table_name);
			throw CApplicationException(-1, s.msg, log.Location);
		}

		if (v_cnt3 <= 0)
		{//若没有可修改列，则报错。
			sprintf(s.msg, "【%s】表没有定义【可新增列】，无法修改。\n请到EPED54画面及时维护。"
				, (const char*)v_table_name);
			throw CApplicationException(-1, s.msg, log.Location);
		} 


		//前台选中的多行信息。FOR 
		//======================
		/*
		--新增表
		insert into hom00
		(REC_CREATOR,REC_CREATE_TIME）
		values(@REC_CREATOR,@REC_CREATE_TIME)
		;
		*/

		i = 0;
		int j = 0;
		CString c_sql_insert_temp = "";
		CString c_sql_insert_temp2 = "";


		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{

			c_sql_where = "  WHERE 1=1  ";
			v_item_ename = "";
			v_item_cname = ""; //列中文描述。
			v_item_type = "";
			v_item_value = "";
			c_sql_insert_temp = "";
			v_item_key_flag = ""; //关键值。

			//初始化新增内容： 创建者+创建时刻
			//@REC_CREATOR,@REC_CREATE_TIME
			c_sql_insert_temp2 = ""; // "'" + userid + "','" + systime + "'";

			//履历信息。
			v_error_remark2 = "";

			//循环获取作为新增的列信息。
			for (j = 0; j< bcls_rec->Tables[v_table_name3].Rows.get_Count(); j++)
			{
				//v_item_ename
				if (bcls_rec->Tables[v_table_name3].Columns.Contains("ITEM_ENAME"))
					v_item_ename = bcls_rec->Tables[v_table_name3].Rows[j]["ITEM_ENAME"].ToString().ToUpper();
				if (bcls_rec->Tables[v_table_name3].Columns.Contains("ITEM_TYPE"))
					v_item_type = bcls_rec->Tables[v_table_name3].Rows[j]["ITEM_TYPE"].ToString().ToUpper();
				if (bcls_rec->Tables[v_table_name3].Columns.Contains("ITEM_CNAME"))
					v_item_cname = bcls_rec->Tables[v_table_name3].Rows[j]["ITEM_CNAME"].ToString().ToUpper();
				if (bcls_rec->Tables[v_table_name3].Columns.Contains("ITEM_CNAME"))
					v_item_cname = bcls_rec->Tables[v_table_name3].Rows[j]["ITEM_CNAME"].ToString().ToUpper();
				if (bcls_rec->Tables[v_table_name3].Columns.Contains("ITEM_KEY_FLAG"))//
				bcls_rec->Tables[v_table_name3].Rows[j]["ITEM_KEY_FLAG"] = v_item_key_flag; //主键标志。

				Log::Trace("", __FUNCTION__, "v_item_ename =[{0}] v_item_type =[{1}]v_item_cname[{2}]"
					, v_item_ename, v_item_type, v_item_cname);
				Log::Trace("", __FUNCTION__, "v_item_ename =[{0}] v_item_key_flag =[{1}] "
					, v_item_ename, v_item_type, v_item_cname); 

				//根据列名称获取对应的内容。
				if (bcls_rec->Tables[0].Columns.Contains(v_item_ename))
					v_item_value = bcls_rec->Tables[0].Rows[i][v_item_ename].ToString().TrimOrBlank();

				Log::Trace("", __FUNCTION__, "v_item_value =[{0}] 序号[{1}]", v_item_value,j+1);

				//新增履历中的备注信息。
				v_error_remark2 = v_error_remark2 + v_item_cname + "[" + v_item_value + "]";

				
				//if (v_item_type.Trim() == "N")
				//{//若是数字型。 
				//	c_sql_insert_temp = v_item_value;
				//}
				//else
				//{//非数字型，都视为字符。
				//	c_sql_insert_temp = "'" + v_item_value + "' ";
				//}

				// values(@mat_no,@mat_wt,@mat_thick) //====转换成如此写法。
				//=======update by zy on 2019/9/2 15:12:47
				c_sql_insert_temp = "@" + v_item_ename;
				//cmd_sql.Parameters.Set("track_seq_no", v_track_seq_no);//跟踪号。 
				cmd_sql.Parameters.Set(v_item_ename, v_item_value); //SQL参数赋值  

				if (j == 0)
				{//若是第一行。
					c_sql_insert_temp2 = c_sql_insert_temp;
				}
				else
				{//非第一行,需要进行拼接。
					c_sql_insert_temp2 = c_sql_insert_temp2 + "," + c_sql_insert_temp;
				}

			}

			Log::Trace("", __FUNCTION__, "最终的c_sql_insert_temp2  ", c_sql_insert_temp2);

			CString v_column_name = "";  //列名称们...
			CString v_column_value = ""; //列内容们...

			v_column_name = "(" + v_inert_item + ")";
			v_column_value = " values(" + c_sql_insert_temp2 + ")";





			//新增前，新增主键重复的提醒。
			// select count(1) from 基表 t where t.主键= @主键;
			// if(个数 >= 1) ｛ 已经主键重复，则提醒。｝
			//========================on 2016-5-21 15:09:42
			c_sql_where = "  WHERE 1=1  ";
			v_item_ename = "";
			v_item_cname = ""; //列中文描述。
			v_item_type = "";
			v_item_value = "";

			//条件列
			CString c_sql_where_temp = "";
			CString c_sql_where_temp2 = "";
			CString v_msg_pk = ""; //主键信息比如： 用户代码[124]地址信息[上海浦东]
			CString v_msg_pk2 = "";

			// WHERE条件的列信息。
			//===================
			for (j = 0; j< bcls_rec->Tables[v_table_name2].Rows.get_Count(); j++)
			{
				//v_item_ename
				if (bcls_rec->Tables[v_table_name2].Columns.Contains("ITEM_ENAME"))
					v_item_ename = bcls_rec->Tables[v_table_name2].Rows[j]["ITEM_ENAME"].ToString().ToUpper();
				if (bcls_rec->Tables[v_table_name2].Columns.Contains("ITEM_CNAME"))
					v_item_cname = bcls_rec->Tables[v_table_name2].Rows[j]["ITEM_CNAME"].ToString().ToUpper();
				if (bcls_rec->Tables[v_table_name2].Columns.Contains("ITEM_TYPE"))
					v_item_type = bcls_rec->Tables[v_table_name2].Rows[j]["ITEM_TYPE"].ToString().ToUpper();


				Log::Trace("", __FUNCTION__, "v_item_ename =[{0}] v_item_type =[{1}]v_item_cname[{2}]"
					, v_item_ename, v_item_type, v_item_cname);

				//and aa = " v_tiem_value " and bb = @bb  ;

				//根据列名称获取对应的内容。
				if (bcls_rec->Tables[0].Columns.Contains(v_item_ename))
					v_item_value = bcls_rec->Tables[0].Rows[i][v_item_ename].ToString().TrimOrBlank();

				Log::Trace("", __FUNCTION__, "v_item_value =[{0}] ", v_item_value);

				if (v_item_type.Trim() == "N")
				{//若是数字型。 
					c_sql_where_temp = " and " + v_item_ename + " = " + v_item_value;
				}
				else
				{//非数字型，都视为字符。
					c_sql_where_temp = " and " + v_item_ename + " = '" + v_item_value + "' ";
				} 
				c_sql_where_temp2 = c_sql_where_temp2 + c_sql_where_temp;

				//拼接主键提示信息。//主键信息比如： 用户代码[124]地址信息[上海浦东]
				v_msg_pk = v_item_cname +"["+ v_item_value +"]";
				v_msg_pk2 = v_msg_pk2 + v_msg_pk;

			}

			//拼接最终的WHERE 语句。
			//====================
			c_sql_where = c_sql_where + c_sql_where_temp2;
			Log::Trace("", __FUNCTION__, "最终的WHERE c_sql_where[{0}]  ", c_sql_where);
			Log::Trace("", __FUNCTION__, "最终的 v_msg_pk2[{0}]  ", v_msg_pk2);
			/*
			// select count(1) from 基表  where 主键= @主键;
			// if(个数 >= 1) ｛ 已经主键重复，则提醒。｝
			*/
			int v_cnt_pk = 0;
			c_sql_condition = " select COUNT(1) from " + v_table_name + c_sql_where;
			Log::Trace("", __FUNCTION__, "主键校验sql ==>c_sql_condition[{0}]  ", c_sql_condition);
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
			v_cnt_pk = cmd_sql.ExecuteScalar().ToInt32();
			cmd_sql.Close();
			
			if (v_cnt_pk >= 1)
			{//若根据主键查询，已经发现信息，则进行‘主键重复’提醒。
				sprintf(s.msg, "主键信息【%s】\n已经存在，无法继续新增。", (const char*)v_msg_pk2);
				throw CApplicationException(-1, s.msg, log.Location); 
			}


		 

			/*
			--新增表
			insert into hom00
			(REC_CREATOR,REC_CREATE_TIME）
			values(@REC_CREATOR,@REC_CREATE_TIME)
			;
			*/  
			c_sql_condition = " insert into " + v_table_name + v_column_name + v_column_value;
			Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);			
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();




			//记录下【新增后】数据,用作异动履历的记录。TGCPMSI99
			//================================
			 v_rec_creator2 = userid;   //记录创建责任者
			 v_rec_create_time2 = systime;   //记录创建时刻  
			 v_track_seq_no2 = CDateTime::Now().ToString("yyyyMMddHHmmssfff");;   //事件跟踪序列号
			 v_table_name99 = v_table_name;   //数据库表名
			 v_event_id2   = "INS";   //事件标识
			// v_error_remark2 = "";   //备注==表中的每个列信息拼接起来。比如： [工序]A1[工序类型]SM11...
			 //为了防止【v_error_remark2】中有单引号的情况， 暂定【单引号】替换成【空格】
			 //===============v_in_str = v_in_str.Replace("'", "");
			 v_error_remark2 = v_error_remark2.Replace("'", " ");


			

			 Log::Trace("", __FUNCTION__, "v_table_name99[{0}]v_error_remark2[{0}]  ", v_table_name99, v_error_remark2);
			 
			 if (v_error_remark2.Trim() == "")
			 {//若没有备注信息，则不用新增。
				 Log::Trace("", __FUNCTION__, "若没有备注信息[{0}]，则不用新增履历。继续下一个", v_error_remark2);
				 continue;
			 }



			v_column_name = "(REC_CREATOR,REC_CREATE_TIME,TRACK_SEQ_NO,TABLE_NAME,EVENT_ID,ERROR_REMARK)";
			v_column_value = CString::Format(" values('%s','%s','%s','%s','%s','%s')"
				, (const char*)v_rec_creator2, (const char*)v_rec_create_time2, (const char*)v_track_seq_no2
				, (const char*)v_table_name99, (const char*)v_event_id2, (const char*)v_error_remark2);
		 
			
			c_sql_condition = " insert into tgcpmsi99 " + v_column_name + v_column_value; 
			Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);
			sqlstr = c_sql_condition;
			cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
			cmd_sql.ExecuteNonQuery();
			cmd_sql.Close();


		}


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