# Tcl script generated by PlanAhead

set reloadAllCoreGenRepositories false

set tclUtilsPath "c:/Xilinx/14.2/ISE_DS/PlanAhead/scripts/pa_cg_utils.tcl"

set repoPaths ""

set cgProjectPath "z:/PiScope/PiScopeCore/PiScopeCore.srcs/sources_1/ip/fifo_generator_v9_2_0/coregen.cgc"

set ipFile "z:/PiScope/PiScopeCore/PiScopeCore.srcs/sources_1/ip/fifo_generator_v9_2_0/fifo_generator_v9_2_0.xco"

set ipName "fifo_generator_v9_2_0"

set hdlType "VHDL"

set cgPartSpec "xc3s400-4tq144"

set chains "BATCH_CUSTOMIZE_CURRENT_CHAIN IMPLEMENTATION_FILES_CHAIN "

set params ""

set bomFilePath "z:/PiScope/PiScopeCore/PiScopeCore.srcs/sources_1/ip/fifo_generator_v9_2_0/pa_cg_bom.xml"

# generate the IP
set result [source "c:/Xilinx/14.2/ISE_DS/PlanAhead/scripts/pa_cg_gen_out_prods.tcl"]

exit $result

