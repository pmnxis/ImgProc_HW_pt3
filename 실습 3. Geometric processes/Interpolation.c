#include "Func.h"

#define  dUp dLeft
#define dDown dRight

struct useTable {
	int use[4];
}typedef useTable;

void Interpolation( Buf *DB )
{
	FILE *afp, *bfp;
	Int i, j, Num;
	Double Rate, Spacing=0, SubSpacing=0;

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
	
	for( i=0; i<DB->SRow ; i++ )
	{
		for( j=0; j<COL; j++ )
		{
		
			//  실수로  Dir을 함수내부에서 거꾸로 처리하게되서... 1->0 , 0->1합니다..
			if( Num == 1 )
				NearesetNeighbor( DB, i, j, Spacing, 1, DB->SRow, DB->SCol );
			else if( Num == 2 )
				Bilinear( DB, i, j, Spacing, SubSpacing, 1, DB->SRow, DB->SCol);
			else if( Num == 3 )
				CubicConvolution( DB, i, j, Spacing, SubSpacing, 1, DB->SRow, DB->SCol);
			else if( Num == 4 )
				B_Spline( DB, i, j, Spacing, SubSpacing, 1, DB->SRow, DB->SCol);
		}
	}

	fwrite( DB->RowScalingImg, sizeof(UChar), DB->SRow * COL, afp );

	for( i=0; i<DB->SRow; i++ )
	{
		for( j=0; j<DB->SCol; j++)
		{
			

			if( Num == 1 )
				NearesetNeighbor( DB, i, j, Spacing, 0, DB->SRow, DB->SCol );
			else if( Num == 2 )
				Bilinear( DB, i, j, Spacing, SubSpacing, 0, DB->SRow, DB->SCol );
			else if( Num == 3 )
				CubicConvolution( DB, i, j, Spacing, SubSpacing, 0, DB->SRow, DB->SCol );
			else if( Num == 4 )
				B_Spline( DB, i, j, Spacing, SubSpacing, 0, DB->SRow, DB->SCol );
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


void NearesetNeighbor(Buf *DB, Int i, Int j, Double Spacing, Int Dir, Int CRow, Int CCol)
{
	Double temp = 0;
	if (Dir)
	{
		//  가우스처리
		temp = ((double)i *(double)ROW / (double)CRow);
		//  get error
		DB->RowScalingImg[i*COL + j] = DB->Input[(int)temp*COL + j];
	}
	else
	{
		temp = ((double)j *(double)COL) / (double)CCol;
		//  가우스처리
		DB->AllScalingImg[i*CCol + j] = DB->RowScalingImg[i*COL + (int)temp];
	}
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
		temp = ((double)i *(double)ROW / (double)CRow);
		rY = (int)temp;
		dUp = temp - rY;
		if (dUp == 0.00) {
			//  get error
			DB->RowScalingImg[i*COL + j] = DB->Input[rY*COL + j];
		}
		else if (rY >= 0 && rY <= COL - 2) {
			dDown = 1.0 - dUp;
			temp = ((double)DB->Input[rY*COL + j])*dDown + ((double)DB->Input[COL * (rY+1) + j] * dUp);
			DB->RowScalingImg[i*COL + j] = (UChar)temp;
		}
		else if (rY >= CRow - 1)
		{
			DB->RowScalingImg[i*COL + j] = DB->Input[rY*COL + j];
		}
	}
	else  //  all
	{
		temp = ((double)j *(double)COL) / (double)CCol;
		rX = (int)temp;
		dLeft = temp - rX;
		if (dLeft == 0.00) {
			DB->AllScalingImg[i*CCol + j] = DB->RowScalingImg[COL*i + (rX)];
		}
		else if (rX >= 0 && rX <= COL - 2) {
			dRight = 1.0 - dLeft;
			temp = ((double)DB->RowScalingImg[COL*i + (rX)] * dRight + ((double)DB->RowScalingImg[COL*i+(rX + 1)])*dLeft);
			DB->AllScalingImg[i*CCol + j] = (UChar)temp;
		}
		else if (rX >= CCol - 1) {
			DB->AllScalingImg[i*CCol + j] = DB->RowScalingImg[rX*CCol + j];
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
		orgPxPoint = ((double)i *(double)ROW / (double)CRow);
		P0 = (int)(orgPxPoint - 1);
		P1 = (int)(orgPxPoint);
		P2 = (int)(orgPxPoint + 1);
		P3 = (int)(orgPxPoint + 2);

		//  SAVE REAL VALUE FROM ARR.
		R0 = DB->Input[ insteadPad(P0, ROW) * COL + j ];
		R1 = DB->Input[ insteadPad(P1, ROW) * COL + j ];
		R2 = DB->Input[ insteadPad(P2, ROW) * COL + j ];
		R3 = DB->Input[ insteadPad(P3, ROW) * COL + j ];

		// GET WEIGHT VALUE
		W0 = get3cha_weightVal(P0, orgPxPoint, 0.5);
		W1 = get3cha_weightVal(P1, orgPxPoint, 0.5);
		W2 = get3cha_weightVal(P2, orgPxPoint, 0.5);
		W3 = get3cha_weightVal(P3, orgPxPoint, 0.5);

		calcVal = R0*W0 + R1 * W1 + R2*W2 + R3*W3;
		//  get error 
		DB->RowScalingImg[i*COL + j] = (UChar)calcVal;
	}
	else
	{
		orgPxPoint = ((double)j *(double)COL) / (double)CCol;

		P0 = (int)(orgPxPoint - 1);
		P1 = (int)(orgPxPoint);
		P2 = (int)(orgPxPoint + 1);
		P3 = (int)(orgPxPoint + 2);

		R0 = DB->Input[i * COL + insteadPad(P0, COL)];
		R1 = DB->Input[i * COL + insteadPad(P1, COL)];
		R2 = DB->Input[i * COL + insteadPad(P2, COL)];
		R3 = DB->Input[i * COL + insteadPad(P3, COL)];

		W0 = get3cha_weightVal(P0, orgPxPoint, 0.5);
		W1 = get3cha_weightVal(P1, orgPxPoint, 0.5);
		W2 = get3cha_weightVal(P2, orgPxPoint, 0.5);
		W3 = get3cha_weightVal(P3, orgPxPoint, 0.5);

		calcVal = R0*W0 + R1 * W1 + R2*W2 + R3*W3;
		DB->AllScalingImg[i*CCol +j] = (UChar)calcVal;

	}
}

void B_Spline( Buf *DB, Int i, Int j, Double Spacing, Double SubSpacing, Int Dir, Int CRow, Int CCol )
{
	Int P0, P1, P2, P3;
	Double W0, W1, W2, W3;
	Double R0, R1, R2, R3;
	Double orgPxPoint, calcVal;
	if (Dir)
	{
		// recheck col and row after go home
		orgPxPoint = ((double)i *(double)ROW / (double)CRow);
		P0 = (int)(orgPxPoint - 1);
		P1 = (int)(orgPxPoint);
		P2 = (int)(orgPxPoint + 1);
		P3 = (int)(orgPxPoint + 2);

		//  SAVE REAL VALUE FROM ARR.
		R0 = DB->Input[insteadPad(P0, ROW) * COL + j];
		R1 = DB->Input[insteadPad(P1, ROW) * COL + j];
		R2 = DB->Input[insteadPad(P2, ROW) * COL + j];
		R3 = DB->Input[insteadPad(P3, ROW) * COL + j];

		// GET WEIGHT VALUE
		W0 = getBSpl_weightVal(P0, orgPxPoint);
		W1 = getBSpl_weightVal(P1, orgPxPoint);
		W2 = getBSpl_weightVal(P2, orgPxPoint);
		W3 = getBSpl_weightVal(P3, orgPxPoint);

		calcVal = R0*W0 + R1 * W1 + R2*W2 + R3*W3;
		// get error
		DB->RowScalingImg[i*COL + j] = (UChar)calcVal;
	}
	else
	{
		orgPxPoint = ((double)j *(double)COL) / (double)CCol;

		P0 = (int)(orgPxPoint - 1);
		P1 = (int)(orgPxPoint);
		P2 = (int)(orgPxPoint + 1);
		P3 = (int)(orgPxPoint + 2);

		R0 = DB->Input[i * COL + insteadPad(P0, COL)];
		R1 = DB->Input[i * COL + insteadPad(P1, COL)];
		R2 = DB->Input[i * COL + insteadPad(P2, COL)];
		R3 = DB->Input[i * COL + insteadPad(P3, COL)];

		W0 = getBSpl_weightVal(P0, orgPxPoint);
		W1 = getBSpl_weightVal(P1, orgPxPoint);
		W2 = getBSpl_weightVal(P2, orgPxPoint);
		W3 = getBSpl_weightVal(P3, orgPxPoint);

		calcVal = R0*W0 + R1 * W1 + R2*W2 + R3*W3;
		DB->AllScalingImg[i*CCol + j] = (UChar)calcVal;

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

Double getBSpl_weightVal(int input, Double orgPxAdr) {
	double temp = orgPxAdr - input;
	if (temp < 0)temp = temp * (-1);

	//     1/2 |??|^3?|??|^2+2/3
	if (0 <= temp && temp < 1) {
		return ((1/2)*pow(temp, 3)) - (pow(temp, 2)) + (2/3);
	}
	//  ?1/6 |??|^3+|??|^2?2|??|  +  4/3
	else if (1 <= temp && temp < 2) {
		return ((-1)/6 * pow(temp, 3)) - (pow(temp, 2)) - (2*temp) - (4/3);
	}
	else if (2 <= temp) {
		return 0;
	}
	return 0;
}
