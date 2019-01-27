#include <stdlib.h>
#include <iostream>
#include <assert.h>

#include "Quest.h"
#include <DS/DSStream.h>

#ifdef DEBUG
#include <dmalloc.h>
#endif

#ifdef macintosh
  #include <console.h>
#endif

/****************************************************************************/
/*                        H A U P T F U N K T I O N                         */
/*                            des QUEST-Projekts                            */
/****************************************************************************/

int main (int argc, char *argv[])
{
  int paras;
  DSStream questOutput(std::cout);

  Quest *q_object;

  Quest::Initialize(questOutput);
  Quest::PrintMessage();

  #ifdef macintosh
    argc = ccommand (&argv);
  #endif
  if (Quest::ParseOptions(&argc, argv) != DS_OK)
  {
    Quest::PrintHelp();
    exit(1);
  }

  for (paras = 1;                             /* alle Parameter durch-    */
       paras < argc;                          /* laufen                   */
       paras++)
  {
    q_object = new Quest(argv[paras]);
    assert(q_object);

    q_object->Run();

    delete q_object;
  }
  return 0;
}
