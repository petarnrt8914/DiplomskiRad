#include "DBAccess.h"

using namespace System;
using namespace System::Data;
using namespace System::Data::OleDb;

#define DB_PATH LR"(..\Project1\Log.mdbl)"	//change when deployed



bool DBAccess::OpenConnection() {
	if (conn->State == ConnectionState::Open) return true;

	try {
		conn->Open();
		return true;
	}
	catch (Exception^ ex){
		if (SetConn()) {
			try {
				conn->Open();
				return true;
			}
			catch (Exception^) {
				return false;
			}
		}
		else return false;
	}
}

bool DBAccess::SetConn() {
	//inicijalizuj konekciju
	OleDbConnectionStringBuilder ^connBuilder = gcnew OleDbConnectionStringBuilder();
	connBuilder->Provider = "Microsoft.ACE.OLEDB.12.0";
	connBuilder->DataSource = DB_PATH;
	if (conn == nullptr) {
		//conn = gcnew OleDbConnection("Provider=Microsoft.ACE.OLEDB.12.0; Data Source="+DB_PATH);
		conn = gcnew OleDbConnection(connBuilder->ToString());
	}

	//ako ne uspe, naci bazu koristici FileDialog
	if (conn->State == ConnectionState::Open) return true;

	for (int i = 0; i<3; i++) {
		try {
			conn->Open(); conn->Close();
			return true;
		}
		catch (Exception^) {
			Windows::Forms::MessageBox::Show("Baza podataka nije nađena, nađite je manualno.");
			Windows::Forms::OpenFileDialog ^openDB = gcnew Windows::Forms::OpenFileDialog();
			openDB->InitialDirectory = "..\\";
			if (openDB->ShowDialog() == Windows::Forms::DialogResult::Cancel) return false;
			connBuilder->DataSource = openDB->FileName;
			conn->ConnectionString = connBuilder->ToString();
		}
	}
	return false;
}

	// mozda odmah da trazi i da se password poklapa (user enumeration prevention)
	// a mozda i ne, jer je SignUp jednostavan
DBAccess::Response DBAccess::LogIn(int& userID, String ^ username, String ^ password) {
	String ^selectText = "SELECT ID, password FROM [USER] WHERE username=?";
	OleDbCommand ^cmdSelectUser = gcnew OleDbCommand(selectText, conn);
	cmdSelectUser->Parameters->AddWithValue("username", username);
	OleDbDataReader^ reader;
	if (OpenConnection()) {
		reader = cmdSelectUser->ExecuteReader();
		Response resp = Response::NoSuchUser; // default
		while (reader->Read()) {
			if (userID) {
				resp = Response::MultipleUsers;	// ne bi trebalo da se desi
				break;
			}
			else if (password==reader["password"]->ToString()) {
				userID = static_cast<int>(reader["ID"]);
				resp = Response::OK;
			}
			else
				resp = Response::WrongPassword;
		}

		delete reader, cmdSelectUser;
		conn->Close();
		return resp;
	}
	else {
		return Response::ConnectionFailed;
		//MessageBox::Show(ex->Message);
	}
}

//mozda da napravim da se korisnik uloguje automatski cim se registruje,
// u suprotnom, userID je suvisan
DBAccess::Response DBAccess::SignUp(int& userID, String ^ username, String ^ password) {
	Response resp;
	String ^selectText = "INSERT INTO [USER] ([username], [password]) VALUES (?, ?)";
	OleDbCommand ^cmdSelectUser = gcnew OleDbCommand(selectText, conn);
	cmdSelectUser->Parameters->AddWithValue("username", username);
	cmdSelectUser->Parameters->AddWithValue("password", password);
	OleDbDataReader^ reader;
	if (OpenConnection()) {
		try {
			resp = cmdSelectUser->ExecuteNonQuery()==1 ? Response::OK : Response::ConnectionFailed;
			return resp;
		}
		finally {
			delete reader, cmdSelectUser;
			conn->Close();
		}
	}
	else return Response::ConnectionFailed;
}

DBAccess::Response DBAccess::UpdateLog(LogRecord *) {
	return Response::ConnectionFailed;
}
