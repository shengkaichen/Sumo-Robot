##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=NEAT
ConfigurationName      :=Debug
WorkspacePath          :=/home/fabien/NEAT
ProjectPath            :=/home/fabien/NEAT
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=fabien
Date                   :=14/03/17
CodeLitePath           :=/home/fabien/.codelite
LinkerName             :=/usr/bin/x86_64-linux-gnu-g++
SharedObjectLinkerName :=/usr/bin/x86_64-linux-gnu-g++ -shared -fPIC
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
ObjectsFileList        :="NEAT.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -O0
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch). $(LibraryPathSwitch)Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/x86_64-linux-gnu-ar rcu
CXX      := /usr/bin/x86_64-linux-gnu-g++
CC       := /usr/bin/x86_64-linux-gnu-gcc
CXXFLAGS := -std=c++11 -Wall -g -Wall $(Preprocessors)
CFLAGS   :=   $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/x86_64-linux-gnu-as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/IDGenerator.cpp$(ObjectSuffix) $(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/Genome.cpp$(ObjectSuffix) $(IntermediateDirectory)/InnovationBank.cpp$(ObjectSuffix) $(IntermediateDirectory)/NEAT.cpp$(ObjectSuffix) $(IntermediateDirectory)/Settings.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/IDGenerator.cpp$(ObjectSuffix): IDGenerator.cpp $(IntermediateDirectory)/IDGenerator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/IDGenerator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/IDGenerator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/IDGenerator.cpp$(DependSuffix): IDGenerator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/IDGenerator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/IDGenerator.cpp$(DependSuffix) -MM IDGenerator.cpp

$(IntermediateDirectory)/IDGenerator.cpp$(PreprocessSuffix): IDGenerator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/IDGenerator.cpp$(PreprocessSuffix) IDGenerator.cpp

$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/Genome.cpp$(ObjectSuffix): Genome.cpp $(IntermediateDirectory)/Genome.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/Genome.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Genome.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Genome.cpp$(DependSuffix): Genome.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Genome.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Genome.cpp$(DependSuffix) -MM Genome.cpp

$(IntermediateDirectory)/Genome.cpp$(PreprocessSuffix): Genome.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Genome.cpp$(PreprocessSuffix) Genome.cpp

$(IntermediateDirectory)/InnovationBank.cpp$(ObjectSuffix): InnovationBank.cpp $(IntermediateDirectory)/InnovationBank.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/InnovationBank.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/InnovationBank.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/InnovationBank.cpp$(DependSuffix): InnovationBank.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/InnovationBank.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/InnovationBank.cpp$(DependSuffix) -MM InnovationBank.cpp

$(IntermediateDirectory)/InnovationBank.cpp$(PreprocessSuffix): InnovationBank.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/InnovationBank.cpp$(PreprocessSuffix) InnovationBank.cpp

$(IntermediateDirectory)/NEAT.cpp$(ObjectSuffix): NEAT.cpp $(IntermediateDirectory)/NEAT.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/NEAT.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/NEAT.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/NEAT.cpp$(DependSuffix): NEAT.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/NEAT.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/NEAT.cpp$(DependSuffix) -MM NEAT.cpp

$(IntermediateDirectory)/NEAT.cpp$(PreprocessSuffix): NEAT.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/NEAT.cpp$(PreprocessSuffix) NEAT.cpp

$(IntermediateDirectory)/Settings.cpp$(ObjectSuffix): Settings.cpp $(IntermediateDirectory)/Settings.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/fabien/NEAT/Settings.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Settings.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Settings.cpp$(DependSuffix): Settings.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Settings.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Settings.cpp$(DependSuffix) -MM Settings.cpp

$(IntermediateDirectory)/Settings.cpp$(PreprocessSuffix): Settings.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Settings.cpp$(PreprocessSuffix) Settings.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


