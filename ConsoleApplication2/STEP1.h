#pragma once
#include"writetoken.h"
#include"total.h"
#include"axil.h"
#include"analyse.h"

void STEP1()
{
    FILE* St_Code = fopen(SOURCE, "r");
    Lx_An(St_Code);
    writetoken();
}