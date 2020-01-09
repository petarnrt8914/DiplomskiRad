#pragma once
using System::DateTime;
using System::String;

ref class LogRecord
{
protected:
	DateTime date_time;
	int user_id;
	int op_id;
	String^ param_values;

public:
	DateTime getDateTime() { return date_time;}
	int getUserID() { return user_id; }
	int getOperationID() { return op_id; }
	String^ getParameters() { return param_values; }

	LogRecord(int userID, int operationID, String^ params)
		: date_time(DateTime::Now), user_id(userID), op_id(operationID), param_values(params) {}
};

