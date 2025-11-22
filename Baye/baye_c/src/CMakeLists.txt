cmake_minimum_required(VERSION 3.5)

project(baye)

set (bayelib_SRCS
FgtCount.c
FgtPkAi.c
Fight.c
FightSub.c
PublicFun.c
Size.c
citycmd.c
citycmdb.c
citycmdc.c
citycmdd.c
citycmde.c
cityedit.c
comIn.c
comOut.c
datman.c
debug.c
gamEng.c
infdeal.c
itoa.c
paccount.c
pconst.c
pstring.c
showface.c
strcnt.c
tactic.c
varma.c
data-bind.c
bind-objects.c
touch.c
config.c
miniz.c
speed.c
platform/common/bios.c
platform/common/gui.c
platform/common/sys.c
)

set (bayelib_js_SRCS
platform/js/fsys.c
platform/js/sem.c
platform/js/timer.c
platform/js/script.c
platform/js/sys-js.c
)

set (bayelib_win_SRCS
platform/win/fsys.c
platform/win/game.c
platform/win/script.c
platform/win/sem.c
platform/win/sys.c
platform/win/timer.c
)

add_library(bayeliba ${bayelib_SRCS})

if (${CMAKE_SYSTEM_NAME} STREQUAL Emscripten)
	target_sources(bayeliba PRIVATE ${bayelib_js_SRCS})
	add_executable(baye.v2
		platform/js/main.c
		version.c
	)
	target_link_libraries(baye.v2 bayeliba)
	target_include_directories(baye.v2 PRIVATE . ./platform/common)
	set_target_properties(baye.v2 PROPERTIES OUTPUT_NAME "baye.v2")
endif()

if (${CMAKE_SYSTEM_NAME} STREQUAL Windows)
	target_sources(bayeliba PRIVATE ${bayelib_win_SRCS})
	add_executable(baye
		platform/win/window.c
		version.c
	)
	find_package (Threads)
	target_link_libraries(baye bayeliba)
	target_link_libraries(baye ${CMAKE_THREAD_LIBS_INIT})
	SET_TARGET_PROPERTIES(baye PROPERTIES LINK_FLAGS "-pthread")
	target_include_directories(baye PRIVATE . ./platform/common)
endif()

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/baye/version.h
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/genver.py > ${CMAKE_CURRENT_SOURCE_DIR}/baye/version.h
)

add_custom_target(ver_header ALL DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/baye/version.h)
add_dependencies(bayeliba ver_header)
#target_link_libraries(baye bayeliba msgpackc-static)

target_include_directories(bayeliba PRIVATE . ./platform/common)


#target_include_directories(bayeliba PRIVATE ${msgpack_SOURCE_DIR}/include)
if (${CMAKE_SYSTEM_NAME} STREQUAL Emscripten)
    # iOS 14 兼容模式 - 增加内存并保留异步支持
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASSERTIONS=0")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s 'EXPORTED_RUNTIME_METHODS=[\"stringToUTF8\", \"getValue\", \"setValue\"]'")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s 'EXPORTED_FUNCTIONS=[\"_main\", \"_bayeSetLcdSize\", \"_bayeGameEnvInit\", \"_bayeLoadPeriod\", \"_bayeSendKey\", \"_bayeSendTouchEvent\", \"_bayeSetDebug\", \"_bayeGetSpeed\", \"_bayeSetSpeed\", \"_bayeAlloc\", \"_free\", \"_malloc\", \"_SetAgricultureMultiplier\", \"_SetCommerceMultiplier\"]'")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=0")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s INITIAL_MEMORY=64MB")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s STACK_SIZE=2MB")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s WASM=1")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s LEGACY_VM_SUPPORT=1")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s STANDALONE_WASM=0")
    # 添加必要的ASYNCIFY支持
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASYNCIFY=1")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s 'ASYNCIFY_IMPORTS=[\"emscripten_sleep\", \"gam_sem_wait\"]'")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O1")
    #set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -gsource-map")
    #set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --source-map-base http://127.0.0.1:8008/js/")
    #set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif (${CMAKE_SYSTEM_NAME} STREQUAL Emscripten)

