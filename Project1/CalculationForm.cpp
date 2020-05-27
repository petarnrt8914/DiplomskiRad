﻿#include "CalculationForm.h"
#include "Calculation.h"
#include "LogRecord.h"
//#include "Point.h"

#define PROPORTIONATE_GRAPH
#define PRECALCULATE

#define MAX(a,b) (a>b ? a : b)
#define MIN(a,b) (a<b ? a : b)

using DiplomskiRad::CalculationForm;
using namespace DiplomskiRad;
using System::Text::StringBuilder;


Void CalculationForm::txtNewPoint_TextChanged(Object ^ sender, EventArgs ^ e) {
	if (sender == txtNewPointY) {
		bool isYEntered = !(String::IsNullOrWhiteSpace(txtNewPointY->Text));
		btnAddPointOrInterpolate->Text = isYEntered ? "Dodaj tačku" : "Izračunaj f(x)";
	}
	btnAddPointOrInterpolate->Enabled = canAddPointOrInterpolate();
}

inline bool CalculationForm::canAddPointOrInterpolate() {
	//can interpolate only if x was entered and there are already points
	//can add point only if both values are entered
	return txtNewPointX->Text->Trim() != String::Empty &&
					(txtNewPointY->Text->Trim() != String::Empty || listPoints->Items->Count != 0);
}


Void CalculationForm::btnAddPointOrInterpolate_Click(Object ^ sender, EventArgs ^ e) {
	bool isNewPoint = (txtNewPointY->Text->TrimStart() != String::Empty
										 && txtNewPointX->Text->TrimStart() != String::Empty);

	bool success = (isNewPoint
									? AddPointToList()
									: Interpolate());

	if (success) {
		//DrawPoints(pnlGraphArea, points, false);
		InterpolationMethod method; // = isNewPoint ? InterpolationMethod::None : ChosenInterpolationMethod; 
		pnlGraphArea->CreateGraphics()->Clear(Color::White);
		if (isNewPoint) {
			method = InterpolationMethod::None;
			DrawPoints(pnlGraphArea, InputPoints, method);
		}
		else {
			method = ChosenInterpolationMethod;
			txtNewPointY->Focus();
			txtNewPointY->SelectAll();
			DrawEverything(pnlGraphArea, InputPoints, method);
		}
	}
}

inline CalculationForm::InterpolationMethod CalculationForm::CurrentInterpolationMethod::get() {
	// mogu i da ga cuvam, ali ipak cu racunati
	if (L_Interpolated != nullptr)
		return (N_Interpolated != nullptr) ? InterpolationMethod::Both : InterpolationMethod::Lagrange;
	else
		return (N_Interpolated != nullptr) ? InterpolationMethod::Newton : InterpolationMethod::None;
}

inline CalculationForm::InterpolationMethod CalculationForm::ChosenInterpolationMethod::get() {
	return (rbLagrangeMethod->Checked ? InterpolationMethod::Lagrange
					: (rbNewtonMethod->Checked ? InterpolationMethod::Newton
						 : (rbBothInterpolations->Checked ? InterpolationMethod::Both
								: InterpolationMethod::None)
						 )
					);
}

inline array<PointF^>^ CalculationForm::InputPoints::get() {
	array<PointF^>^ points = gcnew array<PointF^>(listPoints->Items->Count);
	listPoints->Items->CopyTo(points, 0);
	return points;
}


Void CalculationForm::listPoints_SelectedIndexChanged(Object^  sender, EventArgs^  e) {
	array<int>^ selectedIndices = nullptr;

	if (listPoints->SelectedIndices->Count == 0) {
		btnDeletePoints->Enabled = false;
	}
	else {
		btnDeletePoints->Enabled = true;

		selectedIndices = gcnew array<int>(listPoints->SelectedIndices->Count);
		listPoints->SelectedIndices->CopyTo(selectedIndices, 0);
	}
	DrawEverything(pnlGraphArea, InputPoints, CurrentInterpolationMethod);
	//DrawNormalizedPoints(pnlGraphArea, normalizedPoints, selectedIndices, CurrentInterpolationMethod);
}

