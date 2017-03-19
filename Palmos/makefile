# 
# Build Definition File
# Building Definition File sections.def for Multiple Code Sections...
#
sections.def:./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/LogoForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/OverviewForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/RollInitForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/TextForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Main.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Performer.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PlayForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/DelayForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/SelectForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditEffectForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditCharForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/CombatQOGC.o
	@echo "Building Definition File sections.def for Multiple Code Sections..." && \
	fnmkdef -s perfsect miscsect -f sections.def 

# 
# Build Linker Script
# Building Linker Script...
#
sections.s sections.ld:sections.def
	@echo "Building Linker Script..." && \
	m68k-palmos-multigen sections.def -b sections

# 
# Build Assembly Stub
# Building Assembly Stub...
#
sections.o:sections.s sections.ld
	@echo "Building Assembly Stub..." && \
	m68k-palmos-gcc -c sections.s sections.ld

# 
# Link Object Files
# Linking Object Files
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Combat-Q:sections.s sections.ld ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/LogoForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/OverviewForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/RollInitForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/TextForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Main.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Performer.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PlayForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/DelayForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/SelectForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditEffectForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditCharForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/CombatQOGC.o
	@echo "Linking Object Files" && \
	m68k-palmos-gcc  -L /cygdrive/c/Program\ Files/Falch.net/DeveloperStudio\ 2.6/palmdev/sdk-5.0/gcc\ libraries/m68k-palmos-coff   sections.s sections.ld ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/LogoForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/OverviewForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/RollInitForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/TextForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Main.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Performer.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PlayForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/DelayForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/SelectForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditEffectForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditCharForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/CombatQOGC.o -o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Combat-Q  -lPalmOSGlue -lnoexcept

# 
# Build Program Database
# Building Program Database ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Combat-Q.prc
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Combat-Q.prc:./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Combat-Q ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/RollInitForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Main.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/OverviewForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PlayForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/DelayForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/SelectForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditEffectForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditCharForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PickNumberForm.ro
	@echo "Building Program Database ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Combat-Q.prc" && \
	build-prc -c PMcq -t appl -n "Combat-Q" -o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Combat-Q.prc sections.def ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Combat-Q ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/RollInitForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Main.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/OverviewForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PlayForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/DelayForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/SelectForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditEffectForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditCharForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PickNumberForm.ro 

# 
# Build Symbol Information
# Building Symbol Information
#
tags:LogoForm.c OverviewForm.cpp RollInitForm.cpp TextForm.c Main.cpp Performer.cpp PlayForm.cpp DelayForm.cpp SelectForm.cpp EditEffectForm.cpp EditCharForm.cpp CombatQOGC.cpp main.h MainRsc.h Performer.h CombatQOGC.h
	@echo "Building Symbol Information" && \
	ctags --excmd=number  --fields=ikmns  --c-types=+px --file-scope LogoForm.c OverviewForm.cpp RollInitForm.cpp TextForm.c Main.cpp Performer.cpp PlayForm.cpp DelayForm.cpp SelectForm.cpp EditEffectForm.cpp EditCharForm.cpp CombatQOGC.cpp main.h MainRsc.h Performer.h CombatQOGC.h

# 
# All
# Build OK.
#
All:./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Combat-Q.prc tags
	@echo "Build OK." && \

# 
# Clean
# Cleaning up build directory
#
Clean:
	@echo "Cleaning up build directory" && \
	rm -f sections.def sections.s sections.ld sections.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Combat-Q ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Combat-Q.prc tags ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/LogoForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/OverviewForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/RollInitForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/TextForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Main.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Performer.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PlayForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/DelayForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/SelectForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditEffectForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditCharForm.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/CombatQOGC.o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/RollInitForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Main.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/OverviewForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PlayForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/DelayForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/SelectForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditEffectForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditCharForm.ro ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PickNumberForm.ro

# 
# C/C++ Source Files
# Building Source File LogoForm.c
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/LogoForm.o:LogoForm.c
	@echo "Building Source File LogoForm.c" && \
	m68k-palmos-gcc -O2 -palmos5.0 -DTRIAL -fno-rtti -fno-exceptions   -c LogoForm.c -o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/LogoForm.o

# 
# C/C++ Source Files
# Building Source File OverviewForm.cpp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/OverviewForm.o:OverviewForm.cpp
	@echo "Building Source File OverviewForm.cpp" && \
	m68k-palmos-gcc -O2 -palmos5.0 -DTRIAL -fno-rtti -fno-exceptions   -c OverviewForm.cpp -o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/OverviewForm.o

# 
# C/C++ Source Files
# Building Source File RollInitForm.cpp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/RollInitForm.o:RollInitForm.cpp
	@echo "Building Source File RollInitForm.cpp" && \
	m68k-palmos-gcc -O2 -palmos5.0 -DTRIAL -fno-rtti -fno-exceptions   -c RollInitForm.cpp -o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/RollInitForm.o

# 
# C/C++ Source Files
# Building Source File TextForm.c
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/TextForm.o:TextForm.c
	@echo "Building Source File TextForm.c" && \
	m68k-palmos-gcc -O2 -palmos5.0 -DTRIAL -fno-rtti -fno-exceptions   -c TextForm.c -o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/TextForm.o

# 
# C/C++ Source Files
# Building Source File Main.cpp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Main.o:Main.cpp
	@echo "Building Source File Main.cpp" && \
	m68k-palmos-gcc -O2 -palmos5.0 -DTRIAL -fno-rtti -fno-exceptions    -isystem /cygdrive/c/Program\ Files/Falch.net/DeveloperStudio\ 2.6//palmdev/sdk-5.0/include/libraries/palmosglue -c Main.cpp -o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Main.o

