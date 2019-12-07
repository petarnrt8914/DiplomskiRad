#include "DBAccess.h"

using namespace System;
using namespace System::Data;
using namespace System::Data::OleDb;

#define DB_PATH LR"(..\Project1\Log.mdb)"



void DBAccess::SetConn() {
	//inicijalizuj konekciju
	if (conn == nullptr)
		conn = gcnew OleDbConnection("Provider=Microsoft.ACE.OLEDB.12.0; Data Source="+DB_PATH);
	//ako ne uspe, naci bazu koristici FileDialog
}

DBAccess::Response DBAccess::LogIn(int& userID, String ^ username, String ^ password) {
	//return ConnectionFailed;

	OleDbDataReader^ reader;
	try {
		if (conn->State != ConnectionState::Open)
			conn->Open();

		String ^selectText =
			"SELECT ID, password FROM [USER] " +
			"WHERE username='" +username+ "'";
		OleDbCommand ^selectUser = gcnew OleDbCommand(selectText, conn);
		reader = selectUser->ExecuteReader();
		while (reader->Read()) {
		//while (selectUser->ExecuteReader()->Read()) {
			if (userID) return Response::MultipleUsers;	// ne bi trebalo da se desi

			if (password==reader["password"]->ToString()) {
				userID = static_cast<int>(reader["ID"]);
				return Response::OK;
			}
			else
				return Response::WrongPassword;
		}
		// if user not found
		return Response::NoSuchUser;
	}
	catch (Exception ^ex) {
		return Response::ConnectionFailed;
		//MessageBox::Show(ex->Message);
	}
	finally {
		delete reader;
		conn->Close();
	}
}

DBAccess::Response DBAccess::SignUp(int& id, String ^ name, String ^ pass) {
	return Response::ConnectionFailed;
}

DBAccess::Response DBAccess::UpdateLog(LogRecord *) {
	return Response::ConnectionFailed;
}
