#include "Func.h"

void main()
{		
	Buf InOut;	

	BufCon( &InOut, TRUE );
	
	SubMain( &InOut );

	BufCon( &InOut, FALSE );
}

void SubMain( Buf *DB )
{
	printf("���� ������ ��� �Է� : ");
	scanf_s(" %lf", &DB->ScalingRate );

	DB->SCol		  = (Int)((Double)COL * DB->ScalingRate);
	DB->SRow		  = (Int)((Double)ROW * DB->ScalingRate);
	DB->PaddingImg    = (UChar*)calloc((ROW + 3 - 1)*(COL + 3 - 1), sizeof(UChar));
	DB->RowScalingImg = (UChar*)calloc(DB->SRow * COL, sizeof(UChar));
	DB->AllScalingImg = (UChar*)calloc(DB->SRow * DB->SCol, sizeof(UChar));
	DB->RotationImg   = (UChar*)calloc(DB->SRow * DB->SCol, sizeof(UChar));
	printf("��ȯ�� ������ ũ��� %d x %d�Դϴ�.\n\n", DB->SRow, DB->SCol);

	if( DB->ScalingRate < 1 )
	{
		ImgPadding( DB, 3 );
		Blurring( DB, 3 );

		memcpy( DB->Input, DB->Output, sizeof(UChar) * ROW * COL );
	}

	Interpolation( DB );
}