#pragma once
#include "LogRecord.h"

using System::String;
using System::Data::OleDb::OleDbConnection;
using System::Collections::Generic::Dictionary;

ref class DBAccess abstract
{
private:
	static OleDbConnection^ conn;
public:
	enum class Response {	
		OK=0, 
		NoSuchUser=1, 
		WrongPassword=2, 
		MultipleUsers=4, 
		ConnectionFailed=8
	};

private:
	static bool OpenConnection();
	//static array<unsigned char>^ EncryptPassword(String^ password);
	static String^ EncryptPassword(String^ password);

public:
	static bool SetConn();

public:
	static Response LogIn(int& id, String^ name, String^ pass);
	static Response SignUp(int& id, String^ name, String^ pass);
	static Response UpdateLog(LogRecord^);
	static Response ReadMathOperations(Dictionary<String^,int>^%);
};