Void CalculationForm::listPoints_PreviewKeyDown(Object ^ sender, PreviewKeyDownEventArgs ^ e) {
	switch (e->KeyCode) {
	case Keys::A:
		if (e->Modifiers == Keys::Control)
			for (int i=0; i<listPoints->Items->Count; ++i)
				listPoints->SetSelected(i, true);
		break;
	case Keys::Delete:
	case Keys::Back:	//backspace
		btnDeletePoints->PerformClick(); break;
	case Keys::Escape:
		listPoints->ClearSelected();
	}
}

Void CalculationForm::btnDeletePoints_Click(Object^  sender, EventArgs^  e) {
	array<PointF^>^ points;
	
	if (listPoints->Items->Count == listPoints->SelectedIndices->Count) { //if removing all
		listPoints->Items->Clear();
		btnDeletePoints->Enabled = false;
		pnlGraphArea->CreateGraphics()->Clear(Color::White); //TEMP clear graph
		delete reciprocalBaricentricWeights, DividedDifferenceTable;
		reciprocalBaricentricWeights = nullptr;
		DividedDifferenceTable = nullptr;
	}
	else {
		listPoints->SelectedIndexChanged -= gcnew EventHandler(this, &CalculationForm::listPoints_SelectedIndexChanged); // STUPID WORKAROUND
		// remove selected points from list
		int i = listPoints->Items->Count - 1;
		for (; i>=0; i--) {
			if (listPoints->GetSelected(i)) // if item[i] is selected
				listPoints->Items->RemoveAt(i);
		}
		listPoints->SelectedIndexChanged -= gcnew EventHandler(this, &CalculationForm::listPoints_SelectedIndexChanged);

		points = InputPoints;

		delete DividedDifferenceTable;
		DividedDifferenceTable = gcnew TriangularMatrix(points);

		delete reciprocalBaricentricWeights;
		reciprocalBaricentricWeights = Calculation::ReciprocalBaricentricWeights(points);
		//redraw remaining
		pnlGraphArea->CreateGraphics()->Clear(Color::White);
		DrawPoints(pnlGraphArea, points, InterpolationMethod::None);
	}

	L_Interpolated = L_Normalized = 
	N_Interpolated = N_Normalized = nullptr;

	btnAddPointOrInterpolate->Enabled = canAddPointOrInterpolate();
}

Void CalculationForm::txtNewPoint_PreviewKeyDown(Object ^ sender, PreviewKeyDownEventArgs ^ e) {
	if (e->KeyData == Keys::Enter && btnAddPointOrInterpolate->Enabled)
		btnAddPointOrInterpolate->PerformClick();
	else if (e->KeyData == Keys::Escape) {
		txtNewPointX->Clear(); 		txtNewPointY->Clear();
		txtNewPointX->Focus();
	}
}


Void CalculationForm::pnlLagrangeGraph_Paint(Object ^ sender, PaintEventArgs ^ e) {
	//DrawEverything(pnlGraphArea, InputPoints, CurrentInterpolationMethod);
	//DrawPoints(pnlGraphArea, InputPoints, CurrentInterpolationMethod);
}



inline bool CalculationForm::AddPointToList() {
	double newX, newY;
	bool parseXSuccess = double::TryParse(txtNewPointX->Text, newX);
	bool parseYSuccess = double::TryParse(txtNewPointY->Text, newY);

	if (parseXSuccess && parseYSuccess) {
		PointF^ newPoint = gcnew PointF(newX, newY);
		int i; // to be index  of the new point

		//if valid, add to list on the appropriate place so it's sorted by point.X
		if (IsNewPointValid(newPoint)) {
			//listPoints->Items->Add(newPoint);
			for (i=0; i<listPoints->Items->Count; i++) {
				PointF^ currentPoint = dynamic_cast<PointF^>(listPoints->Items[i]);
				if (newPoint->X < currentPoint->X) {
					listPoints->Items->Insert(i, newPoint);
					goto newPointAdded;	// MAYBE entertaining
					//break;
				}
			}
			listPoints->Items->Add(newPoint);  // if newPoint.x is the biggest x value
			newPointAdded:
			//if (i==listPoints->Items->Count) // if newPoint.x is the biggest x value
			//	listPoints->Items->Add(newPoint);

			auto points = InputPoints;
			if (DividedDifferenceTable == nullptr || DividedDifferenceTable->Expand(newPoint) == false) {
				delete DividedDifferenceTable;
				DividedDifferenceTable = gcnew TriangularMatrix(points);
			}
			if (reciprocalBaricentricWeights == nullptr)
				reciprocalBaricentricWeights = Calculation::ReciprocalBaricentricWeights(points);
			else
				Calculation::AddBaricentricWeight(reciprocalBaricentricWeights, points, i);

			txtNewPointY->Clear();
			txtNewPointX->Clear();
		}
		else return false; //if point is invalid

		txtNewPointX->Focus();

		L_Interpolated = L_Normalized = 
		N_Interpolated = N_Normalized = nullptr;
		
		return true; 
	}
	else { //if parse failed
		MessageBox::Show("Jedna ili obe koordinatne vrednosti nisu validne", "Nevalidan unos",
										 MessageBoxButtons::OK, MessageBoxIcon::Error);
		TextBox^ invalid = (parseXSuccess ? txtNewPointY : txtNewPointX);
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
		// else if A.x==B.x then change
		else if (newPoint->X==point->X) {
			auto response = MessageBox::Show("Tačka sa datom vrednošću za koordinatu x je već uneta.\n"+
																			 "Želite li da je zamenite novom tačkom?", "Druga tačka na datoj koordinati",
																			 MessageBoxButtons::YesNo, MessageBoxIcon::Question);
			if (response==System::Windows::Forms::DialogResult::Yes) {
				listPoints->Items->Remove(point);
				return true;
			}
			else return false;
		}
	}
	return true; // if list is empty
}


