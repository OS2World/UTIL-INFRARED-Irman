#ifndef REXXFUNC_HPP
#define REXXFUNC_HPP

#define RX_MAX_PARAMS 4
#define RX_MIN_PARAMS 1
#define RX_ARGS 1
#define RX_STARTPROGRAM "IRSTARTPROGRAM"
#define RX_SCRIPT "irman.rx"
#define RX_PROGRAMNAME 0
#define RX_PARAMETERS 1
#define RX_ENVIRONMENT 2
#define RX_OPTIONS 3

int RegisterFunctions();
int DeRegisterFunctions();
void _Optlink EventHandler(void *XString);
ULONG APIENTRY IrStartProgram(PSZ Name,
                            LONG Argc,
                            RXSTRING Argv[],
                            PSZ Queuename,
                            PRXSTRING Retstr);

#endif