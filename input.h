/* ========================================================================
   $HEADER FILE
   $File: input.h $
   $Program: $
   $Developer: Jordan Marling $
   $Created On: 2015/11/18 $
   $Description: $
   $Revisions: $
   ======================================================================== */

#if !defined(INPUT_H)
#define INPUT_H

#include "render.h"
#include "simulator.h"
#include "universe.h"

void handleInput (struct simulation *, struct universe *,
		  struct renderstate *);

#endif
