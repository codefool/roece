#include "roece.h"

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
    {+1,-2},  // KLUP
    {+2,-1},  // KUPL
    {+2,+1},  // KUPR       
    {+1,+2},  // KRUP
    {-1,+2},  // KRDN
    {-2,+1},  // KDNR
    {-2,-1},  // KDNL
    {-1,-2}   // KLDN
};

const DirList none_dirs;
const DirList axes_dirs   = { UP,   DN,   LFT,  RGT };
const DirList diag_dirs   = { UPR,  UPL,  DNR,  DNL };
const DirList omni_dirs   = { UP,   DN,   LFT,  RGT,  UPR,  UPL,  DNR,  DNL };
const DirList knight_dirs = { KLUP, KUPL, KUPR, KRUP, KRDN, KDNR, KDNL, KLDN };
const DirList pawn_black  = { DNL, DNR };
const DirList pawn_white  = { UPL, UPR };

