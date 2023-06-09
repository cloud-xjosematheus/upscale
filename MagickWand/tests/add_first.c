#include <stdio.h>
#include <MagickWand/MagickWand.h>

/* Simplify the exception handling
 * technically we should abort the program if
 *      severity >= ErrorException
 */
void ThrowWandException(MagickWand *wand)
{ char
  *description;

  ExceptionType
  severity;

  description=MagickGetException(wand,&severity);
  (void) fprintf(stderr,"%s %s %lu %s\n",GetMagickModule(),description);
  description=(char *) MagickRelinquishMemory(description);
}

/* useful function especially after appending two wands together */
#define SwapWands(a,b) { MagickWand *tmp=a; a=b; b=tmp; }

int main(int argc, char *argv[])
{
  MagickWand
    *wand,
    *input,
    *output;

  MagickBooleanType
    status;

  printf("Check prepend when using 'FirstIterator' on empty wand\n");
  printf("Result should be: 3210\n");

  MagickWandGenesis();

  wand = NewMagickWand();

  MagickSetFirstIterator(wand);    /* set first iterator to empty wand */

  status = MagickReadImage(wand, "font_0.gif" );
  if (status == MagickFalse)
    ThrowWandException(wand);

  status = MagickReadImage(wand, "font_1.gif" );
  if (status == MagickFalse)
    ThrowWandException(wand);

  status = MagickReadImage(wand, "font_2.gif" );
  if (status == MagickFalse)
    ThrowWandException(wand);

  status = MagickReadImage(wand, "font_3.gif" );
  if (status == MagickFalse)
    ThrowWandException(wand);

  /* append all images together to create the output wand */
  MagickResetIterator(wand); /* append all images */
  output = MagickAppendImages(wand,MagickFalse);
  wand = DestroyMagickWand(wand);  /* finished - could swap here */

  /* Final output */
  status = MagickWriteImage(output,"show:");
  if (status == MagickFalse)
    ThrowWandException(output);

  output = DestroyMagickWand(output);

  MagickWandTerminus();
}

