#include "Func.h"

void Rotation( Buf *DB )
{
	FILE *fp;
	Int RotaIdx, RotaInterpolationIdx;
	Int New_X, New_Y, SourcePoint;
	Int i, j;

	// own var
	double Cx = 0, Cy = 0, Hy = 0;
	// own var

	Int Center_X, Center_Y;
	Double NW=0, NE=0, SW=0, SE=0;
	Double EWweight, NSweight;
	Double Basis_X, Basis_Y;
	Double Seta = 0.0;

	fopen_s(&fp, "Rotation.raw", "w" );

	while( 1 )
	{
		printf("\n1. (0, 0) 기준 회전 \n2. 중심점 기준 회전\n\n");
		printf("수행 할 프로세스를 선택하시오 : ");
		scanf_s(" %d", &RotaIdx );

		if( RotaIdx == 1 || RotaIdx == 2 )
			break;
		else
			printf("\n다시 입력 하세요!!\n");
	}

	while( 1 )
	{
		printf("\n1. 인접 화소 보간법 회전 \n2. 양선형 보간법 회전 \n3. 3차 회선 보간법 회전 \n4. B-Spline 회전\n\n");
		printf("수행 할 프로세스를 선택하시오 : ");
		scanf_s(" %d", &RotaInterpolationIdx );

		if( RotaInterpolationIdx == 1 || RotaInterpolationIdx == 2 || RotaInterpolationIdx == 3 || RotaInterpolationIdx == 4 )
		{
			system("cls");	
			break;
		}
		else
			printf("\n다시 입력 하세요!!\n");
	}

	while (1)
	{
		printf("\n회전할 각도를 입력하시오\n\n");
		scanf_s(" %lf", &Seta);

		if (Seta+1.0 != 1.0)
			break;
		else
			printf("\n다시 입력 하세요!!\n");
	}


	for( i=0; i<COL; i++ )
	{
		for( j=0; j<ROW; j++ )
		{
			if( RotaIdx == 1 )
			{
				//  (0, 0) 기준 회전
				Cx = 0.00;
				Cy = (double)((int)ROW);
				Hy = (double)ROW;
			}
			else
			{  //  중심점 기준 회전
				Cx = (double)((int)COL / 2);
				Cy = (double)((int)ROW / 2);
				Hy = (double)ROW;
			}

			/*
			if( New_X < 0 || New_X >= COL-1 || New_Y < 0 || New_Y >= ROW-1 )
				DB->RotationImg[i * COL + j] = 0;
			else
			*/
			{


				if (RotaInterpolationIdx == 1)
					Rotation_NearesetNeighbor(DB, Cx, Cy, (double)COL, (double)ROW, Seta, i, j);
				//else if( RotaInterpolationIdx == 2 )
					//Rotation_Bilinear( DB, EWweight, NSweight, SourcePoint, NW, NE, SW, SE, i, j );
				//else if( RotaInterpolationIdx == 3 )
					//Rotation_CubicConvolution( DB, EWweight, NSweight, SourcePoint, i, j );
				//else
					//Rotation_B_Spline( DB, EWweight, NSweight, SourcePoint, i, j );
			}
		}
	}	

	fwrite( DB->RotationImg, sizeof(UChar), ROW * COL, fp );

	fclose(fp);
}

void Rotation_NearesetNeighbor( Buf *DB, Double Cx, Double Cy, Double Hx, Double Hy, Double seta, Int i, Int j )
{
	double temp = 0.0;
	double srcX = 0, srcY = 0;
	double dstX = (double)i, dstY = (double)j;
	
	srcX = (dstX - Cx)*cos(seta) + ((Hy - dstY) - Cy)*sin(seta) + Cx;
	srcY = ((-1)*(dstX - Cx))*sin(seta) + (Hy - ((Hy - dstY) - Cy)*cos(seta))+Cy;

	if (srcX > Hx || srcY > Hy || srcX < 0 || srcY < 0) {
		DB->RotationImg[i*(int)Hx + j] = 0;
	}
	else {
		temp = DB->Input[((int)srcY) * ((int)Hx) + ((int)srcX)];
		DB->RotationImg[i*(int)Hx + j] = (UChar)temp;
	}

}

void Rotation_Bilinear(Buf *DB, Double Cx, Double Cy, Double Hx, Double Hy, Double seta, Int i, Int j)
{
	double srcX = 0, srcY = 0;
	double dstX = (double)i, dstY = (double)j;

	srcX = (dstX - Cx)*cos(seta) + ((Hy - dstY) - Cy)*sin(seta) + Cx;
	srcY = ((-1)*(dstX - Cx))*sin(seta) + (Hy - ((Hy - dstY) - Cy)*cos(seta)) + Cy;

	
}

void Rotation_CubicConvolution(Buf *DB, Double Cx, Double Cy, Double Hx, Double Hy, Double seta, Int i, Int j)
{
	double srcX = 0, srcY = 0;
	double dstX = (double)i, dstY = (double)j;

	srcX = (dstX - Cx)*cos(seta) + ((Hy - dstY) - Cy)*sin(seta) + Cx;
	srcY = ((-1)*(dstX - Cx))*sin(seta) + (Hy - ((Hy - dstY) - Cy)*cos(seta)) + Cy;

		
	
}

void Rotation_B_Spline(Buf *DB, Double Cx, Double Cy, Double Hx, Double Hy, Double seta, Int i, Int j)
{
	double srcX = 0, srcY = 0;
	double dstX = (double)i, dstY = (double)j;

	srcX = (dstX - Cx)*cos(seta) + ((Hy - dstY) - Cy)*sin(seta) + Cx;
	srcY = ((-1)*(dstX - Cx))*sin(seta) + (Hy - ((Hy - dstY) - Cy)*cos(seta)) + Cy;


	
}


