#include "Func.h"

void Blurring( Buf *DB, Int MaskSize )
{
	Int i, j, k, l;
	Int ResultPix = 0;
	Double *OriImg;

	OriImg = (Double*)calloc(MaskSize * MaskSize, sizeof(double));

	for( i=0; i<COL; i++ )
	{
		for( j=0; j<ROW; j++ )
		{
			for( k=0; k<MaskSize; k++ )
			{
				for( l=0; l<MaskSize; l++ )
				{
										
				}
			}



			DB->Output[ (i * ROW) + j ] = ResultPix > 255 ? 255 : ResultPix < 0 ? 0 : ResultPix;
			ResultPix = 0;
		}
	}

	free(OriImg);
}