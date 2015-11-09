#include "InOut.h"

typedef struct _Buffer
{
	Double  ScalingRate;
	Double  RotationAngle;
	Char    RotationFlag;
	Int     SRow, SCol;

	UChar  *Input;
	UChar  *Output;
	UChar  *PaddingImg;
	UChar  *RowScalingImg;
	UChar  *AllScalingImg;
	UChar  *RotationImg;
}Buffer;

typedef Buffer Buf;

void SubMain( Buf *DB );

void BufCon( Buf *DB, Int OrderFlag );
void ImgPadding( Buf *DB, Int MaskSize );
void Blurring( Buf *DB, Int MaskSize );

void Interpolation( Buf *DB );

void NearesetNeighbor( Buf *DB, Int i, Int j, Double Spacing, Int Dir, Int CRow, Int CCol );
void Bilinear( Buf *DB, Int i, Int j, Double Spacing, Double SubSpacing, Int Dir, Int CRow, Int CCol );
void CubicConvolution( Buf *DB, Int i, Int j, Double Spacing, Double SubSpacing, Int Dir, Int CRow, Int CCol );
void B_Spline( Buf *DB, Int i, Int j, Double Spacing, Double SubSpacing, Int Dir, Int CRow, Int CCol );

void Rotation( Buf *DB );

void Rotation_NearesetNeighbor( Buf *DB, Double EWweight, Double NSweight, Int SourcePoint, Double NW, Double NE, Double SW, Double SE, Int i, Int j );
void Rotation_Bilinear( Buf *DB, Double EWweight, Double NSweight, Int SourcePoint, Double NW, Double NE, Double SW, Double SE, Int i, Int j );
void Rotation_CubicConvolution( Buf *DB, Double EWweight, Double NSweight, Int SourcePoint, Int i, Int j );
void Rotation_B_Spline( Buf *DB, Double EWweight, Double NSweight, Int SourcePoint, Int i, Int j );

int insteadPad(double input, int fullSize);