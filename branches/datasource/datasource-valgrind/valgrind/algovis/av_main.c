
/*--------------------------------------------------------------------*/
/*--- Nulgrind: The minimal Valgrind tool.               nl_main.c ---*/
/*--------------------------------------------------------------------*/

/*
   This file is part of Nulgrind, the minimal Valgrind tool,
   which does no instrumentation or analysis.

   Copyright (C) 2002-2009 Nicholas Nethercote
      njn@valgrind.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA. 

   The GNU General Public License is contained in the file COPYING.
*/

#include "pub_tool_basics.h"
#include "pub_tool_tooliface.h"
#include "pub_tool_libcbase.h"
#include "pub_tool_libcprint.h"
#include "pub_tool_mallocfree.h"
#include "pub_tool_debuginfo.h"
#include "pub_tool_machine.h"      // VG_(fnptr_to_fnentry)

#include "av_vis.h"

static void av_post_clo_init(void)
{
}


static void add_call(IRSB* sb, IRExpr* dAddr, Int dSize, Char* helperName, void* helperAddr)
{
   // Create argument vector with two IRExpr* arguments.
   IRExpr** argv = mkIRExprVec_2(dAddr, mkIRExpr_HWord(dSize));
   // Create call statement to function at "helperAddr".
   IRDirty* di = unsafeIRDirty_0_N( /*regparms*/2, helperName, VG_(fnptr_to_fnentry)(helperAddr), argv);
   addStmtToIRSB(sb, IRStmt_Dirty(di));
}

int loadCount = 0;
int bytesLoaded = 0;

static VG_REGPARM(2) void trace_load(Addr addr, SizeT size)
{
   ++loadCount;
   bytesLoaded += size;
}

static void handle_load(IRSB* sb, IRExpr* dAddr, Int dSize) {
   add_call(sb, dAddr, dSize, "trace_load", trace_load);
}

char* funcNames[256];
int namesUsed = 0;

static VG_REGPARM(3) void trace_func(void (*drawFunc)(void*), Int nameId, void* thisPtr, Int* value)
{
   //Int data = *valAddress;
   VG_(printf)("Function: %s this: %p value: %d, drawFunc: %p\n", funcNames[nameId], thisPtr, *value, drawFunc);
   drawFunc(thisPtr);
   //Char name[100];
   //PtrdiffT offset;
   //VG_(get_datasym_and_offset)(thisPtr, name, 100, &offset);
}

static void typeNameFromFuncName(Char* funcName, Char* typeName)
{
   // TODO: Consider templated types, though they will cause issues elsewhere aswell.
   Char* start = VG_(strchr)(funcName, '<')+1;
   Char* end = VG_(strchr)(start, ',');
   VG_(strncpy)(typeName, start, end - start);
}

