#include "CommandParameters.h"

FWinCommandParameters::FWinCommandParameters(HINSTANCE InInstance, HINSTANCE InPrevInstance, PSTR InCmdLine,
                                             int InCmdShow)
    : Instance(InInstance)
      , PrevInstance(InPrevInstance)
      , CmdLine(InCmdLine)
      , CmdShow(InCmdShow)
{
}
