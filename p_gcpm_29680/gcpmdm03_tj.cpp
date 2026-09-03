/*============================================================================*/
/*== [service名  ]:  gcpmdm03_tj        ||  [对应VC#画面 ]:GCPMDM03          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2017-9-29 20:48:12==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMDM03                                          ==*/
/*== [调用函数   ]： 无				                                          ==*/
/*== [service功能]： 表TGCPMDM03_信息统计                               ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h" 

 


/*<remark>=========================================================
/// <summary>
/// 表TGCPMDM03_信息统计
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmdm03_tj)

int f_gcpmdm03_tj(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);
	

	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmdm03_tj";                //定义函数英文名称  
	CString FunctionCname = "表TGCPMDM03_信息统计";              //定义函数中文名称 


	//程序用变量
	int   i = 0;
	int   fun_i = 0; //功能个数。
	int   fetchRowCount = 0;
	int   doFlag = 0;

	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  userid = s.userid;  
	CString  sqlstr = "";  //SQL 信息。 
	CDecimal v_cnt = 0; 


	try
	{ 
		//CTGCPMDM03 tgcpmdm03(conn);
		CString v_model_tname = "TGCPMDM03"; //业务表名称。
		CModel tgcpmdm03 = CModel(v_model_tname); //创建动态头文件 
		
		
		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString    c_order_by = " order by t.order_no ";


		/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2 = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition2 = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";

		/* 数据库操作类定义3 */
		CDbCommand cmd_sql3(conn); //与DB 建立连接。
		CString    c_sql_where3 = "  WHERE   1 = 1 "; //修改条件。
		CString    c_sql_condition3 = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";

		  
		//==查询条件信息。
		//CString CODE_CLASS;   //代码编号
		//CString CODE_NAME;   //代码名称
		  
		  //从1#BLK 中获取静态表的表名称。
		CString v_code_class_in = "";
		if (bcls_rec->Tables[0].Columns.Contains("CODE_CLASS"))
			v_code_class_in = bcls_rec->Tables[0].Rows[0]["CODE_CLASS"].ToString();
	 
		Log::Trace("", __FUNCTION__, "in ==v_code_class_in[{0}]  ", v_code_class_in);

		//信息初始化条件。
		c_sql_where = " where 1 = 1   "; //信息初始化条件。  

		//拼接前台传入的查询条件。
		if (v_code_class_in.Trim() != "")
		{
			c_sql_where = c_sql_where + " AND t.code_class like @code_class  || '%' ";
		}
		

		 
		//----03
		//v_code_desc_1_name03
		//==========
		CString v_valid_flag03 = "";
		CString v_code_desc_1_name03 = "";
		CString v_remark03 = "";
	 

		//排序信息。
		/*
		CString CODE_CLASS;   //代码编号
		CString ITEM_ENAME;   //项目号
		CString CONDITION_01; //条件
		CString CODE;   //代码
		*/
		
		CString v_code_class = ""; //代码编号。
		CString v_item_cname = "";   //字段中文名 ==>项目说明
		CString v_item_ename = "";
		CString v_condition_01 = "";
		c_sql_condition = " select distinct t.ITEM_ENAME,t.CONDITION_01  "
			" ,t.CODE_CLASS "
			" from     tgcpmdm03 t "
			//" where    t.code_class like  @code_class   "
			//" order by t.item_ename ,t.condition_01 " //项目号，条件
			//"         ,t.code "
			;  
		c_order_by = " order by  t.code_class,t.item_ename,t.condition_01 ";
		//信息初始化语句+ WHERE 语句。
		c_sql_condition = c_sql_condition + c_sql_where + c_order_by;
		Log::Trace("", __FUNCTION__, "c_sql_condition[{0}]  ", c_sql_condition);  

		sqlstr = c_sql_condition;
		cmd_sql.Parameters.Set("code_class", v_code_class_in);//入口信息。
		cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句  
		cmd_sql.ExecuteReader();
		while (cmd_sql.Read())
		{
			v_item_ename = cmd_sql.GetString(1);
			v_condition_01 = cmd_sql.GetString(2);
			v_code_class = cmd_sql.GetString(3);

			
			/*
				CString CODE_CLASS;   //代码编号
				CString ITEM_ENAME;   //字段英文名
				CString CONDITION_01;   //条件01
				CString CODE;   //代码
				*/
			tgcpmdm03.Reset();
			tgcpmdm03["CODE_CLASS"] = v_code_class;
			tgcpmdm03["ITEM_ENAME"] = v_item_ename;
			tgcpmdm03["CONDITION_01"] = v_condition_01;

			//项目A: 项目说明。 条件xxxoooooo
			c_sql_condition2 = " select "
				" '项目' || t.item_ename  || ':' || t.item_cname " //项目A: 项目说明。
				" from     tgcpmdm02 t "
				" where    t.code_class   = @code_class   "
				" and      t.item_ename   = @item_ename " 
				;
			Log::Trace("", __FUNCTION__, "c_sql_condition2[{0}]  ", c_sql_condition2); 
			sqlstr = c_sql_condition2;
			cmd_sql2.Parameters.Set("code_class", v_code_class);//代码编号
			cmd_sql2.Parameters.Set("item_ename", v_item_ename);//项目号 
			cmd_sql2.SetCommandText(c_sql_condition2);// 设置执行的SQL语句  
			cmd_sql2.ExecuteReader();
			while (cmd_sql2.Read())
			{
				v_item_cname = cmd_sql2.GetString(1);
			}
			cmd_sql2.Close();



			//项目A：管理阶段
			//项目号：项目说明。条件，的标题信息，借用如下字段。
			//项目A: 项目说明。 条件xxxoooooo
			//CString CODE;   //代码  = 项目A: 项目说明。 条件xxxoooooo
			tgcpmdm03["CODE"] = v_item_cname + "。" + v_condition_01;
			tgcpmdm03.MergeTo(bcls_ret->Tables[0], false);


			//根据代码编号+项目号+条件，获取对应的代码明细
			//==============
			c_sql_condition2 = " select t.* "
				" from     tgcpmdm03 t "
				" where    t.code_class   = @code_class   "
				" and      t.item_ename   = @item_ename "
				" and      t.condition_01 = @condition_01 "
				" order by t.item_ename ,t.condition_01 "
				"         ,t.code "
				;
			Log::Trace("", __FUNCTION__, "c_sql_condition2[{0}]  ", c_sql_condition2);

			sqlstr = c_sql_condition2;
			cmd_sql2.Parameters.Set("code_class", v_code_class);//代码编号
			cmd_sql2.Parameters.Set("item_ename", v_item_ename);//项目号
			cmd_sql2.Parameters.Set("condition_01", v_condition_01);//条件说明
			cmd_sql2.SetCommandText(c_sql_condition2);// 设置执行的SQL语句  
			cmd_sql2.ExecuteReader();
			while (cmd_sql2.Read())
			{
				cmd_sql2.Fetch(tgcpmdm03);

				//CString VALID_FLAG;   //生效标记
				//CString CODE_DESC_1_NAME;   //代码描述一名称= 值集代码，
				//CString REMARK;   //备注 = sql脚本信息。。。。
				v_valid_flag03 = tgcpmdm03["VALID_FLAG"];
				v_code_desc_1_name03 = tgcpmdm03["CODE_DESC_1_NAME"];
				v_remark03 = tgcpmdm03["REMARK"];


				Log::Trace("", __FUNCTION__, "独立定义[{0}]", v_valid_flag03);
				Log::Trace("", __FUNCTION__, "值集代码[{0}]", v_code_desc_1_name03);
				Log::Trace("", __FUNCTION__, "SQL脚本[{0}]", v_remark03);

				//若代码明细是‘独立定义’，则获取对应的‘值集’对应的内容。tgcpmsi00
				//============================
				if (v_valid_flag03.Trim().ToUpper() == "Y"
					&& v_code_desc_1_name03.Trim() != "")
				{//若是独立定义,并有‘值集代码’，则获取值集内容。

					c_sql_condition3 = " select "
						" t.code ,t.code_desc_1_content "
						" from  tgcpmsi00 t "
						" where t.code_class = @code_class "
						" order by t.code "
						;
					sqlstr = c_sql_condition3;
					cmd_sql3.Parameters.Set("code_class", v_code_desc_1_name03);//值集代码
					cmd_sql3.SetCommandText(c_sql_condition3);// 设置执行的SQL语句  
					cmd_sql3.ExecuteReader();
					while (cmd_sql3.Read())
					{
						tgcpmdm03["CODE"] = cmd_sql3.GetString(1);
						tgcpmdm03["CODE_DESC"] = cmd_sql3.GetString(2);
						//返回前台。
						tgcpmdm03.MergeTo(bcls_ret->Tables[0], false);

					}
					cmd_sql3.Close();
					
					 
				}
				else if (v_remark03.Trim() != "" )
				{//根据自定义SQL语句，获取代码明细信息： 代码+内容

					c_sql_condition3 = v_remark03;
					sqlstr = c_sql_condition3;
					//cmd_sql3.Parameters.Set("code_class", v_code_desc_1_name03);//值集代码
					cmd_sql3.SetCommandText(c_sql_condition3);// 设置执行的SQL语句  
					cmd_sql3.ExecuteReader();
					while (cmd_sql3.Read())
					{
						tgcpmdm03["CODE"] = cmd_sql3.GetString(1);
						tgcpmdm03["CODE_DESC"] = cmd_sql3.GetString(2);
						//返回前台。
						tgcpmdm03.MergeTo(bcls_ret->Tables[0], false);

					}
					cmd_sql3.Close();

				}
				else
				{//非独立定义。
					tgcpmdm03.MergeTo(bcls_ret->Tables[0], false);
				}

			}
			cmd_sql2.Close();



		} 
		cmd_sql.Close();



		//返回的记录数。 
		fetchRowCount = bcls_ret->Tables[0].Rows.get_Count();

		/*设置系统返回参数*/ 
		sprintf(s.msg, "查询到[%d]条记录。", fetchRowCount);
		  
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

	//将来可能要拆service处理，SO ，此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;

}







