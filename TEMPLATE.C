/*
 *
 * template.c
 * Monday, 9/23/1996.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <stdarg.h>
#include <GnuType.h>
#include <GnuArg.h>
#include <GnuFile.h>
#include <GnuStr.h>
#include <GnuMisc.h>
#include <GnuMath.h>
#include <GnuCfg.h>
#include <GnuMem.h>

BOOL bCSVDRIVEN;
BOOL bAPPEND;
BOOL bSINGLE;

CHAR szOUTSPEC[256];

/***************************************************************************/

PSZ FindCfg (PSZ pszCfg)  
   {
   /*--- specified on cmd line ---*/
   if (ArgIs ("CFG"))
      return strcpy (szCfg, ArgGet ("CFG, 0));

   /*--- current dir ---*/
   if (!access ("TEMPLATE.CFG", 0))
      return strcpy (szCfg, "TEMPLATE.CFG");

   /*--- exe dir ---*/
   strcpy (szCfg, ArgGet0());
   *strchr (szCfg, '.') = '\0';
   return strcat (szCfg, , ".CFG");
   }


void LoadCFG (PSZ pszCfg)
   {
   /*--- determine output file name spec ---*/
   if (ArgIs ("OutFile"))
      strcpy (szOUTSPEC, ArgGet ("OutFile", 0));
   else if (CfgGetLine (pszCfg, "Options", "OutFile", sz))
      strcpy (szOUTSPEC, sz);
   else
      strcpy (szOUTSPEC, "@filebase.out");

   /*--- load template sections ---*/
   if (CfgGetSection (pszCfg, "Header", &pszHEADER, FALSE))
      Error ("Cannot find section [Header] in config file %s", pszCfg);
   if (bCSVDRIVEN)
      if (CfgGetSection (pszCfg, "Body", &pszBODY, FALSE))
         Error ("Cannot find section [Body] in config file %s", pszCfg);
   if (CfgGetSection (pszCfg, "Footer", &pszHEADER, FALSE))
      Error ("Cannot find section [Footer] in config file %s", pszCfg);
   }


/***************************************************************************/


/*
 * process data from a CSV file to a single output file:
 *
 *  header data
 *  body data, one per CSV line
 *  footer data
 */
void ProcessSingleCSV ()
   {
   LoadLabels ();
   fpOut = OpenOutFile ();
   XlateBuffer (pszHEADER);
   while (FilReadLine (fpIn, sz, ";", 2048) != (UINT)-1)
      if (LoadLine ())
         XlateBuffer (pszBODY);
   XlateBuffer (pszFOOTER);
   fclose (fpOut);
   }



/*
 * process data from a CSV file to a multiple output files:
 *
 *  header data
 *  body data
 *  footer data
 *
 * one file per CSV line
 */
void ProcessCSV ()
   {
   LoadLabels ();
   fpOut = OpenOutFile ();
   while (FilReadLine (fpIn, sz, ";", 2048) != (UINT)-1)
      if (LoadLine ())
         {
         XlateBuffer (pszHEADER);
         XlateBuffer (pszBODY);
         XlateBuffer (pszFOOTER);
         }
   fclose (fpOut);
   }


/*
 * process data from a filespec to a single output file:
 *
 *  header data
 *  body data, one per matching input file
 *  footer data
 */
void ProcessSingleFilespec ();
   {
   pfoList = DirFindAll ();
   fpOut = OpenOutFile ();
   XlateBuffer (pszHEADER);
   for (pfo = pfoList; pfo; pfo = pfo->next)
      {
      MakeFileName ()...
      fpIn = fopen ();
      XlateFile (fpOut, fpIn);
      fclose (fpIn);
      }
   XlateBuffer (pszFOOTER);
   fclose (fpOut);
   }


/*
 * process data from a filespec to a multiple output files:
 *
 *  header data
 *  body data
 *  footer data
 *
 * one file per matching input file
 */
void ProcessFilespec ()
   {
   pfoList = DirFindAll ();
   for (pfo = pfoList; pfo; pfo = pfo->next)
      {
      fpOut = OpenOutFile ();
      XlateBuffer (pszHEADER);
      MakeFileName ()...
      fpIn = fopen ();
      XlateFile (fpOut, fpIn);
      fclose (fpIn);
      XlateBuffer (pszFOOTER);
      }
   fclose (fpOut);
   }

/***************************************************************************/

int main (int argc, char *argv[])
   {
   if (ArgBuildBlk ("? *^Help *^Cfg% *^CSV% *^OutFile% *^Single *^Append"))
      Error ("%s", ArgGetErr ());

   if (ArgFillBlk (argv))
      Error ("%s", ArgGetErr ());

   if (ArgIs ("?") || ArgIs ("Help"))
      Usage ();

   if (bCSVDRIVEN = ArgIs ("CSV"))
      pszCSV = ArgGet ("CSV", 0);

   if (!bCSVDRIVEN && !ArgIs (NULL))
      Usage ();

   FindCfg (pszCFG);
   LoadCFG (pszCfg);

   if      (bCSVDRIVEN && bSINGLE)
      ProcessSingleCSV ();
   else if (bCSVDRIVEN && !bSINGLE)
      ProcessCSV ();
   else if (!bCSVDRIVEN && bSINGLE)
      ProcessSingleFilespec ();
   else if (!bCSVDRIVEN && !bSINGLE)
      ProcessFile ();

   printf ("Done.");
   return 0;
   }

