#include "CalculationForm.h"
#include "Calculation.h"
#include "LogRecord.h"
//#include "Point.h"

using namespace DiplomskiRad;
using System::Text::StringBuilder;
using System::Windows::Forms::DialogResult;

Void CalculationForm::txtNewPointX_TextChanged(Object ^ sender, EventArgs ^ e) {
		btnAddPoint->Enabled = txtNewPointX->Text->Trim() != String::Empty;
}

Void CalculationForm::txtNewPointY_TextChanged(Object ^ sender, EventArgs ^ e) {
	btnAddPoint->Text = txtNewPointY->Text->Trim() == String::Empty
											? "Izračunaj f(x)"
											: "Dodaj tačku";
}

Void CalculationForm::btnAddPoint_Click(Object ^ sender, EventArgs ^ e) {
	bool isNewPoint = (txtNewPointY->Text->TrimStart() != String::Empty
										 && txtNewPointX->Text->TrimStart() != String::Empty);

	bool success = (isNewPoint
									? AddPointToList()
									: PerformLagrangeInterpolation());
	
	if (success) {
		//show on graph
	}
}

Void CalculationForm::listPoints_SelectedIndexChanged(Object^  sender, EventArgs^  e) {
	btnDeletePoints->Enabled = listPoints->SelectedIndices->Count > 0;
}

Void CalculationForm::btnDeletePoints_Click(Object^  sender, EventArgs^  e) {
	//if removing all
	if (listPoints->Items->Count==listPoints->SelectedIndices->Count) { 
		listPoints->Items->Clear();
		btnDeletePoints->Enabled = false;
		pnlLagrangeGraph->CreateGraphics()->Clear(pnlLagrangeGraph->BackColor);
		return;
	}

	// remove selected points from list
	int i = listPoints->Items->Count - 1;
	for (; i>=0; i--) {
		if (listPoints->GetSelected(i)) // if item[i] is selected
			listPoints->Items->RemoveAt(i);
	}

	//redraw remaining
	array<PointF^>^ points = gcnew array<PointF^>(listPoints->Items->Count);
	listPoints->Items->CopyTo(points, 0);
	DrawPoints(pnlLagrangeGraph, points, false);
}

Void CalculationForm::txtNewPoint_PreviewKeyDown(Object ^ sender, PreviewKeyDownEventArgs ^ e) {
	if (e->KeyData == Keys::Enter && btnAddPoint->Enabled)
		btnAddPoint->PerformClick();
}

Void CalculationForm::pnlLagrangeGraph_MouseUp(Object ^ sender, MouseEventArgs ^ e) {
	//Graphics^ g = pnlLagrangeGraph->CreateGraphics();

}

Void CalculationForm::pnlLagrangeGraph_MouseMove(Object ^ sender, MouseEventArgs ^ e) {
	return; //temporarily
	Graphics^ g = pnlLagrangeGraph->CreateGraphics();
	currentGraphState = g->Save();
	
	g->Clear(pnlLagrangeGraph->BackColor);
	g->DrawLine(Pens::Blue,
							e->X, 0,
							e->X, pnlLagrangeGraph->Height);
	g->DrawLine(Pens::Blue,
							0, e->Y,
							pnlLagrangeGraph->Width, e->Y);
	
	//TODO fix error: doesn't show the interpolated point on graph
	if (listPoints->Items->Count < 3) return;
	array<PointF^>^ points = gcnew array<PointF^>(listPoints->Items->Count);
	for (int i = 0; i<listPoints->Items->Count; i++) {
		points[i] = dynamic_cast<PointF^>(listPoints->Items[i]);
	}
	DrawPoints(pnlLagrangeGraph, points, false);
	//g->Restore(currentGraphState);

	//idea:
	//drawing points saves graphics
	//mousemove makes new graphics out of saved one and draws cross on it
}




inline bool CalculationForm::AddPointToList() {
	double newX, newY;
	bool parseXSuccess = double::TryParse(txtNewPointX->Text, newX);
	bool parseYSuccess = double::TryParse(txtNewPointY->Text, newY);

	if (parseXSuccess && parseYSuccess) {
		//new Point(x,y)
		PointF^ newPoint = gcnew PointF(newX, newY);

		//if valid, add to list
		if (IsNewPointValid(newPoint)) {
			listPoints->Items->Add(newPoint);
			txtNewPointY->Clear();
			txtNewPointX->Clear();
		}
		txtNewPointX->Focus();

		//draw on graph 
		//mora lepse i citljivije da se odradi
		//if (listPoints->Items->Count < 3) return ;
		array<PointF^>^ points = gcnew array<PointF^>(listPoints->Items->Count);
		listPoints->Items->CopyTo(points, 0);
		DrawPoints(pnlLagrangeGraph, points, false);

		return true;
	}
	else {
		MessageBox::Show("Jedna ili obe koordinatne vrednosti nisu validne", "Nevalidan unos",
										 MessageBoxButtons::OK, MessageBoxIcon::Error);
		TextBox^ invalid = (parseXSuccess ? txtNewPointX : txtNewPointY);
		invalid->Focus();
		invalid->SelectAll();
		return false;
	}
}

