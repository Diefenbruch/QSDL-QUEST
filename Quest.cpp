/****************************************************************************
 *                 Modul: $RCSfile: Quest.cpp,v $
 *
 * $Author: hirche $
 * $Date: 1998/02/17 17:29:20 $
 * $Revision: 1.1 $
 *
 * Aufgabe: Hauptmodul des kompletten QUEST-Projekts.
 *
 ****************************************************************************/

#ifdef USE_PRAGMA
#pragma interface
#endif

/****************************************************************************
 * Konstanten
 ****************************************************************************/

/****************************************************************************
 * Include-Anweisungen
 ****************************************************************************/

#include <stdio.h>
#include <string.h> // Suns CC needs this

#include "Quest.h"

#include <CG/CGWriter.h>
#include <QP/QPMain.h>
#include <DS/DSSystem.h>
#include <DS/DSString.h>
#include <DS/DSStream.h>

#ifdef DEBUG
#include <dmalloc.h>
#endif

DSStream * Quest::questCout;

/****************************************************************************
 * Initialisierung statischer Komponenten
 ****************************************************************************/

DSBoolean    Quest::generate_code = DS_FALSE;
DSBoolean    Quest::verbose = DS_FALSE;
DSBoolean    Quest::warnings = DS_FALSE;
DSBoolean    Quest::close_open_system = DS_FALSE;
DSBoolean    Quest::linear_transitions = DS_FALSE;
DSBoolean    Quest::full_linear_transitions = DS_FALSE;
DSBoolean    Quest::support_pev = DS_FALSE;
DSBoolean    Quest::generate_makefile = DS_FALSE;
DSBoolean    Quest::generate_experimentfile = DS_FALSE;
DSBoolean    Quest::generate_template_init = DS_FALSE;
DSBoolean    Quest::generate_informal_text = DS_FALSE;
DSBoolean    Quest::incremental_code_generation = DS_FALSE;
#ifndef _PRODUCTION_RELEASE
DSBoolean    Quest::generate_debug = DS_FALSE;
#endif
DSBoolean    Quest::pretty_printing = DS_FALSE;

/****************************************************************************
 * Konstruktoren
 ****************************************************************************/

Quest::Quest(DSString *file)
{
  assert(file);
  assert(questCout);

  system = NULL;
  filename = file;

  writer = NULL;
}

Quest::Quest(const char *file)
{
  assert(file);
  assert(questCout);

  system = NULL;
  filename = new DSString(file);
  assert(filename);

  writer = NULL;
}

/****************************************************************************
 * Destruktor
 ****************************************************************************/

Quest::~Quest(void)
{
  if (system) delete system;
  if (filename) delete filename;
  if (writer) delete writer;
}

DSResult Quest::Run(void)
{
  QPResult qp_result;
  DSResult ds_result;
  DSResult cg_result;

  qp_result =  Parser();
  if (qp_result != QP_OK) return DS_ERROR;

  ds_result =  Depository();
  if (ds_result != DS_OK) return DS_ERROR;

  cg_result =  Generator();
  if (cg_result != DS_OK) return DS_ERROR;

  return DS_OK;
}

/****************************************************************************
 * PrintMessage: Gibt eine Copyrightmeldung aus
 ****************************************************************************/

DSResult Quest::PrintMessage(void)
{
  char str[80];

  *questCout << "*************************************************" << std::endl;
  sprintf(str, "* QUEST - Queueing SDL Tool v%s                *",
          QUEST_VERSION);
  *questCout << str << std::endl;
  *questCout << "*                                               *" << std::endl;
  sprintf(str, "* Parser    : %s                             *",
          QP_VERSION);
  *questCout << str << std::endl;
  sprintf(str, "*             %s      *",
          QP_DATE);
  *questCout << str << std::endl;
  sprintf(str, "* Depository: %s                             *",
          DS_VERSION);
  *questCout << str << std::endl;
  sprintf(str, "*             %s      *",
          DS_DATE);
  *questCout << str << std::endl;
  sprintf(str, "* Generator : %s                             *",
          CG_VERSION);
  *questCout << str << std::endl;
  sprintf(str, "*             %s      *",
          CG_DATE);
  *questCout << str << std::endl;
  *questCout << "*                                               *" << std::endl;
  *questCout << "* Authors: Marc Diefenbruch,                    *" << std::endl;
  *questCout << "*          Axel Hirche,                         *" << std::endl;
  *questCout << "*          Wolfgang Textor                      *" << std::endl;
  *questCout << "*                                               *" << std::endl;
  *questCout << "* Copyright (C) 1995-1998 University of Essen   *" << std::endl;
  *questCout << "*                                               *" << std::endl;
  *questCout << "* All rights reserved                           *" << std::endl;
  *questCout << "*                                               *" << std::endl;
  *questCout << "*************************************************" << std::endl << std::endl;

  return DS_OK;
}

/****************************************************************************
 * PrintHelp: Gibt eine Hilfemeldung aus
 ****************************************************************************/

