#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#define MAX_MSGLEN     500
#define MSGFILE        "IRM.MSG"

#define MSG_PRODUCT         1
#define MSG_COPYRIGHT       2
#define MSG_LICENSE         3
#define MSG_QUIT            4
#define MSG_USAGE           5
#define MSG_PARAMETERS      6
#define MSG_COMPORT         7
#define MSG_COMDETECT       8
#define MSG_COMRESPONSE     9
#define MSG_IRMANFOUND     10
#define MSG_COMPRESENTUSE  11
#define MSG_NORMALEND      12
#define MSG_ERROREND       13
#define MSG_UNHANDLEDEVENT 14
#define MSG_PROGRAMSTARTED 15
#define MSG_INSTANCE       21
#define MSG_SEMERROR       22
#define MSG_REXXERROR      23
#define MSG_XCPTERROR      24
#define MSG_THRDERROR      25
#define MSG_STARTERROR     26
#define MSG_COMSETUP       27
#define MSG_COMBAUD        28
#define MSG_COMLINECTRL    29
#define MSG_COMFLUSH       30
#define MSG_COMREAD        31
#define MSG_COMRELEASE     32
#define MSG_COMDCB         33
#define MSG_ERRORTIMER     34

void PrintMessage(char **Table, int count, int msgno);

#endif