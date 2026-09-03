/*============================================================================*/
/*== [service名  ]:  gcpmsi02_ins       ||  [对应VC#画面 ]:GCPMSI02          ==*/
/*== [程序编制人 ]:  张颖               ||  [程序定稿日期]:2018-3-15 15:24:43==*/
/*== [程序修改人 ]：                    ||  [程序修改日期]:                  ==*/
/*========================================================================*/
/*== [数据库表   ]： TGCPMSI02                                          ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： 表TGCPMSI02_信息新增                               ==*/
/*========================================================================*/
/******框架头******/
#include "stdafx.h" 

/*<remark>=========================================================
/// <summary>
/// 表TGCPMSI02_信息新增
/// <para>
///    功能叙述段落
/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/
// service入口
BM2F_ENTERACE(gcpmsi02_ins)

int f_gcpmsi02_ins(EIClass * bcls_rec, EIClass * bcls_ret, CDbConnection* conn)
{
	CTracer log(__FUNCTION__);


	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_gcpmsi02_ins";                //定义函数英文名称  
	CString FunctionCname = "表TGCPMSI02_信息新增";              //定义函数中文名称 


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
		CModel tgcpmsi02("TGCPMSI02");

		//=================================  
		CString  v_blk_name_ds = "PMOA21CHK_OK"; //创建一个返回BLK 信息。
		bcls_ret->Tables.Add(v_blk_name_ds);
		//根据BLK 名称，获取对应的BLK ID
		int v_blk_id = bcls_ret->AtBlkName((const char*)v_blk_name_ds);
		Log::Trace("", __FUNCTION__, "in get==v_blk_id[{0}]  ", v_blk_id);





		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString    c_sql_where = "  WHERE   1 = 1 "; //新增条件。
		CString    c_sql_condition = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";
		CString    c_order_by = " order by t.order_no ";


		/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString    c_sql_where2 = "  WHERE   1 = 1 "; //新增条件。
		CString    c_sql_condition2 = " SELECT  t.* FROM tpmof01 t  where t.order_no = @order_no";




		CString v_col_table_name = "";
		CString v_col_func_id = "";


		CString TABLE_NAME;   //数据库表名
		CString TABLE_CNAME;   //数据库表中文名称
		CString SEQ_NO;   //序号
		CString MOID;   //模块号
		CString SEQ_CODE;   //序号代码
		CString FORM_CODE;   //画面编号



		for (i = 0; i < bcls_rec->Tables[0].Rows.get_Count(); i++)
		{

			tgcpmsi02.Reset(); //为了保证数据的准确性，暂定加个 RESET();
			tgcpmsi02.MergeFrom(bcls_rec->Tables[0].Rows[i]); //前台传入的数据。


			//==主键信息。
			//CString TABLE_NAME; //数据库表名 
			//CString SEQ_NO;    //序号


			Log::Trace("", __FUNCTION__, "第[{0}]个，TABLE_NAME =[{1}]SEQ_NO[{2}]"
				, i + 1, tgcpmsi02["TABLE_NAME"].ToString(), tgcpmsi02["SEQ_NO"].ToDecimal());

			if (tgcpmsi02["TABLE_NAME"].ToString().Trim() == "")
			{
				sprintf(s.msg, "[数据库表名]不能为空。");
				throw CApplicationException(-1, s.msg, log.Location);
			}
			//大写处理。
			tgcpmsi02["TABLE_NAME"] = tgcpmsi02["TABLE_NAME"].ToString().ToUpper();

			/*if (tgcpmsi02["SEQ_NO"].ToDecimal().Trim() == "")
			{
			sprintf(s.msg, "[序号]不能为空。");
			throw CApplicationException(-1, s.msg, log.Location);
			}*/


			//根据TABLE_NAME ，获取最大序号+1
			//=================
			v_cnt = 0;
			c_sql_condition = "select MAX(t.seq_no) + 1 "
				" from tgcpmsi02 t "
				" where t.table_name  = @table_name "
				//" and   t.seq_no     = @seq_no " 
				;
			sqlstr = c_sql_condition;
			cmd_sql.Parameters.Set("table_name", tgcpmsi02["TABLE_NAME"].ToString());
			//cmd_sql.Parameters.Set("seq_no", tgcpmsi02["SEQ_NO"].ToDecimal());
			cmd_sql.SetCommandText(c_sql_condition);
			v_cnt = cmd_sql.ExecuteScalar();
			cmd_sql.Close();
			tgcpmsi02["SEQ_NO"] = v_cnt;




			////校验通过过，进行新增操作。
			////=========================  
			//给指定表的指定列赋值。
			tgcpmsi02["REC_CREATOR"] = userid;
			tgcpmsi02["REC_CREATE_TIME"] = dateNow;
			tgcpmsi02["REC_REVISOR"] = "";
			tgcpmsi02["REC_REVISE_TIME"] = "";
			tgcpmsi02["VALID_FLAG"] = "1";   //生效标记=1有效
			//清空部分列。
			//tgcpmsi02["MOID"] = "";   //二级模块
			tgcpmsi02["PAGE_NUM"] = 0;   //页面数目
			tgcpmsi02["SEQ_CODE"] = "";   //序号代码
			tgcpmsi02["FORM_CODE"] = "";   //画面编号
			tgcpmsi02["KEYVALUE_1"] = "";   //关键字串1
			tgcpmsi02["KEYVALUE_2"] = "";   //关键字串2
			tgcpmsi02["KEYVALUE_3"] = "";   //关键字串3
			tgcpmsi02["KEYVALUE_4"] = "";   //关键字串4
			tgcpmsi02["KEYVALUE_5"] = "";   //关键字串5
			tgcpmsi02["KEYVALUE_6"] = "";   //关键字串6			
			tgcpmsi02["REMARK"] = "";   //备注500
			tgcpmsi02["FLAG_POS_1"] = "";   //标志位1
			tgcpmsi02["FLAG_POS_2"] = "";   //标志位2
			tgcpmsi02["FLAG_POS_3"] = "";   //标志位3
			tgcpmsi02["FLAG_POS_4"] = "";   //标志位4
			tgcpmsi02["FLAG_POS_5"] = "";   //标志位5
			tgcpmsi02["FLAG_POS_6"] = "";   //标志位6
			tgcpmsi02["FLAG_POS_7"] = "";   //标志位7
			tgcpmsi02["FLAG_POS_8"] = "";   //标志位8
			tgcpmsi02["FLAG_POS_9"] = "";   //标志位9 

			tgcpmsi02.TrimOrBlank();
			sqlstr = "insert into tgcpmsi02";
			tgcpmsi02.TrimOrBlank();
			tgcpmsi02.Insert();

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

	//将来可能要拆service处理，SO ，此处记录一下当前service名称,用于显示前台。
	//=============
	strcpy(s.sysmsg, s.svc_name);
	s.flag = doFlag;
	return doFlag;

}







