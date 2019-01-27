#ifndef _QUEST_H_
#define _QUEST_H_

#define QUEST_VERSION "1.2"

/****************************************************************************
 * Include-Anweisungen
 ****************************************************************************/

#include <QP/QPBasicTypes.h>  // Parser
#include <CG/CGBasicTypes.h>  // Codegenerator
#include <DS/DSBasicTypes.h>  // Depository

/******************************************************************************
 * Klasse Quest
 * Zweck: Hauptklasse des kompletten QUEST-Projekts. Steuert alle anderen
 *        Toolkomponenten die als Form von Bibliotheken hinzugebunden werden.
 ******************************************************************************/

class Quest
{
  public:
                        Quest(class DSString * file_name);
                        Quest(const char * file_name);
                       ~Quest(void);

    static void         Initialize(class DSStream &myCout)
                        {
                          questCout = &myCout;
												}

    static DSResult     ParseOptions(int *, char **);
    static DSResult     PrintHelp(void);
    static DSResult     PrintMessage(void);
           DSResult     Run(void);

           QPResult     Parser(void);
           DSResult     Depository(void);
           DSResult     Generator(void);

    DSBoolean           GenerateCode(void) const { return generate_code; }
    void                GenerateCode(DSBoolean do_gen) { generate_code = do_gen; }
    DSBoolean           Verbose(void) const { return generate_code; }
    void                Verbose(DSBoolean is_verb) { verbose = is_verb; }
#ifndef _PRODUCTION_RELEASE
    DSBoolean           PrettyPrinting(void) const { return pretty_printing; }
    void                PrettyPrinting(DSBoolean pretty) { pretty_printing = pretty; }
#endif
    DSBoolean           Warnings(void) const { return warnings; }
    void                Warnings(DSBoolean do_warn) { warnings = do_warn; }
    DSBoolean           CloseOpenSystem(void) const { return close_open_system; }
    void                CloseOpenSystem(DSBoolean do_gen) { close_open_system = do_gen; }
    DSBoolean           LinearTransitions(void) const { return linear_transitions; }
    void                LinearTransitions(DSBoolean lin_trans) { linear_transitions = lin_trans; }
    DSBoolean           FullLinearTransitions(void) const { return full_linear_transitions; }
    void                FullLinearTransitions(DSBoolean full_lin_trans) { full_linear_transitions = full_lin_trans; }
    DSBoolean           GenerateMakefile(void) const { return generate_makefile; }
    void                GenerateMakefile(DSBoolean gen_make) { generate_makefile = gen_make; }
    DSBoolean           GenerateExperimentfile(void) const { return generate_experimentfile; }
    void                GenerateExperimentfile(DSBoolean experi) { generate_experimentfile = experi; }
    DSBoolean           GenerateTemplateInit(void) const { return generate_template_init; }
    void                GenerateTemplateInit(DSBoolean temp_init) { generate_template_init = temp_init; }
    DSBoolean           GenerateInformalText(void) const { return generate_informal_text; }
    void                GenerateInformalText(DSBoolean inf_text) { generate_informal_text = inf_text; }
    DSBoolean           IncrementalCodeGeneration(void) const { return incremental_code_generation; }
    void                IncrementalCodeGeneration(DSBoolean inc_code) { incremental_code_generation = inc_code; }
#ifndef _PRODUCTION_RELEASE
    DSBoolean           GenerateDebug(void) const { return generate_debug; }
    void                GenerateDebug(DSBoolean do_debug) { generate_debug = do_debug; }
#endif
    DSBoolean           SupportPEV(void) const { return support_pev; }
    void                SupportPEV(DSBoolean sup_pev) { support_pev = sup_pev; }
    class DSSystem *    GetSystem(void) const { return system; }
    class DSString *    GetFilename(void) const { return filename; }
    class CGWriter *    GetWriter(void) const { return writer; }

  private:

    // Zum Speichern der Laufzeitparameter:
    static DSBoolean    generate_code;
    static DSBoolean    verbose;
    static DSBoolean    pretty_printing;
    static DSBoolean    warnings;
    static DSBoolean    close_open_system;
    static DSBoolean    linear_transitions;
    static DSBoolean    full_linear_transitions;
    static DSBoolean    support_pev;
    static DSBoolean    generate_makefile;
    static DSBoolean    generate_experimentfile;
    static DSBoolean    generate_template_init;
    static DSBoolean    generate_informal_text;
    static DSBoolean    incremental_code_generation;
    static DSBoolean    generate_debug;

    class  DSSystem    *system;
    class  DSString    *filename;
    class  CGWriter    *writer;

    static DSStream    *questCout;
};

#endif
