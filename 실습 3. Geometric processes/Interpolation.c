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

void NearesetNeighbor( Buf *DB, Int i, Int j, Double Spacing, Int Dir, Int CRow, Int CCol )
{
	if( Dir )
	{
		
	}
	else
	{
		
	}
}

void Bilinear( Buf *DB, Int i, Int j, Double Spacing, Double SubSpacing, Int Dir, Int CRow, Int CCol )
{
	if( Dir )
	{
		
	}
	else
	{
		
	}
}

void CubicConvolution( Buf *DB, Int i, Int j, Double Spacing, Double SubSpacing, Int Dir, Int CRow, Int CCol )
{
	Double W0, W1, W2, W3;
	
	if( Dir )
	{
		
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