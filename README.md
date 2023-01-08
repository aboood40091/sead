# sead
 Decompilation of the engine and libraries developed by Nintendo EAD (now EPD) present in NSMBU and other games.  

## Engine (`engine/`)
 The main part of `sead`. Nintendo EAD's standard library (framework).  

## Packages

### nw_ptcl (`packages/nw_ptcl/`)
 Subset of `sead`, serving as a wrapper for the [NintendoWareForCafe Eft](https://github.com/nw4f/Eft) library.  

### agl (`packages/agl/`)
 Nintendo EAD's main graphics library.  

## Goal
 The original goal was to get a 1:1 decompilation of all sead (engine only) classes/methods present in NSMBU. However, that is sadly impossible for many reasons including no access to the compiler version used in NSMBU.  
 Later on, the goal was changed to include many of Nintendo EAD's other libraries.  
 While a 1:1 decompilation is impossible, the current goal is to create code with 1:1 functionality and no less efficiency than the original code.  
