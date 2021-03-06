cmake_minimum_required (VERSION 2.6)

message("* ui-agar")


set(UI_AGAR_STDSRC 
                       agar_cfg.cpp
		       agar_cmd.cpp
		       agar_draw.cpp
		       agar_guicore.cpp
		       agar_osd.cpp
		       agar_osd_led.cpp
		       agar_osd_stat.cpp
		       agar_osd_tape.cpp
		       agar_osd_vfd.cpp
		       agar_sch.cpp
		       agar_sdl.cpp
		       agar_sdlscaler.cpp
		       agar_sdlview.c
		       agar_svram.cpp
		       agar_vramutil.cpp
		       agar_main.cpp
		       agar_logger.cpp
		       AgarKbdInterface.cpp
)

set(UI_AGAR_GUISRC

                       agar_gui_menu.cpp
		       agar_gui_pref.cpp
		       agar_gui_pref_disp.cpp
		       agar_gui_pref_cl.cpp
		       agar_gui_sound.cpp
		       agar_gui_input.cpp
		       agar_menu_about.cpp
		       agar_menu_debug.cpp
		       agar_menu_disk.cpp
		       agar_menu_tape.cpp
		       agar_menu_tool.cpp
		       agar_menu_help.cpp
		       agar_ui_vkey.cpp
		       agar_toolbox.cpp
		       agar_mouse.cpp
)

if(WITH_DEBUGGER)
   set(UI_AGAR_DEBUGSRC
                	agar_surfaceconsole.cpp
			agar_dbg_core.cpp
			agar_dbg_disasm.cpp
			agar_dbg_fdcdump.cpp
			agar_dbg_memdump.cpp
			agar_dbg_mmr.cpp
			agar_dbg_regdump.cpp
			)
endif()

if(USE_OPENCL)
#  set(UI_AGAR_CLSRC_RENDER )
  function(convert_cl2cpp cl_arg)
     add_custom_command(
       OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${cl_arg}.cpp 
       COMMAND gawk -f ${CMAKE_CURRENT_SOURCE_DIR}/cl2cpp.awk -v VARNAME=${cl_arg} ${CMAKE_CURRENT_SOURCE_DIR}/${cl_arg}.cl > ${CMAKE_CURRENT_BINARY_DIR}/${cl_arg}.cpp
       DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${cl_arg}.cl ${CMAKE_CURRENT_SOURCE_DIR}/cl2cpp.awk
     )
  endfunction(convert_cl2cpp)
  foreach(clname cl_render_8 cl_render_4096 cl_render_256k cl_render_copyvram cl_render_tables)
       convert_cl2cpp(${clname})
       set(UI_AGAR_CLSRC_RENDER  ${UI_AGAR_CLSRC_RENDER}
	    ${CMAKE_CURRENT_BINARY_DIR}/${clname}.cpp
           )
  endforeach(clname)
   set(UI_AGAR_CLSRC ${UI_AGAR_CLSRC_RENDER}
      agar_glcl.cpp
      )
 # Remove generated c/cpp file on "make clean".
 set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${UI_AGAR_CLSRC_RENDER})

endif()

if(USE_OPENGL)
  set(UI_AGAR_GLSRC 
                         agar_gldraw2.cpp
			 agar_glutil.cpp
			 )
endif()

add_library(xm7_ui_agar 
		       ${UI_AGAR_STDSRC}
		       ${UI_AGAR_GUISRC}
		       ${UI_AGAR_GLSRC}
		       ${UI_AGAR_CLSRC}
		       ${UI_AGAR_DEBUGSRC}
)

