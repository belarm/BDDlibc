##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=tests
ConfigurationName      :=Debug
WorkspacePath          :=/home/belarm/Documents/BDDlibc
ProjectPath            :=/home/belarm/source/bddlibc/tests
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=belarm
Date                   :=09/10/16
CodeLitePath           :=/home/belarm/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="tests.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IntermediateDirectory)/bddlib.c$(ObjectSuffix) $(IntermediateDirectory)/bddlib_pointers.c$(ObjectSuffix) $(IntermediateDirectory)/bdd2.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.c$(ObjectSuffix): main.c $(IntermediateDirectory)/main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/belarm/source/bddlibc/tests/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.c$(DependSuffix): main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/main.c$(DependSuffix) -MM main.c

$(IntermediateDirectory)/main.c$(PreprocessSuffix): main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.c$(PreprocessSuffix)main.c

$(IntermediateDirectory)/bddlib.c$(ObjectSuffix): bddlib.c $(IntermediateDirectory)/bddlib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/belarm/source/bddlibc/tests/bddlib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bddlib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bddlib.c$(DependSuffix): bddlib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bddlib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/bddlib.c$(DependSuffix) -MM bddlib.c

$(IntermediateDirectory)/bddlib.c$(PreprocessSuffix): bddlib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bddlib.c$(PreprocessSuffix)bddlib.c

$(IntermediateDirectory)/bddlib_pointers.c$(ObjectSuffix): bddlib_pointers.c $(IntermediateDirectory)/bddlib_pointers.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/belarm/source/bddlibc/tests/bddlib_pointers.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bddlib_pointers.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bddlib_pointers.c$(DependSuffix): bddlib_pointers.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bddlib_pointers.c$(ObjectSuffix) -MF$(IntermediateDirectory)/bddlib_pointers.c$(DependSuffix) -MM bddlib_pointers.c

$(IntermediateDirectory)/bddlib_pointers.c$(PreprocessSuffix): bddlib_pointers.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bddlib_pointers.c$(PreprocessSuffix)bddlib_pointers.c

$(IntermediateDirectory)/bdd2.c$(ObjectSuffix): bdd2.c $(IntermediateDirectory)/bdd2.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/belarm/source/bddlibc/tests/bdd2.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/bdd2.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/bdd2.c$(DependSuffix): bdd2.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/bdd2.c$(ObjectSuffix) -MF$(IntermediateDirectory)/bdd2.c$(DependSuffix) -MM bdd2.c

$(IntermediateDirectory)/bdd2.c$(PreprocessSuffix): bdd2.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/bdd2.c$(PreprocessSuffix)bdd2.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