inline bool CalculationForm::Interpolate() {
	double newX, newY;
	bool parseXSuccess = double::TryParse(txtNewPointX->Text, newX);

	if (parseXSuccess) {
		//get points from list
		array<PointF^>^ points = InputPoints;

		//check if x is between min.x and max.x
		float minX = points[0]->X,
					maxX = points[points->Length-1]->X;
		if (newX < minX || maxX < newX) {
			MessageBox::Show(
				String::Format("Vrednost za interpolaciju mora biti između najmanje ({0}) j najveće vrednosti ({1})", minX, maxX),
				"Vrednost van dozvoljenog domena",
				MessageBoxButtons::OK, MessageBoxIcon::Stop);
			txtNewPointX->Focus();
			txtNewPointX->SelectAll();
			return false;
		}
		InterpolationMethod method = ChosenInterpolationMethod;

		//interpolate
		if (method == InterpolationMethod::Lagrange) {
		 #ifdef PRECALCULATE
			newY = Calculation::LagrangeInterpolation(points, reciprocalBaricentricWeights, newX);
		 #else
			newY = Calculation::LagrangeInterpolation(points, newX);
		 #endif
			L_Interpolated = gcnew PointF(newX, newY);
			N_Interpolated = nullptr;
			N_Normalized = nullptr;
			txtNewPointY->Text = newY.ToString();
		}
		else if (method == InterpolationMethod::Newton) {
		 #ifdef PRECALCULATE
			newY = Calculation::NewtonInterpolation(points, DividedDifferenceTable, newX);
		 #else
			newY = Calculation::NewtonInterpolation(points, newX);
		 #endif
			N_Interpolated = gcnew PointF(newX, newY);
			L_Interpolated = nullptr;
			L_Normalized = nullptr;
			txtNewPointY->Text = newY.ToString();
		}
		else // if (method == InterpolationMethod::Both)
		{
			double LagrangeNewY = Calculation::LagrangeInterpolation(points, newX);
		 #ifdef PRECALCULATE
			LagrangeNewY = Calculation::LagrangeInterpolation(points, reciprocalBaricentricWeights, newX);
		 #else
			LagrangeNewY = Calculation::LagrangeInterpolation(points, newX);
		 #endif
			L_Interpolated = gcnew PointF(newX, LagrangeNewY);

			double NewtonNewY = Calculation::NewtonInterpolation(points, newX);
		 #ifdef PRECALCULATE
			NewtonNewY = Calculation::NewtonInterpolation(points, DividedDifferenceTable, newX);
		 #else
			NewtonNewY = Calculation::NewtonInterpolation(points, newX);
		 #endif
			N_Interpolated = gcnew PointF(newX, NewtonNewY);
			if (LagrangeNewY == NewtonNewY)
				txtNewPointY->Text = LagrangeNewY.ToString();
			else {
				//ispisuje krace
				newY = (LagrangeNewY.ToString()->Length < NewtonNewY.ToString()->Length
								? LagrangeNewY
								: NewtonNewY);
				MessageBox::Show("Lagranžov metod interpolacije: f(x) = "+LagrangeNewY+
												 "\nNjutnov metod interpolacije: f(x) = "+NewtonNewY);
			}
		}
		//points[points->Length-1] = gcnew PointF(newX, newY); //last element

		//redraw -- (CHANGED) will redraw once the method returns true
		//DrawPoints(pnlGraphArea, points, InterpolationMethod::Lagrange);

		//update log
		StringBuilder params("Tačke: {", 255);
		for each (PointF^ point in points) {
			params.AppendFormat(" ({0}, {1})", point->X, point->Y);
		}
		params.Append(" }, Tražena vrednost: " + newX);

		int mathOp = (method==InterpolationMethod::Lagrange
									? this->mathOperations["Lagranžova interpolacija"]
									: (method==InterpolationMethod::Newton
										 ? this->mathOperations["Njutnova interpolacija"]
										 : this->mathOperations["Obe interpolacije"])
									);

		LogRecord record(this->userID, mathOp, params.ToString());
		//TODO switch when ready for deployment
		// return DBAccess::UpdateLog(%record) == DBAccess::Response::OK;
		return true; //TEMP
	}
	else { //if parse X failed
		MessageBox::Show("Vrednost za x nije validan broj", "Nevalidan unos",
										 MessageBoxButtons::OK, MessageBoxIcon::Error);
		txtNewPointX->Focus();
		txtNewPointX->SelectAll();
		return false;
	}
}


