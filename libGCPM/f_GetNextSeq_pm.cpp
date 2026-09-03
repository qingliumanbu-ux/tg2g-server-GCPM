/*==================================================================================*/
/*== [service名  ]:  f_GetNextSeq_pm        ||  [对应VC#画面 ]:  ALL              ==*/
/*== [程序编制人 ]:  张颖                   ||  [程序定稿日期]:2015-11-24 10:09:28==*/
/*== [程序修改人 ]：                        ||  [程序修改日期]:                   ==*/
/*========================================================================*/
/*== [数据库表   ]： tgcpm21                                            ==*/
/*== [调用函数   ]： 无				                                    ==*/
/*== [service功能]： 生产合同管理_流水号获取函数                        ==*/
/*========================================================================*/
/***** C/C++ 的标准头文件部分 *****/
// New Include
#include "stdafx.h"
#include "math.h"
  
 
 
/*<remark>=========================================================
/// <summary>
///  PM_流水号获取函数
启用 GCPM 自己的业务流水号基表.
/// <para>
///    功能叙述段落
//

/// </para>
/// </summary>
/// <param name="bcls_rec">传入块  </param>
/// <param name="bcls_ret">返回块  </param>
/// <returns>返回：是否成功</returns>
===========================================================</remark>*/

//序号名称，序号长度，序号类型（函数返回的内容=流水号内容），流水号的设置，无须手工在TGCPM21配置，根据业务逻辑动态生成。   
//v_seq_type =0/1/2/3=最大值循环/年循环/月循环/日循环
//序号名称，序号长度，序号类型,conn

