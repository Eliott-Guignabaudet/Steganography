#pragma once
#include "ASteganoAlgo.h"
class SteganoAlgoLSB :
    public ASteganoAlgo
{
public:
    void HideMessage() override;
    std::string FindMessage() override;
};