void CalculationForm::DrawEverything(Control^ graphArea, array<PointF^>^ points, InterpolationMethod method) {
	graphArea->CreateGraphics()->Clear(Color::White);

	PointF ^lowerBound, ^upperBound;
	// REPEATED 
	if (method==InterpolationMethod::Lagrange && L_Interpolated!=nullptr) {
		lowerBound = gcnew PointF(L_Interpolated->X, L_Interpolated->Y);
		upperBound = gcnew PointF(L_Interpolated->X, L_Interpolated->Y);
	}
	else if (method==InterpolationMethod::Newton && N_Interpolated!=nullptr) {
		lowerBound = gcnew PointF(N_Interpolated->X, N_Interpolated->Y);
		upperBound = gcnew PointF(N_Interpolated->X, N_Interpolated->Y);
	}
	else if (method==InterpolationMethod::Both && L_Interpolated!=nullptr && N_Interpolated!=nullptr) {
		lowerBound = gcnew PointF(MIN(L_Interpolated->X, N_Interpolated->X), MIN(L_Interpolated->Y, N_Interpolated->Y));
		upperBound = gcnew PointF(MAX(L_Interpolated->X, N_Interpolated->X), MAX(L_Interpolated->Y, N_Interpolated->Y));
	}

	getMinAndMax(points, graphArea->Size, lowerBound, upperBound);



	DrawPoints(graphArea, points, method);
}


inline void CalculationForm::DrawLine(Control^ graphArea, array<PointF>^ normalizedPoints, InterpolationMethod method) {
	Pen^ LagrangePen = Pens::Blue;
	Pen^ NewtonPen = gcnew Pen(Color::Red, 1.5f);
	NewtonPen->DashStyle = Drawing2D::DashStyle::Dash;

	Graphics^ g = graphArea->CreateGraphics();

	switch (method) {
	case DiplomskiRad::CalculationForm::InterpolationMethod::Both:
	case DiplomskiRad::CalculationForm::InterpolationMethod::Lagrange:
		if (normalizedPoints->Length > 2)
			g->DrawLines(LagrangePen, normalizedPoints);
		else {
			g->DrawLine(LagrangePen, normalizedPoints[0], normalizedPoints[1]);
		}
		if (method != CalculationForm::InterpolationMethod::Both) break;
	case DiplomskiRad::CalculationForm::InterpolationMethod::Newton:
		if (normalizedPoints->Length > 2)
			g->DrawLines(NewtonPen, normalizedPoints);
		else {
			g->DrawLine(NewtonPen, normalizedPoints[0], normalizedPoints[1]);
		}
		break;
	}
}


