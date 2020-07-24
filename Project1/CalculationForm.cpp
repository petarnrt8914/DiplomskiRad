#include "CalculationForm.h"
#include "Calculation.h"
#include "LogRecord.h"
//#include "Point.h"

#define SMOOTHNESS 25.0
#define POINT_RADIUS 3.5f
//#define STEP_LINES


using namespace DiplomskiRad;
using DiplomskiRad::CalculationForm;
using System::Text::StringBuilder;


Void CalculationForm::CalculationForm_Load(Object ^ sender, EventArgs ^ e) {
 #if defined TESTING && defined INITIAL_POINTS
	//listPoints->Items->Add(gcnew PointF(-3, 9));
	listPoints->Items->Add(gcnew PointF(-1.0, -5));
	listPoints->Items->Add(gcnew PointF(-0.5, -1.125));
	listPoints->Items->Add(gcnew PointF(1.0, -3.0));
	listPoints->Items->Add(gcnew PointF(3.5, -6.125));

	//DrawPoints(pnlGraphArea, InputPoints, InterpolationMethod::None);

	txtNewPointX->Text = "1.5";
	txtNewPointX->Focus();
 #endif
}

Void CalculationForm::txtNewPoint_TextChanged(Object ^ sender, EventArgs ^ e) {
	if (listPoints->Items->Count == 0)
		btnAddPointOrInterpolate->Text = "Dodaj tačku";
	else {
		bool isYBlank = String::IsNullOrWhiteSpace(txtNewPointY->Text);
		bool isXBlank = String::IsNullOrWhiteSpace(txtNewPointX->Text);
		//btnAddPointOrInterpolate->Text = isYBlank ? "Izračunaj f(x)" : "Dodaj tačku";

		btnAddPointOrInterpolate->Text = ! isYBlank || isXBlank ? "Dodaj tačku" : "Izračunaj f(x)";
		//btnAddPointOrInterpolate->Text = (isYBlank && isXBlank ? "Dodaj tačku"
		//																	: isYBlank ? "Izračunaj f(x)": "Dodaj tačku");
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
									: PerformInterpolation());

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

inline array<PointF>^ CalculationForm::InputPoints::get() {
	array<PointF>^ points = gcnew array<PointF>(listPoints->Items->Count);
	//listPoints->Items->CopyTo(points, 0);
	for (int i=0; i<points->Length; ++i) {
		//PointF^ tmp = dynamic_cast<PointF^>(listPoints->Items[i]);
		points[i] = safe_cast<PointF>(listPoints->Items[i]);
	}
	return points;
}


inline double CalculationForm::Interpolate(InterpolationMethod method, array<PointF>^ points, double newX, Object^ precalculatedPart) {
	if (precalculatedPart == nullptr) return Interpolate(method, points, newX);

	if (method==InterpolationMethod::Lagrange) {
		auto weights = safe_cast<array<double>^>(precalculatedPart);
		return Calculation::LagrangeInterpolation(points, weights, newX);
	}
	else if (method==InterpolationMethod::Newton) {
		auto dividedDifferences = safe_cast<TriangularMatrix^>(precalculatedPart);
		return Calculation::NewtonInterpolation(points, dividedDifferences, newX);
	}
	else {
		throw gcnew Exception("A single interpolation method must be chosen");
	}
}

inline double CalculationForm::Interpolate(InterpolationMethod method, array<PointF>^ points, double newX) {
	if (method==InterpolationMethod::Lagrange) {
		return Calculation::LagrangeInterpolation(points, reciprocalBaricentricWeights, newX);
	}
	else if (method==InterpolationMethod::Newton) {
		return Calculation::NewtonInterpolation(points, DividedDifferenceTable, newX);
	}
	else {
		throw gcnew Exception("A single interpolation method must be chosen");
	}
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
	auto method = CurrentInterpolationMethod;
	if (method == InterpolationMethod::None) {
		Graphics^ g = pnlGraphArea->CreateGraphics();
		g->Clear(Color::White);
		delete g;
		DrawNormalizedPoints(pnlGraphArea, normalizedPoints, selectedIndices, method);
		//DrawPoints(pnlGraphArea, InputPoints, method);
	}
	else
		DrawEverything(pnlGraphArea, InputPoints, method);
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
	L_Interpolated = L_Normalized =
	N_Interpolated = N_Normalized = nullptr;

	if (listPoints->Items->Count == listPoints->SelectedIndices->Count) { //if removing all
		listPoints->Items->Clear();
		btnDeletePoints->Enabled = false;
		pnlGraphArea->CreateGraphics()->Clear(Color::White);
		delete reciprocalBaricentricWeights, DividedDifferenceTable;
		reciprocalBaricentricWeights = nullptr;
		DividedDifferenceTable = nullptr;
	}
	else {
		auto tmpEventHandler = gcnew EventHandler(this, &CalculationForm::listPoints_SelectedIndexChanged); // STUPID WORKAROUND
		listPoints->SelectedIndexChanged -= tmpEventHandler;
		// remove selected points from list
		int i = listPoints->Items->Count - 1;
		for (; i>=0; i--) {
			if (listPoints->GetSelected(i)) // if item[i] is selected
				listPoints->Items->RemoveAt(i);
		}
		listPoints->SelectedIndexChanged += tmpEventHandler;

		array<PointF>^ points = InputPoints;

		delete DividedDifferenceTable;
		DividedDifferenceTable = gcnew TriangularMatrix(points);

		delete reciprocalBaricentricWeights;
		reciprocalBaricentricWeights = Calculation::ReciprocalBaricentricWeights(points);
		//redraw remaining
		pnlGraphArea->CreateGraphics()->Clear(Color::White);
		DrawPoints(pnlGraphArea, points, InterpolationMethod::None);
	}

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

Void CalculationForm::pnlGraphArea_Resize(Object ^ sender, EventArgs ^ e) {
	auto method = CurrentInterpolationMethod;
	if (method == InterpolationMethod::None)
		DrawPoints(pnlGraphArea, InputPoints, method);
	else 
		DrawEverything(pnlGraphArea, InputPoints, method);
}

inline bool CalculationForm::AddPointToList() {
	double newX, newY;
	bool parseXSuccess = double::TryParse(txtNewPointX->Text, newX);
	bool parseYSuccess = double::TryParse(txtNewPointY->Text, newY);

	if (parseXSuccess && parseYSuccess) {
		PointF newPoint(newX, newY);
		int i; // to be index  of the new point

		//if valid, add to list on the appropriate place so it's sorted by point.X
		if (IsNewPointValid(newPoint)) {
			//listPoints->Items->Add(newPoint);
			for (i=0; i<listPoints->Items->Count; i++) {
				PointF^ currentPoint = dynamic_cast<PointF^>(listPoints->Items[i]);
				if (newPoint.X < currentPoint->X) {
					listPoints->Items->Insert(i, newPoint);
					goto newPointAdded;	// MAYBE entertaining
					//break;
				}
			}
			listPoints->Items->Add(newPoint);  // if newPoint.x is the biggest x value
			newPointAdded:
			//if (i==listPoints->Items->Count) // if newPoint.x is the biggest x value
			//	listPoints->Items->Add(newPoint);

		 #ifdef PRECALCULATE
			auto points = InputPoints;
			if (DividedDifferenceTable == nullptr || DividedDifferenceTable->Expand(newPoint) == false) {
				delete DividedDifferenceTable;
				DividedDifferenceTable = gcnew TriangularMatrix(points);
			}
			if (reciprocalBaricentricWeights == nullptr)
				reciprocalBaricentricWeights = Calculation::ReciprocalBaricentricWeights(points);
			else
				Calculation::AddBaricentricWeight(reciprocalBaricentricWeights, points, i);
		 #endif

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
				auto points = InputPoints;
				reciprocalBaricentricWeights = Calculation::ReciprocalBaricentricWeights(points);
				DividedDifferenceTable = gcnew TriangularMatrix(points);
				return true;
			}
			else return false;
		}
	}
	return true; // if list is empty
}


inline bool CalculationForm::PerformInterpolation() {
	//check if there are at least 2 points to interpolate between
	if (listPoints->Items->Count < 2) {
		MessageBox::Show(
			"Za interpolaciju je potrebno uneti bar 2 tačke između kojih se računa interpolant",
			"Neispunjen uslov",
			MessageBoxButtons::OK, MessageBoxIcon::Warning
		);
		txtNewPointY->Focus();
		return false;
	}

	double newX, newY;
	bool parseXSuccess = double::TryParse(txtNewPointX->Text, newX);

	if (parseXSuccess) {
		//get points from list
		array<PointF>^ points = InputPoints;

		//check if x already has y in points
		for (int i=0; i<points->Length; i++) {
		//for each (PointF^ point in points) {
			if (static_cast<float>(newX) == points[i].X) {
				MessageBox::Show(
					String::Format("Tačka sa tom vrednošću za x je uneta:\n{0}", points[i].ToString()),
					"Redundantna operacija",
					MessageBoxButtons::OK, MessageBoxIcon::Warning
				);
				listPoints->SelectedIndex = i;
				txtNewPointX->Focus();
				txtNewPointX->SelectAll();
				return false;
			}
		}

		//check if x is between min.x and max.x
		float minX = points[0].X,
					maxX = points[points->Length-1].X;
		if (newX < minX || maxX < newX) {
			MessageBox::Show(
				String::Format("Vrednost za interpolaciju mora biti između najmanje ({0}) i najveće vrednosti ({1})", minX, maxX),
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
			N_Interpolated = N_Normalized = nullptr;
			txtNewPointY->Text = newY.ToString("G6");
		}
		else if (method == InterpolationMethod::Newton) {
		 #ifdef PRECALCULATE
			newY = Calculation::NewtonInterpolation(points, DividedDifferenceTable, newX);
		 #else

			newY = Calculation::NewtonInterpolation(points, newX);
		 #endif
			N_Interpolated = gcnew PointF(newX, newY);
			L_Interpolated = L_Normalized = nullptr;
			txtNewPointY->Text = newY.ToString("G6");
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
				txtNewPointY->Text = LagrangeNewY.ToString("G6");
			else {
				//ispisuje krace
				newY = (LagrangeNewY.ToString()->Length < NewtonNewY.ToString()->Length
								? LagrangeNewY
								: NewtonNewY);
				txtNewPointY->Text = newY.ToString("G6");
				MessageBox::Show("Lagranžov metod interpolacije: f(x) = "+LagrangeNewY+
												 "\nNjutnov metod interpolacije: f(x) = "+NewtonNewY +
												 "\n\nRazlika: " + (NewtonNewY-LagrangeNewY).ToString());
			}
		}

		//update log
		StringBuilder params("Tačke: {", 255);
		for each (PointF point in points) {
			params.AppendFormat(" ({0:G6}, {1:G6})", point.X, point.Y);
		}
		params.Append(" }, Tražena vrednost: " + newX);

		int mathOp = (method==InterpolationMethod::Lagrange
									? this->mathOperations["Lagranžova interpolacija"]
									: (method==InterpolationMethod::Newton
										 ? this->mathOperations["Njutnova interpolacija"]
										 : this->mathOperations["Obe interpolacije"])
									);

		LogRecord record(this->userID, mathOp, params.ToString());
		#if defined TESTING && defined SUPRESS_LOG
		return true;
		#else
		return DBAccess::UpdateLog(%record) == DBAccess::Response::OK;
		#endif
	}
	else { //if parse X failed
		MessageBox::Show("Vrednost za x nije validan broj", "Nevalidan unos",
										 MessageBoxButtons::OK, MessageBoxIcon::Error);
		txtNewPointX->Focus();
		txtNewPointX->SelectAll();
		return false;
	}
}


void CalculationForm::DrawEverything(Control^ graphArea, array<PointF>^ points, InterpolationMethod method) {
	Graphics ^g = graphArea->CreateGraphics();
	g->Clear(Color::White);
	if (points == nullptr || points->Length==0 || graphArea->Width==0) return;	// graphArea width is 0 when the window is minimized

	//// get interpoints that will fit into the graph area
	//array<array<PointF>^>^ normalizedLines = getNormalizedLines(points, graphArea->ClientSize);
	//
	////draw lines
	//for each (array<PointF>^ line in normalizedLines)
	//	DrawLine(graphArea, line, method);
	//
	////draw points on top of lines
	//DrawPoints(graphArea, points, normalizedLines[0][0], method);

	normalizedPoints = calculatePoints(graphArea->ClientSize, points, method);

	array<PointF>^ NormalizedInterpoints;
	//array<PointF>^ interpoints;
	switch (method) {
	case InterpolationMethod::Both:
	case InterpolationMethod::Lagrange: {
		NormalizedInterpoints = getInterpoints(normalizedPoints, InterpolationMethod::Lagrange);
		//interpoints = getInterpoints(points, InterpolationMethod::Lagrange);
		//NormalizedInterpoints = FitInterpoints(interpoints, normalizedPoints[0], LAST_IN_ARRAY(normalizedPoints));
		DrawLine(g, NormalizedInterpoints, InterpolationMethod::Lagrange);
		//delete interpoints; interpoints = nullptr;
		if (method != InterpolationMethod::Both) break;
	}
	case InterpolationMethod::Newton:
		//interpoints = getInterpoints(points, InterpolationMethod::Newton);
		//NormalizedInterpoints = FitInterpoints(interpoints, normalizedPoints[0], LAST_IN_ARRAY(normalizedPoints));
		NormalizedInterpoints = getInterpoints(normalizedPoints, InterpolationMethod::Newton);
		DrawLine(g, NormalizedInterpoints, InterpolationMethod::Newton);
		//delete interpoints; interpoints = nullptr;
	}
	
	array<int>^ selectedIndices = gcnew array<int>(listPoints->SelectedIndices->Count);
	listPoints->SelectedIndices->CopyTo(selectedIndices, 0);

	DrawNormalizedPoints(graphArea, normalizedPoints, selectedIndices, method);
}


inline void CalculationForm::DrawLine(Graphics^ g, array<PointF>^ normalizedPoints, InterpolationMethod method) {
	Pen ^ LagrangePen = gcnew Pen(Color::MediumBlue, 2.0f);
	Pen^ NewtonPen = gcnew Pen(Color::Red, 2.0f);
	//NewtonPen->DashStyle = Drawing2D::DashStyle::Dash;
	//NewtonPen->Alignment = Drawing2D::PenAlignment::
	LagrangePen->CompoundArray = gcnew array<float>(4) {0.0f, 0.5f, 1.0f, 1.0f};	//interesting
	NewtonPen->CompoundArray = gcnew array<float>(4) {0.0f, 0.0f, 0.5f, 1.0f};
	//LagrangePen->Alignment = Drawing2D::PenAlignment::Inset;
	//NewtonPen->Alignment = Drawing2D::PenAlignment::Outset;
	NewtonPen->DashStyle = Drawing2D::DashStyle::DashDotDot;

	#if defined TESTING && defined STEP_LINES
	for each (PointF point in normalizedPoints) {
		g->DrawLine(Pens::Blue, PointF(point.X, 0), PointF(point.X, pnlGraphArea->ClientSize.Height));
	}
	#endif

	switch (method) {
	case InterpolationMethod::Both:
	case InterpolationMethod::Lagrange:
		if (normalizedPoints->Length > 2)
			g->DrawLines(LagrangePen, normalizedPoints);
		else
			g->DrawLine(LagrangePen, normalizedPoints[0], normalizedPoints[1]);

		if (method != InterpolationMethod::Both) break;
	case InterpolationMethod::Newton:
		if (normalizedPoints->Length > 2)
			g->DrawLines(NewtonPen, normalizedPoints);
		else
			g->DrawLine(NewtonPen, normalizedPoints[0], normalizedPoints[1]);
	}
}


///<summary>Draws <paramref name="points" /> on the specified <paramref name="graphArea"/></summary>
///<param name="graphArea">The control on which the points will be drawn</param>
///<param name="points">Points to be drawn, asside from the interpolated point</param>
///<param name="method">Interpolation method that was or will be used</param>
inline void CalculationForm::DrawPoints(Control^ graphArea, array<PointF>^ points, InterpolationMethod method) {
	if (points==nullptr || points->Length==0) return;
	//if (method == InterpolationMethod::None) graphArea->CreateGraphics()->Clear(Color::White);
	normalizedPoints = calculatePoints(graphArea->ClientSize, points, method);
	array<int>^ selectedIndices = gcnew array<int>(listPoints->SelectedIndices->Count);
	listPoints->SelectedIndices->CopyTo(selectedIndices, 0);
	DrawNormalizedPoints(graphArea, normalizedPoints, selectedIndices, method);
}


inline void CalculationForm::DrawNormalizedPoints(
	Control^ graphArea,
	array<PointF>^ normalizedPoints,
	array<int>^ indicesOfSelectedPoints,
	InterpolationMethod method)
{
	if (normalizedPoints==nullptr || normalizedPoints->Length==0) {
		//graphArea->CreateGraphics()->Clear(Color::White);
		return;
	}
	// choose pens and brushes depending on method
	Pen^		pen;
	Brush^	brushForLInterpolatedPoint;
	Pen^	penForNInterpolatedPoint;
	Pen^	penForLInterpolatedPoint;
	Brush^	brushForPoints;
	Pen^	penForEmphasis;

	Graphics^ g = graphArea->CreateGraphics();
	//g->Clear(graphArea->BackColor); //temp
	if (method == InterpolationMethod::None)
		g->Clear(Color::White);

	pen = gcnew Pen(Color::Purple, 2.0f);
	//pen->DashStyle = Drawing2D::DashStyle::
	brushForPoints = pen->Brush;
	brushForLInterpolatedPoint = Brushes::White;
	//pen = gcnew Pen(Color::Orange, 2.0f);
	//pen->DashStyle = Drawing2D::DashStyle::Dash;
	penForNInterpolatedPoint = Pens::Red;
	penForLInterpolatedPoint = Pens::Red;
	penForEmphasis = Pens::Blue;

	//float pointRadius = 3.5f;

	//draw input points
	for (int i = 0; i<normalizedPoints->Length; i++)
		g->FillEllipse(brushForPoints,
									 normalizedPoints[i].X-POINT_RADIUS, normalizedPoints[i].Y-POINT_RADIUS,
									 POINT_RADIUS*2.0f, POINT_RADIUS*2.0f);

	//emphasise selected
	if (indicesOfSelectedPoints!=nullptr)
		for each (int i in indicesOfSelectedPoints)
			g->DrawRectangle(penForEmphasis,
											 normalizedPoints[i].X-POINT_RADIUS-2.0f, normalizedPoints[i].Y-POINT_RADIUS-2.0f,
											 POINT_RADIUS*2.0f+4.0f, POINT_RADIUS*2.0f+4.0f);

	//draw interpolated point
	switch (method) {
	case InterpolationMethod::Both:
	case InterpolationMethod::Lagrange:
		//cross
		g->DrawLine(penForLInterpolatedPoint,
								L_Normalized->X, L_Normalized->Y - 10.0f,
								L_Normalized->X, L_Normalized->Y + 10.0f);
		g->DrawLine(penForLInterpolatedPoint,
								L_Normalized->X - 10.0f, L_Normalized->Y,
								L_Normalized->X + 10.0f, L_Normalized->Y);
		//point
		g->FillEllipse(brushForLInterpolatedPoint,
									 L_Normalized->X - POINT_RADIUS,
									 L_Normalized->Y - POINT_RADIUS,
									 POINT_RADIUS*2.0f, POINT_RADIUS*2.0f);
		if (method != InterpolationMethod::Both) break;
	case InterpolationMethod::Newton:
		g->DrawEllipse(penForNInterpolatedPoint,
									 N_Normalized->X - 4.0f,
									 N_Normalized->Y - 4.0f,
									 8.0f, 8.0f);
		g->DrawEllipse(penForNInterpolatedPoint,
									 N_Normalized->X - POINT_RADIUS,
									 N_Normalized->Y - POINT_RADIUS,
									 POINT_RADIUS*2.0f, POINT_RADIUS*2.0f);
	}
}


//MAYBE rename to FitPoints
//TODO add interpoints for a smoother line
///<summary>Calculates the positions of points that will be displayed on the graph</summary>
///<param name="panelSize">The Size object containing the width and height of the graph area</param>
///<param name="points">Points that will be fit into the graph</param>
///<param name="interpolatedToBeFit">Point which will contain x- and y-coordinates of the interpolated point relative to the graph</param>
///<param name="method">The interpolation method used</param>
inline array<PointF>^ CalculationForm::calculatePoints(
		Drawing::Size panelSize, 
		array<PointF>^ points,
		InterpolationMethod method)
{
	if (points->Length == 0) return gcnew array<PointF>(0);

	//calculating proportions of the displayed part of graph
	PointF ^min, ^max;

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
	auto newPoints = gcnew array<PointF>(points->Length);
	for (int i = 0; i<points->Length; i++) {
		PointF newPoint;
		newPoint.X = ((points[i].X - min->X) / width) * (panelSize.Width - 2.0f*POINT_RADIUS) + POINT_RADIUS;
		newPoint.Y = (1.0f - (points[i].Y - min->Y) / height) * (panelSize.Height - 2.0f*POINT_RADIUS) + POINT_RADIUS;
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

//obsolete, I guess
inline array<PointF>^ CalculationForm::calculatePoints(Drawing::Size panelSize, 
																											 array<PointF>^ points,
																											 PointF^ firstPointOfGraph,
																											 InterpolationMethod method) {
	array<PointF>^ normalizedPoints = calculatePoints(panelSize ,points, method);
	if (firstPointOfGraph != nullptr) {
		double offset = firstPointOfGraph->Y - normalizedPoints[0].Y;
		if (offset != 0)
			for (int i = 0; i<normalizedPoints->Length; ++i)
				normalizedPoints[i].Y += offset;

		switch (method) {
		case InterpolationMethod::Both:
		case InterpolationMethod::Lagrange:
			L_Normalized->Y += offset;
			if (method != InterpolationMethod::Both) break;
		case InterpolationMethod::Newton:
			N_Normalized->Y += offset;
		}
	}

	return normalizedPoints;
}


inline array<PointF>^ CalculationForm::getInterpoints(array<PointF>^ points, InterpolationMethod method) {
	double step = (LAST_IN_ARRAY(points).X - points[0].X) / SMOOTHNESS; // smoothness of interpolated line
	if (step>15) step=15;

	List<PointF>^ interpoints = gcnew List<PointF>(SMOOTHNESS + points->Length); //so there's no need to resize
	interpoints->Add(points[0]);

	PointF^ interpolatedPoint;
	Object^ precalculatedPart;

	if (method==InterpolationMethod::Lagrange) {
		//interpolatedPoint = L_Interpolated;
		interpolatedPoint = L_Normalized;
		#ifdef PRECALCULATE
		precalculatedPart = Calculation::ReciprocalBaricentricWeights(points);
		#endif
	}
	else if (method==InterpolationMethod::Newton) {
		//interpolatedPoint = N_Interpolated;
		interpolatedPoint = N_Normalized;
		#ifdef PRECALCULATE
		precalculatedPart = gcnew TriangularMatrix(points);
		#endif
	}
	else throw gcnew Exception("Interpolation method was not chosen");
	
	for (int i = 0; i<points->Length-1; ++i) {
		double interpointX = points[i].X + step;
		double interpointY;

		for (; Math::Round(interpointX, 6) < points[i+1].X; interpointX += step) {
			if (interpointX > interpolatedPoint->X && interpointX-step < interpolatedPoint->X) {
				interpointX = interpolatedPoint->X + 0.1; // so interpointX-step would definitely be > interpolatedPoint->X
				interpoints->Add( *interpolatedPoint );
				//auto randArray = gcnew array<PointF>(50);
				//interpoints->CopyTo(0, randArray, 0, MIN(interpoints->Count, randArray->Length));
				//interpoints->Add( safe_cast<PointF>(interpolatedPoint) );
			}
			else {
				//interpointY = Interpolate(method, points, interpointX);
				interpointY = Interpolate(method, points, interpointX, precalculatedPart);
				interpoints->Add(PointF(interpointX, interpointY));
			}
		}
		interpoints->Add(points[i+1]);
	}

	return interpoints->ToArray();
}

//obsolete, but can be used, it's maybe even better
inline array<PointF>^ CalculationForm::FitInterpoints(array<PointF>^ interpoints, PointF normalizedFirst, PointF normalizedLast) {
	double panelWidth = static_cast<double>(normalizedLast.X) - normalizedFirst.X;
	double width = LAST_IN_ARRAY(interpoints).X - static_cast<double>(interpoints[0].X);
	double ratio = panelWidth / width;

	array<PointF>^ normalizedInterpoints = gcnew array<PointF>(interpoints->Length);
	normalizedInterpoints[0] = normalizedFirst;
	LAST_IN_ARRAY(normalizedInterpoints) = normalizedLast;


	for (int i = 1; i < interpoints->Length-1; i++) {
		double normalizedX = normalizedFirst.X + (interpoints[i].X - interpoints[0].X) * ratio;
		double normalizedY = normalizedFirst.Y - (interpoints[i].Y - interpoints[0].Y) * ratio; // minus, because (0,0) is top left, not bottom left
		normalizedInterpoints[i] = PointF(normalizedX, normalizedY);
	}

	return normalizedInterpoints;
}




///<summary>Calculates the minimum and maximum values for both x and y coordinates</summary>
///<param name="points">points that are considered for minimum and maximum values</param>
///<param name="min">The point which will contain the minimum values for x and y</param>
///<param name="max">The point which will contain the maximum values for x and y</param>
inline void CalculationForm::getMinAndMax(array<PointF>^ points, PointF^ %lowerBound, PointF^ %upperBound) {
	if (points==nullptr || points->Length==0) return;

	auto method = CurrentInterpolationMethod;
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

	int i;
	if (lowerBound == nullptr) { //or max, whatever
		lowerBound = gcnew PointF(points[0].X, points[0].Y);
		upperBound = gcnew PointF(LAST_IN_ARRAY(points).X, points[0].Y);
		i=1;
	}
	else {
		lowerBound->X = points[0].X;
		upperBound->X = LAST_IN_ARRAY(points).X;
		i=0;
	}

	//FIXED points are sorted by X, so the first is smallest, last is biggest
	for (; i<points->Length; i++) {
		if (points[i].Y < lowerBound->Y)
			lowerBound->Y = points[i].Y;
		else if (points[i].Y > upperBound->Y)
			upperBound->Y = points[i].Y;
	}
}

inline void CalculationForm::getMinAndMax(array<PointF>^ points, Drawing::Size panelSize, PointF^ %min, PointF^ %max) {
	getMinAndMax(points, min, max);

	double width = max->X - min->X,
		height = max->Y - min->Y;
	if (width==0) width = 1.0; //=panelSize.Width;
	if (height==0) height = 1.0; //= panelSize.Height;
		
	double graphRatio = width / height;
	double graphAreaRatio = static_cast<double>(panelSize.Width) / panelSize.Height;
	Drawing::SizeF panelToGraphRatio(panelSize.Width / width, panelSize.Height / height);

	double differenceHalf;
	if (graphRatio > graphAreaRatio) {
		//differenceHalf = (panelSize.Height - height * (panelSize.Width / width))/ (panelSize.Width / width) / 2.0;
		differenceHalf = (panelSize.Height - height * panelToGraphRatio.Width)/ panelToGraphRatio.Width / 2.0;
		min->Y -= differenceHalf;
		max->Y += differenceHalf;
		//height = max->Y - min->Y;
	}
	else {
		differenceHalf = (panelSize.Width - width * panelToGraphRatio.Height) / panelToGraphRatio.Height / 2.0;
		min->X -= differenceHalf;
		max->X += differenceHalf;
		//width = max->X - min->X;
	}
}


