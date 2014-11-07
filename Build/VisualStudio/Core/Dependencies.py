import os

# Some info about the ezEngine
EzConfig = {
    "Root": os.getenv("EZ_ENGINE_DIR"),
    "SupportedConfigs": [ "Debug", "Release" ],
    "ConfigNameMapping": {
        "Debug": "Debug",
        "Release": "RelDeb", # In our release build, we want to use ez's RelWithDebInfo
    },
    "SupportedArchitectures": [ "32", "64" ],
}

# Some info about GLEW
GlewConfig = {
    "Root": os.getenv("OPENGL_GLEW_DIR"),
    # Supporting Debug and Release, but only the release version will be used.
    "SupportedConfigs": [ "Debug", "Release" ],
    "SupportedArchitectures": [ "32", "64" ],
}

#####################################################################

# Generates ezEngine module dependency generator functions.
def EzModule(Name):
    def MapConfigName(LocalConfigName):
        return EzConfig["ConfigNameMapping"][LocalConfigName] # Return ez's configuration
    def GenerateFilePattern(FileList, Config, Arch):
        assert Config in EzConfig["SupportedConfigs"],       "Unsupported configuration '{0}'".format(Config)
        assert Arch   in EzConfig["SupportedArchitectures"], "Unsupported architecture '{0}'".format(Arch)
        FileName = os.path.join(EzConfig["Root"], "Output", "Bin", "WinVs2013{0}{1}".format(MapConfigName(Config), Arch), Name)
        FileList.append(FileName + ".dll")
        if Config != "Release":
            FileList.append(FileName + ".pdb")
    return GenerateFilePattern

def GlewModule(Name):
    def GenerateFilePattern(FileList, Config, Arch):
        assert Config in GlewConfig["SupportedConfigs"],       "Unsupported configuration '{0}'".format(Config)
        assert Arch   in GlewConfig["SupportedArchitectures"], "Unsupported architecture '{0}'".format(Arch)
        SubDir = "Win32" if Arch == "32" else "x64"
        FileList.append(os.path.join(GlewConfig["Root"], "bin", "Release", SubDir, Name + ".dll"))
    return GenerateFilePattern

#####################################################################

Dependencies = [
    EzModule("ezFoundation"),
    EzModule("ezCore"),
    EzModule("ezSystem"),
    EzModule("ezInputXBox360"),
    EzModule("ezThirdParty"),
    EzModule("ezInspectorPlugin"),

    GlewModule("glew32"),
]

# Required by the API.
def GetDependencies(Configuration, PlatformArchitecture):
    FileList = []
    for fileNameGenerator in Dependencies:
        fileNameGenerator(FileList, Configuration, PlatformArchitecture)
    return FileList