///<summary>Draws <paramref name="points" /> on the specified <paramref name="graphArea"/></summary>
///<param name="graphArea">The control on which the points will be drawn</param>
///<param name="points">Points to be drawn, asside from the interpolated point</param>
///<param name="method">Interpolation method that was or will be used</param>
inline void CalculationForm::DrawPoints(Control^ graphArea, array<PointF^>^ points, InterpolationMethod method) {
	//fit points (scale them to the panel size)
	normalizedPoints = calculatePoints(graphArea->ClientSize, points, method);
	array<int>^ selectedIndices = gcnew array<int>(listPoints->SelectedIndices->Count);
	listPoints->SelectedIndices->CopyTo(selectedIndices, 0);
	DrawNormalizedPoints(graphArea, normalizedPoints, selectedIndices, method);
}


inline void CalculationForm::DrawNormalizedPoints(
	Control^ graphArea,
	array<PointF^>^ points,
	array<int>^ indicesOfSelectedPoints,
	InterpolationMethod method)
{
	if (points==nullptr || points->Length==0) {
		graphArea->CreateGraphics()->Clear(Color::White);
		return;
	}
	// choose pens and brushes depending on method
	Pen^		pen;
	Brush^	brushForLInterpolatedPoint;
	Pen^	penForNInterpolatedPoint;
	Brush^	brushForPoints;
	Pen^	penForEmphasis;

	Graphics^ g = graphArea->CreateGraphics();
	//g->Clear(graphArea->BackColor); //temp

	pen = gcnew Pen(Color::Purple, 2.0f);
	//pen->DashStyle = Drawing2D::DashStyle::
	brushForPoints = pen->Brush;
	brushForLInterpolatedPoint = Brushes::MediumBlue;
	//pen = gcnew Pen(Color::Orange, 2.0f);
	//pen->DashStyle = Drawing2D::DashStyle::Dash;
	penForNInterpolatedPoint = Pens::Red;
	penForEmphasis = Pens::Blue;

	//draw input points
	for (int i = 0; i<points->Length; i++)
		g->FillEllipse(brushForPoints, normalizedPoints[i]->X-2.5f, normalizedPoints[i]->Y-2.5f, 5.0f, 5.0f);

	//emphasise selected
	if (indicesOfSelectedPoints!=nullptr)
		for each (int i in indicesOfSelectedPoints)
			g->DrawRectangle(penForEmphasis, normalizedPoints[i]->X-4.0f, normalizedPoints[i]->Y-4.0f, 8.0f, 8.0f);

	//draw interpolated point
	if (method == InterpolationMethod::None)
		return;
	if (method == InterpolationMethod::Lagrange || method == InterpolationMethod::Both) {
		g->FillEllipse(brushForLInterpolatedPoint,
									 L_Normalized->X - 2.5f,
									 L_Normalized->Y - 2.5f,
									 5.0f, 5.0f);
	}
	if (method == InterpolationMethod::Newton || method == InterpolationMethod::Both)	{
		g->DrawEllipse(penForNInterpolatedPoint,
									 N_Normalized->X - 4.5f,
									 N_Normalized->Y - 4.5f,
									 9.0f, 9.0f);
	}
}