DSResult Quest::PrintHelp(void)
{
  *questCout << "Available Options:" << std::endl;
  *questCout << "  -h        : Display this help." << std::endl;
  *questCout << "  -v        : Verbose message output." << std::endl;
  *questCout << "  -w        : Parser displays warnings." << std::endl;
  *questCout << "  -c        : Close open SDL systems, i.e. generate SDL environment." << std::endl;
  *questCout << "  -l        : Linearize transitions of nondeterministic decisions." << std::endl;
  *questCout << "  -L        : Full linearization, i.e. dissolve all decisions." << std::endl;
  *questCout << "  -g        : Generate evaluation code." << std::endl;
  *questCout << "  -i        : Generate template file (only with -g)." << std::endl;
  *questCout << "  -m        : Generate Makefile (only with -g)." << std::endl;
  *questCout << "  -n        : Incremental code generation (only with -g)." << std::endl;
  *questCout << "  -p        : Support PEV (only with -g)." << std::endl;
  *questCout << "  -e        : Generate default experiment description (only with -g and -p)." << std::endl;
  *questCout << "  -x        : Treat informal text as C++ (only with -g)." << std::endl;
#ifndef _PRODUCTION_RELEASE
  *questCout << "  -d        : Generate debugging code (only with -g)." << std::endl;
#endif
  *questCout << "  -y        : Pretty printing of generated code (only with -g)." << std::endl;
  *questCout << std::endl;
  *questCout << "Options without additional information can be aggregated:" << std::endl;
  *questCout << "  '-vgmi' is exactly the same as '-v -g -m -i'." << std::endl << std::endl;
  *questCout << "Examples:" << std::endl;
  *questCout << "  quest -vgmi example1.pr" << std::endl;
#ifndef _PRODUCTION_RELEASE
  *questCout << "  quest -vwgmiylpen example2.pr" << std::endl;
#else
  *questCout << "  quest -vwgmiylpen example2.pr" << std::endl;
#endif

  return DS_OK;
}

DSResult Quest::ParseOptions(int *argc, char **argv)
{
  unsigned int ci, errs = 0;
  int ai;
  int new_argc = 1;

  for (ai = 1;
       ai < *argc;
       ai++)
  {
    if (argv[ai][0] == '-')
    {
      for (ci = 1;
           ci < strlen(argv[ai]);
           ci++)
      {
        switch(argv[ai][ci])
        {  
          case 'h':
            PrintHelp();
            exit(0);
            break;

          case 'v':
            verbose = DS_TRUE;
            break;

          case 'w':
            warnings = DS_TRUE;
            break;

          case 'g':
            generate_code = DS_TRUE;
            break;

          case 'x':
            generate_informal_text = DS_TRUE;
            break;

#ifndef _PRODUCTION_RELEASE
          case 'd':
            generate_debug = DS_TRUE;
            break;
#endif

          case 'y':
            pretty_printing = DS_TRUE;
            break;

          case 'c':
            close_open_system = DS_TRUE;
            break;

          case 'l':
            linear_transitions = DS_TRUE;
            break;

          case 'L':
            linear_transitions = DS_TRUE;
            full_linear_transitions = DS_TRUE;
            break;

          case 'i':
            generate_template_init = DS_TRUE;
            break;

          case 'm':
            generate_makefile = DS_TRUE;
            break;

           case 'n':
            incremental_code_generation = DS_TRUE;
            break;

          case 'p':
            support_pev = DS_TRUE;
            break;

          case 'e':
            generate_experimentfile = DS_TRUE;
            break;

          default:
            std::cerr << "Unknown option '" << argv[ai][ci] << "' !" << std::endl << std::endl;
            errs++;
            break;
        }
      }
    }
    else
    {
      argv[new_argc] = argv[ai];
      new_argc++;
    }
  }
  *argc = new_argc;
  if (errs > 0)
  {
    return DS_ERROR;
  }
  return DS_OK;
}

QPResult Quest::Parser(void)
{
  assert(filename);

  return ParseSpecification(&system,
                            filename,
                            *questCout,
                            verbose, warnings);
}

DSResult Quest::Depository(void)
{
  assert(system);

  if (system->IsOpen() && close_open_system)
  {
    if (verbose)
    {
      *questCout << "\nClosing open system..." << std::endl;
    }
    system->Close(*questCout, verbose);
    if (verbose)
    {
      *questCout << "done." << std::endl;
    }
  }

  if (system->HasAsteriskStates(DS_TRUE))
  {
    if (verbose)
    {
      *questCout << "\nDissolving asterisk states... ";
    }
    system->DissolveAsteriskStates();
    if (verbose)
    {
      *questCout << "done." << std::endl;
    }
  }

  system->CheckForComplexProcedures(DS_TRUE);
  
  if (verbose)
  {
    *questCout << "\nDissolving dash nextstates... ";
  }
  system->DissolveNextStateDash();
  if (verbose)
  {
    *questCout << "done." << std::endl;
  }

  if (linear_transitions)
  {
    if (verbose)
    {
      *questCout << "\nDissolving decisions... ";
    }
    system->DissolveDecisions(full_linear_transitions);
    if (verbose)
    {
      *questCout << "done." << std::endl;
    }
  }

  return DS_OK;
}

DSResult Quest::Generator(void)
{
  DSResult result;

  if (!generate_code) return DS_OK;

  if (writer)
  {
    delete writer;
  }
  writer = new CGWriter(system,
                        *questCout,
                        generate_makefile,
                        generate_template_init,
                        generate_experimentfile,
                        verbose,
// #ifndef _PRODUCTION_RELEASE
                        pretty_printing,
//#else
//                        DS_FALSE,
//#endif
                        generate_informal_text,
                        incremental_code_generation,
                        support_pev,
#ifndef _PRODUCTION_RELEASE
                        generate_debug);
#else
                        DS_FALSE);
#endif

  assert(writer);

  if (verbose)
  {
    *questCout << "\nGenerating evaluation code for specification '";
    *questCout << system->GetName() << "'..." << std::endl;
  }

  result = writer->CGWrite();

  if (result == DS_OK)
  {
    if (verbose) *questCout << "Code generation complete." << std::endl << std::endl;;
  }
  else
  {
    *questCout << "Code generation failed." << std::endl << std::endl;;
  }

//  delete writer;

  return result;
}
