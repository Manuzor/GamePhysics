import os

# Some info about the ezEngine
EzConfig = {
    "Root" : os.getenv("EZ_ENGINE_DIR"),
    "SupportedConfigs" : [ "Debug", "Release" ],
    "SupportedArchitectures" : [ "32", "64" ],
}

# Generates ezEngine module dependency generator functions.
def EzModule(Name):
    def GenerateFilePattern(FileList, Config, Arch):
        assert Config in EzConfig["SupportedConfigs"],       "Unsupported configuration '{0}'".format(Config)
        assert Arch   in EzConfig["SupportedArchitectures"], "Unsupported architecture '{0}'".format(Arch)
        FileName = os.path.join(EzConfig["Root"], "Output", "Bin", "WinVs2013{0}{1}".format(Config, Arch), Name)
        FileList.append(FileName + ".dll")
        if Config != "Release":
            FileList.append(FileName + ".pdb")
    return GenerateFilePattern

Dependencies = [
    EzModule("ezTestFramework"),
]

# Required by the API.
def GetDependencies(Configuration, PlatformArchitecture):
    FileList = []
    for fileNameGenerator in Dependencies:
        fileNameGenerator(FileList, Configuration, PlatformArchitecture)
    return FileList