BM2_FUNCTION_EXPORT
CString f_GetNextSeq_pm(CString v_seq_name, CDecimal v_seq_length, CString v_seq_type, CDbConnection* conn)
{
CTracer log(__FUNCTION__);  // 系统日志，必须在代码段开始处定义


	/****** 定义函数名称 ***** */
	CString FunctionEname = "f_GetNextSeq_pm";                //定义函数英文名称  
	CString FunctionCname = "PM_流水号获取函数";              //定义函数中文名称 


	//程序用变量
	int   doFlag = 0;
	int   fetchRowCount = 0;
	int   i = 0;
	int   j = 0;


	CString  dateNow = CDateTime::Now().ToString("yyyyMMddHHmmss");
	CString  function_id = "OMOM00B2_INS";  //自定义显示项目功能号    
	CString  sqlstr = "";


	CString  v_table_name = "xx";
	CString v_item_ename = "";
	CString v_item_cname = "";

	CString v_update = "";  //修改的字段信息
	CString v_condi = "";  //过滤的字段信息。

	CString v_func_id = "";
	CDecimal v_cnt = 0;
	CString v_seqValue = ""; //获取的流水号内容。
	int     v_seqValue_len = 0; //流水号内容的长度。


	char v_dateNow[15] = ""; //当前日期
	char v_dateNow_2[15] = ""; //当前日期2//v_dateNow_2
	CString v_yyyy_now = "";
	CString v_yyyy = "";

	CString v_yyyymm_now = "";
	CString v_yyyymm = "";

	CString v_yyyymmdd_now = "";
	CString v_yyyymmdd = "";  


	try
	{


		/* 实体类定义 */
	CModel tgcpm21("TGCPM21");
	CModel tgcpm21_chk("TGCPM21");
	CModel tgcpm21_chk2("TGCPM21");


		/* 数据库操作类定义 */
		CDbCommand cmd_sql(conn); //与DB 建立连接。
		CString  c_sql_where = "   "; //查询条件。
		CString  c_sql_condition = " SELECT  t.* FROM tgcpm21 t  where t.seq_name = @seq_name   ";


		/* 数据库操作类定义2 */
		CDbCommand cmd_sql2(conn); //与DB 建立连接。
		CString  c_sql_where2 = "   "; //查询条件。
		CString  c_sql_condition2 = " SELECT  t.* FROM tgcpm21 t  where t.seq_name = @seq_name   ";

		Log::Trace("", __FUNCTION__, "IN== v_seq_name=[{0}]", v_seq_name);
		Log::Trace("", __FUNCTION__, "IN== v_seq_type=[{0}]", v_seq_type);
		Log::Trace("", __FUNCTION__, "IN== v_seq_length=[{0}]", v_seq_length);


		

		if (v_seq_name.Trim() == "")
		{
			strcpy(s.msg, "[流水号名称]不允许为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		}
		if (v_seq_type.Trim() == "")
		{
			strcpy(s.msg, "[流水号循环标志]不允许为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		}
		if (v_seq_length <= 0)
		{
			strcpy(s.msg, "[流水号长度]不允许为空。");
			throw CApplicationException(-1, s.msg, log.Location);
		} 


		//流水号名称。
		tgcpm21_chk["SEQ_NAME"] = v_seq_name;



		if (tgcpm21_chk.Query("SEQ_NAME") != true)
		{//序号信息不存在，则进行新增操作。


			Log::Trace("", __FUNCTION__, "insert== v_seq_name=[{0}]", v_seq_name);


			tgcpm21["REC_CREATOR"] = ""; // s.userid;
			tgcpm21["REC_CREATE_TIME"] = dateNow;
			tgcpm21["REC_REVISOR"] = "";
			tgcpm21["REC_REVISE_TIME"] = dateNow;

			//序号名称，序号长度，序号类型，返回的序号      
			tgcpm21["SEQ_NAME"] = v_seq_name;/* 序号名称 */
			tgcpm21["SEQ_DESC"] = v_seq_name;/* 序号描述 */
			tgcpm21["SEQ_BEGIN"] = 1;          /* 起始序号 */
			tgcpm21["SEQ_NOW"] = tgcpm21["SEQ_BEGIN"];            /* 当前序号 */
			tgcpm21["SEQ_LEN"] = v_seq_length;/* 序号长度 */
			tgcpm21["SEQ_RECYCLE_FLAG"] = v_seq_type;/* 流水号复位标记 */ 
			Log::Trace("", __FUNCTION__, "==in==SEQ_LEN =[{0}] ====", tgcpm21["SEQ_LEN"].ToDecimal());


			//终止序号的处理       /* 终止序号*/
			//======================================
			//10的N次方。n = tgcpm21.seq_len 
			CDecimal v_10_tmp = 10;
			tgcpm21["SEQ_END"] = v_10_tmp.Power(tgcpm21["SEQ_LEN"].ToDecimal().ToDouble())-1; 
			Log::Trace("", __FUNCTION__, "==out==SEQ_END =[{0}] ====", tgcpm21["SEQ_END"].ToDecimal());


			/// <summary> 
			///  新增表
			/// </summary>
			sqlstr = "insert into tgcpm21,seq_name =[" + tgcpm21["SEQ_NAME"].ToString() + "]";
			tgcpm21.TrimOrBlank();
			tgcpm21.Insert(); 

		}
		else
		{//序号信息已经存在，则进行修改操作。
			Log::Trace("", __FUNCTION__, "update== v_seq_name=[{0}]", v_seq_name);

			//获取一遍现有的流水号信息。
			//业务并发的时候，序号会重复， SO ，此处必须 for update. 
			//update by zy on 2014-10-24 10:03:49
			c_sql_condition = " SELECT  t.* FROM tgcpm21 t  where t.seq_name = @seq_name for update  ";
			//分数据库的逻辑处理
			//==========
			switch (conn->DatabaseKind)
			{
			case DB_KIND_MSSQL:
				c_sql_condition = " SELECT  t.* FROM tgcpm21 t  where t.seq_name = @seq_name    ";
				break;
			case DB_KIND_ORACLE:
				c_sql_condition = " SELECT  t.* FROM tgcpm21 t  where t.seq_name = @seq_name for update  "; //oracle 的行锁
				break;
			case DB_KIND_DB2:
				c_sql_condition = " SELECT  t.* FROM tgcpm21 t  where t.seq_name = @seq_name FOR UPDATE with rs  "; //db2 的行锁
				break;
			default:
				c_sql_condition = " SELECT  t.* FROM tgcpm21 t  where t.seq_name = @seq_name  "; //default
				break;
			} 
			cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句
			// 设置SQL中的变量
			cmd_sql.Parameters.Set("seq_name", v_seq_name);
			cmd_sql.ExecuteReader(); //执行读取
			if (cmd_sql.Read()) //只读取单记录，可用IF 语句。
			{
				cmd_sql.Fetch(tgcpm21);		 //整个表结构的获取。	
			}
			cmd_sql.Close(); //关闭游标

			tgcpm21.TrimOrBlank();
			//0/1/2/3=最大值循环/年循环/月循环/日循环
			////EDLog(1,1,"流水号复位标记[%s]。",(const char*)tgcpm21["SEQ_RECYCLE_FLAG"].ToString());
			if (tgcpm21["SEQ_RECYCLE_FLAG"].ToString() == "0")
			{//最大值归零
				if (tgcpm21["SEQ_NOW"].ToDecimal() < tgcpm21["SEQ_END"].ToDecimal())
				{
					tgcpm21["SEQ_NOW"] = tgcpm21["SEQ_NOW"].ToDecimal() + 1;
				}
				else
				{//重新循环=归零
					tgcpm21["SEQ_NOW"] = tgcpm21["SEQ_BEGIN"];
				}

			}
			else if (tgcpm21["SEQ_RECYCLE_FLAG"].ToString() == "1")
			{//按年归零 , v_yyyy //dateNow  

				//当前日期的年份 
				v_yyyy_now = dateNow.Substring(0, 4);
				v_yyyy = tgcpm21["REC_REVISE_TIME"].ToString().Substring(0, 4);
				if (v_yyyy_now == v_yyyy)
				{//同年
					tgcpm21["SEQ_NOW"] = tgcpm21["SEQ_NOW"].ToDecimal() + 1;
				}
				else
				{//重新循环=归零
					tgcpm21["SEQ_NOW"] = tgcpm21["SEQ_BEGIN"];
				}

			}
			else if (tgcpm21["SEQ_RECYCLE_FLAG"].ToString() == "2")
			{//按月归零 , v_yyyymm 

				//当前日期的年月份
				v_yyyymm_now = dateNow.Substring(0, 6);
				v_yyyymm = tgcpm21["REC_REVISE_TIME"].ToString().Substring(0, 6);

				if (v_yyyymm_now == v_yyyymm)
				{//同年月
					tgcpm21["SEQ_NOW"] = tgcpm21["SEQ_NOW"].ToDecimal() + 1;
				}
				else
				{//重新循环=归零
					tgcpm21["SEQ_NOW"] = tgcpm21["SEQ_BEGIN"];
				}

			}
			else if (tgcpm21["SEQ_RECYCLE_FLAG"].ToString() == "3")
			{//按日归零 , v_yyyymmdd

				//当前日期的年月日
				v_yyyymmdd_now = dateNow.Substring(0, 8);
				v_yyyymmdd = tgcpm21["REC_REVISE_TIME"].ToString().Substring(0, 8);

				if (v_yyyymmdd_now == v_yyyymmdd)
				{//同年月日
					tgcpm21["SEQ_NOW"] = tgcpm21["SEQ_NOW"].ToDecimal() + 1;
				}
				else
				{//重新循环=归零
					tgcpm21["SEQ_NOW"] = tgcpm21["SEQ_BEGIN"];
				} 

				Log::Trace("", __FUNCTION__, "day== tgcpm21.SEQ_NOW.=[{0}]", tgcpm21["SEQ_NOW"].ToDecimal());
			}
			else
			{
				sprintf(s.msg, "序号循环标志[%s]暂不支持。", (const char*)tgcpm21["SEQ_RECYCLE_FLAG"].ToString());
				throw CApplicationException(-1, s.msg, log.Location); 
			}


			//更新修改者，修改时间
			tgcpm21["REC_REVISOR"] = s.userid;
			tgcpm21["REC_REVISE_TIME"] = dateNow;
			v_update = "REC_REVISOR,REC_REVISE_TIME,SEQ_NOW";//修改字段信息。
			v_condi = "SEQ_NAME"; //查询条件  

			c_sql_condition = " update tgcpm21 t "
				" set t.rec_revise_time = @rec_revise_time "
				" ,   t.seq_now         = @seq_now "
				" where t.seq_name = @seq_name     "
				;
			sqlstr = c_sql_condition; 
			cmd_sql.Parameters.Set("rec_revise_time", tgcpm21["REC_REVISE_TIME"].ToString());
			cmd_sql.Parameters.Set("seq_now", tgcpm21["SEQ_NOW"].ToDecimal());
			cmd_sql.Parameters.Set("seq_name", tgcpm21["SEQ_NAME"].ToString());
			cmd_sql.SetCommandText(c_sql_condition);// 设置执行的SQL语句 
			cmd_sql.ExecuteNonQuery(); 
			cmd_sql.Close(); //关闭游标 

			Log::Trace("", __FUNCTION__, "update end.== tgcpm21.SEQ_NAME. =[{0}]", tgcpm21["SEQ_NAME"].ToString());


		}//update 操作完毕。



	 

		//考虑流水号不够用的情况。
		//若当前值已经大于最大值了，那么，利用[字母]/[字符]替代第一位流水号信息。
		//=============add on 2015-11-23 16:25:50
		CDecimal  l_xxx = 0;
		CDecimal  v_ii = 0;
		char v_seq_now_A[2] = ""; //存放一个字母
		char v_seq_now_char[101] = "";
		CString v_seq_now_string_a = ""; //存放首子母。
		CString v_seq_now_string = "";

		//INT类型
		int v_seq_len_int = 0; //流水号长度
		int v_seq_now_int = 0; //当前流水号
		int v_seq_end_int = 0; //最大流水号
		
		int l_xxx_int = 0; /* 终止序号*/
		int v_ii_int = 0; 
		int v_begin_int = 0; //开始截取的位置。 

		Log::Trace("", __FUNCTION__, "====SEQ_NOW =[{0}]SEQ_END[{1}] ===="
			, tgcpm21["SEQ_NOW"].ToDecimal(), tgcpm21["SEQ_END"].ToDecimal()); 

		if (tgcpm21["SEQ_NOW"].ToDecimal() > tgcpm21["SEQ_END"].ToDecimal())
		{//若当前值已经大于流水号的最大值，  
			
			//终止序号的处理       /* 终止序号*/
			//======================================
			//10的N次方。n = tgcpm21.seq_len 
			v_seq_len_int = tgcpm21["SEQ_LEN"].ToDecimal().ToInt32();
			CDecimal v_10_tmp = 10;
			l_xxx_int = v_10_tmp.Power(tgcpm21["SEQ_LEN"].ToDecimal().ToDouble()-1).ToInt32();  

			Log::Trace("", __FUNCTION__, "====l_xxx_int =[{0}] ====", l_xxx_int);  
			v_seq_now_int = tgcpm21["SEQ_NOW"].ToDecimal().ToInt32();
			v_seq_end_int = tgcpm21["SEQ_END"].ToDecimal().ToInt32();

			////若当前值已经大于最大值了，那么，利用字母+字符替代第一位流水号信息。
			//(字符--->ASCII码)
			//A ~~ Z--->65 ~~~ 90
			//a ~~ z--->97 ~~~ 122
			//i = (int)(tgcpm21.seq_now - tgcpm21.seq_end - 1) / l_xxx + 65;                   // A、B、C……、Y、Z~~a、b、c、d、~~~z
			//v_seq_now_A[0] = i; //ASCII转换成字符  
			v_ii = (v_seq_now_int - v_seq_end_int - 1) / l_xxx_int ; 
			//向下取整。
			v_ii = v_ii.Floor();
			//v_ii = v_ii + 65; //ASCII码 = 65 = 字母A 
			Log::Trace("", __FUNCTION__, "v_seq_now_int =[{0}]v_seq_end_int[{1}]=向下取整v_ii[{2}]"
				, v_seq_now_int, v_seq_end_int, v_ii); 
 
			//(字符--->ASCII码)
			//A ~~ Z--->65 ~~~ 90
			//a ~~ z--->97 ~~~ 122
			//字母全部用完后，最终用@替代返回，作为报警信息，提示流水号严重不足。 
			if (v_ii >= 0 && v_ii < 26)
			{//启用大写字母。 
				v_ii = v_ii + 65; //ASCII码 = 65 = 65+0 = 字母A ~~ Z--->65 ~~~ 90
			}
			else if(v_ii >= 26 && v_ii < 52)
			{//启用小写字母。
				v_ii = v_ii + 71; //ASCII码 = 97 = 71+26= 字母a ~~ z--->97 ~~~ 122
			}
			else
			{//大小写字母都用完后，只能报错了，流水号定义的长度太不复合实际使用长度了。！！！
				sprintf(s.msg, "流水号[%s]的当前值[%d]，已经【严重超过】设定的最大值[%d]，超长跨度v_ii[%d],进行强制返@处理。"
					, (const char*)v_seq_name, v_seq_now_int, v_seq_end_int, v_ii.ToInt32());
				/*throw CApplicationException(-1, s.msg, log.Location); */
				Log::Trace("", __FUNCTION__, " 强制返回@处理。s.msg=[{0}]", s.msg); 

				//返回的ASCII = 64=@
				v_ii = 64;   
			} 

			v_ii_int = v_ii.ToInt32();
			v_seq_now_A[0] = v_ii_int; //ASCII转换成字符 
			v_seq_now_string_a = v_seq_now_A;// string <--char 

			Log::Trace("", __FUNCTION__, "v_seq_now_A =[{0}]ASCII码v_ii_int[{1}]v_seq_now_string_a[{2}]"
				, v_seq_now_A, v_ii_int, v_seq_now_string_a);


			//流水号长度=2 ，流水号最大值= 99 ，
			//若当前流水号已经到了 100,vii = 0,则新流水号 =A0
			//=====================110,======1,============B0
			//=====================120,======2,============C0
			//新流水号 = 字母[1]+ 截取后N位（TGCPM21.当前流水号）； 其中N=流水号长度-1。
			sprintf(v_seq_now_char,"%d",v_seq_now_int); //CHAR<----INT 
			v_seq_now_string = v_seq_now_char; //STRING<----- CHAR

			//开始截取的位置= 当前流水号的总长度-设计要求长度+1 
			v_begin_int = v_seq_now_string.GetLength() - v_seq_len_int +1;
			v_seq_now_string = v_seq_now_string.Substring(v_begin_int);


			Log::Trace("", __FUNCTION__, "v_begin_int =[{0}]v_seq_now_string[{1}]"
				, v_begin_int, v_seq_now_string);


			//v_seqValue=首子母[1]+新流水
			v_seqValue = v_seq_now_string_a + v_seq_now_string;
			Log::Trace("", __FUNCTION__, "流水号超最大值，首位字母处理，v_seqValue =[{0}]",v_seqValue); 

		}
		else
		{  

			v_seqValue = tgcpm21["SEQ_NOW"].ToDecimal().ToString();
			v_seqValue_len = v_seqValue.GetLength();

			//用循环处理左补零的操作,与数据库无关。
			//========================
			for (i = 1; i <= tgcpm21["SEQ_LEN"].ToDecimal() - v_seqValue_len; i++)
			{
				v_seqValue = "0" + v_seqValue;
			} 
			Log::Trace("", __FUNCTION__, " 一般处理，v_seqValue =[{0}]", v_seqValue);

		} 

		
		sprintf(s.msg,"获取流水号[%s]最终结果[%s]",(const char*)v_seq_name,(const char*)v_seqValue);
		Log::Trace("", __FUNCTION__, "s.msg[{0}]", s.msg); 
		 
	}



	catch (CDbException& ex)  //捕获数据库操作异常
	{
		CString str = "DB err:[" + sqlstr + "]\r\n" + ex.GetMsg();
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
	catch (CException& ex)
	{
		strncpy(s.msg, (const char*)ex.GetMsg(), sizeof(s.msg) - 1);
		s.flag = ex.GetCode();
		doFlag = -1;
	}


	s.flag = doFlag; 
	return v_seqValue;
}







