#include "Func.h"

void Interpolation( Buf *DB )
{
	FILE *afp, *bfp;
	Int i, j, Num;
	Double Rate, Spacing, SubSpacing;

	fopen_s(&afp, "RowInterpolation.raw", "w" );
	fopen_s(&bfp, "AllInterpolation.raw", "w" );

	while( 1 )
	{
		printf("1. 인접 화소 보간법 \n2. 양선형 보간법 \n3. 3차 회선 보간법 \n4. B-Spline\n\n");
		printf("수행 할 프로세스를 선택하시오 : ");
		scanf_s(" %d", &Num );

		if( Num == 1 || Num == 2 || Num == 3 || Num == 4 )
		{
			system("cls");	
			break;
		}
		else
			printf("\n다시 입력 하세요!!\n");
	}
	
	for( i=0; i<COL; i++ )
	{
		for( j=0; j<DB->SRow; j++ )
		{
		

			if( Num == 1 )
				NearesetNeighbor( DB, i, j, Spacing, 0, DB->SRow, COL );
			else if( Num == 2 )
				Bilinear( DB, i, j, Spacing, SubSpacing, 0, DB->SRow, COL );
			else if( Num == 3 )
				CubicConvolution( DB, i, j, Spacing, SubSpacing, 0, DB->SRow, COL );
			else if( Num == 4 )
				B_Spline( DB, i, j, Spacing, SubSpacing, 0, DB->SRow, COL );
		}
	}

	fwrite( DB->RowScalingImg, sizeof(UChar), DB->SRow * COL, afp );

	for( i=0; i<DB->SRow; i++ )
	{
		for( j=0; j<DB->SCol; j++)
		{
			

			if( Num == 1 )
				NearesetNeighbor( DB, i, j, Spacing, 1, DB->SRow, DB->SCol );
			else if( Num == 2 )
				Bilinear( DB, i, j, Spacing, SubSpacing, 1, DB->SRow, DB->SCol );
			else if( Num == 3 )
				CubicConvolution( DB, i, j, Spacing, SubSpacing, 1, DB->SRow, DB->SCol );
			else if( Num == 4 )
				B_Spline( DB, i, j, Spacing, SubSpacing, 1, DB->SRow, DB->SCol );
		}
	}

	fwrite( DB->AllScalingImg, sizeof(UChar), DB->SRow * DB->SCol, bfp );

	while( 1 )
	{
		printf("영상 회전을 하시겠습니까? (y or n) ");
		scanf_s(" %c", &DB->RotationFlag );

		if( DB->RotationFlag == 'y' || DB->RotationFlag == 'n' )
			break;
		else
			printf("\n다시 입력 하세요!!\n");
	}

	if( DB->RotationFlag == 'y' )
	{
		printf("영상 회전 각도 입력 : ");
		scanf_s(" %lf", &DB->RotationAngle );

		Rotation( DB );
	}

	fclose(afp);
	fclose(bfp);
}

void Bilinear(Buf *DB, Int i, Int j, Double Spacing, Double SubSpacing, Int Dir, Int CRow, Int CCol)
{
	//  i = new y
	//  j = new x
	Int rX, rY;  //  reverse X Y
	double dLeft, dRight;
	double temp;
	if (Dir)  //  row
	{
		temp = j / CRow*ROW;
		rX = (int)temp;
		dLeft = temp - rX;
		if (dLeft == 0.00) {
			DB->RowScalingImg[i*CRow + j] = DB->Input[i*ROW + rX];
		}
		else if (rX >= 0 && rX <= ROW - 2) {
			dRight = 1.0 - dLeft;
			temp = (DB->Input[i*ROW + rX])*dRight + (DB->Input[i*ROW + rX + 1] * dLeft);
			DB->RowScalingImg[i*CRow + j] = (UChar)temp;
		}
		else if (rX >= CRow - 1)
		{
			DB->RowScalingImg[i*CRow + j] = DB->Input[i*ROW + rX];
		}
	}
	else  //  all
	{
		temp = i / CCol*COL;
		rY = (int)temp;
		dUp = temp - rY;
		if (dUp == 0.00) {
			DB->AllScalingImg[i*CRow + j] = DB->RowScalingImg[rY*CRow + j];
		}
		else if (rY >= 0 && rY <= COL - 2) {
			dDown = 1.0 - dUp;
			temp = (DB->RowScalingImg[rY*CRow + j] * dDown + (DB->RowScalingImg[(rY + 1)*CRow + j])*dUp);
			DB->AllScalingImg[i*CRow + j] = (UChar)temp;
		}
		else if (rY >= CCol - 1) {
			DB->AllScalingImg[i*CRow + j] = DB->RowScalingImg[rY*CRow + j];
		}
	}
}

void CubicConvolution( Buf *DB, Int i, Int j, Double Spacing, Double SubSpacing, Int Dir, Int CRow, Int CCol )
{
	Int P0, P1, P2, P3;
	Double W0, W1, W2, W3;
	Double R0, R1, R2, R3;
	Double orgPxPoint, calcVal;
	if( Dir )
	{
		// recheck col and row after go home
		orgPxPoint = j / CCol * COL;
		P1 = insteadPad(orgPxPoint, CCol);
		P0 = insteadPad(orgPxPoint - 1, CCol);
		P2 = insteadPad(orgPxPoint + 1, CCol);
		P3 = insteadPad(orgPxPoint + 2, CCol);
		
		//  SAVE REAL VALUE FROM ARR.
		R0 = DB->Input[i * COL + P0];
		R1 = DB->Input[i * COL + P1];
		R2 = DB->Input[i * COL + P2];
		R3 = DB->Input[i * COL + P3];

		// GET WEIGHT VALUE
		W0 = get3cha_weightVal(W0, orgPxPoint, 0.5);
		W1 = get3cha_weightVal(W1, orgPxPoint, 0.5);
		W2 = get3cha_weightVal(W2, orgPxPoint, 0.5);
		W3 = get3cha_weightVal(W3, orgPxPoint, 0.5);

		calcVal = R0*W0 + R1 * W1 + R2*W2 + R3*W3;
		



	}
	else
	{
		
	}
}

void B_Spline( Buf *DB, Int i, Int j, Double Spacing, Double SubSpacing, Int Dir, Int CRow, Int CCol )
{
	Double W0, W1, W2, W3;

	if( Dir )
	{
	
	}
	else
	{
	
	}
}

int insteadPad(double input, int fullSize) {
	if (input < 0)return 0;
	else if (input > fullSize - 1)return fullSize - 1;
	else return (int)input;
}

Double get3cha_weightVal(int input, Double orgPxAdr, Double alpha) {
	double temp = orgPxAdr - input;
	if (temp < 0)temp = temp * (-1);

	//  (??+2) |??|^3?(??+3) |??|^2+1,  0≤|??|<1
	if (0 <= temp && temp < 1) {
		return ((alpha + 2)*pow(temp, 3)) - ((alpha + 3)*pow(temp, 2)) + 1;
	}
	//  ??|??|^3?5??|??|^2+8??|??|?4??,   1≤|??|<2
	else if (1<=temp && temp < 2) {
		return (alpha * pow(temp, 3)) - (5 * alpha*pow(temp, 2)) + (8 * alpha*temp) - (4 * alpha);
	}
	else if (2 <= temp) {
		return 0;
	}
	return 0;
}