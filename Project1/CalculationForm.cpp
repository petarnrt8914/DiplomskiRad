#include "CalculationForm.h"
#include "Calculation.h"
#include "Point.h"

using namespace System;
using namespace DiplomskiRad;

Void CalculationForm::txtNewPointX_TextChanged(Object ^ sender, EventArgs ^ e) {
		btnAddPoint->Enabled = txtNewPointX->Text->Trim() != String::Empty;
}

Void CalculationForm::txtNewPointY_TextChanged(Object ^ sender, EventArgs ^ e) {
	btnAddPoint->Text = txtNewPointY->Text->Trim() == String::Empty
											? "Izračunaj f(x)"
											: "Dodaj tačku";
}

Void CalculationForm::btnAddPoint_Click(Object ^ sender, EventArgs ^ e) {
	double PointX, PointY;
	bool isNewPoint = txtNewPointY->Text->TrimStart() != String::Empty;
	bool parseXSuccess = double::TryParse(txtNewPointX->Text, PointX);
	bool parseYSuccess = false;

	if (isNewPoint) {
		parseYSuccess = double::TryParse(txtNewPointY->Text, PointY);
		if(parseXSuccess && parseYSuccess) {
			//new Point(x,y)
			PointF^ newPoint = gcnew PointF(PointX, PointY);
			//add to list
			listPoints->Items->Add(newPoint);
			txtNewPointY->Clear();
			txtNewPointX->Focus();
			txtNewPointX->Clear();
		}
	}
	else if (parseXSuccess) {
		//get points from list
		array<PointF^>^ points = gcnew array<PointF^>(listPoints->Items->Count);
		listPoints->Items->CopyTo(points, 0);
		//interpolate
		PointY = Calculation::LagrangeInterpolation(points, listPoints->Items->Count, PointX);
		txtNewPointY->Text = PointY.ToString();
		//new Point(x,y)

		//update log
	}
	
	//show on graph
}


Void CalculationForm::listPoints_SelectedIndexChanged(Object^  sender, EventArgs^  e) {
	btnDeletePoints->Enabled = listPoints->SelectedIndices->Count > 0;
}

Void CalculationForm::btnDeletePoints_Click(Object^  sender, EventArgs^  e) {
	array<int>^ sortedIndices = gcnew array<int>(listPoints->SelectedIndices->Count);
	listPoints->SelectedIndices->CopyTo(sortedIndices,0);
	Array::Sort(sortedIndices);
	int i = sortedIndices->Length-1;
	for (int curIndex; i>=0; i--) { //unazad da se indexi ne bi menjali
		curIndex = sortedIndices[i];
		listPoints->Items->RemoveAt(curIndex);
	}
}

