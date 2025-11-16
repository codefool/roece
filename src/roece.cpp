#include "roece.h"

const char *PieceName[] = {
    "PT_NONE",
    "PT_KING",
    "PT_QUEEN",
    "PT_BISHOP",
    "PT_KNIGHT",
    "PT_ROOK",
    "PT_PAWN",
};

const Offset offs[] = {
	{+1,+0},  // UP
	{-1,+0},  // DN
	{+0,-1},  // LFT
	{+0,+1},  // RGT
	{+1,+1},  // UPR
	{+1,-1},  // UPL
	{-1,+1},  // DNR
	{-1,-1},  // DNL
    // knight offsets
    {+1,-2},  // NLUP
    {+2,-1},  // NUPL
    {+2,+1},  // NUPR       
    {+1,+2},  // NRUP
    {-1,+2},  // NRDN
    {-2,+1},  // NDNR
    {-2,-1},  // NDNL
    {-1,-2}   // NKLDN
};

const DirList none_dirs;
const DirList axes_dirs   = { UP,   DN,   LFT,  RGT };
const DirList diag_dirs   = { UPR,  UPL,  DNR,  DNL };
const DirList omni_dirs   = { UP,   DN,   LFT,  RGT,  UPR,  UPL,  DNR,  DNL };
const DirList knight_dirs = { NLUP, NUPL, NUPR, NRUP, NRDN, NDNR, NDNL, NLDN };
const DirList pawn_black  = { DNL, DNR };
const DirList pawn_white  = { UPL, UPR };