//MAYBE rename to FitPoints
//TODO add interpoints for a smoother line
///<summary>Calculates the positions of points that will be displayed on the graph</summary>
///<param name="panelSize">The Size object containing the width and height of the graph area</param>
///<param name="points">Points that will be fit into the graph</param>
///<param name="interpolatedToBeFit">Point which will contain x- and y-coordinates of the interpolated point relative to the graph</param>
///<param name="method">The interpolation method used</param>
inline array<PointF^>^ CalculationForm::calculatePoints(
		Drawing::Size panelSize, 
		array<PointF^>^ points,
		InterpolationMethod method)
{
	if (points->Length == 0) return gcnew array<PointF^>(0);

	//calculating proportions of the displayed part of graph
	PointF ^min, ^max;
	// REPEATED 
	if (method==InterpolationMethod::Lagrange && L_Interpolated!=nullptr) {
		min = gcnew PointF(L_Interpolated->X, L_Interpolated->Y);
		max = gcnew PointF(L_Interpolated->X, L_Interpolated->Y);
	} 
	else if (method==InterpolationMethod::Newton && N_Interpolated!=nullptr) {
		min = gcnew PointF(N_Interpolated->X, N_Interpolated->Y);
		max = gcnew PointF(N_Interpolated->X, N_Interpolated->Y);
	}
	else if (method==InterpolationMethod::Both && L_Interpolated!=nullptr && N_Interpolated!=nullptr) {
		min = gcnew PointF(MIN(L_Interpolated->X, N_Interpolated->X), MIN(L_Interpolated->Y, N_Interpolated->Y));
		max = gcnew PointF(MAX(L_Interpolated->X, N_Interpolated->X), MAX(L_Interpolated->Y, N_Interpolated->Y));
	}

 #ifdef PROPORTIONATE_GRAPH
	getMinAndMax(points, panelSize, min, max);
 #else
	getMinAndMax(points, min, max);
 #endif

	double width = max->X - min->X,
		height = max->Y - min->Y;
	if (width==0) width = 1.0;
	if (height==0) height = 1.0; 

	//calculating relative position of points
	auto newPoints = gcnew array<PointF^>(points->Length);
	for (int i = 0; i<points->Length; i++) {
		PointF^ newPoint = gcnew PointF();
		newPoint->X = ((points[i]->X - min->X) / width) * (panelSize.Width-6) +3;
		newPoint->Y = (panelSize.Height-6) * (1.0f - (points[i]->Y - min->Y) / height) +3;
		newPoints[i] = newPoint;
	}
	//calculating relative position of the interpolated points
	switch (method) {
	case InterpolationMethod::Both:
	case InterpolationMethod::Lagrange:
		L_Normalized = gcnew PointF();
		L_Normalized->X = ((L_Interpolated->X - min->X) / width) * (panelSize.Width-6) +3;
		L_Normalized->Y = (panelSize.Height-6) * (1.0f - (L_Interpolated->Y - min->Y) / height) +3;
		if (method != InterpolationMethod::Both) break; //if it's both, continue to Newton
	case InterpolationMethod::Newton:
		N_Normalized = gcnew PointF();
		N_Normalized->X = ((N_Interpolated->X - min->X) / width) * (panelSize.Width-6) +3;
		N_Normalized->Y = (panelSize.Height-6) * (1.0f - (N_Interpolated->Y - min->Y) / height) +3;
	//case InterpolationMethod::None:
	//default: {}
	}
	
	return newPoints;
}



///<summary>Calculates the minimum and maximum values for both x and y coordinates</summary>
///<param name="points">points that are considered for minimum and maximum values</param>
///<param name="min">The point which will contain the minimum values for x and y</param>
///<param name="max">The point which will contain the maximum values for x and y</param>
inline void CalculationForm::getMinAndMax(array<PointF^>^ points, PointF^ %min, PointF^ %max) {
	if (points==nullptr || points->Length==0) return;

	int i;
	if (min == nullptr) { //or max, whatever
		min = gcnew PointF(points[0]->X, points[0]->Y);
		max = gcnew PointF(points[points->Length-1]->X, points[0]->Y);
		i=1;
	}
	else {
		min->X = points[0]->X;
		max->X = points[points->Length-1]->X;
		i=0;
	}

	//FIXED points are sorted by X, so the first is smallest, last is biggest
	for (; i<points->Length; i++) {
		if (points[i]->Y < min->Y)
			min->Y = points[i]->Y;
		else if (points[i]->Y > max->Y)
			max->Y = points[i]->Y;
	}
}

inline void CalculationForm::getMinAndMax(array<PointF^>^ points, Drawing::Size panelSize, PointF^ %min, PointF^ %max) {
	getMinAndMax(points, min, max);

	double width = max->X - min->X,
		height = max->Y - min->Y;
	if (width==0) width = 1.0; //=panelSize.Width;
	if (height==0) height = 1.0; //= panelSize.Height;
		
	double graphRatio = width / height;
	double graphAreaRatio = static_cast<double>(panelSize.Width) / panelSize.Height;

	double differenceHalf;
	if (graphRatio > graphAreaRatio) {
		differenceHalf = (panelSize.Height - height * (panelSize.Width / width))/ (panelSize.Width / width) / 2.0;
		min->Y -= differenceHalf;
		max->Y += differenceHalf;
		height = max->Y - min->Y;
	}
	else {
		differenceHalf = (panelSize.Width - width * (panelSize.Height / height)) / (panelSize.Height / height) / 2.0;
		min->X -= differenceHalf;
		max->X += differenceHalf;
		width = max->X - min->X;
	}
}

inline Void DiplomskiRad::CalculationForm::pnlGraphArea_Resize(Object ^ sender, EventArgs ^ e) {
	DrawEverything(pnlGraphArea, InputPoints, CurrentInterpolationMethod);
}


