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
		printf("1. ���� ȭ�� ������ \n2. �缱�� ������ \n3. 3�� ȸ�� ������ \n4. B-Spline\n\n");
		printf("���� �� ���μ����� �����Ͻÿ� : ");
		scanf_s(" %d", &Num );

		if( Num == 1 || Num == 2 || Num == 3 || Num == 4 )
		{
			system("cls");	
			break;
		}
		else
			printf("\n�ٽ� �Է� �ϼ���!!\n");
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
		printf("���� ȸ���� �Ͻðڽ��ϱ�? (y or n) ");
		scanf_s(" %c", &DB->RotationFlag );

		if( DB->RotationFlag == 'y' || DB->RotationFlag == 'n' )
			break;
		else
			printf("\n�ٽ� �Է� �ϼ���!!\n");
	}

	if( DB->RotationFlag == 'y' )
	{
		printf("���� ȸ�� ���� �Է� : ");
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