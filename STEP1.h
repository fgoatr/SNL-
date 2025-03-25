#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include"total.h"
#include"writetoken.h"
#include"axil.h"
#include"analyse.h"

void STEP1()
{
    FILE* St_Code = fopen(SOURCE, "r");
    Lx_An(St_Code);
    writetoken();
}