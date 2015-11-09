#include "Func.h"

void BufCon( Buf *DB, Int OrderFlag )
{
	if( OrderFlag )
	{
		FILE *fp;
		fopen_s( &fp, FILENAME, "rb" );	

		DB->Input      = (UChar*)calloc(ROW*COL, sizeof(UChar));
		DB->Output     = (UChar*)calloc(ROW*COL, sizeof(UChar));

		fread( DB->Input, sizeof(UChar), ROW*COL, fp );

		fclose(fp);
	}	
	else
	{
		free( DB->Input );
		free( DB->Output );
		free( DB->PaddingImg );
		free( DB->RowScalingImg );
		free( DB->AllScalingImg );
		free( DB->RotationImg );
	}
}

void ImgPadding( Buf *DB, Int MaskSize )
{
	Int line, i, j;

	for( line=0; line<(MaskSize / 2) - 1; line++ )
	{
		for( i=0; i<ROW; i++ )
		{
			DB->PaddingImg[i + ((MaskSize - 1) / 2) + (line * ROW)] = DB->Input[i];
			DB->PaddingImg[i + ((ROW + MaskSize - 1) * (COL + MaskSize - 2)) + ((MaskSize - 1) / 2)] = DB->Input[i + (ROW * (COL  - 1))];
		}

		for( i=0; i<COL; i++ )
		{
			DB->PaddingImg[(i + 1) * (ROW + MaskSize - 1)] = DB->Input[i * ROW];
			DB->PaddingImg[(i + 1) * (ROW + MaskSize - 1) + (ROW + MaskSize - 2)] = DB->Input[i * ROW + (ROW - 1)];
		}
	}

	for( line=0; line<4; line++ )
	{
		for( i=0; i<(MaskSize / 2); i++ )
		{
			for( j=0; j<(MaskSize / 2); j++ )
			{
				if( line == 0 )
				{
					DB->PaddingImg[i * (ROW + MaskSize - 1) + j] = DB->Input[0];
				}
				else if( line == 1 )
				{
					DB->PaddingImg[i * (ROW + MaskSize - 1) + j + ROW + (MaskSize / 2)] = DB->Input[ROW - 1];
				}
				else if( line == 2 )
				{
					DB->PaddingImg[i * (ROW + MaskSize - 1) + j + ((ROW + MaskSize - 1) * (COL + (MaskSize / 2)))] = DB->Input[ROW * (COL - 1)];
				}
				else
				{
					DB->PaddingImg[i * (ROW + MaskSize - 1) + j + ((ROW + MaskSize - 1) * (COL + (MaskSize / 2))) + ROW + (MaskSize / 2)] = DB->Input[ROW * COL - 1];
				}
			}
		}
	}

	for( i=0; i<COL; i++ )
	{
		for( j=0; j<ROW; j++ )
		{
			DB->PaddingImg[(ROW + MaskSize - 1) + ((MaskSize - 1) / 2) + j + (i * (ROW + MaskSize - 1))] = DB->Input[j + (i * ROW)];
		}
	}
}

double Clamping(double * data) {
	if (*data > 255)*data = 255;
	if (*data < 0)*data = 0;
	return *data;
}