static void handle_func(IRSB* sb, Char* name) {
   Int nameIndex = namesUsed++;
   funcNames[nameIndex] = VG_(strdup)("av.main.hp.1", name);
   
   char typeName[256];
   typeNameFromFuncName(funcNames[nameIndex], typeName);
   int typeId = getTypeId(typeName);
   if (typeId < 0) // Type not found
	  return; // Don't bother to insrument, we don't know what to do with the data
   drawFuncType drawFunc = getDrawFunc(typeId);
   
   IRTemp esp = newIRTemp(sb->tyenv, Ity_I32);
   IRStmt* getEsp = IRStmt_WrTmp(esp, IRExpr_Get(16, Ity_I32));
   addStmtToIRSB(sb, getEsp);
   
   IRTemp thisOffset = newIRTemp(sb->tyenv, Ity_I32);
   IRStmt* calcThisOffset = IRStmt_WrTmp(thisOffset, IRExpr_Binop(Iop_Add32, IRExpr_RdTmp(esp), mkIRExpr_HWord(4)));
   addStmtToIRSB(sb, calcThisOffset);
   
   IRTemp thisAddr = newIRTemp(sb->tyenv, Ity_I32);
   IRStmt* getThisAddr = IRStmt_WrTmp(thisAddr, IRExpr_Load(Iend_LE, Ity_I32, IRExpr_RdTmp(thisOffset)));
   addStmtToIRSB(sb, getThisAddr);
   
   IRTemp valueOffset = newIRTemp(sb->tyenv, Ity_I32);
   IRStmt* calcValueOffset = IRStmt_WrTmp(valueOffset, IRExpr_Binop(Iop_Add32, IRExpr_RdTmp(esp), mkIRExpr_HWord(8)));
   addStmtToIRSB(sb, calcValueOffset);
   
   IRTemp valueAddr = newIRTemp(sb->tyenv, Ity_I32);
   IRStmt* getValueAddr = IRStmt_WrTmp(valueAddr, IRExpr_Load(Iend_LE, Ity_I32, IRExpr_RdTmp(valueOffset)));
   addStmtToIRSB(sb, getValueAddr);
   
   IRExpr** argv = mkIRExprVec_4(mkIRExpr_HWord(drawFunc), mkIRExpr_HWord(nameIndex), IRExpr_RdTmp(thisAddr), IRExpr_RdTmp(valueAddr));
   IRDirty* di = unsafeIRDirty_0_N( /*regparms*/3, "trace_func", VG_(fnptr_to_fnentry)(trace_func), argv);
   addStmtToIRSB(sb, IRStmt_Dirty(di));
}

static
IRSB* av_instrument ( VgCallbackClosure* closure,
                      IRSB* sbIn,
                      VexGuestLayout* layout, 
                      VexGuestExtents* vge,
                      IRType gWordTy, IRType hWordTy )
{
   Int i;
   
   IRSB* sbOut = deepCopyIRSBExceptStmts(sbIn);
   
   Bool printing = 0;
   
   for (i = 0; i < sbIn->stmts_used; i++)
   {
	  IRStmt* st = sbIn->stmts[i];
	  if (!st)
		 continue;
	  
	  if (printing)
	  {
		 ppIRStmt(st);
		 VG_(printf)("\n");
	  }
	  
	  switch (st->tag)
	  {
		 case Ist_IMark:
		 {
            Addr64 instr_addr = st->Ist.IMark.addr;
            //isize = st->Ist.IMark.len;
			Char func[256];
			Bool found_fn = VG_(get_fnname_if_entry)(instr_addr, func, 256);
			if (found_fn)
			{
			   if (VG_(strstr)(func, "::push_back(") != NULL || VG_(strstr)(func, "::push_front(") != NULL)
			   {
				  handle_func(sbOut, func);
				  //printing = 1; // no printing
			   }
			}
			break;
		 }
		 case Ist_WrTmp: // Loads only occur in temp assignments as the expression tree is flattened
		 {
			IRExpr* data = st->Ist.WrTmp.data;
			if (data->tag == Iex_Load)
			{
			   handle_load(sbOut, data->Iex.Load.addr, sizeofIRType(data->Iex.Load.ty));
			}
			break;
		 }
	  }
	 
	  addStmtToIRSB(sbOut, st);
   }
   return sbOut;
}

static void av_fini(Int exitcode)
{
   VG_(printf)("memory reads: %d, total bytes read %d\n", loadCount, bytesLoaded);
}

static void av_pre_clo_init(void)
{
   VG_(details_name)            ("Algovis");
   VG_(details_version)         (NULL);
   VG_(details_description)     ("algorithm visualisation valgrind tool");
   VG_(details_copyright_author)(
      "Copyright (C) 2010, by David Olsen and Nathan Pitman.");
   VG_(details_bug_reports_to)  (VG_BUGS_TO);

   VG_(basic_tool_funcs)        (av_post_clo_init,
                                 av_instrument,
                                 av_fini);

   /* No needs, no core events to track */
   
   // Register drawing functions for known types
   registerDrawFuncs();
}

VG_DETERMINE_INTERFACE_VERSION(av_pre_clo_init)

/*--------------------------------------------------------------------*/
/*--- end                                                          ---*/
/*--------------------------------------------------------------------*/
