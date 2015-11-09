#include "Func.h"

void Blurring( Buf *DB, Int MaskSize )
{
	Int i, j, k, l;
	Int ResultPix = 0;
	Double *OriImg;
	double Blurring_table[9] = { InfOne, InfOne, InfOne, InfOne, InfOne, InfOne, InfOne, InfOne, InfOne };
	OriImg = (Double*)calloc(MaskSize * MaskSize, sizeof(double));
	ImgPadding(DB, MaskSize);
	Convolution(DB, MaskSize, Blurring_table, 0);
}

void Convolution(Buf *DB, Int MaskSize, Double * FilterMask, int arithVal)
{
	Int oh, ow, dbh, dbw;
	int i;
	Int ResultPix = 0;
	double convolTmp = 0;
	Double *OriImg;
	int padWidth = COL + MaskSize - 1;
	int padHeight = ROW + MaskSize - 1;

	//printf("\nAA");
	OriImg = (Double*)calloc(MaskSize * MaskSize, sizeof(double));
	memset(DB->Output, 0, ROW*COL);


	for (dbh = 0; dbh<COL; dbh++)
	{
		for (dbw = 0; dbw<ROW; dbw++)
		{
			/*** OriImg ���ۿ� �������� ���� �ڵ��ϰ��� �ϴ� �κ��� �ҷ��� ***/
			//  oh : height of OriImg , ow : width of OriImg
			//  dbh : height of DB output , dbw : width of DB output.
			//  PaddingImg ��  �Ϻκ��� oriImg�� �ֽ��ϴ�.
			//printf("padSrc to oriImg Dst\n");
			for (oh = 0; oh<MaskSize; oh++)
				for (ow = 0; ow < MaskSize; ow++) {
					//printf("[%d,%d] : offset dst : %d , \toffset src : %d\n", dbh, dbw, oh*MaskSize + ow, (dbh + oh)*padWidth + (dbw + ow));
					OriImg[oh*MaskSize + ow] = (DB->PaddingImg)[(dbh + oh)*padWidth + (dbw + ow)];
					//printf("%lf\n",OriImg[oh*MaskSize + ow]);
				}
			for (i = 0; i<MaskSize * MaskSize; i++)
			{
				/*** ���� ȸ�� ***/
				convolTmp = (OriImg[i] * (FilterMask[i])) + convolTmp;
				//printf("OriImg [ %d ] : %lf  *  %lf += convolTemp  : %lf \n", i, OriImg[i], (DB->FilterMask[i]), convolTmp);
			}
			// arithVal �� ����ٰ� ���� �ֽ��ϴ�. (���� �����ҽ� Ʋ���ִ� offset �Լ��� �Ƹ� ����� ����� �Ұ����� ����.
			if (arithVal != 0)convolTmp += arithVal;
			// ���� arithVal �� 0�̸� �۾��� ���մϴ�.
			Clamping(&(convolTmp));
			DB->Output[dbh*COL + dbw] = (UChar)convolTmp;
			convolTmp = 0.0;
		}
	}
	free(OriImg);
}