#include "Func.h"

void Rotation( Buf *DB )
{
	FILE *fp;
	Int RotaIdx, RotaInterpolationIdx;
	Int New_X, New_Y, SourcePoint;
	Int i, j;
	Int Center_X, Center_Y;
	Double NW=0, NE=0, SW=0, SE=0;
	Double EWweight, NSweight;
	Double Basis_X, Basis_Y;
	Double Seta;

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

	for( i=0; i<COL; i++ )
	{
		for( j=0; j<ROW; j++ )
		{
			if( RotaIdx == 1 )
			{
				
			}
			else
			{
				
			}

			if( New_X < 0 || New_X >= COL-1 || New_Y < 0 || New_Y >= ROW-1 )
				DB->RotationImg[i * COL + j] = 0;
			else
			{


				if( RotaInterpolationIdx == 1)
					Rotation_NearesetNeighbor( DB, EWweight, NSweight, SourcePoint, NW, NE, SW, SE, i, j );
				else if( RotaInterpolationIdx == 2 )
					Rotation_Bilinear( DB, EWweight, NSweight, SourcePoint, NW, NE, SW, SE, i, j );
				else if( RotaInterpolationIdx == 3 )
					Rotation_CubicConvolution( DB, EWweight, NSweight, SourcePoint, i, j );
				else
					Rotation_B_Spline( DB, EWweight, NSweight, SourcePoint, i, j );
			}
		}
	}	

	fwrite( DB->RotationImg, sizeof(UChar), ROW * COL, fp );

	fclose(fp);
}

void Rotation_NearesetNeighbor( Buf *DB, Double EWweight, Double NSweight, Int SourcePoint, Double NW, Double NE, Double SW, Double SE, Int i, Int j )
{
	
}

void Rotation_Bilinear( Buf *DB, Double EWweight, Double NSweight, Int SourcePoint, Double NW, Double NE, Double SW, Double SE, Int i, Int j )
{
	Double Top, Bottom;

	
}

void Rotation_CubicConvolution( Buf *DB, Double EWweight, Double NSweight, Int SourcePoint, Int i, Int j )
{
	Int    k, l;
	Double W0, W1, W2, W3;
	Double RowPixel[4][4];
	Double ColPixel[4];
		
	
}

void Rotation_B_Spline( Buf *DB, Double EWweight, Double NSweight, Int SourcePoint, Int i, Int j )
{
	Int    k, l;
	Double W0, W1, W2, W3;
	Double RowPixel[4][4];
	Double ColPixel[4];

	
}