inline bool CalculationForm::IsNewPointValid(PointF^ newPoint) {
	for each (PointF^ point in listPoints->Items) {
			// if A==B reject and report
		if (newPoint->Equals(point)) {
			MessageBox::Show("Data tačka je već uneta", "Duplikat", MessageBoxButtons::OK, MessageBoxIcon::Information);
			return false;
		}
		// else if A.x==B.x then change?
		else if (newPoint->X==point->X) {
			auto response = MessageBox::Show("Tačka sa datom vrednošću za koordinatu x je već uneta."+
																			 "Želite li da je zamenite novom tačkom?", "Druga tačka na datoj koordinati",
																			 MessageBoxButtons::YesNo, MessageBoxIcon::Question);
			if (response==System::Windows::Forms::DialogResult::Yes)
				listPoints->Items->Remove(point);
			else
				return false;

			break;
		}
	}
	return true;
}


inline bool CalculationForm::PerformLagrangeInterpolation() {
	double newX, newY;
	bool parseXSuccess = double::TryParse(txtNewPointX->Text, newX);

	if (parseXSuccess) {
		//get points from list
		array<PointF^>^ points = gcnew array<PointF^>(listPoints->Items->Count+1);
		listPoints->Items->CopyTo(points, 0);

		//interpolate
		newY = Calculation::LagrangeInterpolation(points, listPoints->Items->Count, newX);
		txtNewPointY->Text = newY.ToString();
		points[points->Length-1] = gcnew PointF(newX, newY); //last element

		//redraw
		DrawPoints(pnlLagrangeGraph, points, true);

		//update log
		StringBuilder params("Tačke: {", 255);
		for each (PointF^ point in points) {
			params.AppendFormat(" ({0}, {1})", point->X, point->Y);
		}
		params.Append(" }, Tražena vrednost: "+newX);

		LogRecord record(this->userID,
										 this->mathOperations["Lagranžova interpolacija"],
										 params.ToString());
		//TODO switch when ready for deployment
		// return DBAccess::UpdateLog(%record) == DBAccess::Response::OK;
		return true; //temporary
	}
	else {
		MessageBox::Show("Vrednost za x nije validan broj", "Nevalidan unos",
										 MessageBoxButtons::OK, MessageBoxIcon::Error);
		txtNewPointX->Focus();
		txtNewPointX->SelectAll();
		return false;
	}
}

void CalculationForm::DrawPoints(Control^ graphArea, array<PointF^>^ points, bool isLastPointInterpolated) {
	//TODO maybe actually make the graph proportionate
	Graphics^ g = graphArea->CreateGraphics();
	g->Clear(graphArea->BackColor);
	int count = points->Length; // Point count

	auto normalizedPoints = calculatePoints(points, pnlLagrangeGraph->Size);

	for (int i=0; i<count-1; i++) 
		g->FillEllipse(Brushes::Black, normalizedPoints[i]->X-2.5f, normalizedPoints[i]->Y-2.5f, 5.0f, 5.0f);
	
	//cross (optional)
	if (isLastPointInterpolated) {
		g->DrawLine(Pens::Blue,
								normalizedPoints[count-1]->X, 0.0f,
								normalizedPoints[count-1]->X, static_cast<float>(pnlLagrangeGraph->Height));
		g->DrawLine(Pens::Blue,
								0.0f, normalizedPoints[count-1]->Y,
								static_cast<float>(pnlLagrangeGraph->Width), normalizedPoints[count-1]->Y);
	}

	//draw last point in the appropriate color
	Brush^ colorOfLastPoint = isLastPointInterpolated ? Brushes::LimeGreen : Brushes::Black;
	g->FillEllipse(colorOfLastPoint,
								 normalizedPoints[count-1]->X-2.5f,
								 normalizedPoints[count-1]->Y-2.5f,
								 5.0f, 5.0f);
}

inline array<PointF^>^ CalculationForm::calculatePoints(array<PointF^>^ points, Drawing::Size panelSize) {
	//calculating min and max values
	PointF^ min = gcnew PointF(points[0]->X, points[0]->Y);
	PointF^ max = gcnew PointF(points[0]->X, points[0]->Y);

	for each (PointF^ point in points) {
		if (point->X < min->X)
			min->X = point->X;
		else if (point->X > max->X)
			max->X = point->X;

		if (point->Y < min->Y)
			min->Y = point->Y;
		else if (point->Y > max->Y)
			max->Y = point->Y;
	}

	//calculating proportions of the displayed part of graph
	auto newPoints = gcnew array<PointF^>(points->Length);
	float width = max->X - min->X,
				height = max->Y - min->Y;
	if (width==0) width = 1; //=panelSize.Width;
	if (height==0) height = 1; //= panelSize.Height;

	//calculating relative position of points
	for (int i = 0; i<points->Length; i++) {
		PointF^ newPoint = gcnew PointF();
		newPoint->X = ((points[i]->X - min->X) / width) * (panelSize.Width-6) +3;
		newPoint->Y = (panelSize.Height-6) * (1.0f - (points[i]->Y - min->Y) / height) +3;
		newPoints[i] = newPoint;
	}

	return newPoints;
}