# 
# C/C++ Source Files
# Building Source File Performer.cpp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Performer.o:Performer.cpp
	@echo "Building Source File Performer.cpp" && \
	m68k-palmos-gcc -O2 -palmos5.0 -DTRIAL -fno-rtti -fno-exceptions    -isystem /cygdrive/c/Program\ Files/Falch.net/DeveloperStudio\ 2.6//palmdev/sdk-5.0/include/libraries/palmosglue -c Performer.cpp -o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Performer.o

# 
# C/C++ Source Files
# Building Source File PlayForm.cpp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PlayForm.o:PlayForm.cpp
	@echo "Building Source File PlayForm.cpp" && \
	m68k-palmos-gcc -O2 -palmos5.0 -DTRIAL -fno-rtti -fno-exceptions   -c PlayForm.cpp -o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PlayForm.o

# 
# C/C++ Source Files
# Building Source File DelayForm.cpp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/DelayForm.o:DelayForm.cpp
	@echo "Building Source File DelayForm.cpp" && \
	m68k-palmos-gcc -O2 -palmos5.0 -DTRIAL -fno-rtti -fno-exceptions   -c DelayForm.cpp -o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/DelayForm.o

# 
# C/C++ Source Files
# Building Source File SelectForm.cpp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/SelectForm.o:SelectForm.cpp
	@echo "Building Source File SelectForm.cpp" && \
	m68k-palmos-gcc -O2 -palmos5.0 -DTRIAL -fno-rtti -fno-exceptions   -c SelectForm.cpp -o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/SelectForm.o

# 
# C/C++ Source Files
# Building Source File EditEffectForm.cpp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditEffectForm.o:EditEffectForm.cpp
	@echo "Building Source File EditEffectForm.cpp" && \
	m68k-palmos-gcc -O2 -palmos5.0 -DTRIAL -fno-rtti -fno-exceptions   -c EditEffectForm.cpp -o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditEffectForm.o

# 
# C/C++ Source Files
# Building Source File EditCharForm.cpp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditCharForm.o:EditCharForm.cpp
	@echo "Building Source File EditCharForm.cpp" && \
	m68k-palmos-gcc -O2 -palmos5.0 -DTRIAL -fno-rtti -fno-exceptions     -isystem /cygdrive/c/Program\ Files/Falch.net/DeveloperStudio\ 2.6//palmdev/mathlib/src -isystem /cygdrive/c/Program\ Files/Falch.net/DeveloperStudio\ 2.6//bin/pose -c EditCharForm.cpp -o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditCharForm.o

# 
# C/C++ Source Files
# Building Source File CombatQOGC.cpp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/CombatQOGC.o:CombatQOGC.cpp
	@echo "Building Source File CombatQOGC.cpp" && \
	m68k-palmos-gcc -O2 -palmos5.0 -DTRIAL -fno-rtti -fno-exceptions     -isystem /cygdrive/c/Program\ Files/Falch.net/DeveloperStudio\ 2.6//palmdev/mathlib/src -isystem /cygdrive/c/Program\ Files/Falch.net/DeveloperStudio\ 2.6//bin/pose -c CombatQOGC.cpp -o ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/CombatQOGC.o

# 
# Palm OS Resource Files
# Building Resource File RollInitForm.rcp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/RollInitForm.ro:RollInitForm.rcp
	@echo "Building Resource File RollInitForm.rcp" && \
	pilrc -q -ro RollInitForm.rcp  ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/RollInitForm.ro

# 
# Palm OS Resource Files
# Building Resource File Main.rcp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Main.ro:Main.rcp
	@echo "Building Resource File Main.rcp" && \
	pilrc -q -ro Main.rcp  ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/Main.ro

# 
# Palm OS Resource Files
# Building Resource File OverviewForm.rcp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/OverviewForm.ro:OverviewForm.rcp
	@echo "Building Resource File OverviewForm.rcp" && \
	pilrc -q -ro OverviewForm.rcp  ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/OverviewForm.ro

# 
# Palm OS Resource Files
# Building Resource File PlayForm.rcp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PlayForm.ro:PlayForm.rcp
	@echo "Building Resource File PlayForm.rcp" && \
	pilrc -q -ro PlayForm.rcp  ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PlayForm.ro

# 
# Palm OS Resource Files
# Building Resource File DelayForm.rcp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/DelayForm.ro:DelayForm.rcp
	@echo "Building Resource File DelayForm.rcp" && \
	pilrc -q -ro DelayForm.rcp  ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/DelayForm.ro

# 
# Palm OS Resource Files
# Building Resource File SelectForm.rcp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/SelectForm.ro:SelectForm.rcp
	@echo "Building Resource File SelectForm.rcp" && \
	pilrc -q -ro SelectForm.rcp  ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/SelectForm.ro

# 
# Palm OS Resource Files
# Building Resource File EditEffectForm.rcp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditEffectForm.ro:EditEffectForm.rcp
	@echo "Building Resource File EditEffectForm.rcp" && \
	pilrc -q -ro EditEffectForm.rcp  ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditEffectForm.ro

# 
# Palm OS Resource Files
# Building Resource File EditCharForm.rcp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditCharForm.ro:EditCharForm.rcp
	@echo "Building Resource File EditCharForm.rcp" && \
	pilrc -q -ro EditCharForm.rcp  ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/EditCharForm.ro

# 
# Palm OS Resource Files
# Building Resource File PickNumberForm.rcp
#
./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PickNumberForm.ro:PickNumberForm.rcp
	@echo "Building Resource File PickNumberForm.rcp" && \
	pilrc -q -ro PickNumberForm.rcp  ./M68k_Palm_OS_-_Multiple_Sections_Release\ Trial/PickNumberForm.ro

