#include "FormLogin.h"
#include "DBAccess.h"

using namespace System;
using namespace Project1;

inline Void FormLogin::btnLogin_Click(Object ^sender, EventArgs ^e) {
	String ^username = txtUsername->Text->Replace("'", ""),
		     ^password = txtPassword->Text->Replace("'", "");
	int id = 0;

	DBAccess::SetConn();
	DBAccess::Response r = DBAccess::LogIn(id, username, password);
	// obradi response
	MessageBox::Show(id.ToString());

}
